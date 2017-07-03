/**
 *   @file  cppi_drv.c
 *
 *   @brief   
 *      This is the CPPI Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
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

/* CPPI Types includes */
#include "cppi_types.h"

/* CPPI LLD includes */
#include "cppi_drv.h"
#include "cppi_listlib.h"
#include "cppi_pvt.h"

/* CPPI OSAL layer */
#include "cppi_osal.h"

/**********************************************************************
 ************************** Globals ***********************************
 **********************************************************************/

/* CPPI object */
#pragma DATA_ALIGN (cppiObject, 128)
#pragma DATA_SECTION (cppiObject, ".cppi");
Cppi_Obj     cppiObject;

/** @brief Global Variable which describes the CPPI LLD Version Information */
const char   cppiLldVersionStr[] = CPPI_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/** @addtogroup CPPI_LLD_FUNCTION
@{ 
*/

/**********************************************************************
 ************************** APIs **************************************
 **********************************************************************/

/**
 *  @b Description
 *  @n  
 *      This function initializes the CPPI low level driver
 *      This function is called once in the system to setup the CPPI 
 *      low level driver with information pertaining to maximum supported Rx priority, 
 *      Tx priority, Rx channels, Tx channels, Rx flows and memory mapped address for each CPPI CPDMA. 
 *
 *  @param[in]  cppiGblCfgParams
 *      Initialization structure that contains the CPPI device specific information.
 *
 *  @post  
 *      CPPI instance is created.
 *
 *  @retval
 *      Success -   CPPI_SOK
 */
