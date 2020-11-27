from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add ft6236 src files.
if GetDepend('PKG_USING_FT6236'):
    src += Glob('src/ft6236.c')

if GetDepend('PKG_USING_FT6236_SAMPLE'):
    src += Glob('examples/example_ft6236.c')

# add ft6236 include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('ft6236', src, depend = ['PKG_USING_FT6236'], CPPPATH = path)

Return('group')