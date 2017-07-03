

#ifndef _CSL_EDMA3AUX_H_
#define _CSL_EDMA3AUX_H_

#include "csl_edma3.h"


CSL_IDEF_INLINE void CSL_edma3GetInfo 
(
    CSL_Edma3Handle     hModule,
    CSL_Edma3QueryInfo* response
)
{
    /* Populate the configuration and peripheral id. */
    response->config   = hModule->regs->TPCC_CFG;
    response->revision = hModule->regs->TPCC_PID;
    return;
}


CSL_IDEF_INLINE void CSL_edma3MapDMAChannelToParamBlock
(
    CSL_Edma3Handle     hModule,
    unsigned char               dmaChannel,
    unsigned short              paramId
)
{
    /* Map the DMA channel to the parameter block. */
    CSL_FINS(hModule->regs->TPCC_DCHMAP[dmaChannel], TPCC_TPCC_DCHMAP0_PAENTRY, paramId);
    return;
}


CSL_IDEF_INLINE unsigned short CSL_edma3GetDMAChannelToParamBlockMapping 
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel
)
{
    /* Get the Param Block to which the DMA channel is mapped to. */
    return (unsigned short) CSL_FEXT(hModule->regs->TPCC_DCHMAP[dmaChannel], TPCC_TPCC_DCHMAP0_PAENTRY);
}


CSL_IDEF_INLINE void CSL_edma3MapQDMAChannelToParamBlock 
(
    CSL_Edma3Handle     hModule,
    unsigned char               qdmaChannel,
    unsigned short              paramId
)
{
    CSL_FINS(hModule->regs->TPCC_QCHMAP[qdmaChannel], TPCC_TPCC_QCHMAP0_PAENTRY, paramId);
    return;
}


CSL_IDEF_INLINE unsigned short CSL_edma3GetQDMAChannelToParamBlockMapping 
(
    CSL_Edma3Handle     hModule,
    unsigned char               qdmaChannel
)
{   
    return (unsigned short) CSL_FEXT(hModule->regs->TPCC_QCHMAP[qdmaChannel], TPCC_TPCC_QCHMAP0_PAENTRY);
}


CSL_IDEF_INLINE void CSL_edma3SetQDMATriggerWord
(
    CSL_Edma3Handle     hModule,
    unsigned char               qdmaChannel,
    unsigned char               trword
)
{
    CSL_FINS(hModule->regs->TPCC_QCHMAP[qdmaChannel], TPCC_TPCC_QCHMAP0_TRWORD, trword);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetQDMATriggerWord
(
    CSL_Edma3Handle     hModule,
    unsigned char               qdmaChannel,
    unsigned char*              trword
)
{
    /* Extract the trigger word. */
    *trword = CSL_FEXT(hModule->regs->TPCC_QCHMAP[qdmaChannel], TPCC_TPCC_QCHMAP0_TRWORD);
}


CSL_IDEF_INLINE void CSL_edma3MapDMAChannelToEventQueue
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel,
    unsigned char           eventQueue    
)
{
    unsigned char   dmaRegIndex;
    unsigned char   lsb;

    /* There are 8 channels per register; use this to determine the DMAQNUM register Index. */
    dmaRegIndex = dmaChannel >> 3;

    /* Compute the bit position where the value is to be written. */
    lsb = (dmaChannel - (dmaRegIndex * 8)) << 2;

    /* Write the event Queue */
    CSL_FINSR(hModule->regs->TPCC_DMAQNUM[dmaRegIndex], lsb + 2, lsb, eventQueue);
}


CSL_IDEF_INLINE unsigned char CSL_edma3GetDMAChannelToEventQueueMapping
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel
)
{
    unsigned char   dmaRegIndex;
    unsigned char   lsb;

    /* There are 8 channels per register; use this to determine the DMAQNUM register Index. */
    dmaRegIndex = dmaChannel >> 3;

    /* Compute the bit position from where the value is to be retreived. */
    lsb = (dmaChannel - (dmaRegIndex * 8)) << 2;

    /* Get the event queue. */
    return CSL_FEXTR(hModule->regs->TPCC_DMAQNUM[dmaRegIndex], lsb + 2, lsb);
}


