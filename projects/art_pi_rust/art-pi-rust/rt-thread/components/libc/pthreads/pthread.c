/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     Bernard      Fix pthread_detach issue for a none-joinable
 *                             thread.
 * 2019-02-07     Bernard      Add _pthread_destroy to release pthread resource.
 */

#include <rthw.h>
#include <pthread.h>
#include <sched.h>
#include "pthread_internal.h"

RT_DEFINE_SPINLOCK(pth_lock);
_pthread_data_t *pth_table[PTHREAD_NUM_MAX] = {NULL};

_pthread_data_t *_pthread_get_data(pthread_t thread)
{
    RT_DECLARE_SPINLOCK(pth_lock);
    _pthread_data_t *ptd;

    if (thread >= PTHREAD_NUM_MAX) return NULL;

    rt_hw_spin_lock(&pth_lock);
    ptd = pth_table[thread];
    rt_hw_spin_unlock(&pth_lock);

    if (ptd && ptd->magic == PTHREAD_MAGIC) return ptd;

    return NULL;
}

pthread_t _pthread_data_get_pth(_pthread_data_t *ptd)
{
    int index;
    RT_DECLARE_SPINLOCK(pth_lock);

    rt_hw_spin_lock(&pth_lock);
    for (index = 0; index < PTHREAD_NUM_MAX; index ++)
    {
        if (pth_table[index] == ptd) break;
    }
    rt_hw_spin_unlock(&pth_lock);

    return index;
}

pthread_t _pthread_data_create(void)
{
    int index;
    _pthread_data_t *ptd = NULL;
    RT_DECLARE_SPINLOCK(pth_lock);

    ptd = (_pthread_data_t*)rt_malloc(sizeof(_pthread_data_t));
    if (!ptd) return PTHREAD_NUM_MAX;

    memset(ptd, 0x0, sizeof(_pthread_data_t));
    ptd->canceled = 0;
    ptd->cancelstate = PTHREAD_CANCEL_DISABLE;
    ptd->canceltype = PTHREAD_CANCEL_DEFERRED;
    ptd->magic = PTHREAD_MAGIC;

    rt_hw_spin_lock(&pth_lock);
    for (index = 0; index < PTHREAD_NUM_MAX; index ++)
    {
        if (pth_table[index] == NULL)
        {
            pth_table[index] = ptd;
            break;
        }
    }
    rt_hw_spin_unlock(&pth_lock);

    /* full of pthreads, clean magic and release ptd */
    if (index == PTHREAD_NUM_MAX)
    {
        ptd->magic = 0x0;
        rt_free(ptd);
    }

    return index;
}

void _pthread_data_destroy(pthread_t pth)
{
    RT_DECLARE_SPINLOCK(pth_lock);

    _pthread_data_t *ptd = _pthread_get_data(pth);
    if (ptd)
    {
        /* remove from pthread table */
        rt_hw_spin_lock(&pth_lock);
        pth_table[pth] = NULL;
        rt_hw_spin_unlock(&pth_lock);

        /* delete joinable semaphore */
        if (ptd->joinable_sem != RT_NULL)
            rt_sem_delete(ptd->joinable_sem);

        /* release thread resource */
        if (ptd->attr.stackaddr == RT_NULL && ptd->tid->stack_addr != RT_NULL)
        {
            /* release thread allocated stack */
            rt_free(ptd->tid->stack_addr);
        }
        /* clean stack addr pointer */
        ptd->tid->stack_addr = RT_NULL;

        /*
        * if this thread create the local thread data,
        * delete it
        */
        if (ptd->tls != RT_NULL) rt_free(ptd->tls);
        rt_free(ptd->tid);

        /* clean magic */
        ptd->magic = 0x0;

        /* free ptd */
        rt_free(ptd);
    }
}

int pthread_system_init(void)
{
    /* initialize key area */
    pthread_key_system_init();
    /* initialize posix mqueue */
    posix_mq_system_init();
    /* initialize posix semaphore */
    posix_sem_system_init();

    return 0;
}
INIT_COMPONENT_EXPORT(pthread_system_init);

static void _pthread_destroy(_pthread_data_t *ptd)
{
    pthread_t pth = _pthread_data_get_pth(ptd);
    if (pth != PTHREAD_NUM_MAX)
    {
        _pthread_data_destroy(pth);
    }

    return;
}

