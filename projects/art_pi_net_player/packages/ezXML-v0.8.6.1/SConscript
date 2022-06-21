from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c')
CPPPATH = [cwd]

group = DefineGroup('ezXML', src, depend = ['RT_USING_DFS', 'PKG_USING_EZXML'], CPPPATH = CPPPATH)

Return('group')
