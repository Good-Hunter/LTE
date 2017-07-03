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
		/* External DDR3, upto 2GB per core */
	DDR3_CODE: 		o = 0x80000000  l = 0x01000000			/*set memory protection attribitue as execution only*/
	DDR3_R_DATA  (R):  	o = 0x81000000  l = 0x02000000		/*set memory protection attribitue as read only*/
	DDR3_RW_DATA (RW): 	o = 0x83000000  l = 0x05000000		/*set memory protection attribitue as read/write*/
	For_SrioData (RW):	o = 0x88000000	l = 0x01000000		/*申请16M的DDR3，用于收发SRIO的数据*/
    For_SrioOrgData (RW):	o = 0x89000000	l = 0x01000000		/*申请16M的DDR3，用于收发SRIO的数据*/
	/* Shared L2 2~4MB*/									//C6670上有2M
	SL2:				o = 0x0C000000  l = 0x00200000

	/* Local (Aliased) Core Memory */						//配置的是核自身L2，共1M
	/* Use for shared code and data */
	LOCAL_L2:         o = 00800300h   l = 000FFD00h
	LOCAL_BOARD_INFO: o = 00800200h   l = 00000100h
	LOCAL_INT_VECT:   o = 00800000h   l = 00000200h

	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data  ，L1P和L1D没使用*/
	CORE0_L2:         o = 10800300h   l = 000FFD00h
	CORE0_BOARD_INFO: o = 10800200h   l = 00000100h
	CORE0_INT_VECT:   o = 10800000h   l = 00000200h

	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data  ，L1P和L1D没使用*/
	CORE1_L2:         o = 11800300h   l = 000FFD00h
	CORE1_BOARD_INFO: o = 11800200h   l = 00000100h
	CORE1_INT_VECT:   o = 11800000h   l = 00000200h

	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data  ，L1P和L1D没使用*/
	CORE2_L2:         o = 12800300h   l = 000FFD00h
	CORE2_BOARD_INFO: o = 12800200h   l = 00000100h
	CORE2_INT_VECT:   o = 12800000h   l = 00000200h

	/* Global (On-chip) Memory Space    */
	/* Use for core specific code and data  ，L1P和L1D没使用*/
	CORE3_L2:         o = 13800300h   l = 000FFD00h
	CORE3_BOARD_INFO: o = 13800200h   l = 00000100h
	CORE3_INT_VECT:   o = 13800000h   l = 00000200h

}

SECTIONS
{
	vecs				>		LOCAL_INT_VECT
	.csl_vect   		> 		LOCAL_L2
    .text       		>       SL2
    .switch     		>       SL2
    .const      		>       DDR3_R_DATA//CORE0_L2
    .cinit      		>       DDR3_R_DATA

    .neardata			> 		LOCAL_L2
    .rodata				> 		LOCAL_L2
    .bss        		>       LOCAL_L2

    .fardata			> 		LOCAL_L2
    .far        		>       LOCAL_L2
    .stack      		>       LOCAL_L2
    .sysmem     		>       LOCAL_L2

    .cio        		>       LOCAL_L2

    .extData 			>     	DDR3_RW_DATA

/*********************************导航相关的数据存储分配**********************************
由CT撰写，搬过来与机箱合并，有一些修改见后面注释   cs 14/11/07
**********************************************************************************/
	QMSS_Data.linkingRAM1 		> 	SL2
	QMSS_Data.Descriptor_SL2 	> 	SL2
	PacketData.buffer_SL2 		> 	SL2
	QMSS_Data.Descriptor_LL2 	> 	LOCAL_L2     //由LL2_RW_DATA变为LOCAL_L2
	PacketData.buffer_LL2 		> 	LOCAL_L2     //由LL2_RW_DATA变为LOCAL_L2
	QMSS_Data.Descriptor_DDR 	> 	DDR3_RW_DATA
	PacketData.buffer_DDR 		> 	DDR3_RW_DATA
/*********************************我们使用的数据存储分配**********************************/
    //在内存For_SrioData，给数据分区
    SrioDataBuffer_in_L2 :			load = LOCAL_L2		//0x88000000
    SrioDataBuffer_in_SL2 :			load = SL2			//0x89000000
    SrioDataBuffer_in_DDR3 :		load = For_SrioData	//0x90000000

   //.testData: load =  For_SrioOrgData
   .testData: load >> DDR3_R_DATA
	.bcp:  load >> SL2
	.shareddata: load >> SL2
	 .newdata :		> For_SrioOrgData	//0x90000000
}
