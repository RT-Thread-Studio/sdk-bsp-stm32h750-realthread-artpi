import os
from building import *
import rtconfig

cwd     = GetCurrentDir()

src     = []
CPPPATH = []

#
# atm
#
if GetDepend(['ATM_ENABLED']):
    src += Glob("iotkit-embedded/src/atm/at_api.c")
    if GetDepend(['AT_TCP_ENABLED']):
        src += Split("""
        iotkit-embedded/src/atm/at_conn_mbox.c
        iotkit-embedded/src/atm/at_conn_mgmt.c
        iotkit-embedded/src/atm/at_tcp.c
        """)
    if GetDepend(['AT_MQTT_ENABLED']):
        src += Glob("iotkit-embedded/src/atm/at_mqtt.c")
    if GetDepend(['AT_PARSER_ENABLED']):
        src += Glob("iotkit-embedded/src/atm/at_parser.c")

    CPPPATH += [cwd + '/iotkit-embedded/src/atm']
#### atm end ####

#
# infra
#
src += Glob('iotkit-embedded/src/infra/infra_defs.c')

if GetDepend(['INFRA_AES']):
    src += Glob('iotkit-embedded/src/infra/infra_aes.c')
if GetDepend(['INFRA_CJSON']):
    src += Glob('iotkit-embedded/src/infra/infra_cjson.c')
if GetDepend(['INFRA_COMPAT']):
    src += Glob('iotkit-embedded/src/infra/infra_compat.c')
if GetDepend(['INFRA_HTTPC']):
    src += Glob('iotkit-embedded/src/infra/infra_httpc.c')
if GetDepend(['INFRA_JSON_PARSER']):
    src += Glob('iotkit-embedded/src/infra/infra_json_parser.c')
if GetDepend(['INFRA_LOG']):
    src += Glob('iotkit-embedded/src/infra/infra_log.c')
if GetDepend(['INFRA_MD5']):
    src += Glob('iotkit-embedded/src/infra/infra_md5.c')
if GetDepend(['INFRA_MEM_STATS']):
    src += Glob('iotkit-embedded/src/infra/infra_mem_stats.c')
if GetDepend(['INFRA_NET']):
    src += Glob('iotkit-embedded/src/infra/infra_net.c')
if GetDepend(['INFRA_PREAUTH']):
    src += Glob('iotkit-embedded/src/infra/infra_preauth.c')
if GetDepend(['INFRA_LOG_NETWORK_PAYLOAD']):
    src += Glob('iotkit-embedded/src/infra/infra_prt_nwk_payload.c')
if GetDepend(['INFRA_REPORT']):
    src += Glob('iotkit-embedded/src/infra/infra_report.c')
if GetDepend(['INFRA_SHA1']):
    src += Glob('iotkit-embedded/src/infra/infra_sha1.c')
if GetDepend(['INFRA_SHA256']):
    src += Glob('iotkit-embedded/src/infra/infra_sha256.c')
if GetDepend(['INFRA_STRING']):
    src += Glob('iotkit-embedded/src/infra/infra_string.c')
if GetDepend(['INFRA_TIMER']):
    src += Glob('iotkit-embedded/src/infra/infra_timer.c')

CPPPATH += [cwd + '/iotkit-embedded/src/infra']
#### infra end ####

#
# mqtt
#
if GetDepend(['MQTT_COMM_ENABLED']):
    src += Glob('iotkit-embedded/src/mqtt/*.c')
    if GetDepend(['MQTT_DEFAULT_IMPL']):
        src += Glob('iotkit-embedded/src/mqtt/impl/*.c')
        CPPPATH += [cwd + '/iotkit-embedded/src/mqtt/impl']

    CPPPATH += [cwd + '/iotkit-embedded/src/mqtt']
#### mqtt end ####

