Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd   = GetCurrentDir()
include_path = [cwd+'/multi_rtimer']

src = Split('''
multi_rtimer/multi_rtimer.c
''')

if GetDepend(['MULTI_RTIMER_USING_RTC_SYSTIME_SERVICE']):
    src += ['multi_rtimer/rtc_systime_service.c']

if GetDepend(['MULTI_RTIMER_USING_TRAGET_MCU_STM32_RTC']):
    src += ['ports/stm32_rtc_adapter/hw_rtc_stm32.c']
    include_path += ['ports/stm32_rtc_adapter']

if GetDepend(['MULTI_RTIMER_USING_TEST_SAMPLE']):
    src += ['samples/multi_rtimer_test_sample.c']

group = DefineGroup('multi_rtimer', src, depend = ['PKG_USING_MULTI_RTIMER'], CPPPATH = include_path)

Return('group')