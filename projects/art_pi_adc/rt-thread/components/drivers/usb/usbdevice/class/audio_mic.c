/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-07     flybreak     the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtservice.h>
#include <rtdevice.h>

#include "drivers/usb_device.h"
#include "audio.h"

#define DBG_TAG              "usbd.audio.mic"
#define DBG_LVL              DBG_INFO
#include <rtdbg.h>

#define RECORD_SAMPLERATE   16000
#define RECORD_CHANNEL      1
#define RESOLUTION_BITS     16

#define RESOLUTION_BYTE     (RESOLUTION_BITS / 8)
#define RECORD_PER_MS_SZ    ((RECORD_SAMPLERATE * RECORD_CHANNEL * RESOLUTION_BYTE) / 1000)
#define RECORD_BUFFER_SZ    (RECORD_PER_MS_SZ * 20)  /* 20ms */

#if defined(RT_USBD_MIC_DEVICE_NAME)
    #define MIC_DEVICE_NAME    RT_USBD_MIC_DEVICE_NAME
#else
    #define MIC_DEVICE_NAME    "mic0"
#endif

#define EVENT_RECORD_START   (1 << 0)
#define EVENT_RECORD_STOP    (1 << 1)
#define EVENT_RECORD_DATA    (1 << 2)

/*
 * uac mic descriptor define
 */

#define UAC_CS_INTERFACE            0x24
#define UAC_CS_ENDPOINT             0x25

#define UAC_MAX_PACKET_SIZE         64
#define UAC_EP_MAX_PACKET_SIZE      32
#define UAC_CHANNEL_NUM             RECORD_CHANNEL
#define UAC_INTR_NUM                1
#define UAC_CH_NUM                  1
#define UAC_FORMAT_NUM              1

struct uac_ac_descriptor
{
#ifdef RT_USB_DEVICE_COMPOSITE
    struct uiad_descriptor iad_desc;
#endif
    struct uinterface_descriptor intf_desc;
    DECLARE_UAC_AC_HEADER_DESCRIPTOR(UAC_INTR_NUM) hdr_desc;
    struct uac_input_terminal_descriptor it_desc;
    struct uac1_output_terminal_descriptor ot_desc;
#if UAC_USE_FEATURE_UNIT
    DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR(UAC_CH_NUM) feature_unit_desc;
#endif
};

struct uac_as_descriptor
{
    struct uinterface_descriptor intf_desc;
    struct uac1_as_header_descriptor hdr_desc;
    DECLARE_UAC_FORMAT_TYPE_I_DISCRETE_DESC(UAC_FORMAT_NUM) format_type_desc;
    struct uendpoint_descriptor ep_desc;
    struct uac_iso_endpoint_descriptor as_ep_desc;
};

/*
 * uac mic device type
 */

struct uac_audio_mic
{
    rt_device_t  dev;
    rt_event_t   event;
    rt_uint8_t   open_count;

    rt_uint8_t  *buffer;
    rt_uint32_t  buffer_index;

    uep_t        ep;
};
static struct uac_audio_mic mic;

ALIGN(4)
static struct udevice_descriptor dev_desc =
{
    USB_DESC_LENGTH_DEVICE,     //bLength;
    USB_DESC_TYPE_DEVICE,       //type;
    USB_BCD_VERSION,            //bcdUSB;
    USB_CLASS_DEVICE,           //bDeviceClass;
    0x00,                       //bDeviceSubClass;
    0x00,                       //bDeviceProtocol;
    UAC_MAX_PACKET_SIZE,        //bMaxPacketSize0;
    _VENDOR_ID,                 //idVendor;
    _PRODUCT_ID,                //idProduct;
    USB_BCD_DEVICE,             //bcdDevice;
    USB_STRING_MANU_INDEX,      //iManufacturer;
    USB_STRING_PRODUCT_INDEX,   //iProduct;
    USB_STRING_SERIAL_INDEX,    //iSerialNumber;Unused.
    USB_DYNAMIC,                //bNumConfigurations;
};

//FS and HS needed
ALIGN(4)
static struct usb_qualifier_descriptor dev_qualifier =
{
    sizeof(dev_qualifier),          //bLength
    USB_DESC_TYPE_DEVICEQUALIFIER,  //bDescriptorType
    0x0200,                         //bcdUSB
    USB_CLASS_AUDIO,                //bDeviceClass
    0x00,                           //bDeviceSubClass
    0x00,                           //bDeviceProtocol
    64,                             //bMaxPacketSize0
    0x01,                           //bNumConfigurations
    0,
};