CSL_IDEF_INLINE void CSL_edma3MapQDMAChannelToEventQueue
(
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel,
    unsigned char           eventQueue    
)
{
    unsigned char   lsb;

    /* Compute the bit position where the value is to be written. */
    lsb = qdmaChannel << 2;

    /* Write the event Queue */
    CSL_FINSR(hModule->regs->TPCC_QDMAQNUM, lsb + 2, lsb, eventQueue);
}


CSL_IDEF_INLINE unsigned char CSL_edma3GetQDMAChannelToEventQueueMapping
(
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel
)
{
    unsigned char   lsb;

    /* Compute the bit position from where the value is to be retreived. */
    lsb = qdmaChannel << 2;

    /* Get the event Queue */
    return CSL_FEXTR(hModule->regs->TPCC_QDMAQNUM, lsb + 2, lsb);
}


CSL_IDEF_INLINE void CSL_edma3MapEventQueueToTC
(
    CSL_Edma3Handle hModule,
    unsigned char           eventQueue,
    unsigned char           tcNum
)
{
    unsigned char   lsb;

    /* Compute the bit position where the value is to be written. */
    lsb = eventQueue << 2;

    /* Write the event Queue */
    CSL_FINSR(hModule->regs->TPCC_QUETCMAP, lsb + 2, lsb, tcNum);
    return;
}


CSL_IDEF_INLINE unsigned char CSL_edma3GetEventQueueToTCMapping
(
    CSL_Edma3Handle hModule,
    unsigned char           eventQueue
)
{
    unsigned char   lsb;

    /* Compute the bit position where the value is to be written. */
    lsb = eventQueue << 2;

    /* Get the TC Number */
    return CSL_FEXTR(hModule->regs->TPCC_QUETCMAP, lsb + 2, lsb);
}


CSL_IDEF_INLINE void CSL_edma3SetEventQueuePriority
(
    CSL_Edma3Handle hModule,
    unsigned char           eventQueue,
    unsigned char           priority
)
{
    unsigned char   lsb;

    /* Compute the bit position where the value is to be written. */
    lsb = eventQueue << 2;

    /* Write the priority. */
    CSL_FINSR(hModule->regs->TPCC_QUEPRI, lsb + 2, lsb, priority);
    return;
}


CSL_IDEF_INLINE unsigned char CSL_edma3GetEventQueuePriority
(
    CSL_Edma3Handle hModule,
    unsigned char           eventQueue
)
{
    unsigned char   lsb;

    /* Compute the bit position where the value is to be written. */
    lsb = eventQueue << 2;

    /* Get the priority. */
    return CSL_FEXTR(hModule->regs->TPCC_QUEPRI, lsb + 2, lsb);
}


CSL_IDEF_INLINE void CSL_edma3GetEventMissed 
(
    CSL_Edma3Handle hModule,
    CSL_BitMask32*  missedLo,
    CSL_BitMask32*  missedHi,
    CSL_BitMask32*  missedQdma
)
{
    /* Reading the missed Status registers */
    *missedLo    = hModule->regs->TPCC_EMR;
    *missedHi    = hModule->regs->TPCC_EMRH;
    *missedQdma  = hModule->regs->TPCC_QEMR;
    return;
}


