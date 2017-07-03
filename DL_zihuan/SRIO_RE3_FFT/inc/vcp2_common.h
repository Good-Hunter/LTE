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
*  Filename       : vcp2_3gpp.h
*  Description    : Header file for VCP2 Local Test Functions 
\*****************************************************************************/

#include <ti/csl/csl_vcp2.h>
#include "vcp2_testInit.h"


#define VCP2_POWER_ON_IN_PROGRESS   0xFEFEFEFE
#define VCP2_POWER_ON_COMPLETE      0xABCDABCD

/* VCP input configuration transfer size */
#define VCPIC_DMA_SIZE  24

/* VCP out register transfer size */
#define VCPOUT_DMA_SIZE  8

#define EDMA_SHADOW_REGION

/*------------------------------------------------------------------------------*/
/* types                                                                        */
/*------------------------------------------------------------------------------*/
typedef struct
{
	Uint32 nBit;
	Uint32 nFrame;
	Uint32 nErroneousBit;
	Uint32 nErroneousFrame;
	Uint32 nErroneousBitThreshold;
	Uint32 nErroneousFrameThreshold;
	Uint32 nBitThreshold;
	Uint32 nCmodelHwdiff;
	Uint32 nErroneousBitCmodel;
	Uint32 nErroneousFrameCmodel;
	double nErroneousBitAverage;
	double nErroneousBitAverageCmodel;
	Uint32 nErroneousFrameTotal;	
} BERData;

/*****************************************************************************\
* Function Decalrations 
\*****************************************************************************/
void Setup_Interrupt();
void Release_Interrupt();
Uint32 enableVcp2(Int32 vcp2Id );


Uint32 edma3Init(Uint32 edmaRegionNum, Uint32 vcp2Inst);
Uint32 edma3Init1(Uint32 edmaRegionNum, Uint32 vcp2Inst);  //cs 15/06/25
Uint32 edma3Init2(Uint32 edmaRegionNum, Uint32 vcp2Inst);  //cs 15/06/25
Uint32 edma3Release(Uint32 edmaRegionNum);
interrupt void EDMA_ISR(void);
interrupt void VCP2_ISR(void);

Uint32 vcp2SubmitEdma(VCP2_UserData *hUserData, VCP2_Params *vcpParameters, VCP2_ConfigIc *configIc,Uint32 *decisions, Uint32 *outParms, VCP2Handle hVcp2);

void Config_VCP2_EDMA3_PaRAM(VCP2_Edma3_Config * vcp2_Edma3_Config,Uint32 VCP2instNum);
	
Uint32 vcp2SubmitEdma_multi_Channel(VCP2_Edma3_Config * vcp2_Edma3_Config, VCP2Handle hVcp2);            
				           
/*****************************************************************************\
* End of vcp2_ber.h 
\*****************************************************************************/