static void _pthread_cleanup(rt_thread_t tid)
{
    _pthread_data_t *ptd;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)tid->user_data;
    RT_ASSERT(ptd != RT_NULL);

    /* clear cleanup function */
    tid->cleanup = RT_NULL;
    if (ptd->attr.detachstate == PTHREAD_CREATE_JOINABLE)
    {
        rt_sem_release(ptd->joinable_sem);
    }
    else
    {
        /* release pthread resource */
        _pthread_destroy(ptd);
    }
}

static void pthread_entry_stub(void *parameter)
{
    void *value;
    _pthread_data_t *ptd;

    ptd = (_pthread_data_t *)parameter;

    /* execute pthread entry */
    value = ptd->thread_entry(ptd->thread_parameter);
    /* set value */
    ptd->return_value = value;
}

int pthread_create(pthread_t            *pid,
                   const pthread_attr_t *attr,
                   void *(*start)(void *), void *parameter)
{
    int ret = 0;
    void *stack;
    char name[RT_NAME_MAX];
    static rt_uint16_t pthread_number = 0;

    pthread_t pth_id;
    _pthread_data_t *ptd;

    /* pid shall be provided */
    RT_ASSERT(pid != RT_NULL);

    /* allocate posix thread data */
    pth_id = _pthread_data_create();
    if (pth_id == PTHREAD_NUM_MAX) 
    {
        ret = ENOMEM;
        goto __exit;
    }
    /* get pthread data */
    ptd = _pthread_get_data(pth_id);

    if (attr != RT_NULL)
    {
        ptd->attr = *attr;
    }
    else
    {
        /* use default attribute */
        pthread_attr_init(&ptd->attr);
    }

    rt_snprintf(name, sizeof(name), "pth%02d", pthread_number ++);

    /* pthread is a static thread object */
    ptd->tid = (rt_thread_t) rt_malloc(sizeof(struct rt_thread));
    if (ptd->tid == RT_NULL)
    {
        ret = ENOMEM;
        goto __exit;
    }
    memset(ptd->tid, 0, sizeof(struct rt_thread));

    if (ptd->attr.detachstate == PTHREAD_CREATE_JOINABLE)
    {
        ptd->joinable_sem = rt_sem_create(name, 0, RT_IPC_FLAG_FIFO);
        if (ptd->joinable_sem == RT_NULL)
        {
            ret = ENOMEM;
            goto __exit;
        }
    }
    else
    {
        ptd->joinable_sem = RT_NULL;
    }

    /* set parameter */
    ptd->thread_entry = start;
    ptd->thread_parameter = parameter;

    /* stack */
    if (ptd->attr.stackaddr == 0)
    {
        stack = (void *)rt_malloc(ptd->attr.stacksize);
    }
    else
    {
        stack = (void *)(ptd->attr.stackaddr);
    }

    if (stack == RT_NULL)
    {
        ret = ENOMEM;
        goto __exit;
    }

    /* initial this pthread to system */
    if (rt_thread_init(ptd->tid, name, pthread_entry_stub, ptd,
                       stack, ptd->attr.stacksize,
                       ptd->attr.schedparam.sched_priority, 5) != RT_EOK)
    {
        ret = EINVAL;
        goto __exit;
    }

    /* set pthread id */
    *pid = pth_id;

    /* set pthread cleanup function and ptd data */
    ptd->tid->cleanup = _pthread_cleanup;
    ptd->tid->user_data = (rt_ubase_t)ptd;

    /* start thread */
    if (rt_thread_startup(ptd->tid) == RT_EOK)
        return 0;

    /* start thread failed */
    rt_thread_detach(ptd->tid);
    ret = EINVAL;

__exit:
    if (pth_id != PTHREAD_NUM_MAX)
        _pthread_data_destroy(pth_id);
    return ret;
}
RTM_EXPORT(pthread_create);

