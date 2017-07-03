/**  
 * @file  csl_cpsgmiiAux.h
 *
 * @brief  
 *  API Auxilary header file for SGMII CSL. Contains the different control 
 *  command and status query functions definations
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

#ifndef _CSL_CPSGMIIAUX_H
#define _CSL_CPSGMIIAUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_cpsgmii.h"

/** @addtogroup CSL_SGMII_FUNCTION
@{ */

/** ============================================================================
 *   @n@b CSL_SGMII_getVersionInfo
 *
 *   @b Description
 *   @n This function retrieves the SGMII version information for the SGMII port
 *      specified.
 *
 *   @b Arguments
     @verbatim
        portNum             SGMII port number for which settings must be retrieved.
        sgmiiVersionInfo    CSL_SGMII_VERSION structure that needs to be populated
                            with the version info read from the SGMII_IDVER register.
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
 *   @n CPSGMII_IDVER_REG_MINOR_VER,
 *      CPSGMII_IDVER_REG_MAJOR_VER,
 *      CPSGMII_IDVER_REG_RTL_VER,
 *      CPSGMII_IDVER_REG_TX_IDENT
 *
 *   @b Example
 *   @verbatim
        CSL_SGMII_VERSION   sgmiiVersionInfo;
        Uint32              portNum = 1;

        CSL_SGMII_getVersion (portNum, &sgmiiVersionInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_getVersionInfo 
(
    Uint32                  portNum,
	CSL_SGMII_VERSION*      sgmiiVersionInfo     
)
{
    sgmiiVersionInfo->minor_version     =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].IDVER_REG, CPSGMII_IDVER_REG_MINOR_VER);
    sgmiiVersionInfo->major_version     =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].IDVER_REG, CPSGMII_IDVER_REG_MAJOR_VER);
    sgmiiVersionInfo->rtl_version       =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].IDVER_REG, CPSGMII_IDVER_REG_RTL_VER);
    sgmiiVersionInfo->ident_val         =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].IDVER_REG, CPSGMII_IDVER_REG_TX_IDENT);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_doSoftReset
 *
 *   @b Description
 *   @n This function initiates a CPSGMII logic software reset for the SGMII port
 *      specified by setting the 'SOFT_RESET' bit of the CPSGMII Software Reset 
 *      register to 1.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which reset must be performed.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Software reset on CPSGMII port is initiated. The Soft reset bit is set
 *       back to 0 once the reset operation is completed.
 *
 *   @b Writes
 *   @n CPSGMII_SOFT_RESET_REG_SOFT_RESET=1
 *
 *   @b Affects
 *   @n CPSGMII_SOFT_RESET_REG_SOFT_RESET=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum = 1;

        CSL_SGMII_doSoftReset (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_doSoftReset 
(
	Uint32                  portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].SOFT_RESET_REG, CPSGMII_SOFT_RESET_REG_SOFT_RESET, 1);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_getSoftResetStatus
 *
 *   @b Description
 *   @n This function reads the 'SOFT_RESET' bit of the CPSGMII Software Reset 
 *      register and returns its contents for the port specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which reset status must be 
                            retrieved.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_SOFT_RESET_REG_SOFT_RESET
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum = 1;

        if (!CSL_SGMII_getSoftResetStatus (portNum))
        {
            // Software reset is completed.
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_getSoftResetStatus 
(
	Uint32                  portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].SOFT_RESET_REG, CPSGMII_SOFT_RESET_REG_SOFT_RESET);
}

/** ============================================================================
 *   @n@b CSL_SGMII_startRxTxSoftReset
 *
 *   @b Description
 *   @n This function initiates a SGMII Receive and Transmit logic software reset 
 *      by setting the 'RT_SOFT_RESET' bit of the CPSGMII Software Reset register 
 *      to 1. This reset is to be started when switching between normal and loopback
 *      modes of operation.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which reset must started.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII Rx/Tx Software logic reset initiated.
 *
 *   @b Writes
 *   @n CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET=1
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum = 1;

        CSL_SGMII_startRxTxSoftReset (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_startRxTxSoftReset 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].SOFT_RESET_REG, CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET, 1);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_endRxTxSoftReset
 *
 *   @b Description
 *   @n This function terminates a SGMII Receive and Transmit logic software reset 
 *      initiated earlier by calling @a CSL_SGMII_startRxTxSoftReset () API. This API
 *      sets the 'RT_SOFT_RESET' bit of the CPSGMII Software Reset register 
 *      to 0 to do so. This API is to be called once the loopback/normal switch is done.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which reset must be ended.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  @a CSL_SGMII_startRxTxSoftReset () API must have been called earlier to
 *       initiate the Rx/Tx Soft reset before calling this API.
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII Rx/Tx Software logic reset cleared.
 *
 *   @b Writes
 *   @n CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_endRxTxSoftReset (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_endRxTxSoftReset 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].SOFT_RESET_REG, CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET, 0);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_getRxTxSoftResetStatus
 *
 *   @b Description
 *   @n This function reads the 'RT_SOFT_RESET' bit of the CPSGMII Software Reset 
 *      register and returns its contents. 
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which reset status must be 
                            retrieved.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        if (!CSL_SGMII_getRxTxSoftResetStatus (portNum))
        {
            // Software reset is completed.
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_getRxTxSoftResetStatus 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].SOFT_RESET_REG, CPSGMII_SOFT_RESET_REG_RT_SOFT_RESET);
}

/** ============================================================================
 *   @n@b CSL_SGMII_enableMasterMode
 *
 *   @b Description
 *   @n This function configures the CPSGMII in 'Master' mode. Master mode allows
 *      a CPSGMII direct connection with auto-negotiation  or with forced link.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which settings must be configured
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII Master mode enabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_MASTER=1
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_enableMasterMode (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_enableMasterMode 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MASTER, 1);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_disableMasterMode
 *
 *   @b Description
 *   @n This function configures the CPSGMII in 'Slave' mode or in other words
 *      disables the master mode for the port specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which settings must be configured.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII Master mode disabled, slave mode enabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_MASTER=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_disableMasterMode (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_disableMasterMode 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MASTER, 0);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_isMasterModeEnabled
 *
 *   @b Description
 *   @n This function reads the 'MASTER' bit of the CPSGMII Control register and 
 *      returns 1 to indicate that the CPSGMII is configured in 'Master' mode 
 *      and 0 otherwise to indicate 'Slave' mode.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which settings must be retrieved.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_CONTROL_REG_MASTER
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        if (!CSL_SGMII_isMasterModeEnabled (portNum))
        {
            // Slave Mode
        }
        else
        {
            // Master mode
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_isMasterModeEnabled 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MASTER);
}

/** ============================================================================
 *   @n@b CSL_SGMII_enableLoopback
 *
 *   @b Description
 *   @n This function configures the CPSGMII in internal loopback mode for the
 *      port number specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which loopback must be enabled.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII internal loopback mode enabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_LOOPBACK=1
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_enableLoopback (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_enableLoopback 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_LOOPBACK, 1);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_disableLoopback
 *
 *   @b Description
 *   @n This function disables CPSGMII internal loopback for the port specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which loopback must be disabled
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII loopback mode disabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_LOOPBACK=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_disableLoopback (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_disableLoopback 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_LOOPBACK, 0);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_isLoopbackModeEnabled
 *
 *   @b Description
 *   @n This function reads the 'LOOPBACK' bit of the CPSGMII Control register and 
 *      returns 1 to indicate that the CPSGMII is configured in 'Loopback' mode 
 *      and 0 otherwise.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which loopback mode must be read.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_CONTROL_REG_LOOPBACK
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        if (CSL_SGMII_isLoopbackModeEnabled (portNum))
        {
            // Loopback mode enabled.
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_isLoopbackModeEnabled 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_LOOPBACK);
}

/** ============================================================================
 *   @n@b CSL_SGMII_restartAutoNegotiation
 *
 *   @b Description
 *   @n This function restarts link auto-negotiation process for the port 
 *      specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which the operation must be 
                            performed
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII auto-negotiation restarted.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_MR_AN_RESTART=1,
 *      CPSGMII_CONTROL_REG_MR_AN_RESTART=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_restartAutoNegotiation (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_restartAutoNegotiation 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MR_AN_RESTART, 1);
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MR_AN_RESTART, 0);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_enableAutoNegotiation
 *
 *   @b Description
 *   @n This function enables auto-negotiation process for the port specified.
 *
 *   @b Arguments
     @verbatim
        portNum             SGMII port number for which the operation must be
                            performed.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII auto-negotiation enabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_MR_AN_ENABLE=1
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_enableAutoNegotiation (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_enableAutoNegotiation 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MR_AN_ENABLE, 1);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_disableAutoNegotiation
 *
 *   @b Description
 *   @n This function disables auto-negotiation process for the port number 
 *      specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which the operation must be 
                            performed
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSGMII auto-negotiation disabled.
 *
 *   @b Writes
 *   @n CPSGMII_CONTROL_REG_MR_AN_ENABLE=0
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        CSL_SGMII_disableAutoNegotiation (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_disableAutoNegotiation 
(
    Uint32                      portNum
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MR_AN_ENABLE, 0);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_isAutoNegotiationEnabled
 *
 *   @b Description
 *   @n This function reads the 'MR_AN_ENABLE' bit of the CPSGMII Control register and 
 *      returns 1 to indicate that the CPSGMII is configured in 'auto-negotiate' mode 
 *      and 0 otherwise.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which autonegotiation must be enabled.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_CONTROL_REG_MR_AN_ENABLE
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;

        if (CSL_SGMII_isAutoNegotiationEnabled (portNum))
        {
            // auto-negotiation enabled.
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_isAutoNegotiationEnabled 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].CONTROL_REG, CPSGMII_CONTROL_REG_MR_AN_ENABLE);
}

/** ============================================================================
 *   @n@b CSL_SGMII_getStatus
 *
 *   @b Description
 *   @n This function retrieves the SGMII status information from the hardware.
 *      The status info returned by this function are valid only if
 *      the 'bIsLocked' bit of status info is set to 1 and must be disregarded
 *      otherwise.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which status must be retrieved.
 *      pSgmiiStatus        CSL_SGMII_STATUS structure pointer that needs to be
 *                          populated with link status information.
 *   @endverbatim                          
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_STATUS_REG_LINK,
 *      CPSGMII_STATUS_REG_AN_ERROR,
 *      CPSGMII_STATUS_REG_MR_AN_COMPLETE,
 *      CPSGMII_STATUS_REG_LOCK
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;
        CSL_SGMII_STATUS    sgmiiStatus;

        // Get link status info
        CSL_SGMII_getStatus (portNum, &sgmiiStatus);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_getStatus 
(
    Uint32                  portNum,
	CSL_SGMII_STATUS*       pSgmiiStatus
)
{
    pSgmiiStatus->bIsLinkUp             =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].STATUS_REG, CPSGMII_STATUS_REG_LINK);
    pSgmiiStatus->bIsAutoNegError       =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].STATUS_REG, CPSGMII_STATUS_REG_AN_ERROR);
    pSgmiiStatus->bIsAutoNegComplete    =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].STATUS_REG, CPSGMII_STATUS_REG_MR_AN_COMPLETE);
    pSgmiiStatus->bIsLocked             =   CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].STATUS_REG, CPSGMII_STATUS_REG_LOCK);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_setAdvAbility
 *
 *   @b Description
 *   @n This function sets up the 'MR_ADV_ABILITY' register as per the input
 *      specified to this function in 'pSgmiiAdvAbility' param.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which configuration must be done.
 *      pSgmiiAdvAbility    CSL_SGMII_ADVABILITY input params structure that contains
 *                          the advertised ability configuration that needs to be
 *                          populated to the hardware registers.
 *   @endverbatim                          
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  MR_ADV_ABILITY register setup accordingly.
 *
 *   @b Writes
 *   @n CPSGMII_MR_ADV_ABILITY_REG
 *
 *   @b Example
 *   @verbatim
        Uint32  portNum =   1;
        CSL_SGMII_STATUS    sgmiiAdvAbility;
        
        sgmiiAdvAbility.bLinkUp     =   1;
        sgmiiAdvAbility.duplexMode  =   CSL_SGMII_FULL_DUPLEX;
        sgmiiAdvAbility.linkSpeed   =   CSL_SGMII_1000_MBPS;

        // Configure SGMII Advertised ability info
        CSL_SGMII_setAdvAbility (portNum, &sgmiiAdvAbility);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_setAdvAbility (
    Uint32                      portNum,
	CSL_SGMII_ADVABILITY*       pSgmiiAdvAbility
)
{
    CSL_FINSR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 11, 10,  pSgmiiAdvAbility->linkSpeed);
    CSL_FINSR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 12, 12,  pSgmiiAdvAbility->duplexMode);
    CSL_FINSR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 15, 15,  pSgmiiAdvAbility->bLinkUp);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_getAdvAbility
 *
 *   @b Description
 *   @n This function rerturns the contents of 'MR_ADV_ABILITY' register in the 
 *      output parameter 'pSgmiiAdvAbility'.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which the read operation must
                            be done.
 *      pSgmiiAdvAbility    CSL_SGMII_ADVABILITY output param structure pointer
 *                          that needs to be populated with advertised ability
 *                          register contents.
 *   @endverbatim                          
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_MR_ADV_ABILITY_REG
 *
 *   @b Example
 *   @verbatim
 *      CSL_SGMII_STATUS    sgmiiAdvAbility;
        Uint32  portNum =   1;

        // Get SGMII Advertised ability info
        CSL_SGMII_getAdvAbility (portNum, &sgmiiAdvAbility);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_getAdvAbility (
    Uint32                      portNum,
	CSL_SGMII_ADVABILITY*       pSgmiiAdvAbility
)
{
    pSgmiiAdvAbility->linkSpeed     =   (CSL_SGMII_LINKSPEED) CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 11, 10);
    pSgmiiAdvAbility->duplexMode    =   (CSL_SGMII_DUPLEXMODE) CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 12, 12);
    pSgmiiAdvAbility->bLinkUp       =   CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_ADV_ABILITY_REG, 15, 15);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_getLinkPartnerAdvAbility
 *
 *   @b Description
 *   @n This function rerturns the contents of 'MR_LP_ADV_ABILITY' register in the 
 *      output parameter 'pSgmiiAdvAbility'.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which read operation must be done
 *      pSgmiiAdvAbility    CSL_SGMII_ADVABILITY output param structure pointer
 *                          that needs to be populated with link partnet advertised 
 *                          ability register contents.
 *   @endverbatim                          
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_MR_LP_ADV_ABILITY_REG
 *
 *   @b Example
 *   @verbatim
 *      CSL_SGMII_STATUS    sgmiiAdvAbility;
        Uint32  portNum =   1;

        // Get SGMII Advertised ability info
        CSL_SGMII_getLinkPartnerAdvAbility (portNum, &sgmiiAdvAbility);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_getLinkPartnerAdvAbility (
    Uint32                      portNum,
	CSL_SGMII_ADVABILITY*       pSgmiiAdvAbility
)
{
    pSgmiiAdvAbility->linkSpeed     =   (CSL_SGMII_LINKSPEED) CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_LP_ADV_ABILITY_REG, 11, 10);
    pSgmiiAdvAbility->duplexMode    =   (CSL_SGMII_DUPLEXMODE) CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_LP_ADV_ABILITY_REG, 12, 12);
    pSgmiiAdvAbility->bLinkUp       =   CSL_FEXTR (hCpSgmiiRegs->SGMII_PORT[portNum].MR_LP_ADV_ABILITY_REG, 15, 15);

    return;        
}

/** ============================================================================
 *   @n@b CSL_SGMII_getTxConfig
 *
 *   @b Description
 *   @n This function retrieves the contents of the Transmit configuration 
 *      register (TX_CFG).
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which read operation must be done.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_TX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      txCfgVal;
        Uint32      portNum =   1;

        txCfgVal = CSL_SGMII_getTxConfig (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_getTxConfig 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].TX_CFG_REG, CPSGMII_TX_CFG_REG_TX_CFG);
}

/** ============================================================================
 *   @n@b CSL_SGMII_setTxConfig
 *
 *   @b Description
 *   @n This function sets up the contents of the Transmit configuration 
 *      register (TX_CFG) for the port specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which configuration must be done
 *      txCfgVal            SERDES transmit configuration value.
 *   @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  TX_CFG register setup with the value provided.
 *
 *   @b Writes
 *   @n CPSGMII_TX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      txCfgVal = 0x00000ea1;
        Uint32      portNum =   1;

        CSL_SGMII_setTxConfig (portNum, txCfgVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_setTxConfig 
(
    Uint32                      portNum,
	Uint32                      txCfgVal             
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].TX_CFG_REG, CPSGMII_TX_CFG_REG_TX_CFG, txCfgVal);

    return;
}

/** ============================================================================
 *   @n@b CSL_SGMII_getRxConfig
 *
 *   @b Description
 *   @n This function retrieves the contents of the Receive configuration 
 *      register (RX_CFG) for the port number specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which read operation must be done
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_RX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      rxCfgVal;
        Uint32      portNum =   1;

        rxCfgVal = CSL_SGMII_getRxConfig (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_getRxConfig 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].RX_CFG_REG, CPSGMII_RX_CFG_REG_RX_CFG);
}

/** ============================================================================
 *   @n@b CSL_SGMII_setRxConfig
 *
 *   @b Description
 *   @n This function sets up the contents of the Receive configuration 
 *      register (RX_CFG) for the port number specified
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which configuration must be done
 *      rxCfgVal            SERDES receive configuration value.
 *   @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  RX_CFG register setup with the value provided.
 *
 *   @b Writes
 *   @n CPSGMII_RX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      rxCfgVal = 0x00081021;
        Uint32      portNum =   1;

        CSL_SGMII_setRxConfig (portNum, rxCfgVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_setRxConfig 
(
    Uint32                      portNum,
	Uint32                      rxCfgVal             
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].RX_CFG_REG, CPSGMII_RX_CFG_REG_RX_CFG, rxCfgVal);

    return;
}

/** ============================================================================
 *   @n@b CSL_SGMII_getAuxConfig
 *
 *   @b Description
 *   @n This function retrieves the contents of the Auxilliary configuration 
 *      register (AUX_CFG) for the port number specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which read operation must be performed.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSGMII_AUX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      auxCfgVal;
        Uint32      portNum =   1;

        auxCfgVal = CSL_SGMII_getAuxConfig (portNum);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_SGMII_getAuxConfig 
(
    Uint32                      portNum
)
{
    return CSL_FEXT (hCpSgmiiRegs->SGMII_PORT[portNum].AUX_CFG_REG, CPSGMII_AUX_CFG_REG_AUX_CFG);
}

/** ============================================================================
 *   @n@b CSL_SGMII_setAuxConfig
 *
 *   @b Description
 *   @n This function sets up the contents of the Auxilliary configuration 
 *      register (AUX_CFG) for the port specified.
 *
 *   @b Arguments   
     @verbatim
        portNum             SGMII port number for which configuration must be done
 *      auxCfgVal           SERDES PLL configuration value.
 *   @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  AUX_CFG register setup with the value provided.
 *
 *   @b Writes
 *   @n CPSGMII_AUX_CFG_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      auxCfgVal = 0x0000000b;
        Uint32      portNum =   1;

        CSL_SGMII_setAuxConfig (portNum, auxCfgVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_SGMII_setAuxConfig 
(
    Uint32                      portNum,
	Uint32                      auxCfgVal             
)
{
    CSL_FINS (hCpSgmiiRegs->SGMII_PORT[portNum].AUX_CFG_REG, CPSGMII_AUX_CFG_REG_AUX_CFG, auxCfgVal);

    return;
}
        

#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
        
