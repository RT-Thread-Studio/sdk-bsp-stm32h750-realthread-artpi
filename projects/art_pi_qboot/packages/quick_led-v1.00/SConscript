from building import *

cwd = GetCurrentDir()
path = [cwd+'/inc']
src  = Glob('src/*.c')
 
group = DefineGroup('quick_led', src, depend = ['PKG_USING_QLED'], CPPPATH = path)

Return('group')