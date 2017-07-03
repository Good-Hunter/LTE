/**  
 * @file  csl_cptsAux.h
 *
 * @brief  
 *  API Auxilary header file for Ethernet Time Synchronization module CSL. 
 *
 *  Contains the different control command and status query functions definations
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
*/

#ifndef _CSL_CPTSAUX_H
#define _CSL_CPTSAUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_cpts.h"

/** @addtogroup CSL_CPTS_FUNCTION
@{ */

/********************************************************************************
********************** Time Synchronization (CPTS) Submodule ********************
********************************************************************************/


/** ============================================================================
 *   @n@b CSL_CPTS_getCptsVersionInfo
 *
 *   @b Description
 *   @n This function retrieves the CPTS module identification and version 
 *      information.
 *
 *   @b Arguments
     @verbatim
        pVersionInfo        CSL_CPTS_VERSION structure that needs to be populated
                            with the version info read from the hardware.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_IDVER_REG_MINOR_VER,
 *      CPTS_IDVER_REG_MAJOR_VER,
 *      CPTS_IDVER_REG_RTL_VER,
 *      CPTS_IDVER_REG_TX_IDENT
 *
 *   @b Example
 *   @verbatim
        CSL_CPTS_VERSION    versionInfo;

        CSL_CPTS_getCptsVersionInfo (&versionInfo);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_getCptsVersionInfo 
(
	CSL_CPTS_VERSION*           pVersionInfo     
)
{

    pVersionInfo->minorVer  =   CSL_FEXT (hCptsRegs->IDVER_REG, CPTS_IDVER_REG_MINOR_VER);
    pVersionInfo->majorVer  =   CSL_FEXT (hCptsRegs->IDVER_REG, CPTS_IDVER_REG_MAJOR_VER);
    pVersionInfo->rtlVer    =   CSL_FEXT (hCptsRegs->IDVER_REG, CPTS_IDVER_REG_RTL_VER);
    pVersionInfo->id        =   CSL_FEXT (hCptsRegs->IDVER_REG, CPTS_IDVER_REG_TX_IDENT);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_isCptsEnabled
 *
 *   @b Description
 *   @n This function indicates if time sync is enabled or not.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   CPTS enabled.
 *	 @n  FALSE                  CPTS disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_CONTROL_REG_CPTS_EN
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPTS_isCptsEnabled () == TRUE)
        {
            // CPTS on
        }
        else
        {
            // CPTS off
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPTS_isCptsEnabled (void)
{

    return CSL_FEXT (hCptsRegs->CONTROL_REG, CPTS_CONTROL_REG_CPTS_EN);
}

/** ============================================================================
 *   @n@b CSL_CPTS_enableCpts
 *
 *   @b Description
 *   @n This function configures the CPTS control register to enable time sync.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPTS_CONTROL_REG_CPTS_EN=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPTS_enableCpts ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_enableCpts (void)
{
    CSL_FINS (hCptsRegs->CONTROL_REG, CPTS_CONTROL_REG_CPTS_EN, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPTS_disableCpts
 *
 *   @b Description
 *   @n This function configures the CPTS control register to disable time sync.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPTS_CONTROL_REG_CPTS_EN=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPTS_disableCpts ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_disableCpts (void)
{
    CSL_FINS (hCptsRegs->CONTROL_REG, CPTS_CONTROL_REG_CPTS_EN, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPTS_getRFTCLKSelectReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the reference clock select 
 *      register.
 *
 *   @b Arguments
     @verbatim
        pRefClockSelect         Reference clock select value read from hardware.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_RFTCLK_SEL_REG_RFTCLK_SEL
 *
 *   @b Example
 *   @verbatim
        Uint32          refClockSelect;

        CSL_CPTS_getRFTCLKSelectReg (&refClockSelect);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_getRFTCLKSelectReg (
	Uint32*                     pRefClockSelect
)
{

    *pRefClockSelect    =   CSL_FEXT (hCptsRegs->RFTCLK_SEL_REG, CPTS_RFTCLK_SEL_REG_RFTCLK_SEL);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_setRFTCLKSelectReg
 *
 *   @b Description
 *   @n This function sets up the reference clock select value. The Reference
 *      clock value can be setup only when the CPTS enable bit is cleared in
 *      the CPTS control register.
 *
 *   @b Arguments
     @verbatim
        refClockSetVal          Reference clock select value to configure.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  @a CSL_CPTS_setTimeSyncControlReg () must be called to clear the
 *      CPTS enable bit before calling this API.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Affects
 *   @n CPTS_RFTCLK_SEL_REG_RFTCLK_SEL
 *
 *   @b Example
 *   @verbatim
        Uint32          refClockSelect;

        refClockSelect  =   0;

        CSL_CPTS_setRFTCLKSelectReg (refClockSelect);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_setRFTCLKSelectReg (
	Uint32                      refClockSetVal
)
{
    CSL_FINS (hCptsRegs->RFTCLK_SEL_REG, CPTS_RFTCLK_SEL_REG_RFTCLK_SEL, refClockSetVal);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_getTSEventPushReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Time Stamp Event Push register.
 *
 *   @b Arguments
     @verbatim
        pTSPushVal              Time stamp push value read from hardware.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_TS_PUSH_REG_TS_PUSH
 *
 *   @b Example
 *   @verbatim
        Uint32          tsPushVal;

        CSL_CPTS_getTSEventPushReg (&tsPushVal);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_getTSEventPushReg (
	Uint32*                     pTSPushVal
)
{
    *pTSPushVal     =   CSL_FEXT (hCptsRegs->TS_PUSH_REG, CPTS_TS_PUSH_REG_TS_PUSH);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_setTSEventPushReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Time Stamp Event Push register.
 *
 *   @b Arguments
     @verbatim
        tsPushVal               Time stamp push value to configure.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Affects
 *   @n CPTS_TS_PUSH_REG_TS_PUSH
 *
 *   @b Example
 *   @verbatim
        Uint32          tsPushVal;

        tsPushVal   =   0;

        CSL_CPTS_setTSEventPushReg (tsPushVal);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_setTSEventPushReg (
	Uint32                      tsPushVal
)
{
    CSL_FINS (hCptsRegs->TS_PUSH_REG, CPTS_TS_PUSH_REG_TS_PUSH, tsPushVal);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_getTSLoadValReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Time Stamp Load Value register.
 *
 *   @b Arguments
     @verbatim
        pTSLoadVal              Time stamp load value read from hardware.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL
 *
 *   @b Example
 *   @verbatim
        Uint32          tsLoadVal;

        CSL_CPTS_getTSLoadValReg (&tsLoadVal);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_getTSLoadValReg (
	Uint32*                     pTSLoadVal
)
{
    *pTSLoadVal     =   CSL_FEXT (hCptsRegs->TS_LOAD_VAL_REG, CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL);

    return;        
}


/** ============================================================================
 *   @n@b CSL_CPTS_setTSLoadValReg
 *
 *   @b Description
 *   @n This function sets up the Time Stamp Load Value.
 *
 *   @b Arguments
     @verbatim
        tsLoadVal               Time stamp load value to configure.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Affects
 *   @n CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL
 *
 *   @b Example
 *   @verbatim
        Uint32          tsLoadVal;

        tsLoadVal   =   0;

        CSL_CPTS_setTSLoadValReg (tsLoadVal);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_setTSLoadValReg 
(
	Uint32                      tsLoadVal
)
{
    CSL_FINS (hCptsRegs->TS_LOAD_VAL_REG, CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL, tsLoadVal);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPTS_isRawInterruptStatusBitSet
 *
 *   @b Description
 *   @n This function checks the Time Sync Raw Pending Interrupt Register to 
 *      determine if there is one or more events in the event FIFO.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   One or more events in FIFO. Raw interrupt status
 *	                            bit set.
 *	 @n  FALSE                  No events in FIFO. Raw interrupt status bit cleared.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_INTSTAT_RAW_REG_TS_PEND_RAW
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPTS_isRawInterruptStatusBitSet () == TRUE)
        {
           // interrupt set 
        }
        else
        {
            // interrupt bit not set
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPTS_isRawInterruptStatusBitSet (void)
{

    return CSL_FEXT (hCptsRegs->INTSTAT_RAW_REG, CPTS_INTSTAT_RAW_REG_TS_PEND_RAW);
}

/** ============================================================================
 *   @n@b CSL_CPTS_isMaskedInterruptStatusBitSet
 *
 *   @b Description
 *   @n This function checks the Time Sync Interrupt Status Masked Register to 
 *      determine if there is one or more events in the event FIFO.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   One or more events in FIFO. masked interrupt status
 *	                            bit set.
 *	 @n  FALSE                  No events in FIFO. masked interrupt status bit cleared.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_INTSTAT_MASKED_REG_TS_PEND
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPTS_isMaskedInterruptStatusBitSet () == TRUE)
        {
           // masked interrupt set 
        }
        else
        {
            // masked interrupt bit not set
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPTS_isMaskedInterruptStatusBitSet (void)
{

    return CSL_FEXT (hCptsRegs->INTSTAT_MASKED_REG, CPTS_INTSTAT_MASKED_REG_TS_PEND);
}

/** ============================================================================
 *   @n@b CSL_CPTS_isInterruptEnabled
 *
 *   @b Description
 *   @n This function indicates if Time sync interrupts are enabled.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   Time sync interrupts enabled.
 *	 @n  FALSE                  Time sync interrupts disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_INT_ENABLE_REG_TS_PEND_EN
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPTS_isInterruptEnabled () == TRUE)
        {
           // interrupts enabled 
        }
        else
        {
            // interrupts disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPTS_isInterruptEnabled (void)
{

    return CSL_FEXT (hCptsRegs->INT_ENABLE_REG, CPTS_INT_ENABLE_REG_TS_PEND_EN);
}

/** ============================================================================
 *   @n@b CSL_CPTS_enableInterrupt
 *
 *   @b Description
 *   @n This function enables the interrupts in Time sync submodule.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPTS_INT_ENABLE_REG_TS_PEND_EN=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPTS_enableInterrupt ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_enableInterrupt (void)
{
    CSL_FINS (hCptsRegs->INT_ENABLE_REG, CPTS_INT_ENABLE_REG_TS_PEND_EN, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPTS_disableInterrupt
 *
 *   @b Description
 *   @n This function disables the interrupts in Time sync submodule.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPTS_INT_ENABLE_REG_TS_PEND_EN=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPTS_disableInterrupt ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_disableInterrupt (void)
{
    CSL_FINS (hCptsRegs->INT_ENABLE_REG, CPTS_INT_ENABLE_REG_TS_PEND_EN, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPTS_popEvent
 *
 *   @b Description
 *   @n This function sets up the Event pop bit in Event pop register. This 
 *      initiates an event to be popped off the event FIFO. Popping an event discards 
 *      the event and causes the next event, if any, to be moved to the top of 
 *      the FIFO ready to be read by software on interrupt.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPTS_EVENT_POP_REG_EVENT_POP=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPTS_popEvent ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_popEvent (void)
{
    CSL_FINS (hCptsRegs->EVENT_POP_REG, CPTS_EVENT_POP_REG_EVENT_POP, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPTS_getEventInfo
 *
 *   @b Description
 *   @n This function retrieves the contents of the Event low and Event High
 *      registers.
 *
 *   @b Arguments
     @verbatim
        pEventInfo              CSL_CPTS_EVENTINFO structure that needs to be filled
                                with time sync event info read from hardware.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPTS_EVENT_LOW_REG_TIME_STAMP,
 *      CPTS_EVENT_HIGH_REG_SEQUENCE_ID,
 *      CPTS_EVENT_HIGH_REG_MESSAGE_TYPE,
 *      CPTS_EVENT_HIGH_REG_EVENT_TYPE,
 *      CPTS_EVENT_HIGH_REG_PORT_NUMBER
 *
 *   @b Example
 *   @verbatim
        CSL_CPTS_EVENTINFO          eventInfo;

        CSL_CPTS_getEventInfo (&eventInfo);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPTS_getEventInfo (
	CSL_CPTS_EVENTINFO*         pEventInfo
)
{
    pEventInfo->timeStamp   =   CSL_FEXT (hCptsRegs->EVENT_LOW_REG, CPTS_EVENT_LOW_REG_TIME_STAMP);
    pEventInfo->seqId       =   CSL_FEXT (hCptsRegs->EVENT_HIGH_REG, CPTS_EVENT_HIGH_REG_SEQUENCE_ID);
    pEventInfo->msgType     =   CSL_FEXT (hCptsRegs->EVENT_HIGH_REG, CPTS_EVENT_HIGH_REG_MESSAGE_TYPE);
    pEventInfo->eventType   =   CSL_FEXT (hCptsRegs->EVENT_HIGH_REG, CPTS_EVENT_HIGH_REG_EVENT_TYPE);
    pEventInfo->portNo      =   CSL_FEXT (hCptsRegs->EVENT_HIGH_REG, CPTS_EVENT_HIGH_REG_PORT_NUMBER);

    return;        
}

#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