Cppi_Result Cppi_init (Cppi_GlobalConfigParams *cppiGblCfgParams)
{
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t          i;
    void             *key;

    /* Begin Critical Section before accessing Shared resources */
    key = Cppi_osalCsEnter ();

    memset ((void *) &cppiObject, 0, sizeof (Cppi_Obj));

    for (i = 0; i < CPPI_MAX_CPDMA; i++)
    {
        /* Get the pointer to dmaobject */
        dmaObjPtr = &(cppiObject.dmaCfg[cppiGblCfgParams[i].dmaNum]);

        /* Store CPDMA related configuration */
        dmaObjPtr->dmaNum = cppiGblCfgParams[i].dmaNum;
        dmaObjPtr->maxRxCh = cppiGblCfgParams[i].maxRxCh;
        dmaObjPtr->maxTxCh = cppiGblCfgParams[i].maxTxCh;
        dmaObjPtr->maxRxFlow = cppiGblCfgParams[i].maxRxFlow;
        dmaObjPtr->rxPriority = cppiGblCfgParams[i].rxPriority;
        dmaObjPtr->txPriority = cppiGblCfgParams[i].txPriority;

        /* Store the register base address */
        dmaObjPtr->gblCfgRegs = cppiGblCfgParams[i].gblCfgRegs;
        dmaObjPtr->rxChRegs = cppiGblCfgParams[i].rxChRegs;
        dmaObjPtr->txChRegs = cppiGblCfgParams[i].txChRegs;
        dmaObjPtr->txSchedRegs = cppiGblCfgParams[i].txSchedRegs;
        dmaObjPtr->rxFlowRegs = cppiGblCfgParams[i].rxFlowRegs;
    }

    /* Writeback Global Object */
    Osal_cppiEndMemAccess ((void *) &cppiObject, sizeof (Cppi_Obj));

    /* End Critical Section */
    Cppi_osalCsExit (key);
    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function deinitializes the CPPI low level driver.
 *      The LLD is deinitialized only if all the CPDMA instances are closed.
 *      This function should be called before re-initializing the CPPI low level driver.
 *
 *  @pre  
 *      Cppi_init function should be called before calling this function.
 *
 *  @post  
 *      CPPI instance is closed.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_CLOSED
 */
Cppi_Result Cppi_exit (void)
{
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        i;

    for (i = 0; i < CPPI_MAX_CPDMA; i++)
    {
        dmaObjPtr = &(cppiObject.dmaCfg[i]);
        
        /* Invalidate Global Object */
        Osal_cppiBeginMemAccess ((void *) dmaObjPtr->refCnt, 1);

        if (dmaObjPtr->refCnt > 0)
            return CPPI_CPDMA_NOT_CLOSED;                
    }
   
    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function opens the CPPI CPDMA instance. 
 *      This function is called by the CPPI CPDMA driver, application to initialize the 
 *      global configuration pertaining to CPPI CPDMA that includes rx priority, 
 *      tx priority, write arbitration FIFO depth, receive starvation timeout and QM base addresses. 
 *      The registers are configured only once when the function 
 *      is called for the first time. Any future calls to this function returns the CPDMA 
 *      object handle created during the first call.
 *
 *  @param[in]  initCfg
 *      Initialization structure that contains the global configuration parameters 
 *      pertaining to CPPI CPDMA. 
 *
 *  @pre  
 *      Cppi_init function should be called before calling this function.
 *
 *  @post  
 *      Global configuration registers are configured with input parameters
 *
 *  @retval
 *      Success -   Handle to CPPI CPDMA object. Used as an input parameter to all other CPPI LLD APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_Handle Cppi_open (Cppi_CpDmaInitCfg *initCfg)
{
    Cppi_DMAObj     *dmaObjPtr;
    void             *key;

    if (initCfg == NULL)
        return NULL;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = &(cppiObject.dmaCfg[initCfg->dmaNum]);

    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if (dmaObjPtr->refCnt > 0)
    {
        dmaObjPtr->refCnt++;

        /* Write back reference count */
        Osal_cppiEndMemAccess ((void *) dmaObjPtr->refCnt, 1);

        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return (Cppi_Handle) dmaObjPtr;
    }

    if (initCfg->writeFifoDepth > 32)
    {
        Cppi_osalCsExit (key);
        return NULL;
    }
    dmaObjPtr->writeFifoDepth = initCfg->writeFifoDepth;

    if (initCfg->writeFifoDepth != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->PERF_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_WARB_FIFO_DEPTH, initCfg->writeFifoDepth);

    dmaObjPtr->timeoutCount = initCfg->timeoutCount;
    CSL_FINS (dmaObjPtr->gblCfgRegs->PERF_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_TIMEOUT_CNT, initCfg->timeoutCount);

    CSL_FINS (dmaObjPtr->gblCfgRegs->PRIORITY_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_TX_PRIORITY, dmaObjPtr->txPriority);
    CSL_FINS (dmaObjPtr->gblCfgRegs->PRIORITY_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_RX_PRIORITY, dmaObjPtr->rxPriority);

    dmaObjPtr->qm0BaseAddress = initCfg->qm0BaseAddress;
    dmaObjPtr->qm1BaseAddress = initCfg->qm1BaseAddress;
    dmaObjPtr->qm2BaseAddress = initCfg->qm2BaseAddress;
    dmaObjPtr->qm3BaseAddress = initCfg->qm3BaseAddress;

    if (initCfg->qm0BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[0], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm0BaseAddress);
    if (initCfg->qm1BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[1], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm1BaseAddress);
    if (initCfg->qm2BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[2], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm2BaseAddress);
    if (initCfg->qm3BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[3], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm3BaseAddress);
 
    dmaObjPtr->refCnt++;

    /* Writeback Global Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_Handle) dmaObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function closes the CPPI CPDMA instance. 
 *      The instance reference count is decremented. 
 *      CPPI CPDMA instance is closed only if the reference count is zero.
 *      CPPI CPDMA instance is closed only if all the Rx, Tx channels and Rx flows are closed.
 *      This function should be called to close all instances before re-opening the CPPI CPDMA instance. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      CPPI CPDMA object is freed if the reference count is zero.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 *  @retval
 *      Failure -   CPPI_TX_CHANNELS_NOT_CLOSED
 *  @retval
 *      Failure -   CPPI_RX_CHANNELS_NOT_CLOSED
 *  @retval
 *      Failure -   CPPI_RX_FLOWS_NOT_CLOSED
 */
Cppi_Result Cppi_close (Cppi_Handle hnd)
{
    Cppi_DMAObj     *dmaObjPtr;
    void             *key;

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if (dmaObjPtr->refCnt == 0)
    {
        return CPPI_CPDMA_NOT_INITIALIZED;
    }

    /* If channels or flows are still open then don't close CPDMA instance */
    if (dmaObjPtr->txChCnt)
    {
        return CPPI_TX_CHANNELS_NOT_CLOSED;
    }
    if (dmaObjPtr->rxChCnt)
    {
        return CPPI_RX_CHANNELS_NOT_CLOSED;
    }
    if (dmaObjPtr->rxFlowCnt)
    {
        return CPPI_RX_FLOWS_NOT_CLOSED;
    }
    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    dmaObjPtr->refCnt--;

    /* Writeback reference count */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr->refCnt, 1);

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return dmaObjPtr->refCnt;
}

/**
 *  @b Description
 *  @n  
 *      This function opens a CPPI transmit channel. 
 *      The channel can be opened in two ways:
 *          1) If channel number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available channel is allocated.
 *          2) If channel number in cfg structure is a valid channel i.e., >= 0, 
 *               then the channel is allocated if free 
 *               else
 *               a handle to a previously opened channel is returned.
 *      The channel is configured only if it is a new channel allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  cfg
 *      Tx channel configuration specifying scheduler priority for the channel, channel number, 
 *      channel enable, PS and EPIB filtering control, special AIF 
 *      Monolithic Mode.
 *
 *  @param[out]  isAllocated
 *      Indicates whether the requested channel number is a new channel allocation(1).
 *      or was already allocated. If the channel was previously allocated this parameter returns 
 *      the reference count.
 *      
 * 
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      Transmit channel is allocated and configured.
 *
 *  @retval
 *      Success -   Channel Handle. Used as a input parameter in all other channel related APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_ChHnd Cppi_txChannelOpen (Cppi_Handle hnd, Cppi_TxChInitCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = NULL;
    uint32_t          index=0, mask, i, bitPos;
    int32_t           chNum = -1;
    void             *key;

    *isAllocated = 0;

    if ((hnd == NULL) || (cfg == NULL))
    {
        return NULL;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if ((cfg->channelNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->channelNum >= dmaObjPtr->maxTxCh))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }

    if (cfg->channelNum >= 0)
    {
        /* Find the channel object */
        chObjPtr = (Cppi_ChObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->txChHnd);
        while (chObjPtr != NULL)
        {
            /* Invalidate channel Object */
            Osal_cppiBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

            if (chObjPtr->channelNum == cfg->channelNum)
            {
                chObjPtr->refCnt++;
                dmaObjPtr->txChCnt++;
                *isAllocated = chObjPtr->refCnt;
            
                /* Writeback channel Object */
                Osal_cppiEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

                /* Writeback CPDMA Object */
                Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_ChHnd) chObjPtr;
            }
            /* Get next list entry */
            chObjPtr = (Cppi_ChObj *) cppi_list_get_next ((Cppi_ListNode *) chObjPtr);
        }
    }
    if (chObjPtr == NULL)
    {
        if (cfg->channelNum >= 0)
            chNum = cfg->channelNum;
        else
        {
            for (i = 0; i < 5; i++)
            {
                index = 0;
                mask = dmaObjPtr->txChMask[i];
                if ((mask & 0xFFFFFFFF) == 0xFFFFFFFF)
                {
                    continue;
                }

                while ((mask & 0x1) && ((index + (i * 32)) < dmaObjPtr->maxTxCh))
                {
                    mask = mask >> 1;
                    index++;
                }
                if (index >= dmaObjPtr->maxTxCh)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
                if (index != 32)
                {
                    chNum = (index + (i * 32));
                    break;
                }
            }
        }
        if (chNum == -1)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        chObjPtr = (Cppi_ChObj *) Cppi_osalMalloc (sizeof (Cppi_ChObj));
        if (chObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Initialize the allocated channel object */
        memset ((void *) chObjPtr, 0, sizeof (Cppi_ChObj));

        chObjPtr->refCnt++;
        dmaObjPtr->txChCnt++;

        index = chNum / 32;
        bitPos = chNum % 32;
        CSL_FINSR (dmaObjPtr->txChMask[index], bitPos, bitPos, 1);

        chObjPtr->channelNum = chNum;
        chObjPtr->dmaNum = dmaObjPtr->dmaNum;
        chObjPtr->chType = Cppi_ChType_TX_CHANNEL;
        chObjPtr->dmaObjHnd = (Cppi_DMAObj *) dmaObjPtr;
        *isAllocated = chObjPtr->refCnt;

        /* Add the channel object to channel list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->txChHnd, (Cppi_ListNode **) &chObjPtr);

    }

    if (*isAllocated == 1)
    {
        CSL_FINS (dmaObjPtr->txSchedRegs->TX_CHANNEL_SCHEDULER_CONFIG_REG[chObjPtr->channelNum], 
                CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_PRIORITY, cfg->priority);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_EINFO, cfg->filterEPIB);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_PSWORDS, cfg->filterPS);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_AIF_MONO_MODE, cfg->aifMonoMode);

        if (cfg->txEnable)
        {
            uint32_t value = 0;
            CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, cfg->txEnable);
            dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
        }
    }     

    /* Writeback channel Object */
    Osal_cppiEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

    /* Writeback CPDMA Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_ChHnd) chObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function opens a CPPI receive channel. 
 *      The channel can be opened in two ways:
 *          1) If channel number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available channel is allocated.
 *          2) If channel number in cfg structure is a valid channel i.e., >= 0, 
 *               then the channel is allocated if free 
 *               else
 *               a handle to a previously opened channel is returned.
 *      The channel is configured only if it is a new channel allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  cfg
 *      Rx channel configuration specifying channel number, channel enable. 
 *
 *  @param[out]  isAllocated
 *      Indicates whether the requested channel number is a new channel allocation(1).
 *      or was already allocated. If the channel was previously allocated this parameter returns 
 *      the reference count.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      Receive channel is allocated and configured.
 *
 *  @retval
 *      Success -   Channel Handle. Used as a input parameter in all other channel related APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_ChHnd Cppi_rxChannelOpen (Cppi_Handle hnd, Cppi_RxChInitCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = NULL;
    uint32_t          index = 0, mask, i, bitPos;
    int32_t           chNum = -1;
    void             *key;
    
    *isAllocated = 0;

    if ((hnd == NULL) || (cfg == NULL))
    {
        return NULL;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if ((cfg->channelNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->channelNum >= dmaObjPtr->maxRxCh))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }

    if (cfg->channelNum >= 0)
    {
        /* Find the channel object */
        chObjPtr = (Cppi_ChObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->rxChHnd);
        while (chObjPtr != NULL)
        {
            /* Invalidate channel Object */
            Osal_cppiBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

            if (chObjPtr->channelNum == cfg->channelNum)
            {
                chObjPtr->refCnt++;
                dmaObjPtr->rxChCnt++;
                *isAllocated = chObjPtr->refCnt;
                
                /* Writeback channel Object */
                Osal_cppiEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

                /* Writeback CPDMA Object */
                Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_ChHnd) chObjPtr;
            }
            /* Get next list entry */
            chObjPtr = (Cppi_ChObj *) cppi_list_get_next ((Cppi_ListNode *) chObjPtr);
        }
    }
    if (chObjPtr == NULL)
    {
        if (cfg->channelNum >= 0)
            chNum = cfg->channelNum;
        else
        {   
            for (i = 0; i < 5; i++)
            {
                index = 0;
                mask = dmaObjPtr->rxChMask[i];
                if ((mask & 0xFFFFFFFF) == 0xFFFFFFFF)
                {
                    continue;
                }

                while ((mask & 0x1) && ((index + (i * 32)) < dmaObjPtr->maxRxCh))
                {
                    mask = mask >> 1;
                    index++;
                }
                if (index >= dmaObjPtr->maxRxCh)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
                if (index != 32)
                {
                    chNum = (index + (i * 32));
                    break;
                }
            }
        }

        if (chNum == -1)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }
        chObjPtr = (Cppi_ChObj *) Cppi_osalMalloc (sizeof (Cppi_ChObj));
        if (chObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Initialize the allocated channel object */
        memset ((void *) chObjPtr, 0, sizeof (Cppi_ChObj));

        chObjPtr->refCnt++;
        dmaObjPtr->rxChCnt++;

        index = chNum / 32;
        bitPos = chNum % 32;

        CSL_FINSR (dmaObjPtr->rxChMask[index], bitPos, bitPos, 1);

        chObjPtr->channelNum = chNum;
        chObjPtr->dmaNum = dmaObjPtr->dmaNum;
        chObjPtr->chType = Cppi_ChType_RX_CHANNEL;
        chObjPtr->dmaObjHnd = (Cppi_DMAObj *) dmaObjPtr;
        *isAllocated = chObjPtr->refCnt;
        
        /* Add the channel object to channel list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->rxChHnd, (Cppi_ListNode **) &chObjPtr);
    }

    if (*isAllocated == 1)
    {
        if (cfg->rxEnable)
        {
            uint32_t value = 0;
            CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, cfg->rxEnable);
            dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
        }
    }        

    /* Writeback channel Object */
    Osal_cppiEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

    /* Writeback CPDMA Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_ChHnd) chObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function enables a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelEnable (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        value = 0;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }

    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function disables a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelDisable (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    dmaObjPtr = chObjPtr->dmaObjHnd;
    
    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = 0;
    else
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = 0;
  
    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function tears down a transmit or a receive channel by writing 
 *      to the channel teardown bit. 
 *      This function can do a blocking wait for the teardown to complete OR
 *      return immediately and it is up to the callee to check for channel 
 *      teardown status. The behavior is based on the "wait" input parameter
 *
 *  @param[in]  hnd
 *      Channel handle.
 *
 *  @param[in]  wait
 *      Specifies Wait or No Wait for teardown complete.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM 
 */
Cppi_Result Cppi_channelTeardown (Cppi_ChHnd hnd, Cppi_Wait wait)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        value = 0;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    dmaObjPtr = chObjPtr->dmaObjHnd;
    
    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN, 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_TEARDOWN, 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }
 
    if (wait)
    {
        uint32_t  enable;
        do
        {
            if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
                enable = CSL_FEXT (dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG, 
                    CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE);
            else
                enable = CSL_FEXT (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A, 
                    CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE);
        } while (enable);
    }
    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function closes a transmit or a receive channel.
 *      The channel reference count is decremented. The channel is freed only if the 
 *      reference count is zero. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @post
 *      Channel is freed if the reference count is zero and available for reallocation.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CHANNEL_NOT_OPEN
 */
