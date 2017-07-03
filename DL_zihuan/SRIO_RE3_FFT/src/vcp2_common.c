/*****************************************************************************\
*           TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION           
*                                                                            
*  Property of Texas Instruments 
*  For  Unrestricted  Internal  Use  Only 
*  Unauthorized reproduction and/or distribution is strictly prohibited.  
*  This product is protected under copyright law and trade secret law 
*  as an unpublished work.  
*  Created 2011, (C) Copyright 2011 Texas Instruments.  All rights reserved.
*------------------------------------------------------------------------------
*  Filename       : vcp2_edma3.c
*  Description    : Common Functions for configuring the EDMA3 for VCP2
\*****************************************************************************/
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>
#include <csl_intc.h>
#include <csl_cpIntc.h>
#include <ti/csl/csl_cpIntcAux.h>

#include "stdio.h"
#include "vcp2_testInit.h"
#include "KeyStone_common.h"

/* VCP input configuration transfer size */
#define VCPIC_DMA_SIZE  24
/* VCP out register transfer size */
#define VCPOUT_DMA_SIZE  8

CSL_Edma3ChannelHandle
            hEdmaVcpRecv = NULL,  /* EDMA channel VCP2 Receive               */
            hEdmaVcpXmit = NULL;  /* EDMA channel VCP2 Transmit              */
        
CSL_Edma3ChannelObj ChObjVcpXmit, ChObjVcpRecv;

CSL_Edma3Handle       	hEdma; 
CSL_Edma3Obj          	edmaObj;
CSL_Edma3ChannelErr 	chErrClear;

CSL_IntcHandle       hIntcEdma;
CSL_IntcHandle              hIntcVcp;
CSL_CPINTC_Handle           hCpintc;

CSL_IntcObj          intcObjEdma;	
CSL_IntcObj          intcObjVcp;
CSL_IntcEventHandlerRecord  eventHandler[2];
CSL_IntcGlobalEnableState   state;  //dummy var since not used to restore state later
CSL_Edma3CmdIntr            regionIntrQuerry;

CSL_Edma3CmdIntr           regionIntr;
CSL_Vcp2RegsOvly 	     hVcp2Vbus; 


volatile Uint32 VcpError = 0;
volatile Uint32 RxCount = 0;

extern VCP2Handle ghVcp2;

// Function declarations
interrupt void EDMA_ISR(void);
interrupt void VCP2_ISR(void);

/****************************************************************************\
 \****************************************************************************/
Uint32 edma3Init1(Uint32 edmaRegionNum, Uint32 vcp2Inst)
{
    CSL_Status             modStatus;
	  CSL_Edma3CmdDrae       draeAttr;
	  CSL_Edma3ChannelAttr   channelParam;
    CSL_Edma3Context       edmaContext;

    CSL_Edma3HwSetup hwSetup;
    CSL_Edma3HwDmaChannelSetup dmahwSetup[CSL_EDMA3_TPCC2_NUM_DMACH ] =
        CSL_EDMA3_DMA_CHANNELSETUP_DEFAULT;//cs 15/04/23 there're 64 channels by default,each one map to queue 0 and param set 0-64 separately


    // Initialize and Open Module
    modStatus = CSL_edma3Init(&edmaContext);//cs 15/04/23 This is the initialization function for the EDMA CSL,Currently, the function just return status CSL_SOK,without doing anything
	  hEdma     = CSL_edma3Open(&edmaObj,CSL_TPCC_2,NULL,&modStatus);//cs 15/04/23 This function returns the handle to the EDMA instance(CSL_TPCC_2,this time)

    //Module Setup
    hwSetup.dmaChaSetup = &dmahwSetup[0];
    hwSetup.qdmaChaSetup = NULL;
    CSL_edma3HwSetup(hEdma,&hwSetup);  //cs   15/06/24 8354 cycles

	   // Setup the DRAE Masks
    draeAttr.region = edmaRegionNum;
    draeAttr.drae = 0xffffff00;//0xffffffff;0xffffffff;
    draeAttr.draeh = 0;

    modStatus = CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_DMAREGION_ENABLE,&draeAttr);  // cs 15/04/23 DRAE Enable(Bits 0-31) for the Shadow Region 0.
#if 0
  	// VCP Channel Open
	channelParam.regionNum = edmaRegionNum;//cs 15/04/13  the Shadow region number which channel mapped to
	channelParam.chaNum    = CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst); // VCP Xmit 11  cs 15/04/13 /** Channel number */
    hEdmaVcpXmit           = CSL_edma3ChannelOpen(&ChObjVcpXmit,CSL_TPCC_2,&channelParam,&modStatus);//cs 15/04/23 The API returns a handle for the specified EDMA Channel for use
    channelParam.chaNum    = CSL_EDMA3_CHA_VCP2REVT(vcp2Inst); // VCP Recv 10
    hEdmaVcpRecv           = CSL_edma3ChannelOpen(&ChObjVcpRecv,CSL_TPCC_2,&channelParam,&modStatus);

	// VCP Channel Setup
	CSL_edma3HwChannelSetupQue(hEdmaVcpXmit,CSL_EDMA3_QUE_0);//cs 15/04/13 Map the channel EdmaVcpXmit to Event Queue 0
	CSL_edma3HwChannelSetupQue(hEdmaVcpRecv,CSL_EDMA3_QUE_0);
#endif
	return 0;
 }
