from building import * 

# get current dir path
cwd = GetCurrentDir()

# init src and inc vars
src = []
inc = [cwd + '/inc']

# add ld3360 source code
src = src + ['src/ld3320.c']
src = src + ['src/ld3320_base.c']

if GetDepend('PKG_USING_LD3320_FINSH'):
    src = src + ['src/ld3320_finsh.c']

# add Signal Led test code
if GetDepend('PKG_LD3320_DEMO_ROLE_MP3'):
    src = src + ['demo/ld3320_mp3_demo.c']

if GetDepend('PKG_LD3320_DEMO_ROLE_ASR'):
    src = src + ['demo/ld3320_asr_demo.c']

# add group to IDE project
group = DefineGroup('ld3320', src, depend = ['PKG_USING_LD3320'], CPPPATH = inc)

Return('group')
