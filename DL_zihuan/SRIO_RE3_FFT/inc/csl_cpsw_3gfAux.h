/**  
 * @file  csl_cpsw_3gfAux.h
 *
 * @brief  
 *  API Auxilary header file for Ethernet switch module CSL. 
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

#ifndef _CSL_CPSW_3GFAUX_H
#define _CSL_CPSW_3GFAUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_cpsw_3gf.h"

/** @addtogroup CSL_CPSW_3GF_FUNCTION
@{ */

/********************************************************************************
************************* Ethernet Switch (CPSW) Submodule **********************
********************************************************************************/


/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getCpswVersionInfo
 *
 *   @b Description
 *   @n This function retrieves the CPSW identification and version information.
 *
 *   @b Arguments
     @verbatim
        pVersionInfo        CSL_CPSW_3GF_VERSION structure that needs to be populated
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
 *   @n CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_MINOR_VER,
 *      CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_MAJ_VER,
 *      CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_RTL_VER,
 *      CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_IDENT
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_VERSION    versionInfo;

        CSL_CPSW_3GF_getCpswVersionInfo (&versionInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getCpswVersionInfo (
	CSL_CPSW_3GF_VERSION*       pVersionInfo     
)
{

    pVersionInfo->minorVer  =   CSL_FEXT (hCpsw3gfRegs->CPSW_ID_VER_REG, CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_MINOR_VER);
    pVersionInfo->majorVer  =   CSL_FEXT (hCpsw3gfRegs->CPSW_ID_VER_REG, CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_MAJ_VER);
    pVersionInfo->rtlVer    =   CSL_FEXT (hCpsw3gfRegs->CPSW_ID_VER_REG, CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_RTL_VER);
    pVersionInfo->id        =   CSL_FEXT (hCpsw3gfRegs->CPSW_ID_VER_REG, CPSW_3GF_CPSW_ID_VER_REG_CPSW_3GF_IDENT);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isFIFOLoopbackEnabled
 *
 *   @b Description
 *   @n This function indicates if FIFO loopback mode is enabled in the CPSW
 *      control register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   FIFO loopback mode enabled. Each packet received is 
 *	                            turned around and sent out on the same port's transmit
 *	                            path.
 *	 @n  FALSE                  FIFO loopback mode disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isFIFOLoopbackEnabled (portNum) == TRUE)
        {
            // FIFO loopback mode enabled
        }
        else
        {
            // FIFO loopback mode disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isFIFOLoopbackEnabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableFIFOLoopback
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable FIFO loopback
 *      mode.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableFIFOLoopback ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableFIFOLoopback (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableFIFOLoopback
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable FIFO loopback
 *      mode
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableFIFOLoopback ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableFIFOLoopback (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isVlanAwareEnabled
 *
 *   @b Description
 *   @n This function indicates if VLAN aware mode is enabled in the CPSW
 *      control register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   VLAN aware mode enabled. 
 *	 @n  FALSE                  VLAN aware mode disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isVlanAwareEnabled (portNum) == TRUE)
        {
            // VLAN aware mode enabled
        }
        else
        {
            // VLAN aware mode disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isVlanAwareEnabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableVlanAware
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable VLAN aware
 *      mode.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableVlanAware ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableVlanAware (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableVlanAware
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable VLAN aware
 *      mode.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableVlanAware ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableVlanAware (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isPort0Enabled
 *
 *   @b Description
 *   @n This function indicates if CPPI Port (Port 0) is enabled in the CPSW
 *      control register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   Port 0 enabled. 
 *	 @n  FALSE                  Port 0 disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isPort0Enabled (portNum) == TRUE)
        {
            // Port 0 enabled
        }
        else
        {
            // Port 0 disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isPort0Enabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enablePort0
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable the Port 0.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enablePort0 ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enablePort0 (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disablePort0
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable the Port 0.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disablePort0 ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disablePort0 (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isPort0PassPriTagEnabled
 *
 *   @b Description
 *   @n This function indicates if priority tagging is enabled for Port 0.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   Port 0 ingress priority tagging enabled. 
 *	 @n  FALSE                  Port 0 ingress priority tagging disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isPort0PassPriTagEnabled (portNum) == TRUE)
        {
            // Port 0 pass priority tagging enabled
        }
        else
        {
            // Port 0 pass priority tagging disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isPort0PassPriTagEnabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enablePort0PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable the Ingress
 *      priority tagging on Port 0.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enablePort0PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enablePort0PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disablePort0PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable the Ingress
 *      priority tagging on Port 0.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disablePort0PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disablePort0PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isPort1PassPriTagEnabled
 *
 *   @b Description
 *   @n This function indicates if priority tagging is enabled for Port 1.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   Port 1 ingress priority tagging enabled. 
 *	 @n  FALSE                  Port 1 ingress priority tagging disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isPort1PassPriTagEnabled (portNum) == TRUE)
        {
            // Port 1 pass priority tagging enabled
        }
        else
        {
            // Port 1 pass priority tagging disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isPort1PassPriTagEnabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enablePort1PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable the Ingress
 *      priority tagging on Port 1.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enablePort1PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enablePort1PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disablePort1PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable the Ingress
 *      priority tagging on Port 1.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disablePort1PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disablePort1PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isPort2PassPriTagEnabled
 *
 *   @b Description
 *   @n This function indicates if priority tagging is enabled for Port 2.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   Port 2 ingress priority tagging enabled. 
 *	 @n  FALSE                  Port 2 ingress priority tagging disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isPort2PassPriTagEnabled (portNum) == TRUE)
        {
            // Port 2 pass priority tagging enabled
        }
        else
        {
            // Port 2 pass priority tagging disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isPort2PassPriTagEnabled (void)
{
    return CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enablePort2PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to enable the Ingress
 *      priority tagging on Port 2.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enablePort2PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enablePort2PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disablePort2PassPriTag
 *
 *   @b Description
 *   @n This function configures the CPSW control register to disable the Ingress
 *      priority tagging on Port 2.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disablePort2PassPriTag ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disablePort2PassPriTag (void)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getCpswControlReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPSW Control register.
 *
 *   @b Arguments
     @verbatim
        pControlRegInfo     CSL_CPSW_CONTROL structure that needs to be populated
                            with the control register contents.
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
 *   @n CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK,
 *      CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE,
 *      CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE,
 *      CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED,
 *      CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED,
 *      CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_CONTROL    controlRegInfo;

        CSL_CPSW_3GF_getCpswControlReg (&controlRegInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getCpswControlReg (
	CSL_CPSW_3GF_CONTROL*   pControlRegInfo     
)
{

    pControlRegInfo->fifoLb         =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK);
    pControlRegInfo->vlanAware      =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE);
    pControlRegInfo->p0Enable       =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE);
    pControlRegInfo->p0PassPriTag   =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED);
    pControlRegInfo->p1PassPriTag   =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED);
    pControlRegInfo->p2PassPriTag   =   CSL_FEXT (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setCpswControlReg
 *
 *   @b Description
 *   @n This function populates the contents of the CPSW Control register.
 *
 *   @b Arguments
     @verbatim
        pControlRegInfo     CSL_CPSW_CONTROL structure that holds the values 
                            that need to be configured to the CPSW control register.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CPSW control register modified with values provided.
 *
 *   @b Writes
 *   @n CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK,
 *      CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE,
 *      CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE,
 *      CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED,
 *      CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED,
 *      CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_CONTROL    controlRegInfo;

        controlRegInfo.fifoLb       =   1;
        controlRegInfo.vlanAware    =   0;
        ...

        CSL_CPSW_3GF_setCpswControlReg (&controlRegInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setCpswControlReg (
	CSL_CPSW_3GF_CONTROL*   pControlRegInfo     
)
{
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_FIFO_LOOPBACK, pControlRegInfo->fifoLb);
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_VLAN_AWARE, pControlRegInfo->vlanAware);
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_ENABLE, pControlRegInfo->p0Enable);
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P0_PASS_PRI_TAGGED, pControlRegInfo->p0PassPriTag);
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P1_PASS_PRI_TAGGED, pControlRegInfo->p1PassPriTag);
    CSL_FINS (hCpsw3gfRegs->CPSW_CONTROL_REG, CPSW_3GF_CPSW_CONTROL_REG_P2_PASS_PRI_TAGGED, pControlRegInfo->p2PassPriTag);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getEmulationControlReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPSW Emulation Control register.
 *
 *   @b Arguments
     @verbatim
        pFree                   Emulation free bit read from the hardware.
        pSoft                   Emulation soft bit read from the hardware.
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
 *   @n CPSW_3GF_EM_CONTROL_REG_FREE,
 *      CPSW_3GF_EM_CONTROL_REG_SOFT
 *
 *   @b Example
 *   @verbatim
        Uint32  free, soft;

        CSL_CPSW_3GF_getEmulationControlReg (&free, &soft);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getEmulationControlReg (
    Uint32*                     pFree,
    Uint32*                     pSoft
)
{

    *pFree      =   CSL_FEXT (hCpsw3gfRegs->EM_CONTROL_REG, CPSW_3GF_EM_CONTROL_REG_FREE);
    *pSoft      =   CSL_FEXT (hCpsw3gfRegs->EM_CONTROL_REG, CPSW_3GF_EM_CONTROL_REG_SOFT);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setEmulationControlReg
 *
 *   @b Description
 *   @n This function sets up the contents of the CPSW Emulation Control register.
 *
 *   @b Arguments
     @verbatim
        free                   Emulation free bit configuration
        soft                   Emulation soft bit configuration
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
 *   @b Writes
 *   @n CPSW_3GF_EM_CONTROL_REG_FREE,
 *      CPSW_3GF_EM_CONTROL_REG_SOFT
 *
 *   @b Example
 *   @verbatim
        Uint32 free, soft;

        free   =   0;
        soft   =   1;

        CSL_CPSW_3GF_setEmulationControlReg (free, soft);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setEmulationControlReg (
    Uint32                      free,
    Uint32                      soft
)
{

    CSL_FINS (hCpsw3gfRegs->EM_CONTROL_REG, CPSW_3GF_EM_CONTROL_REG_FREE, free);
    CSL_FINS (hCpsw3gfRegs->EM_CONTROL_REG, CPSW_3GF_EM_CONTROL_REG_SOFT, soft);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortStatsEnableReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPSW Port Statistics
 *      Enable register.
 *
 *   @b Arguments
     @verbatim
        pPortStatsCfg       CSL_CPSW_3GF_PORTSTAT structure that needs to be populated
                            with the port statistics enable register contents.
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
 *   @n CPSW_3GF_STAT_PORT_EN_REG_P0A_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P0B_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P1_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P2_STAT_EN
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_PORTSTAT       portStatsCfg;

        CSL_CPSW_3GF_getPortStatsEnableReg (&portStatsCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortStatsEnableReg (
	CSL_CPSW_3GF_PORTSTAT*  pPortStatsCfg     
)
{
    pPortStatsCfg->p0AStatEnable    =   CSL_FEXT (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P0A_STAT_EN);
    pPortStatsCfg->p0BStatEnable    =   CSL_FEXT (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P0B_STAT_EN);
    pPortStatsCfg->p1StatEnable     =   CSL_FEXT (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P1_STAT_EN);
    pPortStatsCfg->p2StatEnable     =   CSL_FEXT (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P2_STAT_EN);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortStatsEnableReg
 *
 *   @b Description
 *   @n This function sets up the contents of the CPSW Port Statistics
 *      Enable register.
 *
 *   @b Arguments
     @verbatim
        pPortStatsCfg       CSL_CPSW_3GF_PORTSTAT structure that contains the values
                            to be used to setup port statistics enable register.
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
 *   @b Writes
 *   @n CPSW_3GF_STAT_PORT_EN_REG_P0A_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P0B_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P1_STAT_EN,
 *      CPSW_3GF_STAT_PORT_EN_REG_P2_STAT_EN
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_PORTSTAT       portStatsCfg;

        portStatsCfg.p0AStatEnable  =   1;
        portStatsCfg.p0BStatEnable  =   1;
        portStatsCfg.p1StatEnable   =   1;
        portStatsCfg.p2StatEnable   =   1;

        CSL_CPSW_3GF_setPortStatsEnableReg (&portStatsCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortStatsEnableReg (
	CSL_CPSW_3GF_PORTSTAT*  pPortStatsCfg     
)
{
    CSL_FINS (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P0A_STAT_EN, pPortStatsCfg->p0AStatEnable);
    CSL_FINS (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P0B_STAT_EN, pPortStatsCfg->p0BStatEnable);
    CSL_FINS (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P1_STAT_EN, pPortStatsCfg->p1StatEnable);
    CSL_FINS (hCpsw3gfRegs->STAT_PORT_EN_REG, CPSW_3GF_STAT_PORT_EN_REG_P2_STAT_EN, pPortStatsCfg->p2StatEnable);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPrioTypeReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPSW Priority Type
 *      register.
 *
 *   @b Arguments
     @verbatim
        pTypeCfg            CSL_CPSW_3GF_PTYPE structure that needs to be populated
                            with the priority type register contents.
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
 *   @n CPSW_3GF_PTYPE_REG_ESC_PRI_LD_VAL,
 *      CPSW_3GF_PTYPE_REG_P0_PTYPE_ESC,
 *      CPSW_3GF_PTYPE_REG_P1_PTYPE_ESC,
 *      CPSW_3GF_PTYPE_REG_P2_PTYPE_ESC
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_PTYPE       pTypeCfg;

        CSL_CPSW_3GF_getPrioTypeReg (&pTypeCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPrioTypeReg (
	CSL_CPSW_3GF_PTYPE*     pTypeCfg     
)
{
    pTypeCfg->escPriLdVal   =   CSL_FEXT (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_ESC_PRI_LD_VAL);
    pTypeCfg->p0PtypeEsc    =   CSL_FEXT (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P0_PTYPE_ESC);
    pTypeCfg->p1PtypeEsc    =   CSL_FEXT (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P1_PTYPE_ESC);
    pTypeCfg->p2PtypeEsc    =   CSL_FEXT (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P2_PTYPE_ESC);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPrioTypeReg
 *
 *   @b Description
 *   @n This function modifies the contents of the CPSW Priority Type
 *      register.
 *
 *   @b Arguments
     @verbatim
        pTypeCfg            CSL_CPSW_3GF_PTYPE structure that contains the values that
                            need to be populated to Priority type register.
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
 *   @b Writes
 *   @n CPSW_3GF_PTYPE_REG_ESC_PRI_LD_VAL,
 *      CPSW_3GF_PTYPE_REG_P0_PTYPE_ESC,
 *      CPSW_3GF_PTYPE_REG_P1_PTYPE_ESC,
 *      CPSW_3GF_PTYPE_REG_P2_PTYPE_ESC
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_PTYPE       pTypeCfg;

        pTypeCfg.escPriLdVal    =   1;
        pTypeCfg.p0PtypeEsc     =   1;
        ...

        CSL_CPSW_3GF_setPrioTypeReg (&pTypeCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPrioTypeReg (
	CSL_CPSW_3GF_PTYPE*     pTypeCfg     
)
{
    CSL_FINS (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_ESC_PRI_LD_VAL, pTypeCfg->escPriLdVal);
    CSL_FINS (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P0_PTYPE_ESC, pTypeCfg->p0PtypeEsc);
    CSL_FINS (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P1_PTYPE_ESC, pTypeCfg->p1PtypeEsc);
    CSL_FINS (hCpsw3gfRegs->PTYPE_REG, CPSW_3GF_PTYPE_REG_P2_PTYPE_ESC, pTypeCfg->p2PtypeEsc);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getShortGapThreshold
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPSW MAC Short Gap Threshold
 *      register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 >=0                    MAC short gap threshold value read from the hardware.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_GAP_THRESH_CPGMAC_SL_REG_GAP_THRESH
 *
 *   @b Example
 *   @verbatim
 *      Uint32          gapThreshVal;

        gapThreshVal    =   CSL_CPSW_3GF_getShortGapThreshold ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_getShortGapThreshold (void)
{
    return CSL_FEXT (hCpsw3gfRegs->GAP_THRESH_CPGMAC_SL_REG, CPSW_3GF_GAP_THRESH_CPGMAC_SL_REG_GAP_THRESH);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setShortGapThreshold
 *
 *   @b Description
 *   @n This function sets up the contents of the CPSW MAC Short Gap Threshold
 *      register.
 *
 *   @b Arguments
     @verbatim
        gapThreshVal        Gap threshold value to use to configure the MAC Short
                            gap threshold value.
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
 *   @b Writes
 *   @n CPSW_3GF_GAP_THRESH_CPGMAC_SL_REG_GAP_THRESH
 *
 *   @b Example
 *   @verbatim
 *      Uint32          gapThreshVal;
 *
 *      gapThreshVal    =   11;

        CSL_CPSW_3GF_setShortGapThreshold (gapThreshVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setShortGapThreshold (
    Uint32                  gapThreshVal                
)
{
    CSL_FINS (hCpsw3gfRegs->GAP_THRESH_CPGMAC_SL_REG, CPSW_3GF_GAP_THRESH_CPGMAC_SL_REG_GAP_THRESH, gapThreshVal);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getTxStartWordsReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Transmit FIFO start words
 *      register.
 *
 *   @b Arguments
     @n None
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
 *   @n CPSW_3GF_TX_START_WDS_REG_TX_START_WDS
 *
 *   @b Example
 *   @verbatim
 *      Uint32          startWordsVal;

        startWordsVal   =   CSL_CPSW_3GF_getTxStartWordsReg ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_getTxStartWordsReg (void)
{
    return CSL_FEXT (hCpsw3gfRegs->TX_START_WDS_REG, CPSW_3GF_TX_START_WDS_REG_TX_START_WDS);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setTxStartWordsReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Transmit FIFO start words
 *      register.
 *
 *   @b Arguments
     @verbatim
        pStartWordsVal      Start words value to be set to the Transmit FIFO start
                            words register.
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
 *   @b Writes
 *   @n CPSW_3GF_TX_START_WDS_REG_TX_START_WDS
 *
 *   @b Example
 *   @verbatim
 *      Uint32          startWordsVal;
 *
 *      startWordsVal   =   32;

        CSL_CPSW_3GF_setTxStartWordsReg (startWordsVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setTxStartWordsReg (
    Uint32                  startWordsVal                
)
{
    CSL_FINS (hCpsw3gfRegs->TX_START_WDS_REG, CPSW_3GF_TX_START_WDS_REG_TX_START_WDS, startWordsVal);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getFlowControlReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Flow control register.
 *
 *   @b Arguments
     @verbatim
        pFlowControlCfg     CSL_CPSW_3GF_FLOWCNTL structure that needs to be populated
                            with contents of Flow control register.
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
 *   @n CPSW_3GF_FLOW_CONTROL_REG_P0_FLOW_EN,
 *      CPSW_3GF_FLOW_CONTROL_REG_P1_FLOW_EN,
 *      CPSW_3GF_FLOW_CONTROL_REG_P2_FLOW_EN
 *
 *   @b Example
 *   @verbatim
 *      CSL_CPSW_3GF_FLOWCNTL       flowControlCfg;

        CSL_CPSW_3GF_getFlowControlReg (&flowControlCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getFlowControlReg (
    CSL_CPSW_3GF_FLOWCNTL*  pFlowControlCfg    
)
{
    pFlowControlCfg->p0FlowEnable   =   CSL_FEXT (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P0_FLOW_EN);
    pFlowControlCfg->p1FlowEnable   =   CSL_FEXT (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P1_FLOW_EN);
    pFlowControlCfg->p2FlowEnable   =   CSL_FEXT (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P2_FLOW_EN);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setFlowControlReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Flow control register.
 *
 *   @b Arguments
     @verbatim
        pFlowControlCfg     CSL_CPSW_3GF_FLOWCNTL structure that contains the values
                            that need to be configured to Flow control register.
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
 *   @b Writes
 *   @n CPSW_3GF_FLOW_CONTROL_REG_P0_FLOW_EN,
 *      CPSW_3GF_FLOW_CONTROL_REG_P1_FLOW_EN,
 *      CPSW_3GF_FLOW_CONTROL_REG_P2_FLOW_EN
 *
 *   @b Example
 *   @verbatim
 *      CSL_CPSW_3GF_FLOWCNTL       flowControlCfg;
 
        flowControlCfg.p0FlowEnable =   1;
        flowControlCfg.p1FlowEnable =   1;:
        flowControlCfg.p2FlowEnable =   1;

        CSL_CPSW_3GF_setFlowControlReg (&flowControlCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setFlowControlReg (
    CSL_CPSW_3GF_FLOWCNTL*  pFlowControlCfg    
)
{
    CSL_FINS (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P0_FLOW_EN, pFlowControlCfg->p0FlowEnable);
    CSL_FINS (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P1_FLOW_EN, pFlowControlCfg->p1FlowEnable);
    CSL_FINS (hCpsw3gfRegs->FLOW_CONTROL_REG, CPSW_3GF_FLOW_CONTROL_REG_P2_FLOW_EN, pFlowControlCfg->p2FlowEnable);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getCppiSourceIdReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the CPPI Source Identification
 *      register.
 *
 *   @b Arguments
     @verbatim
        pTxASrcId           CPPI Info Word0 Source Id Value on TxA.
        pTxBSrcId           CPPI Info Word0 Source Id Value on TxB.
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
 *   @n CPSW_3GF_P0_CPPI_SRC_ID_REG_TXA_SRC_ID,
 *      CPSW_3GF_P0_CPPI_SRC_ID_REG_TXB_SRC_ID
 *
 *   @b Example
 *   @verbatim
 *      Uint32      txASrcId, txBSrcId;

        CSL_CPSW_3GF_getCppiSourceIdReg (&txASrcId, &txBSrcId);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getCppiSourceIdReg (
    Uint32*                 pTxASrcId,
    Uint32*                 pTxBSrcId
)
{
    *pTxASrcId      =   CSL_FEXT (hCpsw3gfRegs->P0_CPPI_SRC_ID_REG, CPSW_3GF_P0_CPPI_SRC_ID_REG_TXA_SRC_ID);
    *pTxBSrcId      =   CSL_FEXT (hCpsw3gfRegs->P0_CPPI_SRC_ID_REG, CPSW_3GF_P0_CPPI_SRC_ID_REG_TXB_SRC_ID);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setCppiSourceIdReg
 *
 *   @b Description
 *   @n This function sets up the contents of the CPPI Source Identification
 *      register.
 *
 *   @b Arguments
     @verbatim
        txASrcId            CPPI Source Id to configure for TxA.
        txBSrcId            CPPI Source Id to configure for TxB.
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
 *   @b Writes
 *   @n CPSW_3GF_P0_CPPI_SRC_ID_REG_TXA_SRC_ID,
 *      CPSW_3GF_P0_CPPI_SRC_ID_REG_TXB_SRC_ID
 *
 *   @b Example
 *   @verbatim
 *      Uint32      txASrcId, txBSrcId;
 
        txASrcId    =   1;
        txBSrcId    =   2;

        CSL_CPSW_3GF_setCppiSourceIdReg (txASrcId, txBSrcId);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setCppiSourceIdReg (
    Uint32                  txASrcId,
    Uint32                  txBSrcId
)
{
    CSL_FINS (hCpsw3gfRegs->P0_CPPI_SRC_ID_REG, CPSW_3GF_P0_CPPI_SRC_ID_REG_TXA_SRC_ID, txASrcId);
    CSL_FINS (hCpsw3gfRegs->P0_CPPI_SRC_ID_REG, CPSW_3GF_P0_CPPI_SRC_ID_REG_TXB_SRC_ID, txBSrcId);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPort0VlanReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Port 0 VLAN Register.
 *
 *   @b Arguments
     @verbatim
        pPortVID                Port VLAN Id
        pPortCFI                Port CFI bit
        pPortPRI                Port VLAN priority (0-7, 7 is highest priority)
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
 *   @n CPSW_3GF_P0_PORT_VLAN_REG_PORT_VID,
 *      CPSW_3GF_P0_PORT_VLAN_REG_PORT_CFI,
 *      CPSW_3GF_P0_PORT_VLAN_REG_PORT_PRI
 *
 *   @b Example
 *   @verbatim
 *      Uint32      portVID, portCFI, portPRI;
 
        CSL_CPSW_3GF_getPort0VlanReg (&portVID, &portCFI, &portPRI);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPort0VlanReg (
    Uint32*                     pPortVID,
    Uint32*                     pPortCFI,
    Uint32*                     pPortPRI
)
{
    *pPortVID   =   CSL_FEXT (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_VID);
    *pPortCFI   =   CSL_FEXT (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_CFI);
    *pPortPRI   =   CSL_FEXT (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_PRI);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPort0VlanReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Port 0 VLAN Register.
 *
 *   @b Arguments
     @verbatim
        portVID                 Port VLAN Id to be configured
        portCFI                 Port CFI bit to be configured
        portPRI                 Port VLAN priority to be configured
                                (0-7, 7 is highest priority)
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
 *   @b Writes
 *   @n CPSW_3GF_P0_PORT_VLAN_REG_PORT_VID,
 *      CPSW_3GF_P0_PORT_VLAN_REG_PORT_CFI,
 *      CPSW_3GF_P0_PORT_VLAN_REG_PORT_PRI
 *
 *   @b Example
 *   @verbatim
 *      Uint32      portVID, portCFI, portPRI;
 
        portVID     =   1;
        portCFI     =   0;
        portPRI     =   7;

        CSL_CPSW_3GF_setPort0VlanReg (portVID, portCFI, portPRI);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPort0VlanReg (
    Uint32                      portVID,
    Uint32                      portCFI,
    Uint32                      portPRI
)
{
    CSL_FINS (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_VID, portVID);
    CSL_FINS (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_CFI, portCFI);
    CSL_FINS (hCpsw3gfRegs->P0_PORT_VLAN_REG, CPSW_3GF_P0_PORT_VLAN_REG_PORT_PRI, portPRI);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPort0RxPriMapReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Port 0 Receive Packet
 *      Priority to Header Priority Mapping Register.
 *
 *   @b Arguments
     @verbatim
        pPortRxPriMap           Array of Port 0 Rx priority map priority values 
                                read from the register.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  The input parameter 'pPortRxPriMap' must be large enough to hold all
 *       the 8 priority values read from the register.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_P0_RX_PRI_MAP_REG_PRI0,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI1,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI2,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI3,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI4,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI5,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI6,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI7
 *
 *   @b Example
 *   @verbatim
 *      Uint32      port0RxPriMap [8];
 
        CSL_CPSW_3GF_getPort0RxPriMapReg (port0RxPriMap);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPort0RxPriMapReg (
    Uint32*                     pPortRxPriMap                
)
{
    pPortRxPriMap [0]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI0);
    pPortRxPriMap [1]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI1);
    pPortRxPriMap [2]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI2);
    pPortRxPriMap [3]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI3);
    pPortRxPriMap [4]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI4);
    pPortRxPriMap [5]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI5);
    pPortRxPriMap [6]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI6);
    pPortRxPriMap [7]   =   CSL_FEXT (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI7);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPort0RxPriMapReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Port 0 Receive Packet
 *      Priority to Header Priority Mapping Register.
 *
 *   @b Arguments
     @verbatim
        pPortRxPriMap           Array of Port 0 Rx priority map priority values 
                                that must be configured to the register.
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
 *   @b Writes
 *   @n CPSW_3GF_P0_RX_PRI_MAP_REG_PRI0,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI1,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI2,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI3,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI4,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI5,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI6,
 *      CPSW_3GF_P0_RX_PRI_MAP_REG_PRI7
 *
 *   @b Example
 *   @verbatim
 *      Uint32      i, port0RxPriMap [8];

        for (i = 0; i < 8; i ++)
            port0RxPriMap [i] = i;

        CSL_CPSW_3GF_setPort0RxPriMapReg (port0RxPriMap);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPort0RxPriMapReg (
    Uint32*                     pPortRxPriMap                
)
{
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI0, pPortRxPriMap [0]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI1, pPortRxPriMap [1]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI2, pPortRxPriMap [2]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI3, pPortRxPriMap [3]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI4, pPortRxPriMap [4]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI5, pPortRxPriMap [5]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI6, pPortRxPriMap [6]);
    CSL_FINS (hCpsw3gfRegs->P0_RX_PRI_MAP_REG, CPSW_3GF_P0_RX_PRI_MAP_REG_PRI7, pPortRxPriMap [7]);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPort0RxMaxLen
 *
 *   @b Description
 *   @n This function retrieves the contents of the Port 0 Receive Maximum Length
 *      Register.
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
 *   @b Reads
 *   @n CPSW_3GF_RX_MAXLEN_REG_RX_MAXLEN
 *
 *   @b Example
 *   @verbatim
 *      Uint32      rxMaxLen;
 
        rxMaxLen    =   CSL_CPSW_3GF_getPort0RxMaxLen ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_getPort0RxMaxLen (void)
{
    return CSL_FEXT (hCpsw3gfRegs->RX_MAXLEN_REG, CPSW_3GF_RX_MAXLEN_REG_RX_MAXLEN);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPort0RxMaxLen
 *
 *   @b Description
 *   @n This function sets up the Port0 Receive Maximum length register.
 *
 *   @b Arguments
     @verbatim
        rxMaxLen            Maximum receive frame length to configure.
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
 *   @b Writes
 *   @n CPSW_3GF_RX_MAXLEN_REG_RX_MAXLEN
 *
 *   @b Example
 *   @verbatim
 *      Uint32      rxMaxLen;
 
        rxMaxLen    =   1518;            
 
        CSL_CPSW_3GF_setPort0RxMaxLen (rxMaxLen);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPort0RxMaxLen (
    Uint32                  rxMaxLen
)
{
    CSL_FINS (hCpsw3gfRegs->RX_MAXLEN_REG, CPSW_3GF_RX_MAXLEN_REG_RX_MAXLEN, rxMaxLen);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortMaxBlocksReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Port Max Blocks Register 
 *      corresponding to the MAC port specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the max block numbers
                                must be retrieved.
        pRxMaxBlks              Receive FIFO Maximum blocks read for this port.                                
        pTxMaxBlks              Transmit FIFO Maximum blocks read for this port.                                
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
 *   @n CPSW_3GF_P_MAX_BLKS_REG_RX_MAX_BLKS,
 *      CPSW_3GF_P_MAX_BLKS_REG_TX_MAX_BLKS
 *
 *   @b Example
 *   @verbatim
 *      Uint32      rxMaxBlks, txMaxBlks, portNum;

        portNum =   1;            
 
        CSL_CPSW_3GF_getPortMaxBlocksReg (portNum, &rxMaxBlks, &txMaxBlks);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortMaxBlocksReg (
    Uint32                      portNum,
    Uint32*                     pRxMaxBlks,
    Uint32*                     pTxMaxBlks
)
{
    *pRxMaxBlks  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_MAX_BLKS_REG, CPSW_3GF_P_MAX_BLKS_REG_RX_MAX_BLKS);
    *pTxMaxBlks  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_MAX_BLKS_REG, CPSW_3GF_P_MAX_BLKS_REG_TX_MAX_BLKS);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortMaxBlocksReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Port Max Blocks Register 
 *      corresponding to the MAC port specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the max block numbers
                                must be configured.
        rxMaxBlks               Maximum number of 4K memory blocks that must be allocated
                                for the port's FIFO logical receive priority queues.
        txMaxBlks               Maximum number of 4K memory blocks that must be allocated
                                for the port's FIFO logical transmit priority queues.
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
 *   @b Writes
 *   @n CPSW_3GF_P_MAX_BLKS_REG_RX_MAX_BLKS,
 *      CPSW_3GF_P_MAX_BLKS_REG_TX_MAX_BLKS
 *
 *   @b Example
 *   @verbatim
 *      Uint32      rxMaxBlks, txMaxBlks, portNum;

        portNum     =   1;            
        rxMaxBlks   =   3;
        txMaxBlks   =   17;
 
        CSL_CPSW_3GF_getPortMaxBlocksReg (portNum, rxMaxBlks, txMaxBlks);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortMaxBlocksReg (
    Uint32                      portNum,
    Uint32                      rxMaxBlks,
    Uint32                      txMaxBlks
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_MAX_BLKS_REG, CPSW_3GF_P_MAX_BLKS_REG_RX_MAX_BLKS, rxMaxBlks);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_MAX_BLKS_REG, CPSW_3GF_P_MAX_BLKS_REG_TX_MAX_BLKS, txMaxBlks);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortBlockCountReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Port Block Count register 
 *      corresponding to the MAC port specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the block count
                                must be retrieved.
        pRxBlkCnt               Receive block count usage read for this port.                                
        pTxBlkCnt               Transmit block count usage read for this port.                                
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
 *   @n CPSW_3GF_P_BLK_CNT_REG_RX_BLK_CNT,
 *      CPSW_3GF_P_BLK_CNT_REG_TX_BLK_CNT
 *
 *   @b Example
 *   @verbatim
 *      Uint32      rxBlkCnt, txBlkCnt, portNum;

        portNum =   1;            
 
        CSL_CPSW_3GF_getPortBlockCountReg (portNum, &rxBlkCnt, &txBlkCnt);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortBlockCountReg (
    Uint32                      portNum,
    Uint32*                     pRxBlkCnt,
    Uint32*                     pTxBlkCnt
)
{
    *pRxBlkCnt  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_BLK_CNT_REG, CPSW_3GF_P_BLK_CNT_REG_RX_BLK_CNT);
    *pTxBlkCnt  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_BLK_CNT_REG, CPSW_3GF_P_BLK_CNT_REG_TX_BLK_CNT);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortVlanReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the VLAN Register corresponding
 *      to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the VLAN register 
                                contents must be read
        pPortVID                Port VLAN Id
        pPortCFI                Port CFI bit
        pPortPRI                Port VLAN priority (0-7, 7 is highest priority)
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
 *   @n CPSW_3GF_P_PORT_VLAN_REG_PORT_VID,
 *      CPSW_3GF_P_PORT_VLAN_REG_PORT_CFI,
 *      CPSW_3GF_P_PORT_VLAN_REG_PORT_PRI
 *
 *   @b Example
 *   @verbatim
 *      Uint32      portVID, portCFI, portPRI, portNum;
 
        portNum =   2;
 
        CSL_CPSW_3GF_getPortVlanReg (portNum, &portVID, &portCFI, &portPRI);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortVlanReg (
    Uint32                      portNum,                
    Uint32*                     pPortVID,
    Uint32*                     pPortCFI,
    Uint32*                     pPortPRI
)
{
    *pPortVID   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_VID);
    *pPortCFI   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_CFI);
    *pPortPRI   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_PRI);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortVlanReg
 *
 *   @b Description
 *   @n This function sets up the contents of the VLAN Register corresponding to
 *      the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the VLAN register 
                                must be configured.
        portVID                 Port VLAN Id to be configured
        portCFI                 Port CFI bit to be configured
        portPRI                 Port VLAN priority to be configured
                                (0-7, 7 is highest priority)
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
 *   @b Writes
 *   @n CPSW_3GF_P_PORT_VLAN_REG_PORT_VID,
 *      CPSW_3GF_P_PORT_VLAN_REG_PORT_CFI,
 *      CPSW_3GF_P_PORT_VLAN_REG_PORT_PRI
 *
 *   @b Example
 *   @verbatim
 *      Uint32      portVID, portCFI, portPRI, portNum;
    
        portNum     =   2;
        portVID     =   1;
        portCFI     =   0;
        portPRI     =   7;

        CSL_CPSW_3GF_setPortVlanReg (portNum, portVID, portCFI, portPRI);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortVlanReg (
    Uint32                      portNum,                
    Uint32                      portVID,
    Uint32                      portCFI,
    Uint32                      portPRI
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_VID, portVID);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_CFI, portCFI);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_PORT_VLAN_REG, CPSW_3GF_P_PORT_VLAN_REG_PORT_PRI, portPRI);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortTxPriMapReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the Transmit Header Priority to
 *      Switch Priority Mapping Register corresponding to the MAC port number
 *      specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register contents
                                must be read and returned.
        pPortTxPriMap           Array of Port Tx priority map priority values 
                                read from the register.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  The input parameter 'pPortTxPriMap' must be large enough to hold all
 *       the 8 priority values read from the register.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_P_TX_PRI_MAP_REG_PRI0,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI1,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI2,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI3,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI4,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI5,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI6,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI7
 *
 *   @b Example
 *   @verbatim
 *      Uint32      portTxPriMap [8], portNum;
 
        portNum =   1;
 
        CSL_CPSW_3GF_getPortTxPriMapReg (portNum, portTxPriMap);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortTxPriMapReg (
    Uint32                      portNum,                
    Uint32*                     pPortTxPriMap                
)
{
    pPortTxPriMap [0]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI0);
    pPortTxPriMap [1]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI1);
    pPortTxPriMap [2]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI2);
    pPortTxPriMap [3]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI3);
    pPortTxPriMap [4]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI4);
    pPortTxPriMap [5]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI5);
    pPortTxPriMap [6]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI6);
    pPortTxPriMap [7]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI7);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortTxPriMapReg
 *
 *   @b Description
 *   @n This function sets up the contents of the Port Transmit Header Priority
 *      to Switch Priority Mapping Register.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register contents
                                must be configured.
        pPortTxPriMap           Array of Port Tx priority map priority values 
                                that must be configured to the register.
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
 *   @b Writes
 *   @n CPSW_3GF_P_TX_PRI_MAP_REG_PRI0,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI1,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI2,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI3,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI4,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI5,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI6,
 *      CPSW_3GF_P_TX_PRI_MAP_REG_PRI7
 *
 *   @b Example
 *   @verbatim
 *      Uint32      i, portTxPriMap [8], portNum;

        for (i = 0; i < 8; i ++)
            portTxPriMap [i] = i;

        portNum =   1;            

        CSL_CPSW_3GF_setPortTxPriMapReg (portNum, portTxPriMap);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortTxPriMapReg (
    Uint32                      portNum,                
    Uint32*                     pPortTxPriMap                
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI0, pPortTxPriMap [0]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI1, pPortTxPriMap [1]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI2, pPortTxPriMap [2]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI3, pPortTxPriMap [3]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI4, pPortTxPriMap [4]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI5, pPortTxPriMap [5]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI6, pPortTxPriMap [6]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TX_PRI_MAP_REG, CPSW_3GF_P_TX_PRI_MAP_REG_PRI7, pPortTxPriMap [7]);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortMACAddress
 *
 *   @b Description
 *   @n This function retreives the source MAC address corresponding to the 
 *      MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the source MAC address
                                must be read and returned.
        pMacAddress             6 byte Source MAC address read.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  The input parameter 'pMacAddres' must be large enough the 6 byte
 *       MAC address returned by this API.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_7_0,
 *      CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_15_8,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_23_16,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_31_24,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_39_32,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_47_40
 *
 *   @b Example
 *   @verbatim
 *      Uint8   macAddress [6], portNum;
 
        portNum =   1;
 
        CSL_CPSW_3GF_getPortMACAddress (portNum, macAddress);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortMACAddress (
    Uint32                      portNum,                
    Uint8*                      pMacAddress                
)
{
    pMacAddress [0]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_LO_REG, CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_7_0);
    pMacAddress [1]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_LO_REG, CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_15_8);
    pMacAddress [2]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_23_16);
    pMacAddress [3]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_31_24);
    pMacAddress [4]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_39_32);
    pMacAddress [5]   =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_47_40);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortMACAddress
 *
 *   @b Description
 *   @n This function sets up the source MAC address corresponding to the 
 *      MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the source MAC address
                                must be setup.
        pMacAddress             6 byte Source MAC address to configure.
 *	 @endverbatim
 *
 *   <b> Return Value </b>
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  The input parameter 'pMacAddres' is expected to be 6 bytes long.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_7_0,
 *      CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_15_8,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_23_16,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_31_24,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_39_32,
 *      CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_47_40
 *
 *   @b Example
 *   @verbatim
 *      Uint8   macAddress [6], portNum;
 
        portNum         =   1;
        macAddress [0]  =   0x01;
        macAddress [1]  =   0x02;
        macAddress [2]  =   0x03;
        macAddress [3]  =   0x04;
        macAddress [4]  =   0x05;
        macAddress [5]  =   0x06;
 
        CSL_CPSW_3GF_setPortMACAddress (portNum, macAddress);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortMACAddress (
    Uint32                      portNum,                
    Uint8*                      pMacAddress                
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_LO_REG, CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_7_0, pMacAddress [0]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_LO_REG, CPSW_3GF_SL_SA_LO_REG_MACSRCADDR_15_8, pMacAddress [1]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_23_16, pMacAddress [2]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_31_24, pMacAddress [3]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_39_32, pMacAddress [4]);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].SL_SA_HI_REG, CPSW_3GF_SL_SA_HI_REG_MACSRCADDR_47_40, pMacAddress [5]);

    return;
}


/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortTimeSyncCntlReg
 *
 *   @b Description
 *   @n This function retreives the contents of Time sync control register  
 *      corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be read.
        pTimeSyncCntlCfg        CSL_CPSW_3GF_TSCNTL that needs to be populated with 
                                contents of time sync control register.
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
 *   @n CPSW_3GF_P_TS_CTL_REG_TS_RX_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE1_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE2_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE1_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE2_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TX_MSG_TYPE_EN_15_0
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum;
        CSL_CPSW_3GF_TSCNTL     tsCtlCfg;
 
        portNum =   1;
 
        CSL_CPSW_3GF_getPortTimeSyncCntlReg (portNum, &tsCtlCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortTimeSyncCntlReg (
    Uint32                      portNum,                
    CSL_CPSW_3GF_TSCNTL*        pTimeSyncCntlCfg                
)
{
    pTimeSyncCntlCfg->tsRxEnable                =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                              CPSW_3GF_P_TS_CTL_REG_TS_RX_EN);
    pTimeSyncCntlCfg->tsRxVlanLType1Enable      =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                              CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE1_EN);
    pTimeSyncCntlCfg->tsRxVlanLType2Enable      =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                               CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE2_EN);
    pTimeSyncCntlCfg->tsTxEnable                =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                               CPSW_3GF_P_TS_CTL_REG_TS_TX_EN);
    pTimeSyncCntlCfg->tsTxVlanLType1Enable      =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                               CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE1_EN);
    pTimeSyncCntlCfg->tsTxVlanLType2Enable      =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                               CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE2_EN);
    pTimeSyncCntlCfg->txMsgTypeEnable           =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, 
                                                               CPSW_3GF_P_TS_CTL_REG_TX_MSG_TYPE_EN_15_0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortTimeSyncCntlReg
 *
 *   @b Description
 *   @n This function sets up the contents of Time sync control register  
 *      corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be 
                                configured.
        pTimeSyncCntlCfg        CSL_CPSW_3GF_TSCNTL containing settings for time
                                sync control register.
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
 *   @b Writes
 *   @n CPSW_3GF_P_TS_CTL_REG_TS_RX_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE1_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE2_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE1_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE2_EN,
 *      CPSW_3GF_P_TS_CTL_REG_TX_MSG_TYPE_EN_15_0
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum;
        CSL_CPSW_3GF_TSCNTL     tsCtlCfg;
 
        portNum =   1;

        tsCtlCfg.tsRxEnable             =   1;
        tsCtlCfg.tsRxVlanLType1Enable   =   0;
        tsCtlCfg.tsRxVlanLType2Enable   =   0;
        ...
 
        CSL_CPSW_3GF_setPortTimeSyncCntlReg (portNum, &tsCtlCfg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortTimeSyncCntlReg (
    Uint32                      portNum,                
    CSL_CPSW_3GF_TSCNTL*        pTimeSyncCntlCfg                
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_RX_EN, 
              pTimeSyncCntlCfg->tsRxEnable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE1_EN,
              pTimeSyncCntlCfg->tsRxVlanLType1Enable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_RX_VLAN_LTYPE2_EN,
              pTimeSyncCntlCfg->tsRxVlanLType2Enable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_TX_EN,
              pTimeSyncCntlCfg->tsTxEnable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE1_EN,
              pTimeSyncCntlCfg->tsTxVlanLType1Enable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TS_TX_VLAN_LTYPE2_EN,
              pTimeSyncCntlCfg->tsTxVlanLType2Enable);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_CTL_REG, CPSW_3GF_P_TS_CTL_REG_TX_MSG_TYPE_EN_15_0,
              pTimeSyncCntlCfg->txMsgTypeEnable);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortTimeSyncSeqIdReg
 *
 *   @b Description
 *   @n This function retreives the contents of Time Sync Sequence Id and   
 *      LTYPE register corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be read.
        pTsLtype                Time sync LTYPE read.
        pTsSeqIdOffset          Time sync sequence Id offset read.
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
 *   @n CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_LTYPE,
 *      CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_SEQ_ID_OFFSET
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum, tsLtype, tsSeqIdOffset;
 
        portNum =   1;
 
        CSL_CPSW_3GF_getPortTimeSyncSeqIdReg (portNum, &tsLtype, &tsSeqIdOffset);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortTimeSyncSeqIdReg (
    Uint32                      portNum,                
    Uint32*                     pTsLtype,                
    Uint32*                     pTsSeqIdOffset
)
{
    *pTsLtype           =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_SEQ_LTYPE_REG, 
                                      CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_LTYPE);
    *pTsSeqIdOffset     =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_SEQ_LTYPE_REG, 
                                      CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_SEQ_ID_OFFSET);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortTimeSyncSeqIdReg
 *
 *   @b Description
 *   @n This function sets up the contents of Time Sync Sequence Id and   
 *      LTYPE register corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be 
                                configured.
        tsLtype                 Time sync LTYPE to be configured.
        tsSeqIdOffset           Time sync sequence Id offset to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_LTYPE,
 *      CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_SEQ_ID_OFFSET
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum, tsLtype, tsSeqIdOffset;
 
        portNum         =   1;
        tsLtype         =   0;
        tsSeqIdOffset   =   30;
 
        CSL_CPSW_3GF_getPortTimeSyncSeqIdReg (portNum, tsLtype, tsSeqIdOffset);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortTimeSyncSeqIdReg (
    Uint32                      portNum,                
    Uint32                      tsLtype,                
    Uint32                      tsSeqIdOffset
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_SEQ_LTYPE_REG, 
              CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_LTYPE, tsLtype);

    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_SEQ_LTYPE_REG, 
              CPSW_3GF_P_TS_SEQ_LTYPE_REG_TS_SEQ_ID_OFFSET, tsSeqIdOffset);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getPortTimeSyncVlanLTypeReg
 *
 *   @b Description
 *   @n This function retreives the contents of Time Sync VLAN LTYPE   
 *      register corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be read.
        pTsVlanLtype1           Time sync VLAN LTYPE1 value read.
        pTsVlanLtype2           Time sync VLAN LTYPE2 value read.
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
 *   @n CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE1,
 *      CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE2
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum, tsLtype1, tsLtype2;
 
        portNum =   1;
 
        CSL_CPSW_3GF_getPortTimeSyncVlanLTypeReg (portNum, &tsLtype1, &tsLtype2);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getPortTimeSyncVlanLTypeReg (
    Uint32                      portNum,                
    Uint32*                     pTsVlanLtype1,                
    Uint32*                     pTsVlanLtype2
)
{
    *pTsVlanLtype1  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_VLAN_LTYPE_REG, 
                                  CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE1);
    *pTsVlanLtype2  =   CSL_FEXT (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_VLAN_LTYPE_REG, 
                                  CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE2);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setPortTimeSyncVlanLTypeReg
 *
 *   @b Description
 *   @n This function sets up the contents of Time Sync VLAN LTYPE   
 *      register corresponding to the MAC port number specified.
 *
 *   @b Arguments
     @verbatim
        portNum                 MAC port number for which the register must be read.
        tsVlanLtype1            Time sync VLAN LTYPE1 value to be configured.
        tsVlanLtype2            Time sync VLAN LTYPE2 value to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE1,
 *      CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE2
 *
 *   @b Example
 *   @verbatim
 *      Uint32              portNum, tsLtype1, tsLtype2;
 
        portNum     =   1;
        tsLtype1    =   0x8100;
        tsLtype2    =   0x8100;
 
        CSL_CPSW_3GF_setPortTimeSyncVlanLTypeReg (portNum, &tsLtype1, &tsLtype2);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setPortTimeSyncVlanLTypeReg (
    Uint32                      portNum,                
    Uint32                      tsVlanLtype1,                
    Uint32                      tsVlanLtype2
)
{
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_VLAN_LTYPE_REG, 
              CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE1, tsVlanLtype1);
    CSL_FINS (hCpsw3gfRegs->PORT_INFO_GROUP[portNum].P_TS_VLAN_LTYPE_REG, 
              CPSW_3GF_P_TS_VLAN_LTYPE_REG_TS_VLAN_LTYPE2, tsVlanLtype2);

    return;
}


