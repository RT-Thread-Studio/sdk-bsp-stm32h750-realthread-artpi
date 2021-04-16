Import('RTT_ROOT')
Import('rtconfig')
from building import *

src = Glob('*.c')
src += Glob('./real/*.c')

SrcRemove(src, ['./real/polyphase.c'])

if rtconfig.CROSS_TOOL == 'gcc':
	src += ['./real/arm/asmmisc_gcc.s', './real/arm/asmpoly_thumb2_gcc.s']
elif rtconfig.CROSS_TOOL == 'keil':
	src += ['./real/arm/asmmisc_rvds.s', './real/arm/asmpoly_thumb2_rvds.s']
elif rtconfig.CROSS_TOOL == 'iar':
    src += ['./real/arm/asmmisc_iar.s', './real/arm/asmpoly_thumb2_iar.s']


cwd = GetCurrentDir()

CPPPATH = [cwd + '/pub', cwd + '/real']

group = DefineGroup('Helix', src, depend = [''], CPPPATH = CPPPATH)

Return('group')
