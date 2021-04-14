/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-04     Ghazigq     the first version
 */

#include "ui_nes_list.h"
#include "ui_nes.h"
#include "rtthread.h"
#include "dfs_posix.h"
#include "gbk2utf8.h"
#include "stddef.h"

static char *pathname = NULL;
lv_obj_t *file_list = NULL;
lv_obj_t *file_list_label = NULL;

static lv_obj_t *ui_obj = NULL;
static struct dfs_fd fd;
static struct dirent dirent;
static rt_sem_t ui_nes_sem = NULL;
static rt_thread_t ui_nes_tid = NULL;

static void nes_list_event_handler(lv_obj_t * obj, lv_event_t event)
{
    const char * btn_name = NULL;
    if(event == LV_EVENT_CLICKED)
    {
        btn_name = lv_list_get_btn_text(obj);
        if(strstr(btn_name, ".nes") || strstr(btn_name, ".NES"))
        {
            char *name = NULL;
            if(utf82gbk(&name, (void *)btn_name, strlen(btn_name)) > 0)
            {
                char *fullpath_name = rt_malloc(strlen(pathname) + strlen(name) + 2);//会在nes_entry中释放fullpath_name
                if(fullpath_name)
                {
                    strcpy(fullpath_name, pathname);
                    if(strcmp(pathname, "/") != 0)
                        strcat(fullpath_name, "/");
                    strcat(fullpath_name, name);
                    lv_obj_set_hidden(file_list, true);
                    ui_nes_enter(ui_obj, (uint8_t *)fullpath_name);
                }
                rt_free(name);
            }
        }
        else if(strstr(btn_name, "..")){
            if(strcmp(pathname, "/")!= 0)
            {
                char *index = strrchr(pathname, '/');
                if(index != NULL)
                {
                    char *path = NULL;
                    if(index - pathname)
                    {
                        path = rt_malloc(index - pathname + 1);
                        if(path)
                        {
                            memcpy(path, pathname, index - pathname);
                            path[index - pathname] = 0;
                        }
                    }
                    else {
                        path = rt_malloc(2);
                        if(path)
                            sprintf(path, "/");
                    }
                    if(path)
                    {
                        rt_free(pathname);
                        pathname = path;
                        char *name = NULL;
                        if(utf82gbk(&name, pathname, strlen(pathname)) > 0)
                        {
                            rt_free(pathname);
                            pathname = name;
                            rt_sem_release(ui_nes_sem);
                        }
                    }
                }
            }
        }
        else {
            char *path = pathname;
            pathname = rt_malloc(strlen(path) + strlen(btn_name) + 2);
            if(pathname == NULL)
                pathname = path;
            else {
                strcpy(pathname, path);
                if(strcmp(path, "/")!= 0)
                    strcat(pathname, "/");
                strcat(pathname, btn_name);
                rt_free(path);
                char *name = NULL;
                if(utf82gbk(&name, pathname, strlen(pathname)) > 0)
                {
                    rt_free(pathname);
                    pathname = name;
                    rt_sem_release(ui_nes_sem);
                }
            }
        }
    }
}

static void nes_list_update(void)
{
    struct stat stat;
    int length = 0;
    char *fullpath = NULL, *path = NULL;
    lv_obj_t * list_btn;

    //copy pathname to path to avoid pathname changed when touching list btn
    path = rt_malloc(strlen(pathname) + 1);
    if (path == NULL)
        return;
    strcpy(path, pathname);

    /* list directory */
    if (dfs_file_open(&fd, path, O_DIRECTORY) == 0)
    {
        char *name = NULL;
        if(gbk2utf8(&name, path, strlen(path)) > 0)
        {
            lv_label_set_text(file_list_label, (const char *)name);
            lv_obj_align(file_list_label, ui_obj, LV_ALIGN_IN_TOP_MID, 0, 2);
            rt_free(name);
        }
        /*Create a list*/
        if(file_list)
            lv_obj_del(file_list);
        file_list = NULL;
        file_list = lv_list_create(ui_obj, NULL);
        if(file_list == NULL)
            return;
        lv_obj_set_size(file_list, 460, 300);
        lv_list_set_edge_flash(file_list, true);
        lv_list_set_scrollbar_mode(file_list, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_align(file_list, file_list_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_set_click(file_list, false);

        if(strcmp(path, "/") != 0)
        {
            list_btn = lv_list_add_btn(file_list, LV_SYMBOL_FILE, "..");
            lv_obj_set_event_cb(list_btn, nes_list_event_handler);
        }

        do
        {

            memset(&dirent, 0, sizeof(struct dirent));
            length = dfs_file_getdents(&fd, &dirent, sizeof(struct dirent));
            if (length > 0)
            {
                memset(&stat, 0, sizeof(struct stat));

                /* build full path for each file */
                fullpath = dfs_normalize_path(path, dirent.d_name);
                if (fullpath == NULL)
                    break;

                if (dfs_file_stat(fullpath, &stat) == 0)
                {
                    char *name = NULL;
                    if(gbk2utf8(&name, dirent.d_name, strlen(dirent.d_name)) > 0)
                    {
                        list_btn = lv_list_add_btn(file_list, LV_SYMBOL_FILE, (const char *)name);
                        lv_obj_set_event_cb(list_btn, nes_list_event_handler);
                        rt_free(name);
                    }
                }

                rt_free(fullpath);
            }
        }
        while (length > 0);

        dfs_file_close(&fd);
    }
    if(path)
        rt_free(path);

}

void ui_nes_list_thread_entry(void *parameter)
{
    while(1)
    {
        if(rt_sem_take(ui_nes_sem, RT_WAITING_FOREVER) == RT_EOK)
            nes_list_update();
    }

    rt_free(pathname);
    pathname = NULL;
}

void ui_nes_list_enter(lv_obj_t *obj)
{
    ui_obj = obj;
    if(pathname == NULL)
    {
        pathname = rt_strdup("/");
        if(pathname == NULL)
            return;
    }

    file_list_label = lv_label_create(obj, NULL);
    lv_obj_set_size(file_list_label, 460, 16);
    lv_obj_align(file_list_label, obj, LV_ALIGN_IN_TOP_MID, 0, 2);

    ui_nes_sem = rt_sem_create("nes_list", 1, RT_IPC_FLAG_FIFO);
    if(!ui_nes_sem)
        return;
    ui_nes_tid = rt_thread_create("nes_list", ui_nes_list_thread_entry, NULL, 4096, 6, 10);
    if(ui_nes_tid)
        rt_thread_startup(ui_nes_tid);
}

void ui_nes_list_exit(void)
{
    lv_obj_del(file_list);
    file_list = NULL;
}

