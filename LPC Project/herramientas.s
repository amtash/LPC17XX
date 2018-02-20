; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -oherramientas.o --depend=herramientas.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=herramientas.crf herramientas\herramientas.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=1

conversor_IaS PROC
        PUSH     {r1-r6,lr}
        MOV      r2,r0
        MOV      r3,r1
        MOV      r0,#0
        MOV      r1,r0
        MOV      r4,#1
        B        |L1.58|
|L1.18|
        MOV      r5,#0xa
        SDIV     r6,r2,r5
        MLS      r5,r5,r6,r2
        ADD      r5,r5,#0x30
        UXTB     r5,r5
        STRB     r5,[sp,r0]
        MOV      r5,#0xa
        SDIV     r2,r2,r5
        ADD      r0,r0,#1
        CBNZ     r2,|L1.58|
        MOV      r4,#0
|L1.58|
        CMP      r4,#0
        BNE      |L1.18|
        MOV      r1,r4
        B        |L1.84|
|L1.66|
        SUB      r5,r0,r1
        SUB      r5,r5,#1
        LDRB     r5,[sp,r5]
        STRB     r5,[r3,r1]
        ADD      r1,r1,#1
|L1.84|
        CMP      r1,r0
        BLT      |L1.66|
        MOV      r5,#0
        STRB     r5,[r3,r0]
        POP      {r1-r6,pc}
        ENDP

conversor_SaI PROC
        PUSH     {r4-r10,lr}
        MOV      r5,r0
        MOV      r4,#0
        MOV      r7,r4
        MOV      r9,r7
        MOV      r0,r5
        BL       strlen
        MOV      r6,r0
        MOV      r8,#1
        NOP      
        NOP      
        B        |L1.188|
|L1.128|
        SUBS     r0,r6,#1
        ADD      r0,r0,r5
        SUBS     r0,r0,r4
        LDRB     r0,[r0,#0]
        CMP      r0,#0x30
        BLT      |L1.182|
        SUBS     r0,r6,#1
        ADD      r0,r0,r5
        SUBS     r0,r0,r4
        LDRB     r0,[r0,#0]
        CMP      r0,#0x39
        BGT      |L1.182|
        SUB      r0,r6,#1
        ADD      r0,r0,r5
        SUB      r0,r0,r4
        LDRB     r0,[r0,#0]
        SUB      r0,r0,#0x30
        MLA      r7,r0,r8,r7
        ADD      r0,r8,r8,LSL #2
        LSL      r8,r0,#1
        B        |L1.184|
|L1.182|
        MOV      r7,r9
|L1.184|
        ADD      r4,r4,#1
|L1.188|
        CMP      r4,r6
        BLT      |L1.128|
        MOV      r0,r7
        POP      {r4-r10,pc}
        ENDP

conversor_HexaS PROC
        PUSH     {r1-r5,lr}
        MOV      r4,r0
        MOV      r2,r1
        MOV      r1,#0
        MOV      r3,r1
        NOP      
        B        |L1.274|
|L1.214|
        LSL      r0,r1,#2
        ASR      r0,r4,r0
        AND      r0,r0,#0xf
        STRB     r0,[sp,r1]
        LDRB     r0,[sp,r1]
        CMP      r0,#9
        BGT      |L1.254|
        LDRB     r0,[sp,r1]
        ADD      r0,r0,#0x30
        UXTB     r0,r0
        STRB     r0,[sp,r1]
        B        |L1.268|
|L1.254|
        LDRB     r0,[sp,r1]
        ADD      r0,r0,#0x37
        UXTB     r0,r0
        STRB     r0,[sp,r1]
|L1.268|
        ADD      r0,r1,#1
        UXTB     r1,r0
|L1.274|
        CMP      r1,#2
        BLT      |L1.214|
        MOV      r3,#0
        B        |L1.308|
|L1.284|
        SUB      r0,r1,r3
        SUB      r0,r0,#1
        LDRB     r0,[sp,r0]
        ADD      r5,r2,r3
        STRB     r0,[r5,#2]
        ADD      r0,r3,#1
        UXTB     r3,r0
|L1.308|
        CMP      r3,r1
        BLT      |L1.284|
        MOV      r0,#0x2c
        ADD      r5,r2,r1
        STRB     r0,[r5,#2]
        MOV      r5,#0
        ADD      r0,r2,r1
        STRB     r5,[r0,#3]
        MOV      r0,#0x30
        STRB     r0,[r2,#0]
        MOV      r0,#0x78
        STRB     r0,[r2,#1]
        POP      {r1-r5,pc}
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "herramientas\\herramientas.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___14_herramientas_c_226e2e09____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___14_herramientas_c_226e2e09____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___14_herramientas_c_226e2e09____REVSH|
#line 128
|__asm___14_herramientas_c_226e2e09____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT conversor_IaS [CODE]
        EXPORT conversor_SaI [CODE]
        EXPORT conversor_HexaS [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT strlen [CODE]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.06"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END