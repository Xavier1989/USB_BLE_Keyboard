;/*
; *-----------------------------------------------------------------------------
; * The confidential and proprietary information contained in this file may
; * only be used by a person authorised under and to the extent permitted
; * by a subsisting licensing agreement from ARM Limited.
; *
; *            (C) COPYRIGHT 2010-2011 ARM Limited.
; *                ALL RIGHTS RESERVED
; *
; * This entire notice must be reproduced on all copies of this file
; * and copies of this file may only be made by a person if such person is
; * permitted to do so under the terms of a subsisting license agreement
; * from ARM Limited.
; *
; *      SVN Information
; *
; *      Checked In          : $Date: Wed Oct 24 09:40:31 2012 $
; *
; *      Revision            : $Revision: 1.1 $
; *
; *      Release Information : BP200-r0p0-00rel0
; *-----------------------------------------------------------------------------
; */

;/*****************************************************************************/
;/* Startup.s: Startup file for ARM Cortex-M0 Device Family                   */
;/*****************************************************************************/


;// <h> Stack Configuration
;//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Stack_Size      EQU     0x00000600

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

;remap  uncomment below expression to have the application remap SYSRAM to 0
;__REMAP_SYSRAM EQU 1

; Vector Table Mapped to Address 0 at Reset

 ;               AREA    RESET, DATA, READONLY
                AREA    RESET,DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
				;ENTRY
__Vectors       DCD     __initial_sp              ; Top of Stack
                IF      :DEF:__REMAP_SYSRAM
                DCD     Reset_Handler+0x20000000             ; Reset Handler
                ELSE
                DCD     Reset_Handler             ; Reset Handler
                ENDIF
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
               
                DCD     BLE_WAKEUP_LP_Handler            				
                DCD     BLE_FINETGTIM_Handler     
                DCD     BLE_GROSSTGTIM_Handler    
                DCD     BLE_CSCNT_Handler          
                DCD     BLE_SLP_Handler 
                DCD     BLE_ERROR_Handler 
                DCD     BLE_RX_Handler
                DCD     BLE_EVENT_Handler	                
                DCD     SWTIM_Handler
                DCD     WKUP_QUADEC_Handler	
                DCD     BLE_RF_DIAG_Handler	
                DCD     BLE_CRYPT_Handler	
                DCD     UART_Handler		
                DCD     UART2_Handler    
                DCD     I2C_Handler    
                DCD     SPI_Handler    
                DCD     ADC_Handler    
                DCD     KEYBRD_Handler    
                DCD     RFCAL_Handler    
                DCD     GPIO0_Handler
                DCD     GPIO1_Handler
                DCD     GPIO2_Handler
                DCD     GPIO3_Handler
                DCD     GPIO4_Handler
__Vectors_End

__Vectors_Size         EQU     __Vectors_End - __Vectors
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
				
;remap 
                IF      :DEF:__REMAP_SYSRAM
                LDR     R0, =0x0
                LDR     R1, [R0]
                LDR     R0, =0x20000000
                LDR     R2, [R0]
                CMP     R2, R1
                BEQ     remap_done
                LDR     R0, =0x50000012
                LDRH    R1, [R0]
                LSRS    R2, R1, #2
                LSLS    R1, R2, #2
                MOVS    R2, #0x2
                ADDS    R1, R1, R2            ;remap SYSRAM to 0
                LSLS    R2, R2, #14
                ADDS    R1, R1, R2         ;SW RESET
                STRH    R1, [R0]
remap_done                
                ENDIF
;remap

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)
                IMPORT NMI_HandlerC
NMI_Handler\
                PROC
                movs r0, #4
                mov r1, lr
                tst r0, r1
                beq NMI_stacking_used_MSP
                mrs r0, psp
                ldr r1,=NMI_HandlerC
                bx r1
NMI_stacking_used_MSP
                mrs r0, msp
                ldr r1,=NMI_HandlerC
                bx r1
                ENDP
                
                IMPORT HardFault_HandlerC
HardFault_Handler\
                PROC
                movs r0, #4
                mov r1, lr
                tst r0, r1
                beq HardFault_stacking_used_MSP
                mrs r0, psp
                ldr r1,=HardFault_HandlerC
                bx r1
HardFault_stacking_used_MSP
                mrs r0, msp
                ldr r1,=HardFault_HandlerC
                bx r1
                ENDP

				
				IMPORT SVC_Handler_c
SVC_Handler     PROC
                movs r0, #4
                mov r1, lr
                tst r0, r1
                beq SVC_stacking_used_MSP
                mrs r0, psp
                ldr r1,=SVC_Handler_c
                bx r1
SVC_stacking_used_MSP
                mrs r0, msp
                ldr r1,=SVC_Handler_c
                bx r1
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP
Default_Handler PROC
                EXPORT BLE_WAKEUP_LP_Handler   [WEAK]
                EXPORT BLE_FINETGTIM_Handler   [WEAK]
                EXPORT BLE_GROSSTGTIM_Handler  [WEAK]
                EXPORT BLE_CSCNT_Handler       [WEAK]
                EXPORT BLE_SLP_Handler         [WEAK]
                EXPORT BLE_ERROR_Handler       [WEAK]
                EXPORT BLE_RX_Handler          [WEAK]
                EXPORT BLE_EVENT_Handler	   [WEAK]
                EXPORT SWTIM_Handler           [WEAK]
                EXPORT WKUP_QUADEC_Handler     [WEAK]
                EXPORT BLE_RF_DIAG_Handler     [WEAK]
				EXPORT BLE_CRYPT_Handler	   [WEAK]
                EXPORT UART_Handler		       [WEAK]
                EXPORT UART2_Handler           [WEAK]
                EXPORT I2C_Handler             [WEAK]
                EXPORT SPI_Handler             [WEAK]
                EXPORT ADC_Handler             [WEAK]
                EXPORT KEYBRD_Handler          [WEAK]
                EXPORT RFCAL_Handler           [WEAK]
                EXPORT GPIO0_Handler           [WEAK]
                EXPORT GPIO1_Handler           [WEAK]
                EXPORT GPIO2_Handler           [WEAK]
                EXPORT GPIO3_Handler           [WEAK]
                EXPORT GPIO4_Handler           [WEAK]
               
				
BLE_WAKEUP_LP_Handler
BLE_FINETGTIM_Handler     
BLE_GROSSTGTIM_Handler    
BLE_CSCNT_Handler          
BLE_SLP_Handler 
BLE_ERROR_Handler 
BLE_RX_Handler
BLE_EVENT_Handler	
SWTIM_Handler
WKUP_QUADEC_Handler 	
BLE_RF_DIAG_Handler 
BLE_CRYPT_Handler	
UART_Handler		
UART2_Handler    
I2C_Handler    
SPI_Handler    
ADC_Handler    
KEYBRD_Handler    
RFCAL_Handler    
GPIO0_Handler
GPIO1_Handler
GPIO2_Handler
GPIO3_Handler
GPIO4_Handler
               B       .
               ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, =  Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END

