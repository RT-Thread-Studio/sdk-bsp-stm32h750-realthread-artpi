import rtconfig

from building import *

src = Glob('*.c')
src += Glob('assets/*.c')

group = DefineGroup('LVGL-demo', src, depend = ['PKG_USING_LV_MUSIC_DEMO'])

Return('group')