ALIGN(4)
const static char *_ustring[] =
{
    "Language",
    "RT-Thread Team.",
    "Microphone",
    "32021919830108",
    "Configuration",
    "Interface",
};

ALIGN(4)
static struct uac_ac_descriptor ac_desc =
{
#ifdef RT_USB_DEVICE_COMPOSITE
    /* Interface Association Descriptor */
    {
        USB_DESC_LENGTH_IAD,
        USB_DESC_TYPE_IAD,
        USB_DYNAMIC,
        0x02,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOSTREAMING,
        0x00,
        0x00,
    },
#endif
    /* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,
        0x00,
        0x00,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOCONTROL,
        0x00,
        0x00,
    },
    /* Header Descriptor */
    {
        UAC_DT_AC_HEADER_SIZE(UAC_INTR_NUM),
        UAC_CS_INTERFACE,
        UAC_HEADER,
        0x0100,    /* Version: 1.00 */
        0x001E,    /* Total length: 30 */
        0x01,      /* Total number of interfaces: 1 */
        {0x01},    /* Interface number: 1 */
    },
    /*  Input Terminal Descriptor */
    {
        UAC_DT_INPUT_TERMINAL_SIZE,
        UAC_CS_INTERFACE,
        UAC_INPUT_TERMINAL,
        0x01,      /* Terminal ID: 1 */
        0x0201,    /* Terminal Type: Microphone (0x0201) */
        0x00,      /* Assoc Terminal: 0 */
        0x01,      /* Number Channels: 1 */
        0x0000,    /* Channel Config: 0x0000 */
        0x00,      /* Channel Names: 0 */
        0x00,      /* Terminal: 0 */
    },
    /*  Output Terminal Descriptor */
    {
        UAC_DT_OUTPUT_TERMINAL_SIZE,
        UAC_CS_INTERFACE,
        UAC_OUTPUT_TERMINAL,
        0x02,      /* Terminal ID: 2 */
        0x0101,    /* Terminal Type: USB Streaming (0x0101) */
        0x00,      /* Assoc Terminal: 0 */
        0x01,      /* Source ID: 1 */
        0x00,      /* Terminal: 0 */
    },
#if UAC_USE_FEATURE_UNIT
    /*  Feature unit Descriptor */
    {
        UAC_DT_FEATURE_UNIT_SIZE(UAC_CH_NUM),
        UAC_CS_INTERFACE,
        UAC_FEATURE_UNIT,
        0x02,
        0x0101,
        0x00,
        0x01,
    },
#endif
};

ALIGN(4)
static struct uinterface_descriptor as_desc0 =
{
    USB_DESC_LENGTH_INTERFACE,
    USB_DESC_TYPE_INTERFACE,
    USB_DYNAMIC,
    0x00,
    0x00,
    USB_CLASS_AUDIO,
    USB_SUBCLASS_AUDIOSTREAMING,
    0x00,
    0x00,
};

ALIGN(4)
static struct uac_as_descriptor as_desc =
{
    /* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,
        0x01,
        0x01,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOSTREAMING,
        0x00,
        0x00,
    },
    /* General AS Descriptor */
    {
        UAC_DT_AS_HEADER_SIZE,
        UAC_CS_INTERFACE,
        UAC_AS_GENERAL,
        0x02,      /* Terminal ID: 2 */
        0x01,      /* Interface delay in frames: 1 */
        UAC_FORMAT_TYPE_I_PCM,
    },
    /* Format type i Descriptor */
    {
        UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(UAC_FORMAT_NUM),
        UAC_CS_INTERFACE,
        UAC_FORMAT_TYPE,
        UAC_FORMAT_TYPE_I,
        UAC_CHANNEL_NUM,
        2,         /* Subframe Size: 2 */
        RESOLUTION_BITS,
        0x01,      /* Samples Frequence Type: 1 */
        {0},       /* Samples Frequence */
    },
    /* Endpoint Descriptor */
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_IN,
        USB_EP_ATTR_ISOC,
        UAC_EP_MAX_PACKET_SIZE,
        0x01,
    },
    /* AS Endpoint Descriptor */
    {
        UAC_ISO_ENDPOINT_DESC_SIZE,
        UAC_CS_ENDPOINT,
        UAC_MS_GENERAL,
    },
};