Cppi_Result Cppi_channelClose (Cppi_ChHnd hnd)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    uint32_t        refCount;
    void            *key;
    
    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    
    /* Invalidate channel Object */
    Osal_cppiBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

    if (chObjPtr->refCnt == 0)
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return CPPI_CHANNEL_NOT_OPEN;
    }
    else
    {
        dmaObjPtr = chObjPtr->dmaObjHnd;

         /* Invalidate CPDMA Object */
        Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));
       
        chObjPtr->refCnt--;
        refCount = chObjPtr->refCnt;
        
        if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        {
            dmaObjPtr->rxChCnt--;
            /* If reference count is zero then free the object */
            if (refCount == 0)
            {
                uint32_t  index, bitPos;
                
                index = chObjPtr->channelNum / 32;
                bitPos = chObjPtr->channelNum % 32;

                CSL_FINSR (dmaObjPtr->rxChMask[index], bitPos, bitPos, 0);
                cppi_list_remove_node ((Cppi_ListNode **) &dmaObjPtr->rxChHnd, (Cppi_ListNode *) chObjPtr);
                Cppi_osalFree (chObjPtr, sizeof (Cppi_ChObj));
            }      
        }
        else
        {
            dmaObjPtr->txChCnt--;
            /* If reference count is zero then free the object */
            if (refCount == 0)
            {
                uint32_t index, bitPos;

                index = chObjPtr->channelNum / 32;
                bitPos = chObjPtr->channelNum % 32;
                
                CSL_FINSR (dmaObjPtr->txChMask[index], bitPos, bitPos, 0);
                cppi_list_remove_node ((Cppi_ListNode**) &dmaObjPtr->txChHnd, (Cppi_ListNode*) chObjPtr);
                Cppi_osalFree (chObjPtr, sizeof (Cppi_ChObj));
            } 
        }
        /* Writeback channel Object */
        Osal_cppiEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));
    }

    /* Writeback CPDMA Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return refCount;
}

/**
 *  @b Description
 *  @n  
 *      This function pauses a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelPause (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t          value = 0;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE, 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_PAUSE, 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }  
    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function returns the enable or disable channel status. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   Channel Status
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelStatus (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        return (CSL_FEXT (dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG, 
            CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE));
    else
        return (CSL_FEXT (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A,
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE));
}

/**
 *  @b Description
 *  @n  
 *      This function configures a receive flow. 
 *      The flow can be configured in two ways:
 *          1) If flow ID number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available flow is allocated.
 *          2) If flow ID number is cfg structure is a valid flow i.e., >= 0, 
 *               then the flow is allocated if free 
 *               else
 *               a handle to a previously opened flow is returned.

 *      The flow is configured only if it is a new flow allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *      
 *  @param[in]  cfg
 *      Rx flow configuration
 * 
 *  @param[out]  isAllocated
 *      Indicates whether the requested flow is a new flow allocation(1).
 *      or was already allocated. If the flow was previously allocated this parameter returns 
 *      the reference count.
 *       
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *       
 *  @post  
 *      Rx flow is allocated and configured.
 *
 *  @retval
 *      Success -   Flow Handle.
 *  @retval
 *      Failure -   NULL
 */