#
# coap
#
if GetDepend(['COAP_COMM_ENABLED']):
    if GetDepend(['COAP_CLIENT']):
        src += Glob('iotkit-embedded/src/coap/client/*.c')
        CPPPATH += [cwd + '/iotkit-embedded/src/coap/client']
    if GetDepend(['COAP_PACKET']):
        src += Glob('iotkit-embedded/src/coap/CoAPPacket/*.c')
        CPPPATH += [cwd + '/iotkit-embedded/src/coap/CoAPPacket']
    if GetDepend(['COAP_SERVER']):
        src += Glob('iotkit-embedded/src/coap/server/*.c')
        CPPPATH += [cwd + '/iotkit-embedded/src/coap/server']
    CPPPATH += [cwd + '/iotkit-embedded/src/coap']
#### coap end ####


#
# device bind
#
if GetDepend(['DEV_BIND_ENABLED']):
    src += Glob("iotkit-embedded/src/dev_bind/impl/*.c")
    src += Glob("iotkit-embedded/src/dev_bind/impl/awss_reset/*.c")
    src += Glob("iotkit-embedded/src/dev_bind/impl/os/*.c")
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_bind']
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_bind/awss_reset']
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_bind/os']
#### device bind ####


#
# device model
#
if GetDepend(['DEVICE_MODEL_ENABLED']):
    src += Split("""
    iotkit-embedded/src/dev_model/client/dm_client.c
    iotkit-embedded/src/dev_model/client/dm_client_adapter.c
    iotkit-embedded/src/dev_model/dm_api.c
    iotkit-embedded/src/dev_model/dm_cota.c
    iotkit-embedded/src/dev_model/dm_fota.c
    iotkit-embedded/src/dev_model/dm_ipc.c
    iotkit-embedded/src/dev_model/dm_manager.c
    iotkit-embedded/src/dev_model/dm_message_cache.c
    iotkit-embedded/src/dev_model/dm_message.c
    iotkit-embedded/src/dev_model/dm_opt.c
    iotkit-embedded/src/dev_model/dm_ota.c
    iotkit-embedded/src/dev_model/dm_msg_process.c
    iotkit-embedded/src/dev_model/dm_utils.c
    iotkit-embedded/src/dev_model/iotx_cm_mqtt.c
    iotkit-embedded/src/dev_model/impl_linkkit.c
    iotkit-embedded/src/dev_model/iotx_cm.c
    """)

    if GetDepend(['LOG_REPORT_TO_CLOUD']):
        src += Split("""
        iotkit-embedded/src/dev_model/dm_log_report.c
        """)

    # ALCS(alink local communication service) is a communication between phone and device
    if GetDepend(['ALCS_ENABLED']):
        src += Glob("iotkit-embedded/src/dev_model/alcs/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/dev_model/alcs']

    if GetDepend(['COAP_COMM_ENABLED']):
        src += Glob("iotkit-embedded/src/dev_model/iotx_cm_coap.c")

    CPPPATH += [cwd + '/iotkit-embedded/src/dev_model']
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_model/client']
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_model/server']
#### device model end ####

#
# device sign
#
if GetDepend(['DEV_SIGN']):
    src += Glob('iotkit-embedded/src/dev_sign/dev_sign_mqtt.c')
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_sign']
#### device sign end ####

#
# device reset
#
if GetDepend(['DEV_RESET']):
    src += Glob('iotkit-embedded/src/dev_reset/dev_reset.c')
    CPPPATH += [cwd + '/iotkit-embedded/src/dev_reset']
#### device reset end ####

#
# dynamic register
#
if GetDepend(['DYNAMIC_REGISTER']):
    src += Glob("iotkit-embedded/src/dynamic_register/*.c")
    CPPPATH += [cwd + '/iotkit-embedded/src/dynamic_register']
#### dynamic register end ####

#
# http
#
if GetDepend(['HTTP_COMM_ENABLED']):
    src += Glob("iotkit-embedded/src/http/*.c")
    CPPPATH += [cwd + '/iotkit-embedded/src/http']
#### http end ####

#
# http2
#
if GetDepend(['HTTP2_COMM_ENABLED']):
    src += Glob("iotkit-embedded/src/http2/http2_api.c")
    src += Glob("iotkit-embedded/src/http2/iotx_http2.c")
    
    if GetDepend(['FS_ENABLED']):
        src += Glob("iotkit-embedded/src/http2/http2_upload_api.c")
    CPPPATH += [cwd + '/iotkit-embedded/src/http2']
#### http end ####

#
# ota
#
if GetDepend(['OTA_ENABLED']):
    src += Split("""
    iotkit-embedded/src/ota/iotx_ota.c
    iotkit-embedded/src/ota/ota_fetch.c
    iotkit-embedded/src/ota/ota_lib.c
    """)
    CPPPATH += [cwd + '/iotkit-embedded/src/ota']
#### ota end ####

#
# wifi provision
#
if GetDepend(['WIFI_PROVISION_ENABLED']):
    if GetDepend(['AWSS_SUPPORT_SMARTCONFIG']):
        src += Glob("iotkit-embedded/src/wifi_provision/smartconfig/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/smartconfig']
    if GetDepend(['AWSS_SUPPORT_SMARTCONFIG_WPS']):
        src += Glob("iotkit-embedded/src/wifi_provision/p2p/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/p2p']

    if GetDepend(['AWSS_SUPPORT_ZEROCONFIG']):
        src += Glob("iotkit-embedded/src/wifi_provision/zero_config/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/zero_config']

    if GetDepend(['AWSS_SUPPORT_AHA']):
        src += Glob("iotkit-embedded/src/wifi_provision/phone_ap/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/phone_ap']
    if GetDepend(['AWSS_SUPPORT_ADHA']):
        src += Glob("iotkit-embedded/src/wifi_provision/router_ap/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/router_ap']

    if GetDepend(['AWSS_SUPPORT_DEV_AP']):
        src += Glob("iotkit-embedded/src/wifi_provision/dev_ap/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/dev_ap']

    if GetDepend(['AWSS_FRAMEWORKS']):
        src += Glob("iotkit-embedded/src/wifi_provision/frameworks/*.c")
        src += Glob("iotkit-embedded/src/wifi_provision/frameworks/*/*.c")
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/frameworks']
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/frameworks/aplist']
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/frameworks/ieee80211']
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/frameworks/statics']
        CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision/frameworks/utils']

    CPPPATH += [cwd + '/iotkit-embedded/src/wifi_provision']
#### wifi provision end ####

#
# wrappers | port
#
src += Split("""
ports/rtthread/HAL_OS_rtthread.c
ports/rtthread/HAL_TCP_rtthread.c
ports/rtthread/HAL_UDP_rtthread.c
ports/wrapper.c
""")

if GetDepend(['SUPPORT_TLS']) or GetDepend(['COAP_DTLS_SUPPORT']):
    src += Glob('iotkit-embedded/certs/root_ca.c')
if GetDepend(['SUPPORT_TLS']):
    src += Glob('ports/tls/mbedtls/HAL_TLS_mbedtls.c')
if GetDepend(['COAP_DTLS_SUPPORT']):
    src += Glob('ports/tls/mbedtls/HAL_DTLS_mbedtls.c')
    
if GetDepend(['HAL_CRYPTO']):
    src += Glob('ports/rtthread/HAL_Crypt_rtthread.c')

CPPPATH += [cwd + '/iotkit-embedded/wrappers']
#### wrappers | port end ####

#
# samples
#
if GetDepend(['PKG_USING_ALI_IOTKIT_DEV_MODEL_SAMPLE']):
    src += Split("""
    samples/dev_model/cJSON.c
    samples/dev_model/linkkit_example_solo.c
    """)
    CPPPATH += [cwd + '/iotkit-embedded/samples/dev_model']

if GetDepend(['PKG_USING_ALI_IOTKIT_MQTT_SAMPLE']):
    src += Split("""
    samples/mqtt/mqtt-example.c
    """)

if GetDepend(['PKG_USING_ALI_IOTKIT_COAP_SAMPLE']):
    src += Split("""
    samples/coap/coap_example.c
    """)

if GetDepend(['PKG_USING_ALI_IOTKIT_OTA_SAMPLE']):
    src += Split("""
    samples/ota/ota_mqtt-example.c
    """)
#### samples end ####

group = DefineGroup('ali-iotkit', src, depend = ['PKG_USING_ALI_IOTKIT'], CPPPATH = CPPPATH)

Return('group')