Uint32 edma3Init2(Uint32 edmaRegionNum, Uint32 vcp2Inst)
{
    CSL_Status             modStatus;
	  CSL_Edma3CmdDrae       draeAttr;
	  CSL_Edma3ChannelAttr   channelParam;
    CSL_Edma3Context       edmaContext;

    CSL_Edma3HwSetup hwSetup;
    CSL_Edma3HwDmaChannelSetup dmahwSetup[CSL_EDMA3_TPCC2_NUM_DMACH ] =
        CSL_EDMA3_DMA_CHANNELSETUP_DEFAULT;//cs 15/04/23 there're 64 channels by default,each one map to queue 0 and param set 0-64 separately

#if 0
    // Initialize and Open Module
    modStatus = CSL_edma3Init(&edmaContext);//cs 15/04/23 This is the initialization function for the EDMA CSL,Currently, the function just return status CSL_SOK,without doing anything
	  hEdma     = CSL_edma3Open(&edmaObj,CSL_TPCC_2,NULL,&modStatus);//cs 15/04/23 This function returns the handle to the EDMA instance(CSL_TPCC_2,this time)

    //Module Setup
    hwSetup.dmaChaSetup = &dmahwSetup[0];
    hwSetup.qdmaChaSetup = NULL;
    CSL_edma3HwSetup(hEdma,&hwSetup);  //cs   15/06/24 8354 cycles

	   // Setup the DRAE Masks
    draeAttr.region = edmaRegionNum;
    draeAttr.drae = 0xffffffff;
    draeAttr.draeh = 0;

    modStatus = CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_DMAREGION_ENABLE,&draeAttr);  // cs 15/04/23 DRAE Enable(Bits 0-31) for the Shadow Region 0.
#endif
  	// VCP Channel Open
    modStatus=1;  //cs 15/06/25
	channelParam.regionNum = edmaRegionNum;//cs 15/04/13  the Shadow region number which channel mapped to
	channelParam.chaNum    = CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst); // VCP Xmit 11  cs 15/04/13 /** Channel number */
    hEdmaVcpXmit           = CSL_edma3ChannelOpen(&ChObjVcpXmit,CSL_TPCC_2,&channelParam,&modStatus);//cs 15/04/23 The API returns a handle for the specified EDMA Channel for use
    channelParam.chaNum    = CSL_EDMA3_CHA_VCP2REVT(vcp2Inst); // VCP Recv 10
    hEdmaVcpRecv           = CSL_edma3ChannelOpen(&ChObjVcpRecv,CSL_TPCC_2,&channelParam,&modStatus);

	// VCP Channel Setup
	CSL_edma3HwChannelSetupQue(hEdmaVcpXmit,CSL_EDMA3_QUE_0);//cs 15/04/13 Map the channel EdmaVcpXmit to Event Queue 0
	CSL_edma3HwChannelSetupQue(hEdmaVcpRecv,CSL_EDMA3_QUE_0);

	return 0;
 }
Uint32 edma3Init(Uint32 edmaRegionNum, Uint32 vcp2Inst)
{
    CSL_Status             modStatus;
	  CSL_Edma3CmdDrae       draeAttr;
	  CSL_Edma3ChannelAttr   channelParam;
    CSL_Edma3Context       edmaContext;

    CSL_Edma3HwSetup hwSetup;
    CSL_Edma3HwDmaChannelSetup dmahwSetup[CSL_EDMA3_TPCC2_NUM_DMACH ] =
        CSL_EDMA3_DMA_CHANNELSETUP_DEFAULT;//cs 15/04/23 there're 64 channels by default,each one map to queue 0 and param set 0-64 separately


    // Initialize and Open Module
    modStatus = CSL_edma3Init(&edmaContext);//cs 15/04/23 This is the initialization function for the EDMA CSL,Currently, the function just return status CSL_SOK,without doing anything
	  hEdma     = CSL_edma3Open(&edmaObj,CSL_TPCC_2,NULL,&modStatus);//cs 15/04/23 This function returns the handle to the EDMA instance(CSL_TPCC_2,this time)

    //Module Setup
    hwSetup.dmaChaSetup = &dmahwSetup[0];
    hwSetup.qdmaChaSetup = NULL;
    CSL_edma3HwSetup(hEdma,&hwSetup);

	   // Setup the DRAE Masks
    draeAttr.region = edmaRegionNum;    
    draeAttr.drae = 0xffffff00;//0xffffffff;
    draeAttr.draeh = 0; 
    
    modStatus = CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_DMAREGION_ENABLE,&draeAttr);  // cs 15/04/23 DRAE Enable(Bits 0-31) for the Shadow Region 0.

  	// VCP Channel Open
	channelParam.regionNum = edmaRegionNum;//cs 15/04/13  the Shadow region number which channel mapped to
	channelParam.chaNum    = CSL_EDMA3_CHA_VCP2XEVT(vcp2Inst); // VCP Xmit  cs 15/04/13 /** Channel number */
    hEdmaVcpXmit           = CSL_edma3ChannelOpen(&ChObjVcpXmit,CSL_TPCC_2,&channelParam,&modStatus);//cs 15/04/23 The API returns a handle for the specified EDMA Channel for use
    channelParam.chaNum    = CSL_EDMA3_CHA_VCP2REVT(vcp2Inst); // VCP Recv 
    hEdmaVcpRecv           = CSL_edma3ChannelOpen(&ChObjVcpRecv,CSL_TPCC_2,&channelParam,&modStatus);
 
	// VCP Channel Setup
	CSL_edma3HwChannelSetupQue(hEdmaVcpXmit,CSL_EDMA3_QUE_0);//cs 15/04/13 Map the channel EdmaVcpXmit to Event Queue 0
	CSL_edma3HwChannelSetupQue(hEdmaVcpRecv,CSL_EDMA3_QUE_0);

	return 0;
 }


