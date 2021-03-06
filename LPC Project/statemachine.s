; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -ostatemachine.o --depend=statemachine.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=statemachine.crf State_Machine\StateMachine.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

State_Machine PROC
        PUSH     {r4,lr}
        LDR      r0,|L1.192|
        LDR      r0,[r0,#0]  ; state
        CMP      r0,#0x19
        BCS      |L1.184|
        TBB      [pc,r0]
        DCB      0x55,0x0d
        DCB      0x10,0x13,0x16,0x19
        DCB      0x1c,0x1f,0x22,0x25
        DCB      0x28,0x2b,0x2e,0x31
        DCB      0x34,0x37,0x3a,0x3d
        DCB      0x40,0x43,0x46,0x49
        DCB      0x4c,0x4f,0x52,0x00
        BL       Menu_Principal
        B        |L1.186|
        BL       Menu_Camara
        B        |L1.186|
        BL       Menu_Motores
        B        |L1.186|
        BL       Config_1
        B        |L1.186|
        BL       Config_2
        B        |L1.186|
        BL       RGB_Configuration
        B        |L1.186|
        BL       Light_Configuration
        B        |L1.186|
        BL       Config_Umbral_Captura
        B        |L1.186|
        BL       Choose_FPS
        B        |L1.186|
        BL       Show_IP
        B        |L1.186|
        BL       Show_Actual_Config
        B        |L1.186|
        BL       Menu_Deteccion
        B        |L1.186|
        BL       Manual_vs_Auto
        B        |L1.186|
        BL       Deteccion_Seguimiento
        B        |L1.186|
        BL       Deteccion_no_Seguimiento
        B        |L1.186|
        BL       Camera_Image
        B        |L1.186|
        BL       Config_Mov_Motores
        B        |L1.186|
        BL       Giro_Manual_Camara
        B        |L1.186|
        BL       Elegir_modo_control
        B        |L1.186|
        BL       Config_var_control
        B        |L1.186|
        BL       Config_control_hor
        B        |L1.186|
        BL       Config_control_ver
        B        |L1.186|
        BL       Menu_juegos
        B        |L1.186|
        BL       Jugando
        B        |L1.186|
|L1.184|
        NOP      
|L1.186|
        NOP      
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L1.192|
        DCD      state

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "State_Machine\\StateMachine.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___14_StateMachine_c_2fa6d74b____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___14_StateMachine_c_2fa6d74b____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___14_StateMachine_c_2fa6d74b____REVSH|
#line 128
|__asm___14_StateMachine_c_2fa6d74b____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT State_Machine [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT Menu_Principal [CODE]
        IMPORT Menu_Camara [CODE]
        IMPORT Menu_Motores [CODE]
        IMPORT Config_1 [CODE]
        IMPORT Config_2 [CODE]
        IMPORT RGB_Configuration [CODE]
        IMPORT Light_Configuration [CODE]
        IMPORT Config_Umbral_Captura [CODE]
        IMPORT Choose_FPS [CODE]
        IMPORT Show_IP [CODE]
        IMPORT Show_Actual_Config [CODE]
        IMPORT Menu_Deteccion [CODE]
        IMPORT Manual_vs_Auto [CODE]
        IMPORT Deteccion_Seguimiento [CODE]
        IMPORT Deteccion_no_Seguimiento [CODE]
        IMPORT Camera_Image [CODE]
        IMPORT Config_Mov_Motores [CODE]
        IMPORT Giro_Manual_Camara [CODE]
        IMPORT Elegir_modo_control [CODE]
        IMPORT Config_var_control [CODE]
        IMPORT Config_control_hor [CODE]
        IMPORT Config_control_ver [CODE]
        IMPORT Menu_juegos [CODE]
        IMPORT Jugando [CODE]
        IMPORT state [DATA]

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