CSL_IDEF_INLINE void CSL_edma3IsDMAChannelMissedEventSet
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel,
    unsigned short*           response
)
{
    /* Determine which register needs to be looked into. */
    if (dmaChannel < 32) 
    {
        /* EMR: Extract the appropriate bit. */
        if (CSL_FEXTR(hModule->regs->TPCC_EMR, dmaChannel, dmaChannel))
            *response = TRUE;
        else
            *response = FALSE;
    } 
    else 
    {
        /* EMRH: Extract the appropriate bit. */
        if (CSL_FEXTR(hModule->regs->TPCC_EMRH, dmaChannel-32, dmaChannel-32))
            *response = TRUE;
        else
            *response = FALSE;
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3IsQDMAChannelMissedEventSet
(
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel,
    unsigned short*           response
)
{
    /* Extract the appropriate QDMA Channel bit. */
    if (CSL_FEXTR(hModule->regs->TPCC_QEMR, qdmaChannel, qdmaChannel))
        *response = TRUE;
    else
        *response = FALSE;
    
    return;
}


CSL_IDEF_INLINE void CSL_edma3EventsMissedClear 
(
    CSL_Edma3Handle hModule,
    CSL_BitMask32   missedLo,
    CSL_BitMask32   missedHi,
    CSL_BitMask32   missedQdma
)
{
    /* Clear the reported missed events. */
    hModule->regs->TPCC_EMCR   = missedLo;
    hModule->regs->TPCC_EMCRH  = missedHi;
    hModule->regs->TPCC_QEMCR  = missedQdma;
    return;
}

 
CSL_IDEF_INLINE void CSL_edma3ClearDMAMissedEvent
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel
)
{
    /* Determine which register needs to be looked into. */
  if (dmaChannel < 32)
    {
        /* EMCR: Set the appropriate DMA Channel bit.  */
        hModule->regs->TPCC_EMCR  = CSL_FMKR (dmaChannel, dmaChannel, 1);
    }
    else
  {
        /* EMCRH: Set the appropriate DMA Channel bit. */
        dmaChannel = dmaChannel - 32;
        hModule->regs->TPCC_EMCRH = CSL_FMKR (dmaChannel, dmaChannel, 1);
  }
    return;
}


CSL_IDEF_INLINE void CSL_edma3ClearQDMAMissedEvent
(
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel
)
{    
    /* Set the appropriate QDMA Channel bit.  */
    CSL_FINSR(hModule->regs->TPCC_QEMCR, qdmaChannel, qdmaChannel, 1);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetControllerError 
(
    CSL_Edma3Handle         hModule,
    CSL_Edma3CtrlErrStat*   ccStat
)
{
    /* Extract the Queue Thresholds and TCC-Error. */
    ccStat->error     = hModule->regs->TPCC_CCERR & 0xFF ; 
    ccStat->exceedTcc = (unsigned short)(CSL_FEXT(hModule->regs->TPCC_CCERR, TPCC_TPCC_CCERR_TCCERR));
    return;     
}

   
CSL_IDEF_INLINE void CSL_edma3ClearControllerError 
(
    CSL_Edma3Handle         hModule,
    CSL_Edma3CtrlErrStat*   ccStat
)
{
    /* Clears the errors */
    hModule->regs->TPCC_CCERRCLR = CSL_FMK(TPCC_TPCC_CCERR_TCCERR, ccStat->exceedTcc) | ccStat->error;
    return;
}


CSL_IDEF_INLINE void CSL_edma3ErrorEval (CSL_Edma3Handle hModule)
{
    hModule->regs->TPCC_EEVAL = CSL_FMK(TPCC_TPCC_EEVAL_EVAL, 1);
    return;
}


CSL_IDEF_INLINE void CSL_edma3DmaRegionAccessEnable 
(
    CSL_Edma3Handle       hModule,
    signed int                   edmaRegion,
    CSL_BitMask32         drae,
    CSL_BitMask32         draeh
)
{
    /* Set the appropriate bit masks. */
    hModule->regs->TPCC_DRA[edmaRegion].DRAE  |= drae;
    hModule->regs->TPCC_DRA[edmaRegion].DRAEH |= draeh;
    return;
}


CSL_IDEF_INLINE void CSL_edma3DmaRegionAccessDisable 
(
    CSL_Edma3Handle hModule,
    signed int             edmaRegion,
    CSL_BitMask32   drae,
    CSL_BitMask32   draeh
)
{
    /* Clear the appropriate bit masks. */ 
    hModule->regs->TPCC_DRA[edmaRegion].DRAE  &= ~drae;
    hModule->regs->TPCC_DRA[edmaRegion].DRAEH &= ~draeh;    
    return;
}


CSL_IDEF_INLINE void CSL_edma3QdmaRegionAccessEnable 
(
    CSL_Edma3Handle hModule,
    signed int             edmaRegion,
    CSL_BitMask32   qrae
)
{
    /* Set the appropriate bit masks. */
    hModule->regs->TPCC_QRAE[edmaRegion] |= qrae;
    return;
}


CSL_IDEF_INLINE void CSL_edma3QdmaRegionAccessDisable 
(
    CSL_Edma3Handle        hModule,
    signed int                    edmaRegion,
    CSL_BitMask32          qrae
)
{ 
    /* Clear the appropriate bits */       
    hModule->regs->TPCC_QRAE[edmaRegion] &= ~qrae;    
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetWaterMark 
(
    CSL_Edma3Handle  hModule,
    unsigned char            eventQueue,
    unsigned char*           waterMark
)
{
    /* Extract the watermark from the appropriate event queue. */
    *waterMark = CSL_FEXT(hModule->regs->TPCC_QSTAT[eventQueue], TPCC_TPCC_QSTAT0_WM);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetNumberValidEntries
(
    CSL_Edma3Handle  hModule,
    unsigned char            eventQueue,
    unsigned char*           numValidEntries
)
{
    /* Extract the number of valid entries from the appropriate event queue. */
    *numValidEntries = CSL_FEXT(hModule->regs->TPCC_QSTAT[eventQueue], TPCC_TPCC_QSTAT0_NUMVAL);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetStartPointer
(
    CSL_Edma3Handle  hModule,
    unsigned char            eventQueue,
    unsigned char*           startPtr
)
{
    /* Extract the start pointer from the appropriate event queue. */
    *startPtr = CSL_FEXT(hModule->regs->TPCC_QSTAT[eventQueue], TPCC_TPCC_QSTAT0_STRPTR);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetThresholdExceeded
(
    CSL_Edma3Handle  hModule,
    unsigned char            eventQueue,
    unsigned short*            thresholdExceeded
)
{
    /* Extract the threshold exceeded from the appropriate event queue. */
    *thresholdExceeded = (unsigned short)CSL_FEXT(hModule->regs->TPCC_QSTAT[eventQueue], TPCC_TPCC_QSTAT0_THRXCD);
    return;
}


CSL_IDEF_INLINE void CSL_edma3EventQueueThresholdSet 
(
    CSL_Edma3Handle  hModule,
    unsigned char            eventQueue,
    unsigned char            threshold
)
{
    /* Determine which register needs to be configured: QWMTHRA or QWMTHRB */
  if (eventQueue < 4)
    {
        /* TPCC_QWMTHRA: Set the correct bits with the threshold value. */
    CSL_FINSR(hModule->regs->TPCC_QWMTHRA, (8 * eventQueue + 4), (8 * eventQueue),threshold);
    }
    else
  {
        /* TPCC_QWMTHRB: Set the correct bits with the threshold value. */
    eventQueue -= 4;
    CSL_FINSR(hModule->regs->TPCC_QWMTHRB, (8 * eventQueue + 4), (8 * eventQueue), threshold);
  }
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetActivityStatus 
(
    CSL_Edma3Handle         hModule,
    CSL_Edma3ActivityStat*  activityStat
)
{
    unsigned int value = hModule->regs->TPCC_CCSTAT;

    /* Populate the activity status structure. */
    activityStat->evtActive      = (unsigned short)CSL_FEXT(value, TPCC_TPCC_CCSTAT_EVTACTV);
    activityStat->qevtActive     = (unsigned short)CSL_FEXT(value, TPCC_TPCC_CCSTAT_QEVTACTV);
    activityStat->trActive       = (unsigned short)CSL_FEXT(value, TPCC_TPCC_CCSTAT_TRACTV);
    activityStat->active         = (unsigned short)CSL_FEXT(value, TPCC_TPCC_CCSTAT_ACTV);
    activityStat->outstandingTcc = CSL_FEXT(value, TPCC_TPCC_CCSTAT_COMP_ACTV);
    activityStat->queActive      = CSL_FEXTR(value, 23,16);
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetMemoryFaultError 
(
    CSL_Edma3Handle             hModule,
    CSL_Edma3MemFaultStat*      memFault
)
{
    unsigned int value = hModule->regs->TPCC_MPFSR;

    /* Extract the memory fault address. */
    memFault->addr  = CSL_FEXT(hModule->regs->TPCC_MPFAR, TPCC_TPCC_MPFAR_FADDR); 

    /* Extract the fault ID */
    memFault->fid   = CSL_FEXT(value, TPCC_TPCC_MPFSR_FID);

    /* Extract the error access bits. */
    memFault->error = CSL_FEXTR(value, 8, 0);
    return;
}


CSL_IDEF_INLINE void CSL_edma3MemFaultClear (CSL_Edma3Handle hModule)
{
    /* Write a 1 to clear the memory fault. */
    hModule->regs->TPCC_MPFCR = CSL_FMK(TPCC_TPCC_MPFCR_MPFCLR, 1);
    return; 
}


CSL_IDEF_INLINE void CSL_edma3GetMemoryProtectionAttrib 
(
    CSL_Edma3Handle     hModule,
    signed int                 region,
    CSL_BitMask32*      mppa
)
{
    /* Determine which region is being queried. */
    if (region < 0)
    {
        /* Get the Global Memory Protection Attributes */
        *mppa = hModule->regs->TPCC_MPPAG;
    }
    else
    {
        /* Get the Memory Protection Attributes for the specific region. */
        *mppa = hModule->regs->TPCC_MPPA[region];
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3SetMemoryProtectionAttrib
(
    CSL_Edma3Handle        hModule,
    signed int                    region,
    CSL_BitMask32          mppa
)
{   
    /* Determine which region is being configured.*/
    if (region < 0) 
    {
        /* Set the Global Memory Protection Attributes */
        hModule->regs->TPCC_MPPAG = mppa;
    }
    else 
    {
        /* Set the Memory Protection Attributes for the specific region. */
        hModule->regs->TPCC_MPPA[region] = mppa;
    }
    return;
}    


CSL_IDEF_INLINE void CSL_edma3IsDMAChannelEventPending
(
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel,
    unsigned short*           response
)
{
    /* Determine which register needs to be looked into. */
    if (dmaChannel < 32) 
    {
        /* ER: Read the specific DMA Channel bits */
        if (CSL_FEXTR(hModule->regs->TPCC_ER, dmaChannel, dmaChannel))
            *response = TRUE;
        else
            *response = FALSE;
    }
    else 
    {
        /* ERH: Read the specific DMA Channel bits */
        if (CSL_FEXTR(hModule->regs->TPCC_ERH, dmaChannel - 32, dmaChannel - 32))
            *response = TRUE;
        else
            *response = FALSE;
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3ClearDMAChannelEvent 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           dmaChannel
)
{
    /* Determine the region for which the event is to be cleared. */
    if (region == CSL_EDMA3_REGION_GLOBAL)
    {
        /* Global: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* ECR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_ECR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* ECRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_ECRH, dmaChannel - 32, dmaChannel - 32, 1);
        }
    }
    else
    {
        /* Shadow: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* ECR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_ECR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* ECRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_ECRH, dmaChannel - 32, dmaChannel - 32, 1);
        }            
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3SetDMAChannelEvent 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           dmaChannel
)
{
    /* Determine the region for which the event is to be set. */
    if (region == CSL_EDMA3_REGION_GLOBAL)
    {
        /* Global: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* ESR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_ESR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* ESRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_ESRH, dmaChannel - 32, dmaChannel - 32, 1);
        }
    }
    else
    {
        /* Shadow: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* ESR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_ESR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* ESRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_ESRH, dmaChannel - 32, dmaChannel - 32, 1);
        }            
    }
    return;
}

 
CSL_IDEF_INLINE void CSL_edma3DMAChannelDisable
(
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           dmaChannel
)
{
    /* Determine the region for which the DMA channel is to be disabled. */
    if (region == CSL_EDMA3_REGION_GLOBAL)
    {
        /* Global: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* EECR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_EECR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* EECRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_EECRH, dmaChannel - 32, dmaChannel - 32, 1);
        }
    }
    else
    {
        /* Shadow: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* EECR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_EECR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* EECRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_EECRH, dmaChannel - 32, dmaChannel - 32, 1);
        }            
    }
    return;
}

 
CSL_IDEF_INLINE void CSL_edma3DMAChannelEnable
(    
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           dmaChannel
)
{
    /* Determine the region for which the DMA channel is to be enabled. */
    if (region == CSL_EDMA3_REGION_GLOBAL)
    {
        /* Global: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* EESR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_EESR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* EESRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->TPCC_EESRH, dmaChannel - 32, dmaChannel - 32, 1);
        }
    }
    else
    {
        /* Shadow: Determine which register needs to be looked into. */
        if (dmaChannel < 32) 
        {
            /* EESR: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_EESR, dmaChannel, dmaChannel, 1);
        }
        else 
        {
            /* EESRH: Write to the specific DMA Channel bits */
            CSL_FINSR(hModule->regs->SHADOW[region].TPCC_EESRH, dmaChannel - 32, dmaChannel - 32, 1);
        }
    }
    return;
}

  
CSL_IDEF_INLINE void CSL_edma3GetDMASecondaryEvents
(    
    CSL_Edma3Handle hModule,
    CSL_BitMask32*  secEventLo,
    CSL_BitMask32*  secEventHi
)
{
    /* Read the Secondary Events */
    *secEventLo = hModule->regs->TPCC_SER;
    *secEventHi = hModule->regs->TPCC_SERH;
    return;
}

 
CSL_IDEF_INLINE void CSL_edma3IsDMAChannelSecondaryEventSet
(    
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel,
    unsigned short*           response
)
{
    /* Determine which register needs to be looked into. */
    if (dmaChannel < 32) 
    {
        /* EMR: Extract the appropriate bit. */
        if (CSL_FEXTR(hModule->regs->TPCC_SER, dmaChannel, dmaChannel))
            *response = TRUE;
        else
            *response = FALSE;
    } 
    else 
    {
        /* EMRH: Extract the appropriate bit. */
        if (CSL_FEXTR(hModule->regs->TPCC_SERH, dmaChannel-32, dmaChannel-32))
            *response = TRUE;
        else
            *response = FALSE;
    }
    return;
}

  
CSL_IDEF_INLINE void CSL_edma3ClearDMASecondaryEvents
(    
    CSL_Edma3Handle hModule,
    CSL_BitMask32   secEventLo,
    CSL_BitMask32   secEventHi
)
{
    /* Clear the Secondary Events */
    hModule->regs->TPCC_SECR  = secEventLo;
    hModule->regs->TPCC_SECRH = secEventHi;
    return;
}

   
CSL_IDEF_INLINE void CSL_edma3ClearDMAChannelSecondaryEvents
(    
    CSL_Edma3Handle hModule,
    unsigned char           dmaChannel
)
{
    /* Determine which register needs to be looked into. */
    if (dmaChannel < 32)
    {
        /* SECR: Write to the appropriate channel. */
        hModule->regs->TPCC_SECR = CSL_FMKR (dmaChannel, dmaChannel, 1);
    }
    else
    {
        /* SECRH: Write to the appropriate channel. */
        dmaChannel = dmaChannel - 32;
        hModule->regs->TPCC_SECRH = CSL_FMKR (dmaChannel, dmaChannel, 1);
    }
}


CSL_IDEF_INLINE void CSL_edma3InterruptLoDisable 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32   intrLo
)
{
    /* Disable the interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {   
        /* Shadow Region */
        hModule->regs->SHADOW[region].TPCC_IECR |= intrLo;
    } 
    else
    {
        /* Global Region */
        hModule->regs->TPCC_IECR |= intrLo;
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3InterruptHiDisable 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32   intrHi
)
{
    /* Disable the interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {   
        /* Shadow Region */            
        hModule->regs->SHADOW[region].TPCC_IECRH |= intrHi;
    } 
    else 
    {
        /* Global Region */
        hModule->regs->TPCC_IECRH |= intrHi;
    }
    return;
}

   
CSL_IDEF_INLINE void CSL_edma3InterruptLoEnable 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32   intrLo
)
{
    /* Enable the interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region */
        hModule->regs->SHADOW[region].TPCC_IESR |= intrLo;
    } 
    else 
    {
        /* Global Region */
        hModule->regs->TPCC_IESR |= intrLo;
    }
    return;
}

    
CSL_IDEF_INLINE void CSL_edma3InterruptHiEnable 
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32   intrHi
)
{
    /* Enable the interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region */
        hModule->regs->SHADOW[region].TPCC_IESRH |= intrHi;
    } 
    else 
    {
        /* Global Region */
        hModule->regs->TPCC_IESRH |= intrHi;
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetLoPendingInterrupts
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32*  intrLo
)
{
    /* Get the pending interrupts depending on the region */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region. */
        *intrLo = hModule->regs->SHADOW[region].TPCC_IPR;
    } 
    else 
    {
        /* Global Region. */
        *intrLo = hModule->regs->TPCC_IPR;
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3GetHiPendingInterrupts
(
    CSL_Edma3Handle hModule,
    signed int             region,
    CSL_BitMask32*  intrHi
)
{
    /* Get the pending interrupts depending on the region */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region. */
        *intrHi = hModule->regs->SHADOW[region].TPCC_IPRH;
    } 
    else 
    {
        /* Global Region. */
        *intrHi = hModule->regs->TPCC_IPRH;
    }
    return;
}

   
CSL_IDEF_INLINE void CSL_edma3ClearLoPendingInterrupts 
(
    CSL_Edma3Handle        hModule,
    signed int                    region,
    CSL_BitMask32          intrLo
)
{
    /* Clear the pending interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region */
        hModule->regs->SHADOW[region].TPCC_ICR  = intrLo;
    } 
    else 
    {
        /* Global Region */
        hModule->regs->TPCC_ICR  = intrLo;
    }   
    return;
}

   
CSL_IDEF_INLINE void CSL_edma3ClearHiPendingInterrupts 
(
    CSL_Edma3Handle        hModule,
    signed int                    region,
    CSL_BitMask32          intrHi
)
{
    /* Clear the pending interrupts depending on the region. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region */
        hModule->regs->SHADOW[region].TPCC_ICRH = intrHi;
    } 
    else 
    {
        /* Global Region */
        hModule->regs->TPCC_ICRH = intrHi;
    }   
    return;
}

    
CSL_IDEF_INLINE void  CSL_edma3InterruptEval 
(
    CSL_Edma3Handle hModule,
    signed int             region
)
{
    /* Determine the region for which the interrupt evaluate needs to be done. */
    if (region != CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Shadow Region. */
        hModule->regs->SHADOW[region].TPCC_IEVAL = CSL_FMK(TPCC_TPCC_IEVAL_EVAL, 1);
    }
    else 
    {
        /* Global Region. */
        hModule->regs->TPCC_IEVAL = CSL_FMK(TPCC_TPCC_IEVAL_EVAL, 1);
    }
    return;
}


CSL_IDEF_INLINE void CSL_edma3IsQDMAChannelEventPending
(
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel,    
    unsigned short*           response
)
{
    /* Read the specific QDMA channel bits. */
    if (CSL_FEXTR(hModule->regs->TPCC_QER, qdmaChannel, qdmaChannel))
        *response = TRUE;
    else
        *response = FALSE;

    return;
}

   
CSL_IDEF_INLINE void CSL_edma3QDMAChannelEnable
(    
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           qdmaChannel
)
{
    /* Determine the region for which the QDMA channel is to be enabled. */
    if (region == CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Global: Write to the specific QDMA Channel bits */
        CSL_FINSR(hModule->regs->TPCC_QEESR, qdmaChannel, qdmaChannel, 1);
    }
    else
    {
        /* Shadow: Write to the specific QDMA Channel bits. */
        CSL_FINSR(hModule->regs->SHADOW[region].TPCC_QEESR, qdmaChannel, qdmaChannel, 1);
    }
    return;
}

  
CSL_IDEF_INLINE void CSL_edma3QDMAChannelDisable
(    
    CSL_Edma3Handle hModule,
    signed int             region,
    unsigned char           qdmaChannel
)
{
    /* Determine the region for which the QDMA channel is to be disabled. */
    if (region == CSL_EDMA3_REGION_GLOBAL) 
    {
        /* Global: Write to the specific QDMA Channel bits */
        CSL_FINSR(hModule->regs->TPCC_QEECR, qdmaChannel, qdmaChannel, 1);
    }
    else
    {
        /* Shadow: Write to the specific QDMA Channel bits. */
        CSL_FINSR(hModule->regs->SHADOW[region].TPCC_QEECR, qdmaChannel, qdmaChannel, 1);
    }
    return;
}

   
CSL_IDEF_INLINE void CSL_edma3GetQDMASecondaryEvents
(    
    CSL_Edma3Handle hModule,
    unsigned int*         qdmaSecEvent
)
{
    /* Read the QDMA Secondary Event. */
    *qdmaSecEvent = hModule->regs->TPCC_QSER;
    return;
}

  
CSL_IDEF_INLINE void CSL_edma3IsQDMAChannelSecondaryEventSet
(    
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel,
    unsigned short*           response
)
{
    /* Check if the QDMA channel bit is set or not? */
    if (CSL_FEXTR(hModule->regs->TPCC_QSER, qdmaChannel, qdmaChannel))
        *response = TRUE;
    else
        *response = FALSE;
    return;
}

  
CSL_IDEF_INLINE void CSL_edma3ClearQDMASecondaryEvents
(    
    CSL_Edma3Handle hModule,
    unsigned int          qdmaSecEvent
)
{
    /* Clears the QDMA Secondary Event. */
    hModule->regs->TPCC_QSECR = qdmaSecEvent;
}

  
CSL_IDEF_INLINE void CSL_edma3ClearQDMAChannelSecondaryEvents
(    
    CSL_Edma3Handle hModule,
    unsigned char           qdmaChannel
)
{
    /* Clears the QDMA Secondary Event. */
    CSL_FINSR (hModule->regs->TPCC_QSECR, qdmaChannel, qdmaChannel, 1);
}

/**
@}
*/

#endif /* _CSL_EDMA3AUX_H_ */
