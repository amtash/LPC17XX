; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -omain.o --depend=main.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=main.crf main.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

main PROC
        PUSH     {r2-r4,lr}
        MOV      r0,#0
        LDR      r1,|L1.288|
        STR      r0,[r1,#0]  ; pass
        LDR      r1,|L1.292|
        STR      r0,[r1,#0]  ; distance_ready
        LDR      r0,|L1.296|
        LDR      r0,[r0,#0]  ; temperature
        BL       __aeabi_ui2d
        MOV      r6,r0
        MOV      r2,#0x33333333
        LDR      r3,|L1.300|
        BL       __aeabi_dmul
        MOV      r4,r0
        MOVS     r2,#0
        LDR      r3,|L1.304|
        BL       __aeabi_dadd
        MOV      r8,r0
        MOVS     r2,#0
        LDR      r3,|L1.308|
        BL       __aeabi_dmul
        STRD     r0,r1,[sp,#0]
        BL       __aeabi_d2uiz
        LDR      r1,|L1.312|
        STR      r0,[r1,#0]  ; speed_sound
        MOV      r0,#1
        LDR      r1,|L1.316|
        STR      r0,[r1,#0]  ; state
        LDR      r1,|L1.320|
        STR      r0,[r1,#0]  ; draw_flag
        MOV      r0,#0
        LDR      r1,|L1.324|
        STR      r0,[r1,#0]  ; screen_flag
        LDR      r1,|L1.328|
        STRH     r0,[r1,#0]
        MOV      r10,r0
        B        |L1.108|
|L1.94|
        MOV      r0,#0x50
        LDR      r1,|L1.332|
        STRH     r0,[r1,r10,LSL #1]
        ADD      r10,r10,#1
|L1.108|
        CMP      r10,#9
        BLT      |L1.94|
        ADR      r0,|L1.336|
        BL       tx_cadena_UART0
        MOV      r0,#0x1c200
        BL       Init_UART0
        ADR      r0,|L1.360|
        BL       tx_cadena_UART0
        BL       Init_OV7670
        ADR      r0,|L1.384|
        BL       tx_cadena_UART0
        BL       Init_TIMER1X
        MOV      r0,#0x3e8
        BL       periodic_signal_ms
        MOVS     r0,#0x23
        BL       periodic_signal2_ms
        ADR      r0,|L1.408|
        BL       tx_cadena_UART0
        MOVS     r0,#1
        BL       DACInit
        BL       DMA_Init
        MOVS     r0,#0
        BL       DMA_DAC_Channel_Init
        MOV      r1,#0xbb8
        MOV      r0,#0x12c
        BL       set_sound
        ADR      r0,|L1.432|
        BL       tx_cadena_UART0
        BL       get_distance
        ADR      r0,|L1.456|
        BL       tx_cadena_UART0
        BL       Init_Servos
        ADR      r0,|L1.480|
        BL       tx_cadena_UART0
        BL       LCD_Initialization
        LDR      r0,|L1.500|
        LDRH     r0,[r0,#0]  ; bgColor
        BL       LCD_Clear
        BL       TP_Init
        BL       TouchPanel_Calibrate
        ADR      r0,|L1.504|
        BL       tx_cadena_UART0
        BL       init_TCP
        ADR      r0,|L1.532|
        BL       tx_cadena_UART0
        B        |L1.286|
|L1.260|
        BL       State_Machine
        MOV      r10,#0
        B        |L1.274|
|L1.270|
        ADD      r10,r10,#1
|L1.274|
        MOV      r0,#0xc350
        CMP      r10,r0
        BLT      |L1.270|
        BL       check_webserver
|L1.286|
        B        |L1.260|
        ENDP

|L1.288|
        DCD      pass
|L1.292|
        DCD      distance_ready
|L1.296|
        DCD      temperature
|L1.300|
        DCD      0x3fe33333
|L1.304|
        DCD      0x4074b000
|L1.308|
        DCD      0x40590000
|L1.312|
        DCD      speed_sound
|L1.316|
        DCD      state
|L1.320|
        DCD      draw_flag
|L1.324|
        DCD      screen_flag
|L1.328|
        DCD      Detection_color
|L1.332|
        DCD      Calibration_vars
|L1.336|
        DCB      "Configurando UART0\n\r",0
        DCB      0
        DCB      0
        DCB      0
|L1.360|
        DCB      "Configurando Camara\n\r",0
        DCB      0
        DCB      0
|L1.384|
        DCB      "Configurando TIMERS\n\r",0
        DCB      0
        DCB      0
|L1.408|
        DCB      "Configurando DAC+DMA\n\r",0
        DCB      0
|L1.432|
        DCB      "Configurando SRF04 \n\r",0
        DCB      0
        DCB      0
|L1.456|
        DCB      "Configurando Motores\n\r",0
        DCB      0
|L1.480|
        DCB      "Configurando LCD\n\r",0
        DCB      0
|L1.500|
        DCD      bgColor
|L1.504|
        DCB      "Configurando WEBSERVER \n\r",0
        DCB      0
        DCB      0
|L1.532|
        DCB      "Todo configurado\n\r",0
        DCB      0

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=0

cadena
        %        10

        AREA ||.data||, DATA, ALIGN=2

distance_aux
        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "main.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___6_main_c_05be4ded____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___6_main_c_05be4ded____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___6_main_c_05be4ded____REVSH|
#line 128
|__asm___6_main_c_05be4ded____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

__ARM_use_no_argv EQU 0

        EXPORT __ARM_use_no_argv
        EXPORT main [CODE]
        EXPORT cadena [DATA,SIZE=10]
        EXPORT distance_aux [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT __aeabi_ui2d [CODE]
        IMPORT __aeabi_dmul [CODE]
        IMPORT __aeabi_dadd [CODE]
        IMPORT __aeabi_d2uiz [CODE]
        IMPORT tx_cadena_UART0 [CODE]
        IMPORT Init_UART0 [CODE]
        IMPORT Init_OV7670 [CODE]
        IMPORT Init_TIMER1X [CODE]
        IMPORT periodic_signal_ms [CODE]
        IMPORT periodic_signal2_ms [CODE]
        IMPORT DACInit [CODE]
        IMPORT DMA_Init [CODE]
        IMPORT DMA_DAC_Channel_Init [CODE]
        IMPORT set_sound [CODE]
        IMPORT get_distance [CODE]
        IMPORT Init_Servos [CODE]
        IMPORT LCD_Initialization [CODE]
        IMPORT LCD_Clear [CODE]
        IMPORT TP_Init [CODE]
        IMPORT TouchPanel_Calibrate [CODE]
        IMPORT init_TCP [CODE]
        IMPORT State_Machine [CODE]
        IMPORT check_webserver [CODE]
        IMPORT pass [DATA]
        IMPORT distance_ready [DATA]
        IMPORT temperature [DATA]
        IMPORT speed_sound [DATA]
        IMPORT state [DATA]
        IMPORT draw_flag [DATA]
        IMPORT screen_flag [DATA]
        IMPORT Detection_color [DATA]
        IMPORT Calibration_vars [DATA]
        IMPORT bgColor [DATA]

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