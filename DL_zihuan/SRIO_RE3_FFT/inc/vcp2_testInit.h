/*****************************************************************************\
*           TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION           
*                                                                            
*  Property of Texas Instruments 
*  For  Unrestricted  Internal  Use  Only 
*  Unauthorized reproduction and/or distribution is strictly prohibited.  
*  This product is protected under copyright law and trade secret law 
*  as an unpublished work.  
*  Created 2004, (C) Copyright 2003 Texas Instruments.  All rights reserved.
*------------------------------------------------------------------------------
*  Filename       : csl_vcp2.h
*  Description    : VCP2 Common Header file
\*****************************************************************************/
#ifndef _CSL_VCP2_testInit_H_
#define _CSL_VCP2_testInit_H_

#include <ti/csl/soc.h>


//**************************************
//* VCP scope and inline control macros
//****************************************
#undef  USEDEFS
#undef  IDECL
#undef  IDEF

#ifndef PASS
#define PASS		(1)
#endif

#ifndef FAIL
#define FAIL		(0)
#endif

#define SUCCESS		PASS
#define FAILURE		FAIL
#ifdef  _VCP2_MOD_
  #define IDECL extern far
  #define USEDEFS
  #define IDEF
#else
  #ifdef  _INLINE
    #define IDECL static inline
    #define USEDEFS
    #define IDEF  static inline
  #else
    #define IDECL extern far
  #endif
#endif  
//**************************************
//* VCP global macro declarations
//**************************************
/** DMA Channel Setup  */
#define CSL_EDMA3_DMA_CHANNELSETUP_DEFAULT   {       \
   {CSL_EDMA3_QUE_0,0}, \
   {CSL_EDMA3_QUE_0,1}, \
   {CSL_EDMA3_QUE_0,2}, \
   {CSL_EDMA3_QUE_0,3}, \
   {CSL_EDMA3_QUE_0,4}, \
   {CSL_EDMA3_QUE_0,5}, \
   {CSL_EDMA3_QUE_0,6}, \
   {CSL_EDMA3_QUE_0,7}, \
   {CSL_EDMA3_QUE_0,8}, \
   {CSL_EDMA3_QUE_0,9}, \
   {CSL_EDMA3_QUE_0,10}, \
   {CSL_EDMA3_QUE_0,11}, \
   {CSL_EDMA3_QUE_0,12}, \
   {CSL_EDMA3_QUE_0,13}, \
   {CSL_EDMA3_QUE_0,14}, \
   {CSL_EDMA3_QUE_0,15}, \
   {CSL_EDMA3_QUE_0,16}, \
   {CSL_EDMA3_QUE_0,17}, \
   {CSL_EDMA3_QUE_0,18}, \
   {CSL_EDMA3_QUE_0,19}, \
   {CSL_EDMA3_QUE_0,20}, \
   {CSL_EDMA3_QUE_0,21}, \
   {CSL_EDMA3_QUE_0,22}, \
   {CSL_EDMA3_QUE_0,23}, \
   {CSL_EDMA3_QUE_0,24}, \
   {CSL_EDMA3_QUE_0,25}, \
   {CSL_EDMA3_QUE_0,26}, \
   {CSL_EDMA3_QUE_0,27}, \
   {CSL_EDMA3_QUE_0,28}, \
   {CSL_EDMA3_QUE_0,29}, \
   {CSL_EDMA3_QUE_0,30}, \
   {CSL_EDMA3_QUE_0,31}, \
   {CSL_EDMA3_QUE_0,32}, \
   {CSL_EDMA3_QUE_0,33}, \
   {CSL_EDMA3_QUE_0,34}, \
   {CSL_EDMA3_QUE_0,35}, \
   {CSL_EDMA3_QUE_0,36}, \
   {CSL_EDMA3_QUE_0,37}, \
   {CSL_EDMA3_QUE_0,38}, \
   {CSL_EDMA3_QUE_0,39}, \
   {CSL_EDMA3_QUE_0,40}, \
   {CSL_EDMA3_QUE_0,41}, \
   {CSL_EDMA3_QUE_0,42}, \
   {CSL_EDMA3_QUE_0,43}, \
   {CSL_EDMA3_QUE_0,44}, \
   {CSL_EDMA3_QUE_0,45}, \
   {CSL_EDMA3_QUE_0,46}, \
   {CSL_EDMA3_QUE_0,47}, \
   {CSL_EDMA3_QUE_0,48}, \
   {CSL_EDMA3_QUE_0,49}, \
   {CSL_EDMA3_QUE_0,50}, \
   {CSL_EDMA3_QUE_0,51}, \
   {CSL_EDMA3_QUE_0,52}, \
   {CSL_EDMA3_QUE_0,53}, \
   {CSL_EDMA3_QUE_0,54}, \
   {CSL_EDMA3_QUE_0,55}, \
   {CSL_EDMA3_QUE_0,56}, \
   {CSL_EDMA3_QUE_0,57}, \
   {CSL_EDMA3_QUE_0,58}, \
   {CSL_EDMA3_QUE_0,59}, \
   {CSL_EDMA3_QUE_0,60}, \
   {CSL_EDMA3_QUE_0,61}, \
   {CSL_EDMA3_QUE_0,62}, \
   {CSL_EDMA3_QUE_0,63} \
}