void mic_entry(void *parameter)
{
    struct rt_audio_caps caps = {0};
    rt_uint32_t e, index;

    mic.buffer = rt_malloc(RECORD_BUFFER_SZ);
    if (mic.buffer == RT_NULL)
    {
        LOG_E("malloc failed");
        goto __exit;
    }

    mic.dev = rt_device_find(MIC_DEVICE_NAME);
    if (mic.dev == RT_NULL)
    {
        LOG_E("can't find device:%s", MIC_DEVICE_NAME);
        goto __exit;
    }

    while (1)
    {
        if (rt_event_recv(mic.event, EVENT_RECORD_START | EVENT_RECORD_STOP,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          1000, &e) != RT_EOK)
        {
            continue;
        }
        if (mic.open_count == 0)
        {
            continue;
        }
        LOG_D("record start");

        rt_device_open(mic.dev, RT_DEVICE_OFLAG_RDONLY);

        caps.main_type               = AUDIO_TYPE_INPUT;
        caps.sub_type                = AUDIO_DSP_PARAM;
        caps.udata.config.samplerate = RECORD_SAMPLERATE;
        caps.udata.config.channels   = RECORD_CHANNEL;
        caps.udata.config.samplebits = RESOLUTION_BITS;
        rt_device_control(mic.dev, AUDIO_CTL_CONFIGURE, &caps);

        while (1)
        {
            if (rt_event_recv(mic.event, EVENT_RECORD_DATA | EVENT_RECORD_STOP,
                              RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                              1000, &e) != RT_EOK)
            {
                if (mic.open_count > 0)
                    continue;
                else
                    break;
            }
            if (e & EVENT_RECORD_DATA)
            {
                index = (mic.buffer_index >= RECORD_BUFFER_SZ / 2) ? 0 : (RECORD_BUFFER_SZ / 2);
                rt_device_read(mic.dev, 0, mic.buffer + index, RECORD_BUFFER_SZ / 2);
            }
            else if (e & EVENT_RECORD_STOP)
            {
                break;
            }
        }
        LOG_D("record stop");
        rt_device_close(mic.dev);
    }

__exit:
    if (mic.buffer)
        rt_free(mic.buffer);
}

static rt_err_t _record_start(ufunction_t func)
{
    mic.ep->request.buffer = RT_NULL;
    mic.ep->request.size = UAC_EP_MAX_PACKET_SIZE;
    mic.ep->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, mic.ep, &mic.ep->request);

    mic.open_count ++;
    rt_event_send(mic.event, EVENT_RECORD_START);
    return 0;
}

static rt_err_t _record_stop(ufunction_t func)
{
    mic.open_count --;
    rt_event_send(mic.event, EVENT_RECORD_STOP);
    return 0;
}

static rt_err_t _ep_data_in_handler(ufunction_t func, rt_size_t size)
{
    RT_ASSERT(func != RT_NULL);
    LOG_D("_ep_data_in_handler");

    mic.ep->request.buffer = mic.buffer + mic.buffer_index;
    mic.ep->request.size = UAC_EP_MAX_PACKET_SIZE;
    mic.ep->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, mic.ep, &mic.ep->request);

    mic.buffer_index += UAC_EP_MAX_PACKET_SIZE;
    if (mic.buffer_index >= RECORD_BUFFER_SZ)
    {
        mic.buffer_index = 0;
        rt_event_send(mic.event, EVENT_RECORD_DATA);
    }
    else if (mic.buffer_index == RECORD_BUFFER_SZ / 2)
    {
        rt_event_send(mic.event, EVENT_RECORD_DATA);
    }

    return RT_EOK;
}

