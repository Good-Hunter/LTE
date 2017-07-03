/* =============================================================================
 * Copyright (c) CommAgility Limited 2011-2012 
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied
 *
 * CommAgility Limited,                		    email:   support@commagility.com  
 * Charnwood Building, Holywell Park,      	    web:     www.commagility.com 
 * Ashby Road, Loughborough,          		    general: +44 (0) 1509 228866
 * LEICESTERSHIRE, LE11 3AQ, UK.
 * =============================================================================
 */
/** 
 * @file   BSLC667X_C6670_TCI6616_cmd.cmd
 * @brief  Sample Linker command file used for linking programs built with the
 *         c compiler and running the resulting .out file on CommAgility BSLC667X
 *         type boards.
 */
/* =============================================================================
 * History:
 * --------    
 * Version 0.3.0
 *   Revision: 0.1.0 [Id:PP] [Date: 11/02/2011] Initial Release
 *   Initial revision 		
 * =============================================================================
 */
 
-c
-heap  0x1000
-stack 0x3000

/* Memory Map (Symmetric Default) */
MEMORY
{
	/* External DDR3 Memory */
	DDR3:            o = 0x80000000  l = 40000000h
	/* Local (Aliased) Core Memory */
	/* Use for shared code and data */
	LOCAL_L1D:        o = 00f00000h   l = 00008000h 
	LOCAL_L1P:        o = 00e00000h   l = 00008000h 
	LOCAL_L2:         o = 00800200h   l = 000FFE00h 
	LOCAL_BOARD_INFO: o = 00800100h   l = 00000100h
	LOCAL_INT_VECT:   o = 00800000h   l = 00000100h
	
	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data */
	CORE0_L1D:        o = 10f00000h   l = 00008000h 
	CORE0_L1P:        o = 10e00000h   l = 00008000h 
	CORE0_L2:         o = 10800200h   l = 000FFE00h 
	CORE0_BOARD_INFO: o = 10800100h   l = 00000100h
	CORE0_INT_VECT:   o = 10800000h   l = 00000100h
	
	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data */
	CORE1_L1D:        o = 11f00000h   l = 00008000h 
	CORE1_L1P:        o = 11e00000h   l = 00008000h 
	CORE1_L2:         o = 11800200h   l = 000FFE00h 
	CORE1_BOARD_INFO: o = 11800100h   l = 00000100h
	CORE1_INT_VECT:   o = 11800000h   l = 00000100h

	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data */
	CORE2_L1D:        o = 12f00000h   l = 00008000h 
	CORE2_L1P:        o = 12e00000h   l = 00008000h 
	CORE2_L2:         o = 12800200h   l = 000FFE00h 
	CORE2_BOARD_INFO: o = 12800100h   l = 00000100h
	CORE2_INT_VECT:   o = 12800000h   l = 00000100h
	
	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data */
	CORE3_L1D:        o = 13f00000h   l = 00008000h 
	CORE3_L1P:        o = 13e00000h   l = 00008000h 
	CORE3_L2:         o = 13800200h   l = 000FFE00h 
	CORE3_BOARD_INFO: o = 13800100h   l = 00000100h
	CORE3_INT_VECT:   o = 13800000h   l = 00000100h
		
} 

SECTIONS
{    
	vectors		>		CORE0_INT_VECT
	.csl_vect   > 		CORE0_L2
    .text       >       CORE0_L2
    .switch     >       CORE0_L2
    .const      >       CORE0_L2
    .cinit      >       CORE0_L2
    
    .neardata	> 		CORE0_L2
    .rodata		> 		CORE0_L2
    .bss        >       CORE0_L2
    
    .fardata	> 		CORE0_L2
    .far        >       CORE0_L2
    .stack      >       CORE0_L2
    .sysmem     >       CORE0_L2
    
    .cio        >       CORE0_L2
    
    .extData 	>     	DDR3    
    
}