/** QDMA Channel Setup  */
#define CSL_EDMA3_QDMA_CHANNELSETUP_DEFAULT   {       \
   {CSL_EDMA3_QUE_0,64,CSL_EDMA3_TRIGWORD_DEFAULT}, \
   {CSL_EDMA3_QUE_0,65,CSL_EDMA3_TRIGWORD_DEFAULT}, \
   {CSL_EDMA3_QUE_0,66,CSL_EDMA3_TRIGWORD_DEFAULT}, \
   {CSL_EDMA3_QUE_0,67,CSL_EDMA3_TRIGWORD_DEFAULT}  \
}

// Select instance of VCP2 (0-3)
//#define  CSL_EDMA3_CHA_VCP2REVT(vcp2Inst)   (CSL_TPCC2_VCP0REVT + (vcp2Inst * 2))
//#define  CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst)   (CSL_TPCC2_VCP0XEVT + (vcp2Inst * 2))
#define  CSL_EDMA3_CHA_VCP2REVT(vcp2Inst)   (CSL_TPCC2_VCP0REVT + (vcp2Inst * 2)) //cs 15/04/12
#define  CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst)   (CSL_TPCC2_VCP0XEVT + (vcp2Inst * 2))//cs 15/04/12
#define  VCP2_CONTROL_BASE(vcp2Inst)        (CSL_VCP2_A_CFG_REGS + (vcp2Inst * 0x4000)) 
#define  VCP2_DATA_BASE(vcp2Inst)           (CSL_VCP2_A_DATA_REGS + (vcp2Inst * 0x100000))

#define VCP2_XMT_TCC(vcp2Inst)          CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst)
#define VCP2_RCV_TCC(vcp2Inst)          CSL_EDMA3_CHA_VCP2REVT(vcp2Inst)


#define  VCP2_ICMEM_ADDR(vcp2Inst)      (VCP2_DATA_BASE(vcp2Inst) + 0x000000u)
#define  VCP2_OPMEM_ADDR(vcp2Inst)      (VCP2_DATA_BASE(vcp2Inst) + 0x000048u)
#define  VCP2_BMMEM_ADDR(vcp2Inst)      (VCP2_DATA_BASE(vcp2Inst) + 0x000080u)
#define  VCP2_HDMEM_ADDR(vcp2Inst)      (VCP2_DATA_BASE(vcp2Inst) + 0x0000C0u)
#define  VCP2_END_ADDR(vcp2Inst)        (VCP2_CONTROL_BASE(vcp2Inst) + 0x000020)
#define  VCP2_EXE_ADDR(vcp2Inst)        (VCP2_CONTROL_BASE(vcp2Inst) + 0x000018)
#define  VCP2_STATUS0_ADDR(vcp2Inst)    (VCP2_CONTROL_BASE(vcp2Inst) + 0x000040)
#define  VCP2_STATUS1_ADDR(vcp2Inst)    (VCP2_CONTROL_BASE(vcp2Inst) + 0x000044)
#define  VCP2_ERROR_ADDR(vcp2Inst)      (VCP2_CONTROL_BASE(vcp2Inst) + 0x000050)
#define  VCP2_EMU_ADDR(vcp2Inst)        (VCP2_CONTROL_BASE(vcp2Inst) + 0x000060)