static rt_err_t _interface_as_handler(ufunction_t func, ureq_t setup)
{
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    RT_ASSERT(setup != RT_NULL);

    LOG_D("_interface_as_handler");

    if ((setup->request_type & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_STANDARD)
    {
        switch (setup->bRequest)
        {
        case USB_REQ_GET_INTERFACE:
            break;
        case USB_REQ_SET_INTERFACE:
            LOG_D("set interface handler");
            if (setup->wValue == 1)
            {
                _record_start(func);
            }
            else if (setup->wValue == 0)
            {
                _record_stop(func);
            }
            break;
        default:
            LOG_D("unknown uac request 0x%x", setup->bRequest);
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

static rt_err_t _function_enable(ufunction_t func)
{
    RT_ASSERT(func != RT_NULL);

    LOG_D("uac function enable");

    return RT_EOK;
}

static rt_err_t _function_disable(ufunction_t func)
{
    RT_ASSERT(func != RT_NULL);

    LOG_D("uac function disable");
    _record_stop(func);
    return RT_EOK;
}

static struct ufunction_ops ops =
{
    _function_enable,
    _function_disable,
    RT_NULL,
};
/**
 * This function will configure uac descriptor.
 *
 * @param comm the communication interface number.
 * @param data the data interface number.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _uac_descriptor_config(struct uac_ac_descriptor *ac,
                                       rt_uint8_t cintf_nr, struct uac_as_descriptor *as, rt_uint8_t sintf_nr)
{
    ac->hdr_desc.baInterfaceNr[0] = sintf_nr;
#ifdef RT_USB_DEVICE_COMPOSITE
    ac->iad_desc.bFirstInterface = cintf_nr;
#endif

    return RT_EOK;
}

static rt_err_t _uac_samplerate_config(struct uac_as_descriptor *as, rt_uint32_t samplerate)
{
    as->format_type_desc.tSamFreq[0][2] = samplerate >> 16 & 0xff;
    as->format_type_desc.tSamFreq[0][1] = samplerate >> 8 & 0xff;
    as->format_type_desc.tSamFreq[0][0] = samplerate & 0xff;
    return RT_EOK;
}

/**
 * This function will create a uac function instance.
 *
 * @param device the usb device object.
 *
 * @return RT_EOK on successful.
 */
ufunction_t rt_usbd_function_uac_mic_create(udevice_t device)
{
    ufunction_t func;
    uintf_t intf_ac, intf_as;
    ualtsetting_t setting_as0;
    ualtsetting_t setting_ac, setting_as;
    struct uac_as_descriptor *as_desc_t;

    /* parameter check */
    RT_ASSERT(device != RT_NULL);

    /* set usb device string description */
    rt_usbd_device_set_string(device, _ustring);

    /* create a uac function */
    func = rt_usbd_function_new(device, &dev_desc, &ops);
    //not support HS
    //rt_usbd_device_set_qualifier(device, &dev_qualifier);

    /* create interface */
    intf_ac = rt_usbd_interface_new(device, RT_NULL);
    intf_as = rt_usbd_interface_new(device, _interface_as_handler);

    /* create alternate setting */
    setting_ac = rt_usbd_altsetting_new(sizeof(struct uac_ac_descriptor));
    setting_as0 = rt_usbd_altsetting_new(sizeof(struct uinterface_descriptor));
    setting_as = rt_usbd_altsetting_new(sizeof(struct uac_as_descriptor));
    /* config desc in alternate setting */
    rt_usbd_altsetting_config_descriptor(setting_ac, &ac_desc,
                                         (rt_off_t) & ((struct uac_ac_descriptor *)0)->intf_desc);
    rt_usbd_altsetting_config_descriptor(setting_as0, &as_desc0, 0);
    rt_usbd_altsetting_config_descriptor(setting_as, &as_desc,
                                         (rt_off_t) & ((struct uac_as_descriptor *)0)->intf_desc);
    /* configure the uac interface descriptor */
    _uac_descriptor_config(setting_ac->desc, intf_ac->intf_num, setting_as->desc, intf_as->intf_num);
    _uac_samplerate_config(setting_as->desc, RECORD_SAMPLERATE);

    /* create endpoint */
    as_desc_t = (struct uac_as_descriptor *)setting_as->desc;
    mic.ep = rt_usbd_endpoint_new(&as_desc_t->ep_desc, _ep_data_in_handler);

    /* add the endpoint to the alternate setting */
    rt_usbd_altsetting_add_endpoint(setting_as, mic.ep);

    /* add the alternate setting to the interface, then set default setting of the interface */
    rt_usbd_interface_add_altsetting(intf_ac, setting_ac);
    rt_usbd_set_altsetting(intf_ac, 0);
    rt_usbd_interface_add_altsetting(intf_as, setting_as0);
    rt_usbd_interface_add_altsetting(intf_as, setting_as);
    rt_usbd_set_altsetting(intf_as, 0);

    /* add the interface to the uac function */
    rt_usbd_function_add_interface(func, intf_ac);
    rt_usbd_function_add_interface(func, intf_as);

    return func;
}

int audio_mic_init(void)
{
    rt_thread_t mic_tid;
    mic.event = rt_event_create("mic_event", RT_IPC_FLAG_FIFO);

    mic_tid = rt_thread_create("mic_thread",
                               mic_entry, RT_NULL,
                               1024,
                               5, 10);

    if (mic_tid != RT_NULL)
        rt_thread_startup(mic_tid);
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(audio_mic_init);

/*
 *  register uac class
 */
struct udclass uac_class =
{
    .rt_usbd_function_create = rt_usbd_function_uac_mic_create
};

int rt_usbd_uac_mic_class_register(void)
{
    rt_usbd_class_register(&uac_class);
    return 0;
}
INIT_PREV_EXPORT(rt_usbd_uac_mic_class_register);