int pthread_detach(pthread_t thread)
{
    int ret = 0;
    _pthread_data_t *ptd = _pthread_get_data(thread);

    rt_enter_critical();
    if (ptd->attr.detachstate == PTHREAD_CREATE_DETACHED)
    {
        /* The implementation has detected that the value specified by thread does not refer
         * to a joinable thread.
         */
        ret = EINVAL;
        goto __exit;
    }

    if ((ptd->tid->stat & RT_THREAD_STAT_MASK) == RT_THREAD_CLOSE)
    {
        /* this defunct pthread is not handled by idle */
        if (rt_sem_trytake(ptd->joinable_sem) != RT_EOK)
        {
            rt_sem_release(ptd->joinable_sem);

            /* change to detach state */
            ptd->attr.detachstate = PTHREAD_CREATE_DETACHED;

            /* detach joinable semaphore */
            if (ptd->joinable_sem)
            {
                rt_sem_delete(ptd->joinable_sem);
                ptd->joinable_sem = RT_NULL;
            }
        }
        else
        {
            /* destroy this pthread */
            _pthread_destroy(ptd);
        }

        goto __exit;
    }
    else
    {
        /* change to detach state */
        ptd->attr.detachstate = PTHREAD_CREATE_DETACHED;

        /* detach joinable semaphore */
        if (ptd->joinable_sem)
        {
            rt_sem_delete(ptd->joinable_sem);
            ptd->joinable_sem = RT_NULL;
        }
    }

__exit:
    rt_exit_critical();
    return ret;
}
RTM_EXPORT(pthread_detach);

int pthread_join(pthread_t thread, void **value_ptr)
{
    _pthread_data_t *ptd;
    rt_err_t result;

    ptd = _pthread_get_data(thread);
    if (ptd && ptd->tid == rt_thread_self())
    {
        /* join self */
        return EDEADLK;
    }

    if (ptd->attr.detachstate == PTHREAD_CREATE_DETACHED)
        return EINVAL; /* join on a detached pthread */

    result = rt_sem_take(ptd->joinable_sem, RT_WAITING_FOREVER);
    if (result == RT_EOK)
    {
        /* get return value */
        if (value_ptr != RT_NULL)
            *value_ptr = ptd->return_value;

        /* destroy this pthread */
        _pthread_destroy(ptd);
    }
    else
    {
        return ESRCH;
    }

    return 0;
}
RTM_EXPORT(pthread_join);

pthread_t pthread_self (void)
{
    rt_thread_t tid;
    _pthread_data_t *ptd;

    tid = rt_thread_self();
    if (tid == NULL) return PTHREAD_NUM_MAX;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    return _pthread_data_get_pth(ptd);
}
RTM_EXPORT(pthread_self);

void pthread_exit(void *value)
{
    _pthread_data_t *ptd;
    _pthread_cleanup_t *cleanup;
    extern _pthread_key_data_t _thread_keys[PTHREAD_KEY_MAX];

    if (rt_thread_self() == NULL) return;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;

    rt_enter_critical();
    /* disable cancel */
    ptd->cancelstate = PTHREAD_CANCEL_DISABLE;
    /* set return value */
    ptd->return_value = value;
    rt_exit_critical();

    /* invoke pushed cleanup */
    while (ptd->cleanup != RT_NULL)
    {
        cleanup = ptd->cleanup;
        ptd->cleanup = cleanup->next;

        cleanup->cleanup_func(cleanup->parameter);
        /* release this cleanup function */
        rt_free(cleanup);
    }

    /* destruct thread local key */
    if (ptd->tls != RT_NULL)
    {
        void *data;
        rt_uint32_t index;

        for (index = 0; index < PTHREAD_KEY_MAX; index ++)
        {
            if (_thread_keys[index].is_used)
            {
                data = ptd->tls[index];
                if (data)
                    _thread_keys[index].destructor(data);
            }
        }

        /* release tls area */
        rt_free(ptd->tls);
        ptd->tls = RT_NULL;
    }

    /* detach thread */
    rt_thread_detach(ptd->tid);
    /* reschedule thread */
    rt_schedule();
}
RTM_EXPORT(pthread_exit);

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    RT_ASSERT(once_control != RT_NULL);
    RT_ASSERT(init_routine != RT_NULL);

    rt_enter_critical();
    if (!(*once_control))
    {
        /* call routine once */
        *once_control = 1;
        rt_exit_critical();

        init_routine();
    }
    rt_exit_critical();

    return 0;
}
RTM_EXPORT(pthread_once);

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    return EOPNOTSUPP;
}
RTM_EXPORT(pthread_atfork);

int pthread_kill(pthread_t thread, int sig)
{
#ifdef RT_USING_SIGNALS
    _pthread_data_t *ptd;

    ptd = _pthread_get_data(thread);
    if (ptd)
    {
        return rt_thread_kill(ptd->tid, sig);
    }

    return EINVAL;
#else
    return ENOSYS;
#endif
}
RTM_EXPORT(pthread_kill);

#ifdef RT_USING_SIGNALS
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
    return sigprocmask(how, set, oset);
}
#endif

