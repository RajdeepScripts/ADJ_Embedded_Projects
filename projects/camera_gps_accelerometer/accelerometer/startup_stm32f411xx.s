.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler

.word _estack

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word 0  /* NMI */
  .word 0  /* HardFault */
  /* (You can add more interrupts if needed) */

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  bl main
  b .

