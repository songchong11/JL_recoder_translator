#include "os/os_api.h"
#include "usb/device/usb_stack.h"
#include "usb/device/hid.h"
#include "usb_config.h"

#include "app_config.h"

#ifdef TCFG_USB_SLAVE_USER_HID
#undef TCFG_USB_SLAVE_HID_ENABLE
#define TCFG_USB_SLAVE_HID_ENABLE           0
#endif

#if TCFG_USB_SLAVE_HID_ENABLE

#define LOG_TAG_CONST       USB
#define LOG_TAG             "[USB]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"
static const u8 sHIDDescriptor[] = {
//HID
    //InterfaceDeszcriptor:
    USB_DT_INTERFACE_SIZE,     // Length
    USB_DT_INTERFACE,          // DescriptorType
    /* 0x04,                      // bInterface number */
    0x00,                       // bInterface number
    0x00,                      // AlternateSetting
    0x01,                      // NumEndpoint
    /* 0x02,                        // NumEndpoint */
    USB_CLASS_HID,             // Class = Human Interface Device
    0x00,                      // Subclass, 0 No subclass, 1 Boot Interface subclass
    0x00,                      // Procotol, 0 None, 1 Keyboard, 2 Mouse
    0x00,                      // Interface Name

    //HIDDescriptor:
    0x09,                      // bLength
    USB_HID_DT_HID,            // bDescriptorType, HID Descriptor
    0x00, 0x01,                // bcdHID, HID Class Specification release NO.
    0x00,                      // bCuntryCode, Country localization (=none)
    0x01,                       // bNumDescriptors, Number of descriptors to follow
    0x22,                       // bDescriptorType, Report Desc. 0x22, Physical Desc. 0x23
    0,//LOW(ReportLength)
    0, //HIGH(ReportLength)

    //EndpointDescriptor:
    USB_DT_ENDPOINT_SIZE,       // bLength
    USB_DT_ENDPOINT,            // bDescriptorType, Type
    USB_DIR_IN | HID_EP_IN,     // bEndpointAddress
    USB_ENDPOINT_XFER_INT,      // Interrupt
    LOBYTE(MAXP_SIZE_HIDIN), HIBYTE(MAXP_SIZE_HIDIN),// Maximum packet size
    10,     // Poll every 10msec seconds

//@Endpoint Descriptor:
    /* USB_DT_ENDPOINT_SIZE,       // bLength
    USB_DT_ENDPOINT,            // bDescriptorType, Type
    USB_DIR_OUT | HID_EP_OUT,   // bEndpointAddress
    USB_ENDPOINT_XFER_INT,      // Interrupt
    LOBYTE(MAXP_SIZE_HIDOUT), HIBYTE(MAXP_SIZE_HIDOUT),// Maximum packet size
    0x01,                       // bInterval, for high speed 2^(n-1) * 125us, for full/low speed n * 1ms */
};

static const u8 sHIDReportDesc[] = {
    USAGE_PAGE(1, CONSUMER_PAGE),
    USAGE(1, CONSUMER_CONTROL),
    COLLECTION(1, APPLICATION),

    LOGICAL_MIN(1, 0x00),
    LOGICAL_MAX(1, 0x01),

    USAGE(1, VOLUME_INC),
    USAGE(1, VOLUME_DEC),
    USAGE(1, MUTE),
    USAGE(1, PLAY_PAUSE),
    USAGE(1, SCAN_NEXT_TRACK),
    USAGE(1, SCAN_PREV_TRACK),
    USAGE(1, FAST_FORWARD),
    USAGE(1, STOP),

    USAGE(1, TRACKING_INC),
    USAGE(1, TRACKING_DEC),
    USAGE(1, STOP_EJECT),
    USAGE(1, VOLUME),
    USAGE(2, BALANCE_LEFT),
    USAGE(2, BALANCE_RIGHT),
    USAGE(1, PLAY),
    USAGE(1, PAUSE),

	REPORT_ID(1, 0x01),
    REPORT_SIZE(1, 0x01),
    REPORT_COUNT(1, 0x10),
    INPUT(1, 0x42),
    END_COLLECTION,

	0x05, 0x01,    //global,  USAGE_PAGE 1 (Generic Desktop)
    0x09, 0x00,    //usage undefined
    0xa1, 0x01,    //main collection
    0x85, 0x05,    //global report ID 0x5
    0x06, 0x00, 0xff, //global usage page
    0x09, 0x01,    //local,  usage ID 01  Consumer Control
    0x15, 0x81,    //global min 81
    0x25, 0x7f,    //global, max 7f
    0x75, 0x08,    //global, report size 8
    0x95, 0x0a,    //report count 10
    0x81, 0x02,    //feature (data, var, abs)
    0xc0,         //main, end collection
};