void pthread_cleanup_pop(int execute)
{
    _pthread_data_t *ptd;
    _pthread_cleanup_t *cleanup;

    if (rt_thread_self() == NULL) return;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    if (execute)
    {
        rt_enter_critical();
        cleanup = ptd->cleanup;
        if (cleanup)
            ptd->cleanup = cleanup->next;
        rt_exit_critical();

        if (cleanup)
        {
            cleanup->cleanup_func(cleanup->parameter);

            rt_free(cleanup);
        }
    }
}
RTM_EXPORT(pthread_cleanup_pop);

void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    _pthread_data_t *ptd;
    _pthread_cleanup_t *cleanup;

    if (rt_thread_self() == NULL) return;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    cleanup = (_pthread_cleanup_t *)rt_malloc(sizeof(_pthread_cleanup_t));
    if (cleanup != RT_NULL)
    {
        cleanup->cleanup_func = routine;
        cleanup->parameter = arg;

        rt_enter_critical();
        cleanup->next = ptd->cleanup;
        ptd->cleanup = cleanup;
        rt_exit_critical();
    }
}
RTM_EXPORT(pthread_cleanup_push);

/*
 * According to IEEE Std 1003.1, 2004 Edition , following pthreads
 * interface support cancellation point:
 * mq_receive()
 * mq_send()
 * mq_timedreceive()
 * mq_timedsend()
 * msgrcv()
 * msgsnd()
 * msync()
 * pthread_cond_timedwait()
 * pthread_cond_wait()
 * pthread_join()
 * pthread_testcancel()
 * sem_timedwait()
 * sem_wait()
 *
 * A cancellation point may also occur when a thread is
 * executing the following functions:
 * pthread_rwlock_rdlock()
 * pthread_rwlock_timedrdlock()
 * pthread_rwlock_timedwrlock()
 * pthread_rwlock_wrlock()
 *
 * The pthread_cancel(), pthread_setcancelstate(), and pthread_setcanceltype()
 * functions are defined to be async-cancel safe.
 */

int pthread_setcancelstate(int state, int *oldstate)
{
    _pthread_data_t *ptd;

    if (rt_thread_self() == NULL) return EINVAL;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    if ((state == PTHREAD_CANCEL_ENABLE) || (state == PTHREAD_CANCEL_DISABLE))
    {
        if (oldstate)
            *oldstate = ptd->cancelstate;
        ptd->cancelstate = state;

        return 0;
    }

    return EINVAL;
}
RTM_EXPORT(pthread_setcancelstate);

int pthread_setcanceltype(int type, int *oldtype)
{
    _pthread_data_t *ptd;

    if (rt_thread_self() == NULL) return EINVAL;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    if ((type != PTHREAD_CANCEL_DEFERRED) && (type != PTHREAD_CANCEL_ASYNCHRONOUS))
        return EINVAL;

    if (oldtype)
        *oldtype = ptd->canceltype;
    ptd->canceltype = type;

    return 0;
}
RTM_EXPORT(pthread_setcanceltype);

void pthread_testcancel(void)
{
    int cancel = 0;
    _pthread_data_t *ptd;

    if (rt_thread_self() == NULL) return;

    /* get pthread data from user data of thread */
    ptd = (_pthread_data_t *)rt_thread_self()->user_data;
    RT_ASSERT(ptd != RT_NULL);

    if (ptd->cancelstate == PTHREAD_CANCEL_ENABLE)
        cancel = ptd->canceled;
    if (cancel)
        pthread_exit((void *)PTHREAD_CANCELED);
}
RTM_EXPORT(pthread_testcancel);

int pthread_cancel(pthread_t thread)
{
    _pthread_data_t *ptd;

    /* get posix thread data */
    ptd = _pthread_get_data(thread);
    RT_ASSERT(ptd != RT_NULL);

    /* cancel self */
    if (ptd->tid == rt_thread_self())
        return 0;

    /* set canceled */
    if (ptd->cancelstate == PTHREAD_CANCEL_ENABLE)
    {
        ptd->canceled = 1;
        if (ptd->canceltype == PTHREAD_CANCEL_ASYNCHRONOUS)
        {
            /*
             * to detach thread.
             * this thread will be removed from scheduler list
             * and because there is a cleanup function in the
             * thread (pthread_cleanup), it will move to defunct
             * thread list and wait for handling in idle thread.
             */
            rt_thread_detach(ptd->tid);
        }
    }

    return 0;
}
RTM_EXPORT(pthread_cancel);