#define  PSVR_BASE                 0x02350000 
#define  PSVR_LCKREG               (PSVR_BASE + 0x4)
#define  PSVR_MDCTL0               (PSVR_BASE + 0x8)
#define  PSVR_MDSTAT0              (PSVR_BASE + 0x14)
#define  PSVR_VCP2_EN              0x5
#define  PSVR_VCP2_LCKVAL          0x0F0A0B00
#define  PSVR_VCP2_MDSTAT_MASK     0x38
#define  PSVR_VCP2_MDSTAT_EN       0x8
#define  VCP2_END_SLEEP_EN         0x300

//#define PSC_START_ADDRESS      0x02350000
//#define PSC_PTSTAT             PSC_START_ADDRESS + 0x00000128
//#define PSC_PTCMD              PSC_START_ADDRESS + 0x00000120
//#define PSC_MDCTL10            PSC_START_ADDRESS + 0x00000a28
//#define PSC_PDCTL5             PSC_START_ADDRESS + 0x00000314  

#define MAX_VCP2_FRAME_LEN 	(10*1024)
#define MAX_VCP2_TAIL_LEN 	8
#define FMAX 	120

/* DUMMY PARAM Number*/
#define DUMMY_PARAM_NUM  127

//Multiple PaRAM are used to support decoding multiple user channels one time 
/* start of VCP2 PARAM */
#define VCP2_0_PARAM_OFFSET  128

/* Length of TCP2 PARAM */
#define VCP2_PARAM_LEN  64

#define  VCP2_PARAM_OFFSET(vcp2Inst)   (VCP2_0_PARAM_OFFSET + (vcp2Inst * VCP2_PARAM_LEN)) 


/*How many user channels to be proccessed every time start VCP2*/
//Max value =VCP2_PARAM_LEN/5, since every user channels need 4 PaRAM
#define VCP2_CHANNELS_PER_TIME  1//(VCP2_PARAM_LEN/4)

#define VCP2_BM_FIFO_SIZE 	128
#define VCP2_HD_FIFO_SIZE 	256

/* Coprocessor specifics */
#define VCP2_NUM_IC 6
#define VCP2_NUM_OP 2


//following is the default EDMA configration for VCP2
//EDMA option for VCP2 Input Configuration
#define  VCP2_INPUT_CONFIG_EDMA_OPT CSL_EDMA3_OPT_MAKE(\
	CSL_EDMA3_ITCCH_DIS,\
	CSL_EDMA3_TCCH_DIS,\
	CSL_EDMA3_ITCINT_DIS,\
	CSL_EDMA3_TCINT_DIS,\
	0,\
	CSL_EDMA3_TCC_NORMAL,\
	CSL_EDMA3_FIFOWIDTH_NONE,\
	CSL_EDMA3_STATIC_DIS,\
	CSL_EDMA3_SYNC_A,\
	CSL_EDMA3_ADDRMODE_INCR,\
	CSL_EDMA3_ADDRMODE_INCR\
)
/*  cs 15/04/23
 * TCC=0,
 * CSL_EDMA3_TCC_NORMAL :a transfer is considered completed after the data has been transferred
 * FIFIWEDTH=CSL_EDMA3_FIFOWIDTH_NONE  0
 * CSL_EDMA3_STATIC_DIS: it may link
 * CSL_EDMA3_SYNC_A:A-synchronized
 * */