/********************************************************************************
*************************  Statistics (STATS) Submodule *************************
********************************************************************************/

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getStats
 *
 *   @b Description
 *   @n The CPSW stats are divided into 2 blocks, i.e., Stats for Host port (switch Port 0) 
 *      and Stats for MAC ports (Port 1 and Port2). This function retreives hardware 
 *      statistics for both the stat blocks.
 *
 *   @b Arguments
     @verbatim
        pCpswStats              Array of CSL_CPSW_3GF_STATS structure that needs to be filled
                                with the stats read from the hardware. This function expects
                                that the array passed to it is big enough to hold the stats
                                for both stat blocks, i.e., size of array passed to this 
                                function must be 2.
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
 *   @n CPSW_RXGOODFRAMES,
 *      CPSW_RXBROADCASTFRAMES,
 *      CPSW_RXMULTICASTFRAMES,
 *      CPSW_RXPAUSEFRAMES,
 *      CPSW_RXCRCERRORS,
 *      CPSW_RXALIGNCODEERRORS,
 *      CPSW_RXOVERSIZEDFRAMES,
 *      CPSW_RXJABBERFRAMES,
 *      CPSW_RXUNDERSIZEDFRAMES,
 *      CPSW_RXFRAGMENTS,
 *      CPSW_RXOCTETS,
 *      CPSW_TXGOODFRAMES,
 *      CPSW_TXBROADCASTFRAMES,
 *      CPSW_TXMULTICASTFRAMES,
 *      CPSW_TXPAUSEFRAMES,
 *      CPSW_TXDEFERREDFRAMES,
 *      CPSW_TXCOLLISIONFRAMES,
 *      CPSW_TXSINGLECOLLFRAMES,
 *      CPSW_TXMULTCOLLFRAMES,
 *      CPSW_TXEXCESSIVECOLLISIONS,
 *      CPSW_TXLATECOLLISIONS,
 *      CPSW_TXUNDERRUN,
 *      CPSW_TXCARRIERSENSEERRORS,
 *      CPSW_TXOCTETS,
 *      CPSW_OCTETFRAMES64,
 *      CPSW_OCTETFRAMES65T127,
 *      CPSW_OCTETFRAMES128T255,
 *      CPSW_OCTETFRAMES256T511,
 *      CPSW_OCTETFRAMES512T1023,
 *      CPSW_OCTETFRAMES1024TUP,
 *      CPSW_NETOCTETS,
 *      CPSW_RXSOFOVERRUNS,
 *      CPSW_RXMOFOVERRUNS,
 *      CPSW_RXDMAOVERRUNS
 *
 *   @b Affects
 *   @n CPSW_RXGOODFRAMES=0,
 *      CPSW_RXBROADCASTFRAMES=0,
 *      CPSW_RXMULTICASTFRAMES=0,
 *      CPSW_RXPAUSEFRAMES=0,
 *      CPSW_RXCRCERRORS=0,
 *      CPSW_RXALIGNCODEERRORS=0,
 *      CPSW_RXOVERSIZEDFRAMES=0,
 *      CPSW_RXJABBERFRAMES=0,
 *      CPSW_RXUNDERSIZEDFRAMES=0,
 *      CPSW_RXFRAGMENTS=0,
 *      CPSW_RXOCTETS=0,
 *      CPSW_TXGOODFRAMES=0,
 *      CPSW_TXBROADCASTFRAMES=0,
 *      CPSW_TXMULTICASTFRAMES=0,
 *      CPSW_TXPAUSEFRAMES=0,
 *      CPSW_TXDEFERREDFRAMES=0,
 *      CPSW_TXCOLLISIONFRAMES=0,
 *      CPSW_TXSINGLECOLLFRAMES=0,
 *      CPSW_TXMULTCOLLFRAMES=0,
 *      CPSW_TXEXCESSIVECOLLISIONS=0,
 *      CPSW_TXLATECOLLISIONS=0,
 *      CPSW_TXUNDERRUN=0,
 *      CPSW_TXCARRIERSENSEERRORS=0,
 *      CPSW_TXOCTETS=0,
 *      CPSW_OCTETFRAMES64=0,
 *      CPSW_OCTETFRAMES65T127=0,
 *      CPSW_OCTETFRAMES128T255=0,
 *      CPSW_OCTETFRAMES256T511=0,
 *      CPSW_OCTETFRAMES512T1023=0,
 *      CPSW_OCTETFRAMES1024TUP=0,
 *      CPSW_NETOCTETS=0,
 *      CPSW_RXSOFOVERRUNS=0,
 *      CPSW_RXMOFOVERRUNS=0,
 *      CPSW_RXDMAOVERRUNS=0
 *
 *   @b Example
 *   @verbatim
 *      CSL_CPSW_3GF_STATS     stats [2];
 
        CSL_CPSW_3GF_getStats (stats);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getStats (
    CSL_CPSW_3GF_STATS*         pCpswStats                
)
{
    Uint32                      numStats, numBlocks;
    volatile Uint32             *pRegAddr;
    Uint32                      *pStatAddr;
    Uint32                      statval;

    pStatAddr   =   (Uint32 *)(pCpswStats);

    /* Read the entire stats block for both the
     * Host and the MAC ports and reset the stats
     * block when done.
     */
    for (numBlocks = 0; numBlocks < CSL_CPSW_3GF_NUMSTATBLOCKS; numBlocks++)
    {
    	pRegAddr    =	&hCpsw3gfRegs->PORT_STATS_GROUP [numBlocks].RXGOODFRAMES;    	
        for (numStats = 0; numStats < CSL_CPSW_3GF_NUMSTATS; numStats++)
        {
            statval         =   *pRegAddr;
            *pRegAddr++     =   statval;
            statval         +=  *pStatAddr;
            *pStatAddr++    =   statval;
        }
    }

    return;
}