Uint32 edma3Release(Uint32 edmaRegionNum)
{
   CSL_Edma3CmdDrae   draeAttr;
   if (hEdmaVcpXmit) {CSL_edma3ChannelClose(hEdmaVcpXmit);hEdmaVcpXmit=NULL;}
   if (hEdmaVcpRecv) {CSL_edma3ChannelClose(hEdmaVcpRecv);hEdmaVcpRecv=NULL;}

	// Setup the DRAE Masks
    draeAttr.region = edmaRegionNum;
   draeAttr.drae = 0xffffffff;
    draeAttr.draeh  = 0x0;
	CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_DMAREGION_DISABLE,&draeAttr);

    CSL_edma3Close(hEdma);   

   return(0);
}

/****************************************************************************\
 * vcp2SubmitEdma: The EDMA is programmed to service the user channel.The IC*
 * values are configured and all EDMA parameters are calculated here.       *
\****************************************************************************/
Uint32 vcp2SubmitEdma(VCP2_UserData *hUserData, VCP2_Params *vcpParameters, VCP2_ConfigIc *configIc,Uint32 *decisions, Uint32 *outParms, VCP2Handle hVcp2)
{
CSL_Edma3ParamSetup     edmaConfig;
VCP2_Rate rate       = vcpParameters->rate;
Uint16 symx         = vcpParameters->bmBuffLen;
Uint16 numBmFrames  = vcpParameters->numBmFrames;
Uint16 numDecFrames = vcpParameters->numDecFrames;
Uint16 symr         = vcpParameters->decBuffLen;
Uint8 traceBack     = vcpParameters->traceBack;
Uint8 readFlag		= vcpParameters->readFlag;
Uint32 numBm;
CSL_Status  Status;

CSL_Edma3ParamHandle 
            hParamVcpIc,          /* EDMA channel used for IC Values         */
            hParamVcpBrMet,       /* EDMA channel used for Branch Metrics    */
            hParamVcpDec,         /* EDMA channel used for Hard Decisions    */                                 
            hParamVcpOutPar,      /* EDMA channel used for Output Parameters */
            
			      hParamDummy;          /* EDMA dummy set of parameters */

 
    hVcp2Vbus =  hVcp2->regs;
  
    hParamVcpIc            = CSL_edma3GetParamHandle(hEdmaVcpXmit,CSL_EDMA3_CHA_VCP2XEVT(hVcp2->perNum),&Status);
	hParamVcpBrMet         = CSL_edma3GetParamHandle(hEdmaVcpXmit,(VCP2_XMT_TCC(hVcp2->perNum) + 8),&Status);
	hParamVcpDec           = CSL_edma3GetParamHandle(hEdmaVcpRecv,CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum),&Status);
	hParamVcpOutPar        = CSL_edma3GetParamHandle(hEdmaVcpRecv,(VCP2_XMT_TCC(hVcp2->perNum) + 16),&Status);
	hParamDummy            = CSL_edma3GetParamHandle(hEdmaVcpXmit,(VCP2_XMT_TCC(hVcp2->perNum) + 24),&Status);

	/* Configure dummy channel - used to avoid event missed situations */
	edmaConfig.option      = 0; edmaConfig.srcAddr     = 0;
    edmaConfig.dstAddr     = 0; edmaConfig.aCntbCnt    = 0;
    edmaConfig.cCnt        = 1; edmaConfig.srcDstBidx  = 0;
	edmaConfig.srcDstCidx  = 0; edmaConfig.linkBcntrld = 0x0000FFFF;

	CSL_edma3ParamSetup(hParamDummy,&edmaConfig);


    /* Configure channel parameters for IC registers */
    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,      CSL_EDMA3_TCCH_DIS,
                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_DIS,
					                       0,                        CSL_EDMA3_TCC_NORMAL,
                                           CSL_EDMA3_FIFOWIDTH_NONE, CSL_EDMA3_STATIC_DIS,
                                           CSL_EDMA3_SYNC_AB,        CSL_EDMA3_ADDRMODE_INCR,
					                       CSL_EDMA3_ADDRMODE_INCR);

    edmaConfig.srcAddr     = (Uint32)GLOBAL_ADDR(configIc);
    edmaConfig.dstAddr     = (Uint32)&(hVcp2Vbus->VCPIC0);
    edmaConfig.aCntbCnt    = CSL_EDMA3_CNT_MAKE (VCPIC_DMA_SIZE, 1);
    edmaConfig.cCnt        = 1;
    edmaConfig.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(0,0);
	edmaConfig.srcDstCidx  = CSL_EDMA3_CIDX_MAKE(0,0);
    edmaConfig.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(hParamVcpBrMet,1);
	CSL_edma3ParamSetup(hParamVcpIc,&edmaConfig);

    /* Configure channel parameters for branch metrics data */
	numBm = 4 << (rate - 1); 

    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,      CSL_EDMA3_TCCH_DIS,
                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_DIS,
					                       0,                        CSL_EDMA3_TCC_NORMAL,
                                           CSL_EDMA3_FIFOWIDTH_64BIT,CSL_EDMA3_STATIC_DIS,
                                           CSL_EDMA3_SYNC_A,         CSL_EDMA3_ADDRMODE_CONST,
					                       CSL_EDMA3_ADDRMODE_INCR);


    edmaConfig.srcAddr     = (Uint32)GLOBAL_ADDR(hUserData);
    edmaConfig.dstAddr     = (Uint32)&(hVcp2Vbus->VCPWBM);
    edmaConfig.aCntbCnt    = CSL_EDMA3_CNT_MAKE(numBm * (symx+1),numBmFrames);
	edmaConfig.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(numBm * (symx+1),0);
	edmaConfig.srcDstCidx  = CSL_EDMA3_CIDX_MAKE(0,0);
    edmaConfig.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(hParamDummy,1);
	CSL_edma3ParamSetup(hParamVcpBrMet,&edmaConfig);
    
    if (readFlag){
    	/* Configure channel parameters for decision data */
		if (traceBack == VCP2_TRACEBACK_TAILED)
		    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,      CSL_EDMA3_TCCH_DIS,
		                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_DIS,
							                       0,                        CSL_EDMA3_TCC_EARLY,
		                                           CSL_EDMA3_FIFOWIDTH_64BIT,CSL_EDMA3_STATIC_DIS,
		                                           CSL_EDMA3_SYNC_AB,         CSL_EDMA3_ADDRMODE_INCR,
							                       CSL_EDMA3_ADDRMODE_CONST);

		else
		    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,      CSL_EDMA3_TCCH_DIS,
		                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_DIS,
							                       0,                        CSL_EDMA3_TCC_EARLY,
		                                           CSL_EDMA3_FIFOWIDTH_64BIT,CSL_EDMA3_STATIC_DIS,
		                                           CSL_EDMA3_SYNC_AB,        CSL_EDMA3_ADDRMODE_INCR,
							                       CSL_EDMA3_ADDRMODE_CONST);

		
    }else{
    	/* Configure channel parameters for decision data */
		if (traceBack == VCP2_TRACEBACK_TAILED)
		    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,    CSL_EDMA3_TCCH_DIS,
		                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_EN,
							                        CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum),                   CSL_EDMA3_TCC_NORMAL,
		                                           CSL_EDMA3_FIFOWIDTH_64BIT,CSL_EDMA3_STATIC_DIS,
		                                           CSL_EDMA3_SYNC_AB,         CSL_EDMA3_ADDRMODE_INCR,
							                       CSL_EDMA3_ADDRMODE_CONST);

		else
		    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,    CSL_EDMA3_TCCH_DIS,
		                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_EN,
							                        CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum),                   CSL_EDMA3_TCC_NORMAL,
		                                           CSL_EDMA3_FIFOWIDTH_64BIT,CSL_EDMA3_STATIC_DIS,
		                                           CSL_EDMA3_SYNC_AB,        CSL_EDMA3_ADDRMODE_INCR,
							                       CSL_EDMA3_ADDRMODE_CONST);

    }	

    edmaConfig.srcAddr     = (Uint32)&(hVcp2Vbus->VCPRDECS);
    if (traceBack == VCP2_TRACEBACK_TAILED){
    	edmaConfig.dstAddr     = (Uint32)GLOBAL_ADDR(((Uint32)(decisions + (2 * (symr) * (numDecFrames)))));
    	edmaConfig.aCntbCnt    = CSL_EDMA3_CNT_MAKE(8, (symr+1) * numDecFrames);
	    edmaConfig.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(0,(Uint32)0xFFF8);
	}else{
    	edmaConfig.dstAddr     = (Uint32)GLOBAL_ADDR(decisions);
    	edmaConfig.aCntbCnt    = CSL_EDMA3_CNT_MAKE(8 * (symr+1),numDecFrames);
	    edmaConfig.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(0,8 * (symr+1));
	}
    edmaConfig.cCnt        = 1;
	edmaConfig.srcDstCidx  = CSL_EDMA3_CIDX_MAKE(0,0);
    if (readFlag) edmaConfig.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(hParamVcpOutPar,1);
    else          edmaConfig.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(CSL_EDMA3_LINK_NULL,1);
	CSL_edma3ParamSetup(hParamVcpDec,&edmaConfig);	
    
    if (readFlag){    	

	    edmaConfig.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,    CSL_EDMA3_TCCH_DIS,
	                                           CSL_EDMA3_ITCINT_DIS,     CSL_EDMA3_TCINT_EN,
						                       CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum),                   CSL_EDMA3_TCC_NORMAL,
	                                           CSL_EDMA3_FIFOWIDTH_NONE, CSL_EDMA3_STATIC_DIS,
	                                           CSL_EDMA3_SYNC_AB,        CSL_EDMA3_ADDRMODE_INCR,
						                       CSL_EDMA3_ADDRMODE_INCR);


        edmaConfig.srcAddr     = (Uint32)&(hVcp2Vbus->VCPOUT0);
        edmaConfig.dstAddr     = (Uint32)GLOBAL_ADDR(outParms);
        edmaConfig.aCntbCnt    = CSL_EDMA3_CNT_MAKE(VCPOUT_DMA_SIZE,1);
        edmaConfig.cCnt        = 1;
        edmaConfig.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(0,0);
    	edmaConfig.srcDstCidx  = CSL_EDMA3_CIDX_MAKE(0,0);
        edmaConfig.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(CSL_EDMA3_LINK_NULL,1);
	    CSL_edma3ParamSetup(hParamVcpOutPar,&edmaConfig);
    }
	    
    /* Enable the transmit and receive channels */
    CSL_edma3HwChannelControl(hEdmaVcpXmit,CSL_EDMA3_CMD_CHANNEL_ENABLE,NULL);
    CSL_edma3HwChannelControl(hEdmaVcpRecv,CSL_EDMA3_CMD_CHANNEL_ENABLE,NULL);
	
     regionIntr.region = hVcp2->perNum; 
    regionIntr.intr  |= 1 << VCP2_RCV_TCC(hVcp2->perNum);
    regionIntr.intrh=0;
   CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_INTR_ENABLE,&regionIntr);                                                                             

    return(0);

} /* end vcp2SubmitEdma() */

