from building import *

cwd = GetCurrentDir()
path = [cwd+'/inc']
src  = Glob('src/*.c')
 
group = DefineGroup('crclib', src, depend = ['PKG_USING_CRCLIB'], CPPPATH = path)

Return('group')