//EDMA option for VCP2 Branch Metrics
#define  VCP2_BRANCH_METRICS_EDMA_OPT(vcp2Inst) CSL_EDMA3_OPT_MAKE(\
	CSL_EDMA3_ITCCH_DIS,\
	CSL_EDMA3_TCCH_DIS,\
	CSL_EDMA3_ITCINT_DIS,\
	CSL_EDMA3_TCINT_DIS,\
	CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst) , \
	CSL_EDMA3_TCC_NORMAL,\
	CSL_EDMA3_FIFOWIDTH_64BIT,\
	CSL_EDMA3_STATIC_DIS,\
	CSL_EDMA3_SYNC_A,\
	CSL_EDMA3_ADDRMODE_CONST,\
	CSL_EDMA3_ADDRMODE_INCR\
)
/*  cs 15/04/23
 * TCC=CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst),
 * CSL_EDMA3_TCC_NORMAL :a transfer is considered completed after the data has been transferred
 * FIFIWEDTH=64bit
 * CSL_EDMA3_STATIC_DIS: it may link
 * CSL_EDMA3_SYNC_A:A-synchronized
 * DAM:FIFO
 * SAM:NOT FIFO
 * */

//EDMA option for VCP2 Hard Decision
#define  VCP2_HARD_DECISION_EDMA_OPT(vcp2Inst) CSL_EDMA3_OPT_MAKE(\
	CSL_EDMA3_ITCCH_DIS,\
	CSL_EDMA3_TCCH_DIS,\
	CSL_EDMA3_ITCINT_DIS,\
	CSL_EDMA3_TCINT_DIS,\
	CSL_EDMA3_CHA_VCP2REVT(vcp2Inst) , \
	CSL_EDMA3_TCC_NORMAL,\
	CSL_EDMA3_FIFOWIDTH_64BIT,\
	CSL_EDMA3_STATIC_DIS,\
	CSL_EDMA3_SYNC_A,\
	CSL_EDMA3_ADDRMODE_INCR,\
	CSL_EDMA3_ADDRMODE_CONST\
)
/*  cs 15/07/13
 * TCC=CSL_EDMA3_CHA_VCP2REVT(vcp2Inst),to the next vcp receviment event
 * CSL_EDMA3_TCC_NORMAL :a transfer is considered completed after the data has been transferred
 * FIFIWEDTH=64bit
 * CSL_EDMA3_STATIC_DIS: it may link
 * CSL_EDMA3_SYNC_A:A-synchronized
 * DAM:not FIFO
 * SAM:FIFO
 * */

//EDMA option for VCP2 Hard Decision in tailed mode
#define  VCP2_TAILED_HARD_DECISION_EDMA_OPT(vcp2Inst) CSL_EDMA3_OPT_MAKE(\
	CSL_EDMA3_ITCCH_DIS,\
	CSL_EDMA3_TCCH_DIS,\
	CSL_EDMA3_ITCINT_DIS,\
	CSL_EDMA3_TCINT_DIS,\
	CSL_EDMA3_CHA_VCP2REVT(vcp2Inst) , \
	CSL_EDMA3_TCC_NORMAL,\
	CSL_EDMA3_FIFOWIDTH_64BIT,\
	CSL_EDMA3_STATIC_DIS,\
	CSL_EDMA3_SYNC_AB,\
	CSL_EDMA3_ADDRMODE_INCR,\
	CSL_EDMA3_ADDRMODE_CONST\
)
//EDMA option for VCP2 Output Parameter
#define  VCP2_OUTPUT_PARAM_EDMA_OPT(vcp2Inst) CSL_EDMA3_OPT_MAKE(\
	CSL_EDMA3_ITCCH_DIS,\
	CSL_EDMA3_TCCH_DIS,\
	CSL_EDMA3_ITCINT_DIS,\
	CSL_EDMA3_TCINT_DIS,\
	CSL_EDMA3_CHA_VCP2REVT(vcp2Inst) , \
	CSL_EDMA3_TCC_NORMAL,\
	CSL_EDMA3_FIFOWIDTH_NONE,\
	CSL_EDMA3_STATIC_DIS,\
	CSL_EDMA3_SYNC_A,\
	CSL_EDMA3_ADDRMODE_INCR,\
	CSL_EDMA3_ADDRMODE_INCR\
)
/*  cs 15/07/13
 * TCC=CSL_EDMA3_CHA_VCP2REVT(vcp2Inst),to the next vcp receviment event
 * CSL_EDMA3_TCC_NORMAL :a transfer is considered completed after the data has been transferred
 * FIFIWEDTH=CSL_EDMA3_FIFOWIDTH_NONE 0
 * CSL_EDMA3_STATIC_DIS: it may link
 * CSL_EDMA3_SYNC_A:A-synchronized
 * DAM:not FIFO
 * SAM:not FIFO
 * */