Cppi_FlowHnd Cppi_configureRxFlow (Cppi_Handle hnd, Cppi_RxFlowCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr = (Cppi_DMAObj *) hnd;
    Cppi_FlowObj    *flowObjPtr = NULL;
    uint32_t          index = 0, mask, i, bitPos;
    int32_t           flowId = -1;
    void             *key;

    *isAllocated = 0;

    if ((dmaObjPtr == NULL) || (cfg == NULL))
        return NULL;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if ((cfg->flowIdNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->flowIdNum >= dmaObjPtr->maxRxFlow))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }

    if (cfg->flowIdNum >= 0)
    {
        /* Find the flow object */
        flowObjPtr = (Cppi_FlowObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd);
        while (flowObjPtr != NULL)
        {
            /* Invalidate flow Object */
            Osal_cppiBeginMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

            if (flowObjPtr->flowId == cfg->flowIdNum)
            {
                flowObjPtr->refCnt++;
                dmaObjPtr->rxFlowCnt++;
                *isAllocated = flowObjPtr->refCnt;

                /* Writeback flow Object */
                Osal_cppiEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

                /* Writeback CPDMA Object */
                Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_FlowHnd) flowObjPtr;
            }
            /* Get next list entry */
            flowObjPtr = (Cppi_FlowObj *) cppi_list_get_next ((Cppi_ListNode *) flowObjPtr);
        }
    }

    /* New flow allocation - either specified or next available one */
    if (flowObjPtr == NULL)
    {
        if (cfg->flowIdNum >= 0)
            flowId = cfg->flowIdNum;
        else
        {
            for (i = 0; i < 5; i++)
            {
                index = 0;
                mask = dmaObjPtr->rxFlowMask[i];
                if ((mask & 0xFFFFFFFF) == 0xFFFFFFFF)
                {
                    continue;
                }

                while ((mask & 0x1) && ((index + (i * 32)) < dmaObjPtr->maxRxFlow))
                {
                    mask = mask >> 1;
                    index++;
                }
                if (index >= dmaObjPtr->maxRxFlow)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
                if (index != 32)
                {
                    flowId = (index + (i * 32));
                    break;
                }
            }
        }

        if (flowId == -1)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        flowObjPtr = (Cppi_FlowObj *) Cppi_osalMalloc (sizeof (Cppi_FlowObj));
        if (flowObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Initialize the allocated flow object */
        memset ((void *) flowObjPtr, 0, sizeof (Cppi_FlowObj));

        flowObjPtr->refCnt++;
        dmaObjPtr->rxFlowCnt++;
        
        index = flowId / 32;
        bitPos = flowId % 32;
        CSL_FINSR (dmaObjPtr->rxFlowMask[index], bitPos, bitPos, 1);

        flowObjPtr->dmaNum = dmaObjPtr->dmaNum;
        flowObjPtr->flowId = flowId;
        flowObjPtr->dmaObjHnd = dmaObjPtr;
        *isAllocated = flowObjPtr->refCnt;
                
        /* Add the flow object to flow list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd, (Cppi_ListNode **) &flowObjPtr);

    }

    if (*isAllocated == 1)
    {
            uint32_t      reg, temp = 0;

            index = flowObjPtr->flowId;
            reg = 0;
    		/* Rx flow configuration register A */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QNUM, cfg->rx_dest_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QMGR, cfg->rx_dest_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_SOP_OFFSET, cfg->rx_sop_offset);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PS_LOCATION, cfg->rx_ps_location);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DESC_TYPE, cfg->rx_desc_type);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_ERROR_HANDLING, cfg->rx_error_handling);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PSINFO_PRESENT, cfg->rx_psinfo_present);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_EINFO_PRESENT, cfg->rx_einfo_present);
		    dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_A = reg;

            reg = 0;
		    /* Rx flow configuration register B */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_LO, cfg->rx_dest_tag_lo);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_HI, cfg->rx_dest_tag_hi);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_LO, cfg->rx_src_tag_lo);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_HI, cfg->rx_src_tag_hi);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_B = reg;

            reg = 0;
            /* Rx flow configuration register C */		
            temp = ((cfg->rx_size_thresh0_en) | (cfg->rx_size_thresh1_en << 1) | (cfg->rx_size_thresh2_en << 2));

            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SIZE_THRESH_EN, temp);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_LO_SEL, cfg->rx_dest_tag_lo_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_HI_SEL, cfg->rx_dest_tag_hi_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_LO_SEL, cfg->rx_src_tag_lo_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_HI_SEL, cfg->rx_src_tag_hi_sel);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_C = reg;

            reg = 0;
			/* Rx flow configuration register D */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QNUM, cfg->rx_fdq1_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QMGR, cfg->rx_fdq1_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QNUM, cfg->rx_fdq0_sz0_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QMGR, cfg->rx_fdq0_sz0_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_D = reg;

            reg = 0;
			/* Rx flow configuration register E */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QNUM, cfg->rx_fdq3_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QMGR, cfg->rx_fdq3_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QNUM, cfg->rx_fdq2_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QMGR, cfg->rx_fdq2_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_E = reg;

            reg = 0;
			/* Rx flow configuration register F */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH1, (cfg->rx_size_thresh1 >> 5));
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH0, (cfg->rx_size_thresh0  >> 5));
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_F = reg;
            
            reg = 0;
            /* Rx flow configuration register G */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QNUM, cfg->rx_fdq0_sz1_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QMGR, cfg->rx_fdq0_sz1_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_SIZE_THRESH2, (cfg->rx_size_thresh2) >> 5);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_G = reg;

            reg = 0;
            /* Rx flow configuration register H */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QNUM, cfg->rx_fdq0_sz3_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QMGR, cfg->rx_fdq0_sz3_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QNUM, cfg->rx_fdq0_sz2_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QMGR, cfg->rx_fdq0_sz2_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_H = reg;

    }

    /* Writeback flow Object */
    Osal_cppiEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    /* Writeback CPDMA Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_FlowHnd) flowObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function closes a receive flow. 
 *      The flow reference count is decremented. The flow is freed only if the 
 *      reference count is zero. 
 *
 *  @param[in]  hnd
 *      flow handle.
 * 
 *  @pre  
 *      Cppi_configureRxFlowfunction should be called before calling this function.
 *
 *  @post
 *      Flow is freed if the reference count is zero and available for reallocation.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_FLOW_NOT_OPEN
 */