static u32 get_hid_report_desc_len(u32 index)
{
    u32 len = 0;
    len = sizeof(sHIDReportDesc);
    return len;
}
static void *get_hid_report_desc(u32 index)
{
    u8 *ptr  = NULL;
    ptr = (u8 *)sHIDReportDesc;
    return ptr;
}


static u32 hid_tx_data(struct usb_device_t *usb_device, const u8 *buffer, u32 len)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    return usb_g_intr_write(usb_id, HID_EP_IN, buffer, len);
}
static void hid_rx_data(struct usb_device_t *usb_device, u32 ep)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    u8 *ep_buffer = usb_get_ep_buffer(usb_id, HID_EP_OUT);
    u32 rx_len = usb_g_intr_read(usb_id, ep, NULL, 64, 0);
    hid_tx_data(usb_device, ep_buffer, rx_len);
}

static void hid_endpoint_init(struct usb_device_t *usb_device, u32 itf)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    u8 *ep_buffer = usb_get_ep_buffer(usb_id, HID_EP_IN | USB_DIR_IN);
    usb_g_ep_config(usb_id, HID_EP_IN | USB_DIR_IN, USB_ENDPOINT_XFER_INT, 0, ep_buffer, MAXP_SIZE_HIDIN);

    ep_buffer = usb_get_ep_buffer(usb_id, HID_EP_OUT);
    usb_g_set_intr_hander(usb_id, HID_EP_OUT, hid_rx_data);
    usb_g_ep_config(usb_id, HID_EP_OUT, USB_ENDPOINT_XFER_INT, 1, ep_buffer, MAXP_SIZE_HIDOUT);
    usb_enable_ep(usb_id, HID_EP_OUT);
}

static void hid_reset(struct usb_device_t *usb_device, u32 itf)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    log_debug("%s", __func__);
#if USB_ROOT2
    usb_disable_ep(usb_id, HID_EP_OUT);
#else
    hid_endpoint_init(usb_device, itf);
#endif
}
static u32 hid_recv_output_report(struct usb_device_t *usb_device, struct usb_ctrlrequest *setup)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    u32 ret = 0;
    u8 read_ep[8];
    u8 mute;
    u16 volume = 0;
    usb_read_ep0(usb_id, read_ep, MIN(sizeof(read_ep), setup->wLength));
    ret = USB_EP0_STAGE_SETUP;
    put_buf(read_ep, 8);


    return ret;
}

static u32 hid_itf_hander(struct usb_device_t *usb_device, struct usb_ctrlrequest *req)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    u32 tx_len;
    u8 *tx_payload = usb_get_setup_buffer(usb_device);
    u32 bRequestType = req->bRequestType & USB_TYPE_MASK;
    switch (bRequestType) {
    case USB_TYPE_STANDARD:
        switch (req->bRequest) {
        case USB_REQ_GET_DESCRIPTOR:
            switch (HIBYTE(req->wValue)) {
            case USB_HID_DT_HID:
                tx_payload = (u8 *)sHIDDescriptor + USB_DT_INTERFACE_SIZE;
                tx_len = 9;
                tx_payload = usb_set_data_payload(usb_device, req, tx_payload, tx_len);
                tx_payload[7] = LOBYTE(get_hid_report_desc_len(req->wIndex));
                tx_payload[8] = HIBYTE(get_hid_report_desc_len(req->wIndex));
                break;
            case USB_HID_DT_REPORT:
                hid_endpoint_init(usb_device, req->wIndex);
                tx_len = get_hid_report_desc_len(req->wIndex);
                tx_payload = get_hid_report_desc(req->wIndex);
                usb_set_data_payload(usb_device, req, tx_payload, tx_len);
                break;
            }// USB_REQ_GET_DESCRIPTOR
            break;
        case USB_REQ_SET_DESCRIPTOR:
            usb_set_setup_phase(usb_device, USB_EP0_STAGE_SETUP);
            break;
        case USB_REQ_SET_INTERFACE:
            if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_CONFIGURED) {
                //只有一个interface 没有Alternate
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            }
            break;
        case USB_REQ_GET_INTERFACE:
            if (req->wLength) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_CONFIGURED) {
                tx_len = 1;
                tx_payload[0] = 0x00;
                usb_set_data_payload(usb_device, req, tx_payload, tx_len);
            }
            break;
        case USB_REQ_GET_STATUS:
            if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            }
            break;
        }//bRequest @ USB_TYPE_STANDARD
        break;

    case USB_TYPE_CLASS: {
        switch (req->bRequest) {
        case USB_REQ_SET_IDLE:
            usb_set_setup_phase(usb_device, USB_EP0_STAGE_SETUP);
            break;
        case USB_REQ_GET_IDLE:
            tx_len = 1;
            tx_payload[0] = 0;
            usb_set_data_payload(usb_device, req, tx_payload, tx_len);
            break;
        case USB_REQ_SET_REPORT:
            usb_set_setup_recv(usb_device, hid_recv_output_report);
            break;
        }//bRequest @ USB_TYPE_CLASS
    }
    break;
    }
    return 0;
}