/********************************************************************************
********************  Address Lookup Engine (ALE) Submodule *********************
********************************************************************************/

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleVersionInfo
 *
 *   @b Description
 *   @n This function retrieves the ALE submodule identification and version 
 *      information.
 *
 *   @b Arguments
     @verbatim
        pVersionInfo        CSL_CPSW_3GF_ALE_VERSION structure that needs to be populated
                            with the ALE version info read from the hardware.
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
 *   @n CPSW_3GF_ALE_ID_REG_ALE_MINOR_VER,
 *      CPSW_3GF_ALE_ID_REG_ALE_MAJOR_VER,
 *      CPSW_3GF_ALE_ID_REG_ALE_IDENT
 *
 *   @b Example
 *   @verbatim
        CSL_CPSW_3GF_ALE_VERSION    versionInfo;

        CSL_CPSW_3GF_getAleVersionInfo (&versionInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleVersionInfo (
	CSL_CPSW_3GF_ALE_VERSION*       pVersionInfo     
)
{

    pVersionInfo->minorVer  =   CSL_FEXT (hCpsw3gfRegs->ALE_ID_REG, CPSW_3GF_ALE_ID_REG_ALE_MINOR_VER);
    pVersionInfo->majorVer  =   CSL_FEXT (hCpsw3gfRegs->ALE_ID_REG, CPSW_3GF_ALE_ID_REG_ALE_MAJOR_VER);
    pVersionInfo->id        =   CSL_FEXT (hCpsw3gfRegs->ALE_ID_REG, CPSW_3GF_ALE_ID_REG_ALE_IDENT);

    return;        
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleRateLimitEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE Broadcast and Multicast Rate Limit is
 *      enabled.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE Broadcast and multicast rate limit enabled.
 *	                            Broadcast/multicast packet reception limited to
 *	                            port control register rate limit fields.
 *	 @n  FALSE                  ALE Broadcast and multicast rate limit disabled. 
 *	                            Broadcast/multicast rates not limited.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleRateLimitEnabled () == TRUE)
        {
            // ALE Broadcast/Multicast rate limit enabled
        }
        else
        {
            // ALE Broadcast/Multicast rate limit disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleRateLimitEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleRateLimit
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable multicast,
 *      broadcast rate limiting.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleRateLimit ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleRateLimit (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleRateLimit
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable multicast,
 *      broadcast rate limiting.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleRateLimit ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleRateLimit (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_RATE_LIMIT, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleMacAuthModeEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE MAC Authorization mode is enabled.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE  is in MAC authorization mode.
 *	 @n  FALSE                  ALE not in MAC authorization mode. 
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleMacAuthModeEnabled () == TRUE)
        {
            // ALE  is in MAC authorization mode
        }
        else
        {
            // ALE not in MAC authorization mode
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleMacAuthModeEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleMacAuthMode
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable MAC authorization
 *      mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleMacAuthMode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleMacAuthMode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleMacAuthMode
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable MAC authorization
 *      mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleMacAuthMode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleMacAuthMode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_AUTH_MODE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleVlanAwareEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to be VLAN aware.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE VLAN aware. ALE drops packets if VLAN not found.
 *	 @n  FALSE                  ALE not VLAN aware. Floods if VLAN not found.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleVlanAwareEnabled () == TRUE)
        {
            // ALE VLAN aware
        }
        else
        {
            // ALE not VLAN aware
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleVlanAwareEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleVlanAware
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable VLAN aware
 *      mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleVlanAware ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleVlanAware (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleVlanAware
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable VLAN aware
 *      mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleVlanAware ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleVlanAware (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_VLAN_AWARE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleTxRateLimitEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to be Tx rate limited.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE Tx rate limit enabled. Broadcast, multicast
 *	                            rate limit counters are transmit port based.
 *	 @n  FALSE                  ALE Tx rate limit disabled. Broadcast, multicast
 *	                            rate limit counters are receive port based.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleTxRateLimitEnabled () == TRUE)
        {
            // ALE Tx rate limit on
        }
        else
        {
            // ALE Tx rate limit off
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleTxRateLimitEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleTxRateLimit
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable Tx rate limiting.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleTxRateLimit ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleTxRateLimit (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleTxRateLimit
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable Tx rate limiting.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleTxRateLimit ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleTxRateLimit (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_RATE_LIMIT_TX, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleBypassEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to be in Bypass mode.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE Bypass mode enabled.
 *	 @n  FALSE                  ALE Bypass mode disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleBypassEnabled () == TRUE)
        {
            // ALE Bypass mode on
        }
        else
        {
            // ALE Bypass mode off
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleBypassEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleBypass
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable Bypass mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleBypass ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleBypass (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleBypass
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable Bypass mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleBypass ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleBypass (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ALE_BYPASS, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleOUIDenyModeEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to be in OUI deny mode.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE OUI deny mode enabled.
 *	 @n  FALSE                  ALE OUI deny mode disabled. 
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleOUIDenyModeEnabled () == TRUE)
        {
            // ALE OUI deny mode on
        }
        else
        {
            // ALE OUI deny mode off
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleOUIDenyModeEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleOUIDenyMode
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable OUI deny mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleOUIDenyMode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleOUIDenyMode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleOUIDenyMode
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable OUI deny mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleOUIDenyMode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleOUIDenyMode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_OUI_DENY, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleVID0ModeEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to be in VID0 (VLAN ID=0) mode.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE VID0 mode enabled. 
 *	                            Process the packet with VLAN Id = 0
 *	 @n  FALSE                  ALE VID0 mode disabled. Process the packet with 
 *	                            VLAN Id =PORT_VLAN[11-0]
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleVID0ModeEnabled () == TRUE)
        {
            // ALE VID0 mode on
        }
        else
        {
            // ALE VID0 mode off
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleVID0ModeEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleVID0Mode
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable VID0 mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleVID0Mode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleVID0Mode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleVID0Mode
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable VID0 mode.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleVID0Mode ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleVID0Mode (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_EN_VID0_MODE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleLearnNoVIDEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE is programmed to not learn VLAN Ids.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE Learn no VID enabled. 
 *	                            VLAN Id is not learned with source address (source 
 *	                            address is not tied to VID)
 *	 @n  FALSE                  ALE VID learning mode enabled. 
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleLearnNoVIDEnabled () == TRUE)
        {
            // ALE VID learning disabled
        }
        else
        {
            // ALE VID learning enabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleLearnNoVIDEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAleLearnNoVID
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable VLAN Id No
 *      Learn, i.e., disable VLAN Id learning.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAleLearnNoVID ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAleLearnNoVID (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAleLearnNoVID
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable VLAN Id learning.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAleLearnNoVID ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAleLearnNoVID (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_LEARN_NO_VID, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_startAleAgeOutNow
 *
 *   @b Description
 *   @n This function configures the ALE control register to initiate an ALE
 *      ageable entry cleanup. This enables the ALE hardware to remove any
 *      ageable table entry that does not have a set touch bit.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_AGE_OUT_NOW=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_startAleAgeOutNow ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_startAleAgeOutNow (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_AGE_OUT_NOW, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleAgeOutDone
 *
 *   @b Description
 *   @n This function reads the ALE control register's AGE_OUT_NOW bit to check
 *      if the ALE ageable entry cleanup process is done.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE age out process done. 
 *	 @n  FALSE                  ALE age out process not yet completed.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_AGE_OUT_NOW
 *
 *   @b Example
 *   @verbatim
 
        if (CSL_CPSW_3GF_isAleAgeOutDone ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleAgeOutDone (void)
{
    if ((CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_AGE_OUT_NOW) == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;            
    }
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_clearAleTable
 *
 *   @b Description
 *   @n This function initiates a full ALE table cleanup. The ALE hardware 
 *      clears all table entries. 
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_CLEAR_TABLE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_clearAleTable ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_clearAleTable (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_CLEAR_TABLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_isAleEnabled
 *
 *   @b Description
 *   @n This function indicates if ALE processing is enabled.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 TRUE                   ALE enabled. ALE packet processing will be done.
 *	 @n  FALSE                  ALE disabled. All packets are dropped by ALE.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE
 *
 *   @b Example
 *   @verbatim
        if (CSL_CPSW_3GF_isAleEnabled () == TRUE)
        {
            // ALE enabled
        }
        else
        {
            // ALE disabled
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_isAleEnabled (void)
{

    return CSL_FEXT (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_enableAle
 *
 *   @b Description
 *   @n This function configures the ALE control register to enable ALE processing.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE=1
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_enableAle ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_enableAle (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE, (Uint32) 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_disableAle
 *
 *   @b Description
 *   @n This function configures the ALE control register to disable ALE processing.
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
 *   @n CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE=0
 *
 *   @b Example
 *   @verbatim
 
        CSL_CPSW_3GF_disableAle ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_disableAle (void)
{
    CSL_FINS (hCpsw3gfRegs->ALE_CONTROL_REG, CPSW_3GF_ALE_CONTROL_REG_ENABLE_ALE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleControlReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the ALE control register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 >=0                ALE control register contents.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_CONTROL_REG
 *
 *   @b Example
 *   @verbatim
        Uint32      aleCtrlVal;
 
        aleCtrlVal  =   CSL_CPSW_3GF_getAleControlReg ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_getAleControlReg (void)
{
    return hCpsw3gfRegs->ALE_CONTROL_REG;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleControlReg
 *
 *   @b Description
 *   @n This function sets up the contents of the ALE control register.
 *
 *   @b Arguments
     @verbatim
        aleCtrlVal          Value to be configured to the ALE control register.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_CONTROL_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32          aleCtrlVal = 0;

        aleCtrlVal      =   CSL_CPSW_3GF_getAleControlReg ();
        aleCtrlVal      |=  CSL_CPSW_3GF_ALECONTROL_CLRTABLE_EN;

        CSL_CPSW_3GF_setAleControlReg (&aleCtrlRegInfo);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleControlReg 
(
    Uint32                      aleCtrlVal
)
{
    hCpsw3gfRegs->ALE_CONTROL_REG  =   aleCtrlVal;

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAlePrescaleReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the ALE Prescale register.
 *
 *   @b Arguments
 *   @n None
 *
 *   <b> Return Value </b>
 *	 @n	 >=0                ALE prescale register contents.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_PRESCALE_REG_ALE_PRESCALE
 *
 *   @b Example
 *   @verbatim
        Uint32      alePrescaleVal;
 
        alePrescaleVal  =   CSL_CPSW_3GF_getAlePrescaleReg ();
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_CPSW_3GF_getAlePrescaleReg (void)
{
    return CSL_FEXT(hCpsw3gfRegs->ALE_PRESCALE_REG, CPSW_3GF_ALE_PRESCALE_REG_ALE_PRESCALE);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAlePrescaleReg
 *
 *   @b Description
 *   @n This function sets up the contents of the ALE prescale register.
 *
 *   @b Arguments
     @verbatim
        alePrescaleVal      Value to be configured to the ALE Prescale register.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_PRESCALE_REG_ALE_PRESCALE
 *
 *   @b Example
 *   @verbatim
 *      Uint32          alePrescaleVal = 0;

        alePrescaleVal  =   10;

        CSL_CPSW_3GF_setAlePrescaleReg (&aleCtrlRegInfo);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAlePrescaleReg 
(
    Uint32                      alePrescaleVal
)
{
    CSL_FINS (hCpsw3gfRegs->ALE_PRESCALE_REG, CPSW_3GF_ALE_PRESCALE_REG_ALE_PRESCALE, alePrescaleVal);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleUnkownVlanReg
 *
 *   @b Description
 *   @n This function retrieves the contents of the ALE Unknown VLAN register.
 *
 *   @b Arguments
     @verbatim
        pUnVlanMemList          Unknown VLAN member list.
        pUnMcastFloodMask       Unknown VLAN Multicast flood mask.
        pUnRegMcastFloodMask    Unknown VLAN Registered Multicast Flood mask.
        pUnForceUntagEgress     Unknown VLAN Force Untagged Egress.
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
 *   @n CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_VLAN_MEMBER_LIST,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_MCAST_FLOOD_MASK,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_REG_MCAST_FLOOD_MASK,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_FORCE_UNTAGGED_EGRESS
 *
 *   @b Example
 *   @verbatim
        Uint32      unVlanMemList, unMcastFloodMask, unRegMcastFloodMask, unForceUntagEgress;
 
        CSL_CPSW_3GF_getAleUnkownVlanReg (&unVlanMemList, 
                                          &unMcastFloodMask,
                                          &unRegMcastFloodMask,
                                          &unForceUntagEgress);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleUnkownVlanReg
(
    Uint32*                        pUnVlanMemList,
    Uint32*                        pUnMcastFloodMask,
    Uint32*                        pUnRegMcastFloodMask,
    Uint32*                        pUnForceUntagEgress
)
{
    *pUnVlanMemList         =   CSL_FEXT(hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_VLAN_MEMBER_LIST);
    *pUnMcastFloodMask      =   CSL_FEXT(hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_MCAST_FLOOD_MASK);
    *pUnRegMcastFloodMask   =   CSL_FEXT(hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_REG_MCAST_FLOOD_MASK);
    *pUnForceUntagEgress    =   CSL_FEXT(hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_FORCE_UNTAGGED_EGRESS);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleUnkownVlanReg
 *
 *   @b Description
 *   @n This function sets up the contents of the ALE Unknown VLAN register.
 *
 *   @b Arguments
     @verbatim
        unVlanMemList           Unknown VLAN member list.
        unMcastFloodMask        Unknown VLAN Multicast flood mask.
        unRegMcastFloodMask     Unknown VLAN Registered Multicast Flood mask.
        unForceUntagEgress      Unknown VLAN Force Untagged Egress.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_VLAN_MEMBER_LIST,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_MCAST_FLOOD_MASK,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_REG_MCAST_FLOOD_MASK,
 *      CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_FORCE_UNTAGGED_EGRESS
 *
 *   @b Example
 *   @verbatim
        Uint32      unVlanMemList, unMcastFloodMask, unRegMcastFloodMask, unForceUntagEgress;

        unVlanMemList           =   0;
        unMcastFloodMask        =   3;
        unRegMcastFloodMask     =   0;
        unForceUntagEgress      =   0;
 
        CSL_CPSW_3GF_setAleUnkownVlanReg (unVlanMemList, 
                                          unMcastFloodMask,
                                          unRegMcastFloodMask,
                                          unForceUntagEgress);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleUnkownVlanReg
(
    Uint32                      unVlanMemList,
    Uint32                      unMcastFloodMask,
    Uint32                      unRegMcastFloodMask,
    Uint32                      unForceUntagEgress
)
{
    CSL_FINS (hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_VLAN_MEMBER_LIST, unVlanMemList);
    CSL_FINS (hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_MCAST_FLOOD_MASK, unMcastFloodMask);
    CSL_FINS (hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_REG_MCAST_FLOOD_MASK, unRegMcastFloodMask);
    CSL_FINS (hCpsw3gfRegs->ALE_UNKNOWN_VLAN_REG, CPSW_3GF_ALE_UNKNOWN_VLAN_REG_UNKNOWN_FORCE_UNTAGGED_EGRESS, unForceUntagEgress);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleTableEntry
 *
 *   @b Description
 *   @n This function retrieves an ALE table entry corresponding to the 
 *      ALE entry index specified in 'index' input parameter. The ALE
 *      entry values corresponding to the ALE_TBLW0, ALE_TBLW1 and
 *      ALE_TBLW2 registers are returned in 'pAleInfoWd0', 'pAleInfoWd1', 'pAleInfoWd2'
 *      output parameters.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pAleInfoWd0             Contents of ALE Table Word 0 Register (ALE_TBLW0).
        pAleInfoWd1             Contents of ALE Table Word 1 Register (ALE_TBLW1).
        pAleInfoWd2             Contents of ALE Table Word 2 Register (ALE_TBLW2).
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0_REG_ENTRY_31_0,
 *      CPSW_3GF_ALE_TABLE_WORD1_REG_ENTRY_63_32,
 *      CPSW_3GF_ALE_TABLE_WORD2_REG_ENTRY_71_64
 *
 *   @b Example
 *   @verbatim
        Uint32      index, info0, info1, info2;

        index   =   0;
 
        CSL_CPSW_3GF_getAleUnkownVlanReg (index, 
                                          &info0,
                                          &info1,
                                          &info2);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleTableEntry
(
    Uint32                      index,
    Uint32*                     pAleInfoWd0,
    Uint32*                     pAleInfoWd1,
    Uint32*                     pAleInfoWd2
)
{
    Uint32                      aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;

    /* Read the ALE table word registers */
    *pAleInfoWd0            =   CSL_FEXT(hCpsw3gfRegs->ALE_TABLE_WORD0_REG, CPSW_3GF_ALE_TABLE_WORD0_REG_ENTRY_31_0);
    *pAleInfoWd1            =   CSL_FEXT(hCpsw3gfRegs->ALE_TABLE_WORD1_REG, CPSW_3GF_ALE_TABLE_WORD1_REG_ENTRY_63_32);
    *pAleInfoWd2            =   CSL_FEXT(hCpsw3gfRegs->ALE_TABLE_WORD2_REG, CPSW_3GF_ALE_TABLE_WORD2_REG_ENTRY_71_64);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleTableEntry
 *
 *   @b Description
 *   @n This function sets up an ALE table entry corresponding to the 
 *      ALE entry index specified in 'index' input parameter. The ALE
 *      entry values corresponding to the ALE_TBLW0, ALE_TBLW1 and
 *      ALE_TBLW2 registers msut be specified in 'aleInfoWd0', 'aleInfoWd1', 'aleInfoWd2'
 *      input parameters.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be written.
        aleInfoWd0              Value to write to ALE Table Word 0 Register (ALE_TBLW0).
        aleInfoWd1              Value to write to Table Word 1 Register (ALE_TBLW1).
        aleInfoWd2              Value to write to ALE Table Word 2 Register (ALE_TBLW2).
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_WORD0_REG_ENTRY_31_0,
 *      CPSW_3GF_ALE_TABLE_WORD1_REG_ENTRY_63_32,
 *      CPSW_3GF_ALE_TABLE_WORD2_REG_ENTRY_71_64,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      
 *
 *   @b Example
 *   @verbatim
        Uint32      index, info0, info1, info2;

        index   =   0;
        info0   =   ...;
        info1   =   ...;
        info2   =   ...;
 
        CSL_CPSW_3GF_setAleTableEntry (index, 
                                          info0,
                                          info1,
                                          info2);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleTableEntry
(
    Uint32                      index,
    Uint32                      aleInfoWd0,
    Uint32                      aleInfoWd1,
    Uint32                      aleInfoWd2
)
{
    Uint32                      aleTblCtrlVal   =   0;

    /* Set the ALE table word registers */
    CSL_FINS (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, CPSW_3GF_ALE_TABLE_WORD0_REG_ENTRY_31_0, aleInfoWd0);
    CSL_FINS (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, CPSW_3GF_ALE_TABLE_WORD1_REG_ENTRY_63_32, aleInfoWd1);
    CSL_FINS (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, CPSW_3GF_ALE_TABLE_WORD2_REG_ENTRY_71_64, aleInfoWd2);

    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getALEEntryType
 *
 *   @b Description
 *   @n This function returns the ALE entry type for any given ALE table
 *      entry index.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  CSL_CPSW_3GF_ALE_ENTRYTYPE
 *	 @n	 ALE_ENTRYTYPE_FREE             ALE entry is free.
 *	 @n  ALE_ENTRYTYPE_ADDRESS          ALE entry contains a unicast/multicast address.
 *	 @n  ALE_ENTRYTYPE_VLAN             VLAN ALE entry.
 *	 @n  ALE_ENTRYTYPE_VLANADDRESS      VLAN Address Entry. 
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD1_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
        if (CSL_CPSW_3GF_getALEEntryType () == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry free
        }
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE CSL_CPSW_3GF_ALE_ENTRYTYPE CSL_CPSW_3GF_getALEEntryType 
(
    Uint32                      index
)
{
    Uint32                      aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;

    return (CSL_CPSW_3GF_ALE_ENTRYTYPE) CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28);
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getALEAddressType
 *
 *   @b Description
 *   @n This function returns the address type of an ALE entry.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  CSL_CPSW_3GF_ALE_ADDRTYPE
 *	 @n	 ALE_ADDRTYPE_UCAST         Address at this entry is unicast
 *	 @n  ALE_ADDRTYPE_MCAST         Address at this entry is multicast 
 *	 @n  ALE_ADDRTYPE_OUI           Address at this entry is OUI address
 *
 *   <b> Pre Condition </b>
 *   @n  This function must be called only for an ALE address entry, i.e., 
 *       if @a CSL_CPSW_3GF_getALEEntryType () returns ALE_ENTRYTYPE_ADDRESS
 *       or ALE_ENTRYTYPE_VLANADDRESS only.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0_REG,
 *      CPSW_3GF_ALE_TABLE_WORD1_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_ADDRESS)
        {
            // ALE entry has an address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_UCAST)
            {
                // Unicast address
            }
        }
        else
        {
            // Do nothing
        }

        ...
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE CSL_CPSW_3GF_ALE_ADDRTYPE CSL_CPSW_3GF_getALEAddressType 
(
    Uint32                      index
)
{
    Uint32                      aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;

    if (CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 0) == 0)            
    {
        /* Lower 24 bits of Word 0 are all zeros for an OUI address */            
        return  ALE_ADDRTYPE_OUI;             
    }

    if (hCpsw3gfRegs->ALE_TABLE_WORD1_REG & (1 << 8))            
    {
        /* 40th bit of MAC address is 1 for a multicast address. */
        return  ALE_ADDRTYPE_MCAST;
    }
    else
    {
        return  ALE_ADDRTYPE_UCAST;
    }
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleMcastAddrEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with Multicast address configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pMcastAddrCfg           ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                              index;
        CSL_CPSW_3GF_ALE_MCASTADDR_ENTRY    mcastAddrCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_ADDRESS)
        {
            // ALE entry has an address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_MCAST)
            {
                // Read Multicast address config from hardware
                CSL_CPSW_3GF_getAleMcastAddrEntry (index, &mcastAddrCfg);
            }
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleMcastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_MCASTADDR_ENTRY*       pMcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pMcastAddrCfg->macAddress[0]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8);
    pMcastAddrCfg->macAddress[1]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0);
    pMcastAddrCfg->macAddress[2]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24);
    pMcastAddrCfg->macAddress[3]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16);
    pMcastAddrCfg->macAddress[4]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8);
    pMcastAddrCfg->macAddress[5]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0);
    pMcastAddrCfg->mcastFwdState        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30);    
    pMcastAddrCfg->superEnable          =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1);
    pMcastAddrCfg->portMask             =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 4, 2);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleMcastAddrEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      Multicast address configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pMcastAddrCfg           ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                              index;
        CSL_CPSW_3GF_ALE_MCASTADDR_ENTRY    mcastAddrCfg;

        index   =   0;
        mcastAddrCfg.macAddress [0] = 0x00;
        mcastAddrCfg.macAddress [1] = 0x01;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add Multicast address entry
            CSL_CPSW_3GF_setAleMcastAddrEntry (index, &mcastAddrCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleMcastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_MCASTADDR_ENTRY*       pMcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the multicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8, pMcastAddrCfg->macAddress[0]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0, pMcastAddrCfg->macAddress[1]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24, pMcastAddrCfg->macAddress[2]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16, pMcastAddrCfg->macAddress[3]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8, pMcastAddrCfg->macAddress[4]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0, pMcastAddrCfg->macAddress[5]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30, pMcastAddrCfg->mcastFwdState);    
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1, pMcastAddrCfg->superEnable);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 4, 2, pMcastAddrCfg->portMask);

    /* set entry type to address entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 1);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleVlanMcastAddrEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with VLAN Multicast address configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pVlanMcastAddrCfg       ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLANMCASTADDR_ENTRY    vlanMcastAddrCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_VLANADDRESS)
        {
            // ALE entry has a VLAN address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_MCAST)
            {
                // Read VLAN Multicast address config from hardware
                CSL_CPSW_3GF_getAleVlanMcastAddrEntry (index, &vlanMcastAddrCfg);
            }
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleVlanMcastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLANMCASTADDR_ENTRY*   pVlanMcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pVlanMcastAddrCfg->macAddress[0]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8);
    pVlanMcastAddrCfg->macAddress[1]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0);
    pVlanMcastAddrCfg->macAddress[2]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24);
    pVlanMcastAddrCfg->macAddress[3]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16);
    pVlanMcastAddrCfg->macAddress[4]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8);
    pVlanMcastAddrCfg->macAddress[5]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0);
    pVlanMcastAddrCfg->vlanId               =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16);
    pVlanMcastAddrCfg->mcastFwdState        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30);    
    pVlanMcastAddrCfg->superEnable          =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1);
    pVlanMcastAddrCfg->portMask             =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 4, 2);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleVlanMcastAddrEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      VLAN Multicast address configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pVlanMcastAddrCfg       ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLANMCASTADDR_ENTRY    vlanMcastAddrCfg;

        index   =   0;
        vlanMcastAddrCfg.macAddress [0] = 0x00;
        vlanMcastAddrCfg.macAddress [1] = 0x01;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add VLAN Multicast address entry
            CSL_CPSW_3GF_setAleVlanMcastAddrEntry (index, &vlanMcastAddrCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleVlanMcastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLANMCASTADDR_ENTRY*   pVlanMcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the VLAN multicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8, pVlanMcastAddrCfg->macAddress[0]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0, pVlanMcastAddrCfg->macAddress[1]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24, pVlanMcastAddrCfg->macAddress[2]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16, pVlanMcastAddrCfg->macAddress[3]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8, pVlanMcastAddrCfg->macAddress[4]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0, pVlanMcastAddrCfg->macAddress[5]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16, pVlanMcastAddrCfg->vlanId);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30, pVlanMcastAddrCfg->mcastFwdState);    
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1, pVlanMcastAddrCfg->superEnable);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 4, 2, pVlanMcastAddrCfg->portMask);
    
    /* set entry type to VLAN address entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 3);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleUnicastAddrEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with Unicast address configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pUcastAddrCfg           ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY      ucastAddrCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_ADDRESS)
        {
            // ALE entry has an address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_UCAST)
            {
                // Read Unicast address config from hardware
                CSL_CPSW_3GF_getAleUnicastAddrEntry (index, &ucastAddrCfg);
            }
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleUnicastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY*     pUcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pUcastAddrCfg->macAddress[0]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8);
    pUcastAddrCfg->macAddress[1]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0);
    pUcastAddrCfg->macAddress[2]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24);
    pUcastAddrCfg->macAddress[3]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16);
    pUcastAddrCfg->macAddress[4]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8);
    pUcastAddrCfg->macAddress[5]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0);
    pUcastAddrCfg->ucastType            =   (CSL_CPSW_3GF_ALE_UCASTTYPE) CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30);
    pUcastAddrCfg->secureEnable         =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 0, 0);    
    pUcastAddrCfg->blockEnable          =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1);
    pUcastAddrCfg->portNumber           =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 3, 2);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleUnicastAddrEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      unicast address configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pUcastAddrCfg           ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY      ucastAddrCfg;

        index   =   0;
        ucastAddrCfg.macAddress [0] = 0x00;
        ucastAddrCfg.macAddress [1] = 0x01;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add Unicast address entry
            CSL_CPSW_3GF_setAleUnicastAddrEntry (index, &ucastAddrCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleUnicastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY*     pUcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the Unicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8, pUcastAddrCfg->macAddress[0]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0, pUcastAddrCfg->macAddress[1]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24, pUcastAddrCfg->macAddress[2]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16, pUcastAddrCfg->macAddress[3]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8, pUcastAddrCfg->macAddress[4]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0, pUcastAddrCfg->macAddress[5]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30, pUcastAddrCfg->ucastType);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 0, 0, pUcastAddrCfg->secureEnable);    
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1, pUcastAddrCfg->blockEnable);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 3, 2, pUcastAddrCfg->portNumber);
    
    /* set entry type to address entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 1);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleOUIAddrEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with OUI address configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pOUIAddrCfg             ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_OUIADDR_ENTRY          ouiAddrCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_ADDRESS)
        {
            // ALE entry has an address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_UCAST)
            {
                // Read Unicast address config from hardware
                CSL_CPSW_3GF_getAleOUIAddrEntry (index, &ouiAddrCfg);
            }
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleOUIAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_OUIADDR_ENTRY*         pOUIAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pOUIAddrCfg->ouiAddress[0]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8);
    pOUIAddrCfg->ouiAddress[1]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0);
    pOUIAddrCfg->ouiAddress[2]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24);
    pOUIAddrCfg->ucastType            =   (CSL_CPSW_3GF_ALE_UCASTTYPE) CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleOUIAddrEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      OUI address configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pOUIAddrCfg             ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_OUIADDR_ENTRY          ouiAddrCfg;

        index   =   0;
        ouiAddrCfg.ouiAddress [0] = 0x00;
        ouiAddrCfg.ouiAddress [1] = 0x01;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add OUI address entry
            CSL_CPSW_3GF_setAleOUIAddrEntry (index, &ouiAddrCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleOUIAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_OUIADDR_ENTRY*         pOUIAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the Unicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8, pOUIAddrCfg->ouiAddress[0]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0, pOUIAddrCfg->ouiAddress[1]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24, pOUIAddrCfg->ouiAddress[2]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30, pOUIAddrCfg->ucastType);
    
    /* set entry type to address entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 1);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}


/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleVlanUnicastAddrEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with VLAN Unicast address configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pVlanUcastAddrCfg       ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLANUNICASTADDR_ENTRY  vlanUcastAddrCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_VLANADDRESS)
        {
            // ALE entry has a VLAN address

            if (CSL_CPSW_3GF_getALEAddressType (index) ==  ALE_ADDRTYPE_UCAST)
            {
                // Read VLAN Unicast address config from hardware
                CSL_CPSW_3GF_getAleVlanUnicastAddrEntry (index, &ucvlanUcastAddrCfgastAddrCfg);
            }
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleVlanUnicastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLANUNICASTADDR_ENTRY* pVlanUcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pVlanUcastAddrCfg->macAddress[0]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8);
    pVlanUcastAddrCfg->macAddress[1]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0);
    pVlanUcastAddrCfg->macAddress[2]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24);
    pVlanUcastAddrCfg->macAddress[3]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16);
    pVlanUcastAddrCfg->macAddress[4]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8);
    pVlanUcastAddrCfg->macAddress[5]        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0);
    pVlanUcastAddrCfg->vlanId               =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16);
    pVlanUcastAddrCfg->ucastType            =   (CSL_CPSW_3GF_ALE_UCASTTYPE) CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30);
    pVlanUcastAddrCfg->secureEnable         =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 0, 0);    
    pVlanUcastAddrCfg->blockEnable          =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1);
    pVlanUcastAddrCfg->portNumber           =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 3, 2);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleVlanUnicastAddrEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      VLAN unicast address configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pVlanUcastAddrCfg       ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLANUNICASTADDR_ENTRY  vlanUcastAddrCfg;

        index   =   0;
        vlanUcastAddrCfg.macAddress [0] = 0x00;
        vlanUcastAddrCfg.macAddress [1] = 0x01;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add VLAN Unicast address entry
            CSL_CPSW_3GF_setAleVlanUnicastAddrEntry (index, &vlanUcastAddrCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleVlanUnicastAddrEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLANUNICASTADDR_ENTRY* pVlanUcastAddrCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the VLAN Unicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 15, 8, pVlanUcastAddrCfg->macAddress[0]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 7, 0, pVlanUcastAddrCfg->macAddress[1]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 31, 24, pVlanUcastAddrCfg->macAddress[2]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 23, 16, pVlanUcastAddrCfg->macAddress[3]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 15, 8, pVlanUcastAddrCfg->macAddress[4]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 7, 0, pVlanUcastAddrCfg->macAddress[5]);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16, pVlanUcastAddrCfg->vlanId);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 31, 30, pVlanUcastAddrCfg->ucastType);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 0, 0, pVlanUcastAddrCfg->secureEnable);    
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 1, 1, pVlanUcastAddrCfg->blockEnable);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD2_REG, 3, 2, pVlanUcastAddrCfg->portNumber);
    
    /* set entry type to address entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 3);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAleVlanEntry
 *
 *   @b Description
 *   @n This function reads the ALE table entry for the index specified and
 *      fills the output parameter structure with VLAN configuration
 *      read from the hardware.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index to be read.
        pVlanCfg                ALE entry contents read.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=0
 *
 *   @b Reads
 *   @n CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLAN_ENTRY             vlanCfg;

        index   =   0;
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_VLAN)
        {
            // ALE entry has a VLAN configuration
            CSL_CPSW_3GF_getAleVlanEntry (index, &vlanCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAleVlanEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLAN_ENTRY*            pVlanCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Set the index in the ALE table to "read" operation and let
     * the hardware load the ALE table entry corresponding to index
     * specified into the ALE table word registers.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, 0);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG =   aleTblCtrlVal;

    /* Read the multicast address configuration */
    pVlanCfg->vlanMemList                   =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 2, 0);
    pVlanCfg->unRegMcastFloodMask           =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 10, 8);
    pVlanCfg->regMcastFloodMask             =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 18, 16);
    pVlanCfg->forceUntaggedEgress           =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 26, 24);
    pVlanCfg->vlanId                        =   CSL_FEXTR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16);
                
    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAleVlanEntry
 *
 *   @b Description
 *   @n This function sets up the ALE table entry for the index specified with 
 *      VLAN configuration specified here.
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
        pVlanCfg                ALE entry contents to be configured.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0,
 *      CPSW_3GF_ALE_TABLE_WORD1,
 *      CPSW_3GF_ALE_TABLE_WORD2
 *
 *   @b Example
 *   @verbatim
        Uint32                                  index;
        CSL_CPSW_3GF_ALE_VLAN_ENTRY             vlanCfg;

        index   =   0;
        vlanCfg.vlanId  = 0x10;
        ...
 
        if (CSL_CPSW_3GF_getALEEntryType (index) == ALE_ENTRYTYPE_FREE)
        {
            // ALE entry is free

            // Add VLAN entry
            CSL_CPSW_3GF_setAleVlanEntry (index, &vlanCfg);
        }
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAleVlanEntry
(
    Uint32                                  index,
    CSL_CPSW_3GF_ALE_VLAN_ENTRY*            pVlanCfg
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Setup the Unicast address configuration */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 2, 0, pVlanCfg->vlanMemList);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 10, 8, pVlanCfg->unRegMcastFloodMask);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 18, 16, pVlanCfg->regMcastFloodMask);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD0_REG, 26, 24, pVlanCfg->forceUntaggedEgress);
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 27, 16, pVlanCfg->vlanId);
    
    /* set entry type to VLAN entry */
    CSL_FINSR (hCpsw3gfRegs->ALE_TABLE_WORD1_REG, 29, 28, 2);
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_clearAleEntry
 *
 *   @b Description
 *   @n This function clears the ALE entry corresponding to the index 
 *      specified
 *
 *   @b Arguments
     @verbatim
        index                   ALE table index.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER,
 *      CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ=1
 *      CPSW_3GF_ALE_TABLE_WORD0=0,
 *      CPSW_3GF_ALE_TABLE_WORD1=0,
 *      CPSW_3GF_ALE_TABLE_WORD2=0
 *
 *   @b Example
 *   @verbatim
        Uint32                              index;

        index   =   0;

        CSL_CPSW_3GF_clearAleEntry (index);
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_clearAleEntry
(
    Uint32                                  index
)
{
    Uint32                                  aleTblCtrlVal   =   0;

    /* Clear all the ALE words */
    hCpsw3gfRegs->ALE_TABLE_WORD0_REG   =   0;
    hCpsw3gfRegs->ALE_TABLE_WORD1_REG   =   0;
    hCpsw3gfRegs->ALE_TABLE_WORD2_REG   =   0;
    
    /* Set the index in the ALE table to "write" operation and let
     * the hardware set up the ALE table entry corresponding to index
     * specified.
     */
    aleTblCtrlVal           =   CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_ENTRY_POINTER, index) |
                                CSL_FMK (CPSW_3GF_ALE_TABLE_CONTROL_REG_WRITE_RDZ, (Uint32) 1);

    hCpsw3gfRegs->ALE_TABLE_CONTROL_REG    =   aleTblCtrlVal;    

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_getAlePortControlReg
 *
 *   @b Description
 *   @n This function retrieves the contents of ALE Port control register
 *      corresponding to the port number specified.
 *
 *   @b Arguments
     @verbatim
        portNo                  Port number for which the ALE port control register
                                must be read.
        pPortControlInfo        CSL_CPSW_3GF_ALE_PORTCONTROL structure that needs to be 
                                filled with Port control register info read from
                                the hardware.
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
 *   @n CPSW_3GF_ALE_PORT_CONTROL_REG_PORT_STATE,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_DROP_UNTAGGED,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_VID_INGRESS_CHECK,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_NO_LEARN,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_MCAST_LIMIT,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_BCAST_LIMIT
 *
 *   @b Example
 *   @verbatim
        Uint32                          index;
        CSL_CPSW_3GF_ALE_PORTCONTROL    portControlInfo;

        index   =   0;
 
        CSL_CPSW_3GF_getAlePortControlReg (index, &portControlInfo);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_getAlePortControlReg
(
    Uint32                          portNo,
    CSL_CPSW_3GF_ALE_PORTCONTROL*   pPortControlInfo
)
{
    pPortControlInfo->portState             =   (CSL_CPSW_3GF_ALE_PORTSTATE) CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_PORT_STATE);
    pPortControlInfo->dropUntaggedEnable    =   CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_DROP_UNTAGGED);
    pPortControlInfo->vidIngressCheckEnable =   CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_VID_INGRESS_CHECK);
    pPortControlInfo->noLearnModeEnable     =   CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_NO_LEARN);
    pPortControlInfo->mcastLimit            =   CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_MCAST_LIMIT);
    pPortControlInfo->bcastLimit            =   CSL_FEXT (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], 
                                                          CPSW_3GF_ALE_PORT_CONTROL_REG_BCAST_LIMIT);

    return;
}

/** ============================================================================
 *   @n@b CSL_CPSW_3GF_setAlePortControlReg
 *
 *   @b Description
 *   @n This function sets up the contents of ALE Port control register
 *      corresponding to the port number specified.
 *
 *   @b Arguments
     @verbatim
        portNo                  Port number for which the ALE port control register
                                must be configured.
        pPortControlInfo        CSL_CPSW_3GF_ALE_PORTCONTROL structure that contains 
                                port control register settings to be written.
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
 *   @b Writes
 *   @n CPSW_3GF_ALE_PORT_CONTROL_REG_PORT_STATE,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_DROP_UNTAGGED,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_VID_INGRESS_CHECK,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_NO_LEARN,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_MCAST_LIMIT,
 *      CPSW_3GF_ALE_PORT_CONTROL_REG_BCAST_LIMIT
 *
 *   @b Example
 *   @verbatim
        Uint32                          index;
        CSL_CPSW_3GF_ALE_PORTCONTROL    portControlInfo;

        index   =   0;
        portControlInfo.portState   =   ALE_PORTSTATE_FORWARD |
                                        ALE_PORTSTATE_LEARN;
 
        CSL_CPSW_3GF_setAlePortControlReg (index, &portControlInfo);
	 
     @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_CPSW_3GF_setAlePortControlReg
(
    Uint32                          portNo,
    CSL_CPSW_3GF_ALE_PORTCONTROL*   pPortControlInfo
)
{
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_PORT_STATE,
              pPortControlInfo->portState);
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_DROP_UNTAGGED,
              pPortControlInfo->dropUntaggedEnable);
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_VID_INGRESS_CHECK, 
              pPortControlInfo->vidIngressCheckEnable);
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_NO_LEARN, 
              pPortControlInfo->noLearnModeEnable);
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_MCAST_LIMIT, 
              pPortControlInfo->mcastLimit);
    CSL_FINS (hCpsw3gfRegs->ALE_PORT_CONTROL_REG [portNo], CPSW_3GF_ALE_PORT_CONTROL_REG_BCAST_LIMIT,
              pPortControlInfo->bcastLimit);

    return;
}

#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