/*
 * ============================================================================
 *   @func   Config_VCP2_EDMA3_PaRAM
 *
 *   @desc
 *     This function Setup EDMA PaRAM for one VCP2 processing task.
 *
 *  @arg  
 *      configration paramter for EDMA3 of VCP2
 *
 *  @return
 *      None
 * ============================================================================
*/
void Config_VCP2_EDMA3_PaRAM(VCP2_Edma3_Config * vcp2_Edma3_Config, Uint32 VCP2instNum)
{
	Uint32 inputConfigPaRAMIndex, branchMetricsPaRAMIndex; 
	Uint32 decisionPaRAMIndex, outPaRAMIndex;
	Uint32 uiPaRAMBaseAddr= (Uint32)gEDMACC2Regs->PARAMSET;
	
	inputConfigPaRAMIndex= vcp2_Edma3_Config->inputConfigPaRAMIndex;//cs 15/04/23 for perNum=0,11
	branchMetricsPaRAMIndex= vcp2_Edma3_Config->branchMetricsPaRAMIndex;//cs 15/04/23 for perNum=0,128
 	decisionPaRAMIndex= vcp2_Edma3_Config->decisionPaRAMIndex;//cs 15/04/23 for perNum=0,10
 	outPaRAMIndex= vcp2_Edma3_Config->outPaRAMIndex;//cs 15/04/23 for perNum=0,129

	//Setup Input Configuration PaRAM
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].OPT = VCP2_INPUT_CONFIG_EDMA_OPT;
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].SRC= (Uint32)GLOBAL_ADDR (vcp2_Edma3_Config->inputConfigAddr);
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].A_B_CNT= 
		CSL_EDMA3_CNT_MAKE (VCP2_NUM_IC<<2, 1);//cs 15/04/23 for perNum=0,acnt=0x18=24,bcnt=1(in accord with VCP2 user's guide 4-3)
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].DST=(Uint32) &hVcp2Vbus->VCPIC0; 
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].SRC_DST_BIDX= 0;
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].LINK_BCNTRLD= 
		(uiPaRAMBaseAddr+(branchMetricsPaRAMIndex<<5))&0xffff;//cs 15/04/23 link the  branchMetricsPaRAMIndex next time,    for perNum=0,left shift 5 because the 5LSBS of the link field should be cleared to 0
	gEDMACC2Regs->PARAMSET[inputConfigPaRAMIndex].CCNT= 1;

 	//Setup  Branch Metrics PaRAM
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].OPT = VCP2_BRANCH_METRICS_EDMA_OPT(VCP2instNum);
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].SRC= (Uint32)GLOBAL_ADDR (vcp2_Edma3_Config->branchMetricsAddr);
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].A_B_CNT= 
		CSL_EDMA3_CNT_MAKE (vcp2_Edma3_Config->bmBuffLen, vcp2_Edma3_Config->numBmFrames);//cs 15/04/23 for perNum=0,acnt=0x80=128,bcnt=4
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].DST=  (Uint32) & (hVcp2Vbus->VCPWBM);
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].SRC_DST_BIDX= 
		CSL_EDMA3_BIDX_MAKE(vcp2_Edma3_Config->bmBuffLen, 0) ;//cs 15/04/23 src=0x80=128,bmBuffLen   des=0
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].LINK_BCNTRLD= 
		(uiPaRAMBaseAddr+(vcp2_Edma3_Config->nextInputPaRAMIndex<<5))&0xffff;//cs 15/04/23 link the  nextInputPaRAMIndex next time,
	gEDMACC2Regs->PARAMSET[branchMetricsPaRAMIndex].CCNT= 1;

 	//Setup Hard Decision PaRAM
 	if(vcp2_Edma3_Config->traceBack== VCP2_TRACEBACK_TAILED)
 	{	//the hard decision is in reverse order for tailed mode, EDMA is configure to swap the HD to normal order
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].OPT = VCP2_TAILED_HARD_DECISION_EDMA_OPT(VCP2instNum);
		//vcp2_Edma3_Config->numDecFrames must less than 1, becasue frame length must less 2048 for tailed mode
		//So, only need one SYNC_AB transfer for all data
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].A_B_CNT= 
			CSL_EDMA3_CNT_MAKE (8, vcp2_Edma3_Config->decBuffLen/8);
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].SRC_DST_BIDX= 
			CSL_EDMA3_BIDX_MAKE(0, 0xFFF8) ; //destination index is -8
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].DST= vcp2_Edma3_Config->hardDecisionAddr
			+vcp2_Edma3_Config->decBuffLen -8;          
 	}
 	else
 	{
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].OPT = VCP2_HARD_DECISION_EDMA_OPT(VCP2instNum);
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].A_B_CNT= 
			CSL_EDMA3_CNT_MAKE (vcp2_Edma3_Config->decBuffLen, vcp2_Edma3_Config->numDecFrames);
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].SRC_DST_BIDX= 
			CSL_EDMA3_BIDX_MAKE(0, vcp2_Edma3_Config->decBuffLen) ;
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].DST=  (Uint32)GLOBAL_ADDR (vcp2_Edma3_Config->hardDecisionAddr);          /* Destination */
 	}
	gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].SRC=  (Uint32)&(hVcp2Vbus->VCPRDECS);
	gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].CCNT= 1;

	if(vcp2_Edma3_Config->outParmFlag) 	//if Read output parameter
	{
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].LINK_BCNTRLD= 
			(uiPaRAMBaseAddr+(outPaRAMIndex<<5))&0xffff;
	 	//Setup Output Parameter PaRAM
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].OPT = VCP2_OUTPUT_PARAM_EDMA_OPT(VCP2instNum);
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].SRC= (Uint32) &hVcp2Vbus->VCPOUT0;
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].A_B_CNT= 
			CSL_EDMA3_CNT_MAKE (VCP2_NUM_OP<<2, 1);
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].DST=  (Uint32)GLOBAL_ADDR (vcp2_Edma3_Config->outParmAddr);
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].SRC_DST_BIDX= 0;
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].LINK_BCNTRLD= 
			(uiPaRAMBaseAddr+(vcp2_Edma3_Config->nextDecisionPaRAMIndex<<5))&0xffff;//cs 15/04/23 link the  nextDecisionPaRAMIndex next time,
		gEDMACC2Regs->PARAMSET[outPaRAMIndex].CCNT= 1;
	}
	else
	{
		gEDMACC2Regs->PARAMSET[decisionPaRAMIndex].LINK_BCNTRLD= 
			(uiPaRAMBaseAddr+(vcp2_Edma3_Config->nextDecisionPaRAMIndex<<5))&0xffff;
	}
}