u32 hid_desc_config(const usb_dev usb_id, u8 *ptr, u32 *cur_itf_num)
{
    log_debug("hid interface num:%d\n", *cur_itf_num);
    u8 *_ptr = ptr;
    memcpy(ptr, sHIDDescriptor, sizeof(sHIDDescriptor));
    ptr[2] = *cur_itf_num;
    if (usb_set_interface_hander(usb_id, *cur_itf_num, hid_itf_hander) != *cur_itf_num) {
        ASSERT(0, "hid set interface_hander fail");
    }
    if (usb_set_reset_hander(usb_id, *cur_itf_num, hid_reset) != *cur_itf_num) {
        ASSERT(0, "hid set interface_reset_hander fail");
    }

    ptr[USB_DT_INTERFACE_SIZE + 7] = LOBYTE(get_hid_report_desc_len(0));
    ptr[USB_DT_INTERFACE_SIZE + 8] = HIBYTE(get_hid_report_desc_len(0));
    *cur_itf_num = *cur_itf_num + 1;
    return sizeof(sHIDDescriptor) ;
}

#if 0
void hid_key_handler(struct usb_device_t *usb_device, u32 hid_key)
{
    const usb_dev usb_id = usb_device2id(usb_device);

    u16 key_buf = hid_key;
    hid_tx_data(usb_device, (const u8 *)&key_buf, 2);
    os_time_dly(2);
    key_buf = 0;
    hid_tx_data(usb_device, (const u8 *)&key_buf, 2);
}
#endif

void hid_key_handler(struct usb_device_t *usb_device, u32 hid_key)
{
    const usb_dev usb_id = usb_device2id(usb_device);

    //u16 key_buf = hid_key;
    u8 key_buf[3];

	key_buf[0] = 0x01;
	key_buf[1] = hid_key & 0xff;
	key_buf[2] = (hid_key << 8) & 0xff;
    hid_tx_data(usb_device, (const u8 *)key_buf, sizeof(key_buf));
    os_time_dly(2);
	key_buf[1] = 0;
	key_buf[2] = 0;

    hid_tx_data(usb_device, (const u8 *)key_buf, sizeof(key_buf));
}

#define	VENDOR_IN_REPORT_ID		0x05
void hid_vender_in_handler(struct usb_device_t *usb_device, u32 hid_key)
{
    const usb_dev usb_id = usb_device2id(usb_device);

    //u16 key_buf = hid_key;
    u8 key_buf[10];

	key_buf[0] = VENDOR_IN_REPORT_ID;
	key_buf[1] = 0x0a;
	key_buf[2] = 0x0b;
	key_buf[3] = 3;
	key_buf[4] = 4;
	key_buf[5] = 5;
	key_buf[6] = 6;
	key_buf[7] = 7;
	key_buf[8] = 8;
	key_buf[9] = 9;
	
    hid_tx_data(usb_device, (const u8 *)key_buf, sizeof(key_buf));
    os_time_dly(2);
}


struct hid_button {
    u8 report_id;
    u8 button1: 1;
    u8 button2: 1;
    u8 button3: 1;
    u8 no_button: 5;
    u8 X_axis;
    u8 Y_axis;
};
struct hid_button hid_key;
void hid_test(struct usb_device_t *usb_device)
{
    static u32 tx_count = 0;

    hid_key_handler(usb_device, BIT(tx_count));
    tx_count ++;
    if (BIT(tx_count) > USB_AUDIO_PAUSE) {
        tx_count = 0;
    }
}
#else
void hid_key_handler(struct usb_device_t *usb_device, u32 hid_key)
{

}
#endif