Cppi_Result Cppi_closeRxFlow (Cppi_FlowHnd hnd)
{
    Cppi_FlowObj    *flowObjPtr = (Cppi_FlowObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        refCount;
    void            *key;


    if (hnd == NULL)
        return CPPI_INVALID_PARAM;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    
    /* Invalidate flow Object */
    Osal_cppiBeginMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    if (flowObjPtr->refCnt == 0)
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return CPPI_FLOW_NOT_OPEN;
    }
    dmaObjPtr = flowObjPtr->dmaObjHnd;
 
    /* Invalidate CPDMA Object */
    Osal_cppiBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    flowObjPtr->refCnt--;
    refCount = flowObjPtr->refCnt;

    /* If reference count is zero then free the object */
    if (refCount == 0)
    {
        uint32_t  index, bitPos;

        index = flowObjPtr->flowId / 32;
        bitPos = flowObjPtr->flowId % 32;

        CSL_FINSR (dmaObjPtr->rxFlowMask[index], bitPos, bitPos, 0);
        cppi_list_remove_node ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd, (Cppi_ListNode *) flowObjPtr);
        Cppi_osalFree (flowObjPtr, sizeof (Cppi_FlowObj));
    }

    dmaObjPtr->rxFlowCnt--;

    /* Writeback flow Object */
    Osal_cppiEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    /* Writeback CPDMA Object */
    Osal_cppiEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return refCount;
}

