from building import *

cwd = GetCurrentDir()
path = [cwd+'/inc']
src  = Glob('src/*.c')
 
group = DefineGroup('qboot', src, depend = ['PKG_USING_QBOOT'], CPPPATH = path)

Return('group')