/*
 * ============================================================================
 *   @func   vcp2SubmitEdma_multi_Channel
 *
 *   @desc
 *     This function Setup EDMA for VCP2
 *
 *  @arg  
 *      vcp2_Edma3_Config: EDMA parameters for VCP2
 *
 *  @return
 *      None
 * ============================================================================
*/
Uint32 vcp2SubmitEdma_multi_Channel(VCP2_Edma3_Config * vcp2_Edma3_Config, VCP2Handle hVcp2)
{
	Uint32 i, uiPaRAM_Index, uiLastRxIndex, uiLastTxIndex;
       CSL_Edma3ParamSetup     edmaConfig;
	
       /* Configure dummy channel - used to avoid event missed situations */
       edmaConfig.option      = 0; edmaConfig.srcAddr     = 0;
       edmaConfig.dstAddr     = 0; edmaConfig.aCntbCnt    = 0;
       edmaConfig.cCnt        = 1; edmaConfig.srcDstBidx  = 0;
	edmaConfig.srcDstCidx  = 0; edmaConfig.linkBcntrld = 0x0000FFFF;

	CSL_edma3ParamSetup((CSL_Edma3ParamHandle)&gEDMACC2Regs->PARAMSET[DUMMY_PARAM_NUM],&edmaConfig);


	/* clear the EDMA error registers */
	chErrClear.missed = TRUE;
	chErrClear.secEvt = TRUE;
	CSL_edma3HwChannelControl (hEdmaVcpXmit, CSL_EDMA3_CMD_CHANNEL_DISABLE, NULL);
	CSL_edma3HwChannelControl (hEdmaVcpRecv, CSL_EDMA3_CMD_CHANNEL_DISABLE, NULL);
	CSL_edma3HwChannelControl (hEdmaVcpXmit, CSL_EDMA3_CMD_CHANNEL_CLEARERR, &chErrClear);
	CSL_edma3HwChannelControl (hEdmaVcpRecv, CSL_EDMA3_CMD_CHANNEL_CLEARERR, &chErrClear);
	CSL_edma3HwChannelControl (hEdmaVcpXmit, CSL_EDMA3_CMD_CHANNEL_CLEAR, NULL);
	CSL_edma3HwChannelControl (hEdmaVcpRecv, CSL_EDMA3_CMD_CHANNEL_CLEAR, NULL);
	//Clear interrupt
	gEDMACC2Regs->TPCC_ICR= (1<<CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum));

	hVcp2Vbus =  hVcp2->regs;

	uiPaRAM_Index=VCP2_PARAM_OFFSET(hVcp2->perNum); 	//cs 15/04/23 calculate start of VCP2 PARAM according to the VCP2 instance num ,for 0 it is 128 ,VCP2_0_PARAM_OFFSET(128) + (vcp2Inst * VCP2_PARAM_LEN(64))

	//Config first user channel
	vcp2_Edma3_Config->inputConfigPaRAMIndex= CSL_EDMA3_CHA_VCP2XEVT(hVcp2->perNum);//cs 15/04/23 for perNum=0,11
	vcp2_Edma3_Config->branchMetricsPaRAMIndex= uiPaRAM_Index++; //cs 15/04/23 for perNum=0,128
	vcp2_Edma3_Config->decisionPaRAMIndex= CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum);//cs 15/04/23 for perNum=0,10
	if(vcp2_Edma3_Config->outParmFlag)
		vcp2_Edma3_Config->outPaRAMIndex= uiPaRAM_Index++;//cs 15/04/23 for perNum=0,129
	vcp2_Edma3_Config->nextInputPaRAMIndex= uiPaRAM_Index;//cs 15/04/23 for perNum=0,130
	vcp2_Edma3_Config->nextDecisionPaRAMIndex= uiPaRAM_Index+2;		//cs 15/04/23 for perNum=0,132
	//Config EDMA for one user channel
	Config_VCP2_EDMA3_PaRAM(vcp2_Edma3_Config, hVcp2->perNum);
	//Different hard decision buffer for different user channel, 
	//the purpose is to make sure every user channel are correct decoded
	vcp2_Edma3_Config->hardDecisionAddr+= ((MAX_VCP2_FRAME_LEN+7)/8+7)/8*8;

	//Config the rest user channels
	for(i=1; i< VCP2_CHANNELS_PER_TIME; i++) 
	{
		vcp2_Edma3_Config->inputConfigPaRAMIndex= uiPaRAM_Index++;//cs 15/04/23 for perNum=0,130   134
		vcp2_Edma3_Config->branchMetricsPaRAMIndex= uiPaRAM_Index++; //cs 15/04/23 for perNum=0,131  135
		vcp2_Edma3_Config->decisionPaRAMIndex= uiPaRAM_Index++;//cs 15/04/23 for perNum=0,132  136
		if(vcp2_Edma3_Config->outParmFlag)
			vcp2_Edma3_Config->outPaRAMIndex= uiPaRAM_Index++;//cs 15/04/23 for perNum=0,133  137
		vcp2_Edma3_Config->nextInputPaRAMIndex= uiPaRAM_Index;//cs 15/04/23 for perNum=0,134  138
		vcp2_Edma3_Config->nextDecisionPaRAMIndex= uiPaRAM_Index+2;	//cs 15/04/23 for perNum=0,136	140
		//Config EDMA for one user channel
		Config_VCP2_EDMA3_PaRAM(vcp2_Edma3_Config, hVcp2->perNum);
		vcp2_Edma3_Config->hardDecisionAddr+= ((MAX_VCP2_FRAME_LEN+7)/8+7)/8*8;
	}

	//Config the link and interrupt for last user channel
	if(vcp2_Edma3_Config->outParmFlag||VCP2_CHANNELS_PER_TIME>1)
		uiLastRxIndex= uiPaRAM_Index-1;//cs 15/04/23  189  VCP2_CHANNELS_PER_TIME=1: 129
	else
		uiLastRxIndex= CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum);
	//link last RX edma to NULL
	gEDMACC2Regs->PARAMSET[uiLastRxIndex].LINK_BCNTRLD= CSL_EDMA3_LINK_NULL;  
	//last RX edma generate interrupt
	gEDMACC2Regs->PARAMSET[uiLastRxIndex].OPT|= CSL_FMKR(20,20,CSL_EDMA3_TCINT_EN); //cs TCINTEN  transfer complete interrupt is enabled

	if(vcp2_Edma3_Config->outParmFlag)
	{
		if((VCP2_CHANNELS_PER_TIME>1))
		 	uiLastTxIndex= uiPaRAM_Index-3;
		else
		 	uiLastTxIndex= uiPaRAM_Index-2;
	}
	else		
	{
		if((VCP2_CHANNELS_PER_TIME>1))
		 	uiLastTxIndex= uiPaRAM_Index-2;
		else
		 	uiLastTxIndex= uiPaRAM_Index-1;
	}
	//Link last TX edma to DUMMY
	gEDMACC2Regs->PARAMSET[uiLastTxIndex].LINK_BCNTRLD=
		( (Uint32)gEDMACC2Regs->PARAMSET+(DUMMY_PARAM_NUM<<5))&0xffff;		//cs 15/07/14 niLastTxIndex 128

	/* Enable Channel */
	CSL_edma3HwChannelControl (hEdmaVcpXmit, CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL);
	CSL_edma3HwChannelControl (hEdmaVcpRecv, CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL);
