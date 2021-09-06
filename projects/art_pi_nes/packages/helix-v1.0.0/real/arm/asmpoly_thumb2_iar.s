    SECTION .text:CODE:NOROOT(2)
    THUMB
	REQUIRE8
	PRESERVE8

#define PCM    r0
#define VB1    r1
#define COEF   r2

#define VLO    r0       
#define VHI    r3       

#define SUM1LL r4
#define SUM1LH r5
#define SUM2LL r6
#define SUM2LH r7
#define SUM1RL r8
#define SUM1RH r9
#define SUM2RL r10
#define SUM2RH r11

#define CF1    r12
#define CF2    r14        

#define SIGN   r12 
#define MAXPOS r14 

#define I      r12    

	PUBLIC RNDVAL
RNDVAL    EQU    (1 << ((32 - 12) + (6 - 1)))

    ; C64TOS - clip 64-bit accumulator to short (no rounding)
    ;   xl, xh = value (lo 32, hi 32)
    ;   input assumed to have 6 fraction bits
    ;   sign = temp variable to use for sign
    ;   maxPos = 0x00007fff (takes 2 instr. to generate - calculating
    ;              once and using repeatedly saves if you do several CTOS in a row) */
    MACRO C64TOS    xl, xh, sign,maxPos
	
    mov        \0, \0, lsr #(20+6)
    orr        \0, \0, \1, lsl #(12-6)
    mov        \2, \0, ASR #31
    cmp        \2, \0, ASR #15
    it ne
    eorne      \0, \2, \3   
    ENDM ; C64TOS

    ; MC0S - process 2 taps, 1 sample per channel (sample 0) */
    ;   x = vb1 offset */
    MACRO MC0S x

    ldr        CF1, [COEF], #4
    ldr        CF2, [COEF], #4
    ldr        VLO, [VB1, #(4*(\0))]
    ldr        VHI, [VB1, #(4*(23 - \0))]

    smlal    SUM1LL, SUM1LH, VLO, CF1
    ldr        VLO, [VB1, #(4*(32 + \0))]
    rsb        CF2, CF2, #0
    smlal    SUM1LL, SUM1LH, VHI, CF2
    ldr        VHI, [VB1, #(4*(32 + 23 - \0))]
    
    smlal    SUM1RL, SUM1RH, VLO, CF1
    smlal    SUM1RL, SUM1RH, VHI, CF2

	ENDM ; MCOS

    ; MC1S - process 2 taps, 1 sample per channel (sample 16)
    ;   x = vb1 offset
    MACRO MC1S    x

    ldr        CF1, [COEF], #4
    ldr        VLO, [VB1, #(4*(\0))]
    ldr        VHI, [VB1, #(4*(32 + \0))]
    smlal    SUM1LL, SUM1LH, VLO, CF1
    smlal    SUM1RL, SUM1RH, VHI, CF1
    
	ENDM ; MC1S

    ; MC2S - process 2 taps, 2 samples per channel
    ;   x = vb1 offset
    MACRO MC2S    x

    ; load data as far as possible in advance of using it
    ldr      CF1, [COEF], #4
    ldr      CF2, [COEF], #4
    ldr      VLO, [VB1, #(4*(\0))]
    ldr      VHI, [VB1, #(4*(23 - \0))]

    smlal    SUM1LL, SUM1LH, VLO, CF1
    smlal    SUM2LL, SUM2LH, VLO, CF2
    rsb      CF2, CF2, #0
    smlal    SUM2LL, SUM2LH, VHI, CF1
    smlal    SUM1LL, SUM1LH, VHI, CF2

    ldr      VHI, [VB1, #(4*(32 + 23 - \0))]
    ldr      VLO, [VB1, #(4*(32 + \0))]

    smlal    SUM1RL, SUM1RH, VHI, CF2
    smlal    SUM2RL, SUM2RH, VHI, CF1
    rsb      CF2, CF2, #0
    smlal    SUM1RL, SUM1RH, VLO, CF1
    smlal    SUM2RL, SUM2RH, VLO, CF2
    
	ENDM ; MC2S


; void PolyphaseStereo(short *pcm, int *vbuf, const int *coefBase)
xmp3_PolyphaseStereo
	EXPORT xmp3_PolyphaseStereo

    stmfd    sp!, {r4-r11, r14}
    
	; clear out stack space for 2 local variables (4 bytes each)
    sub        sp, sp, #8
    str        PCM , [sp, #4]                ; sp[1] = pcm pointer

    ; special case, output sample 0
    mov        SUM1LL, #RNDVAL               ; load rndVal (low 32)
    mov        SUM1RL, #RNDVAL               ; load rndVal (low 32)
    mov        SUM1LH, #0
    mov        SUM1RH, #0

    MC0S    0
    MC0S    1
    MC0S    2
    MC0S    3
    MC0S    4
    MC0S    5
    MC0S    6
    MC0S    7

    ldr        PCM, [sp, #4]         ; load pcm pointer
    mov        MAXPOS, #0x7f00
    orr        MAXPOS, MAXPOS, #0xff
    
    C64TOS    SUM1LL, SUM1LH, SIGN, MAXPOS
    C64TOS    SUM1RL, SUM1RH, SIGN, MAXPOS

    strh    SUM1LL, [PCM, #(2*0)]
    strh    SUM1RL, [PCM, #(2*1)]

	; special case, output sample 16
    add        COEF, COEF, #(4*(256-16))    ; coef = coefBase + 256 (was coefBase + 16 after MC0S block)
    add        VB1, VB1, #(4*1024)          ; vb1 = vbuf + 64*16
    
    mov        SUM1LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM1RL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM1LH, #0
    mov        SUM1RH, #0

    MC1S    0
    MC1S    1
    MC1S    2
    MC1S    3
    MC1S    4
    MC1S    5
    MC1S    6
    MC1S    7

    ldr     PCM, [sp, #4]        ; load pcm pointer
    mov     MAXPOS, #0x7f00
    orr     MAXPOS, MAXPOS, #0xff
    
    C64TOS  SUM1LL, SUM1LH, SIGN, MAXPOS
    C64TOS  SUM1RL, SUM1RH, SIGN, MAXPOS

    strh    SUM1LL, [PCM, #(2*(2*16+0))]
    strh    SUM1RL, [PCM, #(2*(2*16+1))]

    ; main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17
    sub        COEF, COEF, #(4*(264-16))    ; coef = coefBase + 16 (was coefBase + 264 after MC1S block)
    sub        VB1, VB1, #(4*(1024-64))     ; vb1 = vbuf + 64 (was vbuf + 64*16 after MC1S block)
    mov        I, #15                       ; loop counter, count down
    add        PCM, PCM, #(2*2)             ; pcm+=2
    
LoopPS
    str        I, [sp, #0]                  ; sp[0] = i (loop counter)
    str        PCM, [sp, #4]                ; sp[1] = pcm (pointer to pcm buffer)
    
    mov        SUM1LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM1RL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM2LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM2RL, #RNDVAL              ; load rndVal (low 32)
    
    mov        SUM1LH, #0
    mov        SUM1RH, #0
    mov        SUM2LH, #0
    mov        SUM2RH, #0

    MC2S    0
    MC2S    1
    MC2S    2
    MC2S    3
    MC2S    4
    MC2S    5
    MC2S    6
    MC2S    7
    
    add        VB1, VB1, #(4*64)    ; vb1 += 64
    
    ldr        PCM, [sp, #4]        ; load pcm pointer
    mov        MAXPOS, #0x7f00
    orr        MAXPOS, MAXPOS, #0xff
    
    C64TOS    SUM1LL, SUM1LH, SIGN, MAXPOS
    C64TOS    SUM1RL, SUM1RH, SIGN, MAXPOS
    C64TOS    SUM2LL, SUM2LH, SIGN, MAXPOS
    C64TOS    SUM2RL, SUM2RH, SIGN, MAXPOS
    
    ldr        I, [sp, #0]            ; load loop counter
    add        CF2, PCM, I, lsl #3    ; CF2 = PCM + 4*i (short offset)
    strh    SUM2LL, [CF2], #2    	  ; *(pcm + 2*2*i + 0)
    strh    SUM2RL, [CF2], #2         ; *(pcm + 2*2*i + 1)

    strh    SUM1LL, [PCM], #2         ; *(pcm + 0)
    strh    SUM1RL, [PCM], #2         ; *(pcm + 1)
    
    subs    I, I, #1
    bne     LoopPS

    ; restore stack pointer
    add        sp, sp, #8

    ldmfd    sp!, {r4-r11, pc}
	
	

; MONOESSING

    ; MC0M - process 2 taps, 1 sample (sample 0)
    ;   x = vb1 offset
    MACRO MC0M    x

    ldr      CF1, [COEF], #4
    ldr      CF2, [COEF], #4
    ldr      VLO, [VB1, #(4*(\0))]
    ldr      VHI, [VB1, #(4*(23 - \0))]

    rsb      CF2, CF2, #0
    smlal    SUM1LL, SUM1LH, VLO, CF1
    smlal    SUM1LL, SUM1LH, VHI, CF2

    ENDM    ; MC0M

	; MC1M - process 2 taps, 1 sample (sample 16)
    ;   x = vb1 offset
    MACRO MC1M    x

    ldr        CF1, [COEF], #4
    ldr        VLO, [VB1, #(4*(\0))]
    smlal    SUM1LL, SUM1LH, VLO, CF1
    
    ENDM    ; MC1M

    ; MC2M - process 2 taps, 2 samples
    ;   x = vb1 offset
	MACRO MC2M    x

    ; load data as far as possible in advance of using it
    ldr        CF1, [COEF], #4
    ldr        CF2, [COEF], #4
    ldr        VLO, [VB1, #(4*(\0))]
    ldr        VHI, [VB1, #(4*(23 - \0))]

    smlal    SUM1LL, SUM1LH, VLO, CF1
    smlal    SUM2LL, SUM2LH, VLO, CF2
    rsb        CF2, CF2, #0
    smlal    SUM1LL, SUM1LH, VHI, CF2
    smlal    SUM2LL, SUM2LH, VHI, CF1

    ENDM    ; MC2M

; void PolyphaseMono(short *pcm, int *vbuf, const int *coefBase)

xmp3_PolyphaseMono
	EXPORT xmp3_PolyphaseMono

    stmfd    sp!, {r4-r11, r14}
    
    ; clear out stack space for 4 local variables (4 bytes each)
    sub        sp, sp, #8
    str        PCM, [sp, #4]            ; sp[1] = pcm pointer

    ; special case, output sample 0
    mov        SUM1LL, #RNDVAL          ; load rndVal (low 32)
    mov        SUM1LH, #0

    MC0M    0
    MC0M    1
    MC0M    2
    MC0M    3
    MC0M    4
    MC0M    5
    MC0M    6
    MC0M    7

    ldr        PCM, [sp, #4]        ; load pcm pointer
    mov        MAXPOS, #0x7f00
    orr        MAXPOS, MAXPOS, #0xff
    
    C64TOS    SUM1LL, SUM1LH, SIGN, MAXPOS
    strh    SUM1LL, [PCM, #(2*0)]

    ; special case, output sample 16
    add        COEF, COEF, #(4*(256-16))    ; coef = coefBase + 256 (was coefBase + 16 after MC0M block)
    add        VB1, VB1, #(4*1024)          ; vb1 = vbuf + 64*16
    
    mov        SUM1LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM1LH, #0

    MC1M    0
    MC1M    1
    MC1M    2
    MC1M    3
    MC1M    4
    MC1M    5
    MC1M    6
    MC1M    7

    ldr        PCM, [sp, #4]        ; load pcm pointer
    mov        MAXPOS, #0x7f00
    orr        MAXPOS, MAXPOS, #0xff
    
    C64TOS    SUM1LL, SUM1LH, SIGN, MAXPOS

    strh    SUM1LL, [PCM, #(2*16)]

    ; main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17
    sub        COEF, COEF, #(4*(264-16))    ; coef = coefBase + 16 (was coefBase + 264 after MC1M block)
    sub        VB1, VB1, #(4*(1024-64))     ; vb1 = vbuf + 64 (was vbuf + 64*16 after MC1M block)
    mov        I, #15                       ; loop counter, count down
    add        PCM, PCM, #(2)               ; pcm++

LoopPM
    str        I, [sp, #0]                  ; sp[0] = i (loop counter)
    str        PCM, [sp, #4]                ; sp[1] = pcm (pointer to pcm buffer)
    
    mov        SUM1LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM2LL, #RNDVAL              ; load rndVal (low 32)
    mov        SUM1LH, #0
    mov        SUM2LH, #0

    MC2M    0
    MC2M    1
    MC2M    2
    MC2M    3
    MC2M    4
    MC2M    5
    MC2M    6
    MC2M    7
    
    add        VB1, VB1, #(4*64)    ; vb1 += 64
    
    ldr        PCM, [sp, #4]        ; load pcm pointer
    mov        MAXPOS, #0x7f00
    orr        MAXPOS, MAXPOS, #0xff
    
    C64TOS    SUM1LL, SUM1LH, SIGN, MAXPOS
    C64TOS    SUM2LL, SUM2LH, SIGN, MAXPOS
    
    ldr        I, [sp, #0]            ; load loop counter*/
    add        CF2, PCM, I, lsl #2    ; CF2 = PCM + 2*i (short offset)*/
    strh    SUM2LL, [CF2], #2    	  ; (pcm + 2*i + 0)
    strh    SUM1LL, [PCM], #2         ; (pcm + 0) ; pcm++
    
    subs    I, I, #1
    bne        LoopPM

    ; restore stack pointer
    add        sp, sp, #8

    ldmfd    sp!, {r4-r11, pc}
    

    END