typedef struct VCP2_Edma3_Config {
	Uint32 inputConfigAddr;
	Uint32 branchMetricsAddr;
	Uint32 hardDecisionAddr;
	Uint32 outParmAddr;
	Uint32 outParmFlag; /**< Output parameters read flag */

	/** Traceback mode */
	Uint8 traceBack;
	/** Branch metrics buffer length in input FIFO */
	Uint32 bmBuffLen;
	/** Decisions buffer length in output FIFO */
	Uint32 decBuffLen;
	/** Number of branch metric frames*/
	Uint32 numBmFrames;
	/** Number of decision frames */
	Uint32 numDecFrames;

	Uint32 inputConfigPaRAMIndex;
	Uint32 branchMetricsPaRAMIndex; 
	Uint32 decisionPaRAMIndex;
	Uint32 outPaRAMIndex;
	Uint32 nextInputPaRAMIndex;
	Uint32 nextDecisionPaRAMIndex;		
}VCP2_Edma3_Config ;


/****************************************\
 * VCP global typedef declarations
\****************************************/
typedef Uint32 VCP2_Standard;
typedef Uint32 VCP2_Mode;
typedef Uint32 VCP2_Map;
typedef Uint8  VCP2_UserData;
typedef Uint8  VCP2_ExtrinsicData;

/****************************************/
/* VCP2 global enable function declarations */
/****************************************/
Uint32 readPeriph();
void writePeriph();


/*****************************************/
/** VCP inline function declarations */
/*****************************************/
#ifdef USEDEFS

// Macros are used for filling Register field from input parameters 

#define CSL_VCP2_IC0_FMKR(poly3,poly2,poly1,poly0) \
    (Uint32)(\
            CSL_FMKR(31,24,poly3) \
            |CSL_FMKR(23,16,poly2) \
            |CSL_FMKR(15,8,poly1) \
            |CSL_FMKR(7,0,poly0))

#define CSL_VCP2_IC1_FMKR(CSL_VCP2_VCPIC1_YAMEN_ENABLE,yamt) \
    (Uint32)(\
            CSL_FMKR(28,28,CSL_VCP2_VCPIC1_YAMEN_ENABLE) \
            |CSL_FMKR(27,16,yamt))

#define CSL_VCP2_IC2_FMKR(r,f) \
    (Uint32)(\
            CSL_FMKR(31,16,r) \
            |CSL_FMKR(15,0,f))

#define CSL_VCP2_IC3_FMKR(out_order,itben,itbi,c) \
    (Uint32)(\
            CSL_FMKR(28,28,out_order) \
            |CSL_FMKR(24,24,itben) \
            |CSL_FMKR(23,16,itbi) \
            |CSL_FMKR(15,0,c))

#define CSL_VCP2_IC4_FMKR(imins,imaxs) \
    (Uint32)(\
            CSL_FMKR(28,16,imins) \
            |CSL_FMKR(12,0,imaxs))

#define CSL_VCP2_IC5_FMKR(sdhd,outf,tb,symr,symx,imaxi) \
    (Uint32)(\
            CSL_FMKR(31,31,sdhd) \
            |CSL_FMKR(30,30,outf) \
            |CSL_FMKR(29,28,tb) \
            |CSL_FMKR(24,20,symr) \
            |CSL_FMKR(19,16,symx) \
            |CSL_FMKR(7,0,imaxi))

/*----------------------------------------------------------------------------*/
#endif  /*USEDEFS*/ 

#endif /* _CSL_VCP2_testInit_H_ */
/******************************************************************************\
 * End of csl_vcp.h
 \******************************************************************************/

