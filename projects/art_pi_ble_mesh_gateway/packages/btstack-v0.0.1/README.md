# RT-Thread_BTstack

[中文页](README_zh.md) |

## 1. Introduce

BTstack is [BlueKitchen's](https://bluekitchen-gmbh.com) implementation of the official Bluetooth stack.
It is well suited for small, resource-constraint devices
such as 8 or 16 bit embedded systems as it is highly configurable and comes with an ultra small memory footprint.

Targeting a variety of platforms is as simple as providing the necessary UART, CPU, and CLOCK implementations. BTstack is currently capable of connecting to Bluetooth-modules via: (H2) HCI USB, (H4) HCI UART + TI's eHCILL, and (H5) HCI Three-Wire UART.

On smaller embedded systems, a minimal run loop implementation allows to use BTstack without a Real Time OS (RTOS).
If a RTOS is already provided, BTstack can be integrated and run as a single thread.

## 2. Supported Protocols and Profiles

**Protocols:** L2CAP (incl. LE Data Channels), RFCOMM, SDP, BNEP, AVDTP, AVCTP, ATT, SM (incl. LE Secure Connections).

**Profiles:** GAP, IOP, HFP, HSP, SPP, PAN, A2DP, AVRCP incl. Browsing, GATT.

**GATT Services:** Battery, Cycling Power, Cycling Speed and Cadence, Device Information, Heart Rate, HID over GATT (HOG), Mesh Provisioning, Mesh Proxy, Nordic SPP, u-Blox SPP. 

GATT Services are in general easy to implement and require short development time. For more GATT Services please contact us, or follow the [implementation guidelines](https://bluekitchen-gmbh.com/btstack/profiles/#gatt-generic-attribute-profile).  

It has been qualified with the Bluetooth SIG (QDID 110883) for GAP 1.1, IOP, HFP 1.7, HSP 1.2, SPP 1.2, PAN 1.0, A2DP 1.3, AVRCP 1.6 profiles and
GATT, SM of the Bluetooth Core 5.0 specification. For information on MFi/iAP2 support, please <a href="mailto:contact@bluekitchen-gmbh.com">contact us</a>.

### 3. License statement

This package contains a snapshot of the BTstack Bluetooth Stack by BlueKitchen GmbH.
It is free for non-commercial use. For commercial use, please contact BlueKitchen at contact@bluekitchen-gmbh.com。

## 4. Attention

​        On RT-thread system，only support AP6212 currently（base ART-H750 board）。We will support other control in future.

## 5. Contents

The Contents is similar to btstack except the flod of rtt_adapter.

```
BTstack
   ├───chipset                   /* Bluetooth control adapter */
   │   ├───bcm                   /* Bluetooth control AP6212(BCM) adapter */
   ├───docs                      /* Markdown about*/
   ├───example                   /* bt stack exampl about*/
   ├───platform
   │   ├───posix                 /* btstack posix api  */
   ├───port
   │   ├───posix-h4-bcm          /* btstack posix port  */
   ├───rtt_adapter               /* the different code about the example */
   └───src                       /* btstack source code */
```

## 6. Package connect

- Master：supperthomas(78900636@qq.com)
- github address：https://github.com/supperthomas/RTT_PACKAGE_BTSTACK