#if 0
 	regionIntr.region = hVcp2->perNum; 
       regionIntr.intr  |= 1 << VCP2_RCV_TCC(hVcp2->perNum);
      regionIntr.intrh=0;
      CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_INTR_ENABLE,&regionIntr);                                                                             
#endif
      return 0;
}
 
#if 1
/*************************************************************************\
 *                    EDMA ISR
 \*************************************************************************/
interrupt void EDMA_ISR()
{
   
    RxCount++;
	
    CSL_CPINTC_clearSysInterrupt(hCpintc, CSL_INTC0_CPU_3_2_TPCCINT0+DNUM);
    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTCLEAR,NULL);

    regionIntrQuerry.region = DNUM; // CSL_EDMA3_REGION_1;
    regionIntrQuerry.intr =  (1<<VCP2_RCV_TCC(DNUM))|
            (1<<VCP2_XMT_TCC(DNUM));
    regionIntrQuerry.intrh=0x0;  

    /* Read EDMA IPR */
    CSL_edma3GetHwStatus(hEdma,CSL_EDMA3_QUERY_INTRPEND,&regionIntrQuerry); 
    /* Clear EDMA IPR */
    CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_INTRPEND_CLEAR,&regionIntrQuerry);

    /* Set Interrupt EVAL */

    regionIntrQuerry.region = DNUM;
    CSL_edma3HwControl(hEdma, CSL_EDMA3_CMD_INTR_EVAL,&regionIntrQuerry.region);
}


