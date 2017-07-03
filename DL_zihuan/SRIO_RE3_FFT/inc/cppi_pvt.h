/*
 *   file  cppi_pvt.h
 *
 *   Private data structure of CPPI Low Level Driver.
 *
 *  ============================================================================
 *      (C) Copyright 2009, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/


#ifndef CPPI_PVT_H_
#define CPPI_PVT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* CPPI LLD includes */
#include "cppi_drv.h"
#include "cppi_desc.h"
#include "cppi_listlib.h"
#include "csl_cppi.h"
        
/* Channel Object */
typedef struct 
{
    /* List of channel objects */
    Cppi_ListNode       links;
    /* Channel number */
    uint8_t             channelNum;
    /* CPDMA opening the channel */
    Cppi_CpDma          dmaNum;
    /* Channel Type - Rx or Tx */
    Cppi_ChType         chType;
    /* Number of times Channel is opened */
    uint8_t               refCnt;
    /* Pointer back to the CPDMA Object that opened this channel */
    struct Cppi_DMAObj  *dmaObjHnd; 
}Cppi_ChObj;

/* Flow Object */
typedef struct 
{
    /* List of flow objects */
    Cppi_ListNode       links;
    /* Channel number */
    uint8_t             flowId;
    /* CPDMA opening the channel */
    Cppi_CpDma          dmaNum;
    /* Number of times Channel is opened */
    uint8_t             refCnt;
    /* Pointer back to the CPDMA Object that opened this channel */
    struct Cppi_DMAObj  *dmaObjHnd; 
}Cppi_FlowObj;

/* CPDMA Object */
typedef struct Cppi_DMAObj
{
    /* CPDMA this object belongs to */
    Cppi_CpDma              dmaNum;
    /* Reference count, the number of times CPDMA called cppi_init */
    uint8_t                 refCnt;
    /* Tx channel reference count, the number of times Tx channel was opened */
    uint8_t                 txChCnt;
    /* Rx channel reference count, the number of times Rx channel was opened */
    uint8_t                 rxChCnt;
    /* Rx flow reference count, the number of times Rx flow was configured */
    uint8_t                 rxFlowCnt;

    /* Depth of write arbitration FIFO */
    uint8_t                 writeFifoDepth;
	/* Minimum amount of time in clock cycles that an Rx channel will be required to wait when it 
     * encounters a buffer starvation */
    uint16_t                timeoutCount;
    /* Queue Manager 0 base address register */
    volatile uint32_t       qm0BaseAddress;
    /* Queue Manager 1 base address register */
    volatile uint32_t       qm1BaseAddress;
    /* Queue Manager 2 base address register */
    volatile uint32_t       qm2BaseAddress;
    /* Queue Manager 3 base address register */
    volatile uint32_t       qm3BaseAddress;

    /* Base address for the CPDMA overlay registers */

    /* Global Config registers */
    CSL_Cppidma_global_configRegs       *gblCfgRegs;
    /* Rx Channel Config registers */
    CSL_Cppidma_rx_channel_configRegs   *rxChRegs;
    /* Tx Channel Config registers */
    CSL_Cppidma_tx_channel_configRegs   *txChRegs;
    /* Rx Flow Config registers */
    CSL_Cppidma_rx_flow_configRegs      *rxFlowRegs;
    /* Tx Channel Scheduler registers */
    CSL_Cppidma_tx_scheduler_configRegs *txSchedRegs;

    /* Maximum supported Rx Channels */
    uint8_t                 maxRxCh;
    /* Maximum supported Tx Channels */
    uint8_t                 maxTxCh;
    /* Maximum supported Rx Flows */
    uint8_t                 maxRxFlow;
    /* Priority for all Rx transactions of this CPDMA */
    uint8_t                 rxPriority;
    /* Priority for all Tx transactions of this CPDMA */
    uint8_t                 txPriority;
    /* Allocated Rx channels */
    unsigned int                  rxChMask[5];
    /* Allocated Tx channels */
    unsigned int                  txChMask[5];
    /* Allocated Rx flows */
    unsigned int                  rxFlowMask[5];
    /* Rx Channel Handles */
    Cppi_ChObj              *rxChHnd;
    /* Tx Channel Handles */
    Cppi_ChObj              *txChHnd;
    /* Rx Flow Handles */
    Cppi_FlowObj            *rxFlowHnd;
}Cppi_DMAObj;

/* CPPI Object */
typedef struct 
{
    /* CPDMA handle */
    Cppi_DMAObj             dmaCfg[CPPI_MAX_CPDMA];
}Cppi_Obj;

#ifdef __cplusplus
}
#endif

#endif /* CPPI_PVT_H_ */

