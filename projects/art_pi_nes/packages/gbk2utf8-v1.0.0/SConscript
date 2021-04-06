from building import *

group = []
if not GetDepend(['PKG_USING_GBK2UTF8']):
    Return('group')

cwd = GetCurrentDir()
CPPPATH = [cwd + '/inc']
src	= Glob('src/*.c')

group = DefineGroup('GBK2UTF8', src, depend = ['PKG_USING_GBK2UTF8'], CPPPATH = CPPPATH)

Return('group')