/*************************************************************************\
 *                    VCP2 ISR
 \*************************************************************************/
interrupt void VCP2_ISR()
{
    CSL_CPINTC_clearSysInterrupt(hCpintc, CSL_INTC0_VCP0INT+(ghVcp2->perNum));//cs 15/04/12  CSL_INTC0_VCP0INT+(ghVcp2->perNum) 04/13 maybe this place doesn't need to change because it pluses perNum,which can change the num
    CSL_intcHwControl(hIntcVcp,CSL_INTC_CMD_EVTCLEAR,NULL);

    // Read the Status Register
      ghVcp2->cfgregs->VCPERR = 0;

      VcpError++;
}

//-------------------------------------------------------------
// Setup_Interrupt() - Performs setup for GEM INTC and CP_INTC
//-------------------------------------------------------------
void Setup_Interrupt()
{
    //---------------------------------
    // Setup GEM internal INTC
    //---------------------------------

    // Declare variables
    CSL_IntcParam vectId1;
    CSL_IntcParam vectId2;
    CSL_IntcGlobalEnableState state;
    CSL_IntcContext intContext; 

    // Setup the global Interrupt
    intContext.numEvtEntries = 2;
    intContext.eventhandlerRecord = eventHandler; 
    CSL_intcInit(&intContext);

    // Enable NMIs
    CSL_intcGlobalNmiEnable();

    // Enable Global Interrupts
    CSL_intcGlobalEnable(&state);

    // Select interrupt vector IDs to use on DSP core
    vectId1 = CSL_INTC_VECTID_9;
    vectId2 = CSL_INTC_VECTID_10;

    // Map GEM input interrupt used for EDMA completion to vectId1  
    hIntcEdma = CSL_intcOpen(&intcObjEdma,
            CSL_GEM_INTC0_OUT_8_PLUS_16_MUL_N,
            &vectId1,
            NULL);

    // Map GEM input interrupt used for VCP error to vectId2  
    hIntcVcp = CSL_intcOpen(&intcObjVcp,
            CSL_GEM_INTC0_OUT_9_PLUS_16_MUL_N,
            &vectId2,
            NULL);

    // Hook the ISR's
    CSL_intcHookIsr(vectId1,&EDMA_ISR);
    CSL_intcHookIsr(vectId2,&VCP2_ISR);

    // Clear any pending interrupts
    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTCLEAR,NULL);
    CSL_intcHwControl(hIntcVcp,CSL_INTC_CMD_EVTCLEAR,NULL);

    // Enable the event and interrupt
    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTENABLE,NULL);
    CSL_intcHwControl(hIntcVcp,CSL_INTC_CMD_EVTENABLE,NULL);


    //----------------------------------------------------------
    // Setup CP_INTC0
    //----------------------------------------------------------

    // Open a handle to CP_INTC instance 0
    hCpintc = CSL_CPINTC_open(CSL_CP_INTC_0);
    if (hCpintc == 0)
    {
        printf ("Error: Unable to open CPINT instance 0\n");
        return;       
    }

    // Map EDMA completion and VCP error system interrupts to proper output interrupts (channels) and enable
    if (DNUM == 0) {
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT0, (16 * DNUM) + 8);
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_VCP0INT,          (16 * DNUM) + 9);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT0);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_VCP0INT);
    } else if (DNUM == 1) {
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT1, (16 * DNUM) + 8);
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_VCP1INT,          (16 * DNUM) + 9);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT1);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_VCP1INT);
    } else if (DNUM == 2) {
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT2, (16 * DNUM) + 8);
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_VCP2INT,          (16 * DNUM) + 9);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT2);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_VCP2INT);
    } else { // DNUM == 3
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT3, (16 * DNUM) + 8);
        CSL_CPINTC_mapSystemIntrToChannel (hCpintc, CSL_INTC0_VCP3INT,          (16 * DNUM) + 9);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_CPU_3_2_TPCCINT3);
        CSL_CPINTC_enableSysInterrupt (hCpintc, CSL_INTC0_VCP3INT);
    }

    /*
    // Clear pending system interrupts
    if (CSL_CPINTC_isInterruptPending(hCpintc) == TRUE)
    {
    // Get the source of the interrupt
    sysIntr = CSL_CPINTC_getPendingInterrupt(hCpintc);

    // Clear the interrupt
    CSL_CPINTC_clearSysInterrupt(hCpintc, sysIntr);
    }
    */

    // Enable output (host) interrupts to GEM
    CSL_CPINTC_enableHostInterrupt (hCpintc, (16 * DNUM) + 8);
    CSL_CPINTC_enableHostInterrupt (hCpintc, (16 * DNUM) + 9);

    // Enable global host interrupt enable
    CSL_CPINTC_enableAllHostInterrupt(hCpintc); 
}

