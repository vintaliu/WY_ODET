
;/*-----------------------------------------------------------------------------------------------------------
;  File Name        : startup_.s
;  Version          : V1.00
;  Date             : 07/07/2017
;  Description      : Startup code.
;-----------------------------------------------------------------------------------------------------------*/
;/* <<< Use Configuration Wizard in Context Menu >>>                                                        */

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;//   <o> Stack Size (in Bytes, must 8 byte aligned) <0-16384:8>
Stack_Size          EQU     512

                    AREA    STACK, NOINIT, READWRITE, ALIGN = 3
Stack_Mem           SPACE   Stack_Size
__initial_sp

;//   <o>  Heap Size (in Bytes) <0-16384:8>
Heap_Size           EQU     0

                    AREA    HEAP, NOINIT, READWRITE, ALIGN = 3
__heap_base
Heap_Mem            SPACE   Heap_Size
__heap_limit

                    PRESERVE8
                    THUMB

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
                    AREA    RESET, CODE, READONLY
                    EXPORT  __Vectors

__Vectors           DCD  __initial_sp                       ; Top address of Stack
                    DCD  Reset_Handler                      ; Reset Handler
                    DCD  NMI_Handler                        ; NMI Handler
                    DCD  HardFault_Handler                  ; Hard Fault Handler
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved						
                    DCD  SVC_Handler            		; SVC Handler
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  PendSV_Handler                     ; PendSV Handler
                    DCD  SysTick_Handler                    ; SysTick Handler

                    ; External Interrupt Handler
                    DCD  WWDG_Handler						;  16+ 0:  WWDG_Handler
                    DCD  CLK_FAIL_Handler				    ;  16+ 1:  CLK_FAIL_Handler
                    DCD  0									;  16+ 2:  Not Used
                    DCD  FLASH_Handler						;  16+ 3:  FLASH_Handler
                    DCD  0           						;  16+ 4:  Not Used
                    DCD  PA_Handler			     			;  16+ 5:  PA_Handler
                    DCD  PB_Handler							;  16+ 6:  PB_Handler
                    DCD  PC_Handler							;  16+ 7:  PC_Handler
                    DCD  PD_Handler							;  16+ 8:  PD_Handler
                    DCD  Comparator0_Handler				;  16+ 9:  Comparator0_Handler
                    DCD  Comparator1_Handler				;  16+ 10: Comparator1_Handler
                    DCD  DAC_Handler					    ;  16+ 11: DAC_Handler
                    DCD  ADC_Handler			            ;  16+ 12: ADC_Handler
                    DCD  TIMER1_Handler		                ;  16+ 13: TIMER1_Handler
                    DCD  TIMER0_Handler		                ;  16+ 14: TIMER0_Handler
                    DCD  TIMER4_Handler		                ;  16+ 15: TIMER4_Handler
                    DCD  TIMER3_Handler		                ;  16+ 17: TIMER3_Handler
                    DCD  TIMER2_Handler		                ;  16+ 17: TIMER2_Handler
                    DCD  0           						;  16+ 18: Not Used
                    DCD  0           						;  16+ 19: Not Used
                    DCD  HIGH_LVD_Handler		            ;  16+ 20: HIGH_LVD_Handler
                    DCD  LOW_LVD_Handler		            ;  16+ 21: LOW_LVD_Handler
                    DCD  0           						;  16+ 22: Not Used
                    DCD  I2C0_Handler						;  16+ 23: I2C0_Handler
                    DCD  I2C1_Handler						;  16+ 24: I2C1_Handler
                    DCD  SPI0_Handler						;  16+ 25: SPI0_Handler
                    DCD  SPI1_Handler						;  16+ 26: SPI1_Handler		
                    DCD  UART0_Handler						;  16+ 27: UART0_Handler					
                    DCD  0									;  16+ 28: Not Used
                    DCD  0						;  16+ 29: RTC_Handler	
                    DCD  0
                    DCD  0
						

; Reset handler routine
Reset_Handler       PROC
                    EXPORT  Reset_Handler                   [WEAK]
                    IMPORT  __main
                    IMPORT  SystemInit
					LDR     R0, =SystemInit
                    BLX     R0
                    LDR     R0, =__main
                    BX      R0
                    ENDP

; Dummy Exception Handlers
NMI_Handler         PROC
                    EXPORT  NMI_Handler                     [WEAK]
                    B       .
                    ENDP

HardFault_Handler   PROC
                    EXPORT  HardFault_Handler               [WEAK]
                    B       .
                    ENDP

SVC_Handler         PROC
                    EXPORT  SVC_Handler                     [WEAK]
                    B       .
                    ENDP

PendSV_Handler      PROC
                    EXPORT  PendSV_Handler                  [WEAK]
                    B       .
                    ENDP

SysTick_Handler     PROC
                    EXPORT  SysTick_Handler                 [WEAK]
                    B       .
                    ENDP


Default_Handler     PROC
                    EXPORT  WWDG_Handler                    [WEAK]
                    EXPORT  CLK_FAIL_Handler                [WEAK]
                    EXPORT  FLASH_Handler                   [WEAK]
                    EXPORT  PA_Handler                      [WEAK]
                    EXPORT  PB_Handler                      [WEAK]
                    EXPORT  PC_Handler                      [WEAK]
                    EXPORT  PD_Handler                      [WEAK]						
                    EXPORT  Comparator0_Handler             [WEAK]
                    EXPORT  Comparator1_Handler             [WEAK]
                    EXPORT  DAC_Handler	                    [WEAK]						
                    EXPORT  ADC_Handler	                    [WEAK]
                    EXPORT  TIMER1_Handler                  [WEAK]
                    EXPORT  TIMER0_Handler                  [WEAK]
                    EXPORT  TIMER4_Handler                  [WEAK]
                    EXPORT  TIMER3_Handler                  [WEAK]
                    EXPORT  TIMER2_Handler                  [WEAK]
                    EXPORT  HIGH_LVD_Handler                [WEAK]
                    EXPORT  LOW_LVD_Handler                 [WEAK]						
                    EXPORT  I2C0_Handler                    [WEAK]
                    EXPORT  I2C1_Handler                    [WEAK]
                    EXPORT  SPI0_Handler                    [WEAK]
                    EXPORT  SPI1_Handler                    [WEAK]
                    EXPORT  UART0_Handler                   [WEAK]



WWDG_Handler
CLK_FAIL_Handler
FLASH_Handler
PA_Handler
PB_Handler
PC_Handler
PD_Handler
Comparator0_Handler
Comparator1_Handler
DAC_Handler
ADC_Handler
TIMER1_Handler
TIMER0_Handler
TIMER4_Handler
TIMER3_Handler
TIMER2_Handler
HIGH_LVD_Handler
LOW_LVD_Handler
I2C0_Handler
I2C1_Handler
SPI0_Handler
SPI1_Handler
UART0_Handler



                    B       .
                    ENDP

                    ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
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
                    LDR     R2, = (Heap_Mem + Heap_Size)
                    LDR     R3, = Stack_Mem
                    BX      LR

                    ALIGN

                    ENDIF

                    END
