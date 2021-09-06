from building import *

cwd     = GetCurrentDir()
CPPPATH = [cwd]
src     = Glob('*.c')

group = DefineGroup('Modules', src, depend = [''], CPPPATH = CPPPATH)

Return('group')
