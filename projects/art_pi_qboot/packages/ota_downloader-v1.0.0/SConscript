from building import *

cwd = GetCurrentDir()

CPPPATH = [cwd]

src = Split('''

''')

if GetDepend(['PKG_USING_HTTP_OTA']):
    src += Glob('src/http_ota.c')

if GetDepend(['PKG_USING_YMODEM_OTA']):
    src += Glob('src/ymodem_ota.c')

group = DefineGroup('ota_downloader', src, depend = ['PKG_USING_OTA_DOWNLOADER'], CPPPATH = CPPPATH)

Return('group')