/** 
 *  @b Description
 *  @n  
 *      Given the channel handle the function returns the channel number
 *      corresponding to the handle
 *
 *  @param[in]  hnd      
 *      Channel handle
 *
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Channel number
 */
uint32_t Cppi_getChannelNumber (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;

    return (chObjPtr->channelNum);
}

/** 
 *  @b Description
 *  @n  
 *      Given the flow handle the function returns the flow ID
 *      corresponding to the handle
 *
 *  @param[in]  hnd      
 *      Flow handle
 *
 *  @pre  
 *      Cppi_configureRxFlowfunction should be called before calling this function.
 *
 *  @retval
 *      Flow ID
 */
uint32_t Cppi_getFlowId (Cppi_FlowHnd hnd)
{
    Cppi_FlowObj    *flowObjPtr = (Cppi_FlowObj *) hnd;

    return (flowObjPtr->flowId);
}

/** 
 *  @b Description
 *  @n  
 *      This function enables or disables CPDMA loopback 
 *
 *  @param[in]  hnd      
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  loopback      
 *      0 - To disable loopback
 *      1 - To enable loopback
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK 
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 */
Cppi_Result Cppi_setCpdmaLoopback (Cppi_Handle hnd, uint8_t loopback)
{
    Cppi_DMAObj     *dmaObjPtr;

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    if (dmaObjPtr->refCnt == 0)
    {
        return CPPI_CPDMA_NOT_INITIALIZED;
    }
    CSL_FINS (dmaObjPtr->gblCfgRegs->EMULATION_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_EMULATION_CONTROL_REG_LOOPBACK_EN, loopback);

    return CPPI_SOK;
}

/** 
 *  @b Description
 *  @n  
 *      This function returns the CPDMA loopback configuration
 *
 *  @param[in]  hnd      
 *      handle returned by Cppi_open API.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @retval
 *      Success 
 *          0 - loopback is disabled 
 *          1 - loopback is enabled
 *          
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 */
Cppi_Result Cppi_getCpdmaLoopback (Cppi_Handle hnd)
{
    Cppi_DMAObj     *dmaObjPtr;

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    if (dmaObjPtr->refCnt == 0)
    {
        return CPPI_CPDMA_NOT_INITIALIZED;
    }
    return (CSL_FEXT (dmaObjPtr->gblCfgRegs->EMULATION_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_EMULATION_CONTROL_REG_LOOPBACK_EN));
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version information of the CPPI LLD.
 *
 *  @retval
 *      Version Information.
 */
uint32_t Cppi_getVersion (void)
{
    return CPPI_LLD_VERSION_ID;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version string for the CPPI LLD.
 *
 *  @retval
 *      Version String.
 */
const char* Cppi_getVersionStr (void)
{
    return cppiLldVersionStr;
}

/**
@}
*/