void Release_Interrupt()
{
	    	CSL_intcClose(hIntcEdma);
    		CSL_intcClose(hIntcVcp);
		    CSL_intcGlobalDisable(&state);
	
}
#endif

Uint32 enableVcp2(Int32 vcp2Id )
{

    Uint32 pdNum;
    Uint32 lpscNum;

    if((vcp2Id < CSL_VCP2_A) && (vcp2Id > CSL_VCP2_D))
    {
        printf("\n[Core%d][enableVcp2] : Wrong VCP2 Id specified.", DNUM);
        return FAILURE;
    }

    pdNum   = (vcp2Id == CSL_VCP2_A) ? CSL_PSC_PD_ALWAYSON : CSL_PSC_PD_PD_VCP_BCD; 
    lpscNum = (vcp2Id == CSL_VCP2_A) ? CSL_PSC_LPSC_VCP2_A : (CSL_PSC_LPSC_VCP2_B + (vcp2Id-1)); 

    /* Enable VCP2-B/C/D power domain */
    CSL_PSC_enablePowerDomain (pdNum);

    /* Enable the clocks */
    CSL_PSC_setModuleNextState (lpscNum, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pdNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pdNum));

    if ((CSL_PSC_getPowerDomainState (pdNum) == PSC_PDSTATE_ON) &&
            (CSL_PSC_getModuleState (lpscNum) == PSC_MODSTATE_ENABLE))
    {
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }

}




