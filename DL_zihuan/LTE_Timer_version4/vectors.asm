;create interrupt vector table for C6000 DSP
;--------------------------------------------------------------
;This file can be modified to add Interrupt Service Routine(ISR) 
;for an interrupt, the steps are:
;1,reference to the externally defined ISR, for example
;	.ref EDMA_ISR
;2,modify the corresponding entry in the interrupt vector table.
;  For example, if interrupt 8 is used for EDMA, then you should
;  modify the entry number 8 like below:
;	VEC_ENTRY EDMA_ISR     	;interrupt 8 
;--------------------------------------------------------------
;Author: Brighton Feng
;Created on 2010-12-6
;--------------------------------------------------------------

;reference to the externally defined ISR
	.ref _c_int00
	.ref Timer_ISR
	.ref DoorbellCallbackFunc

	.global vectors 

;--------------------------------------------------------------
	.sect ".text"
	
;create interrupt vector for reset (interrupt 0)
VEC_RESET .macro addr
	MVKL  addr,B0
	MVKH  addr,B0
	B     B0
	MVC   PCE1,B0
	NOP   4
	.align 32
	.endm

;create interrupt vector for other used interrupts	
VEC_ENTRY .macro addr
	STW   B0,*--B15
	MVKL  addr,B0
	MVKH  addr,B0
	B     B0
	LDW   *B15++,B0
	NOP   4
	.align 32
	.endm

;create interrupt vector for unused interrupts	
VEC_DUMMY .macro
unused_int?:
	B    unused_int? ;dead loop for unused interrupts
	NOP  5
	.align 32
	.endm

	
;--------------------------------------------------------------
;interrupt vector table	
	.sect "vecs"
	.align 1024
vectors:
	VEC_RESET _c_int00     		;RESET 
	VEC_DUMMY 			;NMI/Exception
	VEC_DUMMY   		;RSVD
	VEC_DUMMY   		;RSVD
	VEC_ENTRY DoorbellCallbackFunc  	;interrupt 4
	VEC_DUMMY   		;interrupt 5
	VEC_DUMMY   		;interrupt 6 
	VEC_DUMMY   		;interrupt 7 
	VEC_DUMMY   		;interrupt 8 
	VEC_DUMMY   		;interrupt 9
	VEC_DUMMY   		;interrupt 10
	VEC_DUMMY   		;interrupt 11
	VEC_DUMMY   		;interrupt 12
	VEC_DUMMY   		;interrupt 13
	VEC_ENTRY Timer_ISR   		;interrupt 14
	VEC_DUMMY   		;interrupt 15
	
	.end
