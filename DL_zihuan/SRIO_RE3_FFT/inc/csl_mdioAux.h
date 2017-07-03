

#ifndef _CSL_MDIOAUX_H
#define _CSL_MDIOAUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_mdio.h"

/** @addtogroup CSL_MDIO_FUNCTION
@{ */

/** ============================================================================
 *   @n@b CSL_MDIO_getVersionInfo
 *
 *   @b Description
 *   @n This function retrieves the MDIO version information.
 *
 *   @b Arguments
     @verbatim
        mdioVersionInfo     CSL_MDIO_VERSION structure that needs to be populated
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
 *   @n MDIO_VERSION_REG_REVMINOR,
 *      MDIO_VERSION_REG_REVMAJ,
 *      MDIO_VERSION_REG_MODID
 *
 *   @b Example
 *   @verbatim
        CSL_MDIO_VERSION    mdioVersionInfo;

        CSL_MDIO_getVersion (&mdioVersionInfo);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_getVersionInfo (
	CSL_MDIO_VERSION*       mdioVersionInfo     
)
{
    mdioVersionInfo->revMin     =   CSL_FEXT (hMdioRegs->VERSION_REG, MDIO_VERSION_REG_REVMINOR);
    mdioVersionInfo->revMaj     =   CSL_FEXT (hMdioRegs->VERSION_REG, MDIO_VERSION_REG_REVMAJ);
    mdioVersionInfo->modId      =   CSL_FEXT (hMdioRegs->VERSION_REG, MDIO_VERSION_REG_MODID);

    return;        
}

/** ============================================================================
 *   @n@b CSL_MDIO_getClkDivVal
 *
 *   @b Description
 *   @n This function retrieves the clock divider value (CLKDIV) from the
 *      MDIO control register.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_CLKDIV
 *
 *   @b Example
 *   @verbatim
 *      Uint16      clkDivVal;

        clkDivVal   =   CSL_MDIO_getClkDivVal ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint16 CSL_MDIO_getClkDivVal (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_CLKDIV);
}

/** ============================================================================
 *   @n@b CSL_MDIO_setClkDivVal
 *
 *   @b Description
 *   @n This function configures the clock divider value (CLKDIV) in the
 *      MDIO control register with the value specified.
 *
 *   @b Arguments   
     @verbatim
        clkDivVal           The value to use for clock divider configuration.
                            When this set to 0, the MDIO clock is disabled.
 *	 @endverbatim
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  CONTROL_REG configured with the clock divider value. Configures the MDIO
 *       clock frequency.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_CLKDIV
 *
 *   @b Example
 *   @verbatim
 *      Uint16      clkDivVal;
 
        clkDivVal = 165;

        // Setup the MDIO clock frequency
        CSL_MDIO_setClkDivVal (clkDivVal);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_setClkDivVal (
    Uint16                 clkDivVal     
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_CLKDIV, clkDivVal);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isFaultDetectEnabled
 *
 *   @b Description
 *   @n This function returns the value of the Fault detect enable bit of the
 *      MDIO control register. The Fault detect enable bit indicates whether
 *      physical layer fault detection is enabled.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_FAULT_DETECT_ENABLE
 *
 *   @b Example
 *   @verbatim
        
        if (CSL_MDIO_isFaultDetectEnabled ())
        {
            // MDIO/Physical Layer fault detection is enabled
        }
        else
        {
            // MDIO/Physical Layer fault detection is disabled
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isFaultDetectEnabled (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_FAULT_DETECT_ENABLE);
}


/** ============================================================================
 *   @n@b CSL_MDIO_enableFaultDetect
 *
 *   @b Description
 *   @n This function enables the Physical layer fault detection mechanism by
 *      setting 'FAULTENB' bit of the MDIO Control register to 1.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables PHY layer fault detection. 
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_FAULT_DETECT_ENABLE=1
 *
 *   @b Example
 *   @verbatim
 *
        // Enable Phy Layer fault detection
        CSL_MDIO_enableFaultDetect ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_enableFaultDetect (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_FAULT_DETECT_ENABLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_disableFaultDetect
 *
 *   @b Description
 *   @n This function disables the Physical layer fault detection mechanism by
 *      setting 'FAULTENB' bit of the MDIO Control register to 0.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Disables PHY layer fault detection.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_FAULT_DETECT_ENABLE=0
 *
 *   @b Example
 *   @verbatim
 *
        // Disable Phy Layer fault detection
        CSL_MDIO_disableFaultDetect ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_disableFaultDetect (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_FAULT_DETECT_ENABLE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isFaultDetected
 *
 *   @b Description
 *   @n This function returns physical layer fault status, i.e., 1 to indicate 
 *      that the MDIO module has detected a physical layer fault and 0 otherwise.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_FAULT
 *
 *   @b Example
 *   @verbatim
        
        if (CSL_MDIO_isFaultDetected ())
        {
            // Physical Layer fault detected
        }
        else
        {
            // No Physical Layer fault detected
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isFaultDetected (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_FAULT);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearFault
 *
 *   @b Description
 *   @n This function clears the 'FAULT' bit of the MDIO control register.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the 'FAULT' bit of MDIO 'CONTROL_REG'.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_FAULT=1
 *
 *   @b Affects
 *   @n MDIO_CONTROL_REG_FAULT=0
 *
 *   @b Example
 *   @verbatim
 *
        // Clear MDIO fault bit
        CSL_MDIO_clearFault ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearFault (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_FAULT, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isPreambleEnabled
 *
 *   @b Description
 *   @n This function returns the value of the Preamble disable bit of the
 *      MDIO control register. It returns 1 to indicate the MDIO preamble is 
 *      enabled and 0 otherwise.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_PREAMBLE
 *
 *   @b Example
 *   @verbatim
        
        if (CSL_MDIO_isPreambleEnabled ())
        {
            // MDIO preamble enabled
        }
        else
        {
            // MDIO preamble disabled
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isPreambleEnabled (
    void
)
{
    Uint32      bIsPreambleDisabled;

    bIsPreambleDisabled = CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_PREAMBLE);

    return (bIsPreambleDisabled == 0 ? 1 : 0);
}


/** ============================================================================
 *   @n@b CSL_MDIO_enablePreamble
 *
 *   @b Description
 *   @n This function enables the MDIO preamble.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables MDIO Preamble frames.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_PREAMBLE=0
 *
 *   @b Example
 *   @verbatim
 *
        // Enable MDIO preamble frames
        CSL_MDIO_enablePreamble ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_enablePreamble (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_PREAMBLE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_disablePreamble
 *
 *   @b Description
 *   @n This function disables the MDIO preamble.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Disables MDIO Preamble frames.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_PREAMBLE=1
 *
 *   @b Example
 *   @verbatim
 *
        // Disable MDIO preamble frames
        CSL_MDIO_disablePreamble ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_disablePreamble (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_PREAMBLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_getHighestUserChannel
 *
 *   @b Description
 *   @n This function returns the highest user access channel available in MDIO 
 *      module.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_HIGHEST_USER_CHANNEL
 *
 *   @b Example
 *   @verbatim
 *      Uint32  userAccChannel;
     
        // Get the highest user access channel.
        userAccChannel = CSL_MDIO_getHighestUserChannel ();
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_getHighestUserChannel (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_HIGHEST_USER_CHANNEL);
}

/** ============================================================================
 *   @n@b CSL_MDIO_isStateMachineEnabled
 *
 *   @b Description
 *   @n This function returns the value of the Enable bit of the MDIO control 
 *      register. It returns 1 to indicate that the MDIO state machine is enabled
 *      and is active and 0 to indicate otherwise.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_ENABLE
 *
 *   @b Example
 *   @verbatim
        
        if (CSL_MDIO_isStateMachineEnabled ())
        {
            // MDIO state machine enabled
        }
        else
        {
            // MDIO state machine disabled
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isStateMachineEnabled (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_ENABLE);
}


/** ============================================================================
 *   @n@b CSL_MDIO_enableStateMachine
 *
 *   @b Description
 *   @n This function enables the MDIO state machine if not already active.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables MDIO State machine.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_ENABLE
 *
 *   @b Example
 *   @verbatim
 *
        // Enable MDIO state machine
        CSL_MDIO_enableStateMachine ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_enableStateMachine (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_ENABLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_disableStateMachine
 *
 *   @b Description
 *   @n This function sets the 'ENABLE' bit of the MDIO control register to 0, 
 *      triggering the disable of MDIO state machine if active.
 *
 *   @b Arguments   None
 *
 *   <b> Return Value </b>  
 *	 @n	 None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  If the MDIO state machine is active at the time it is disabled, will
 *       complete the current operation before halting it and setting the
 *       idle bit.
 *
 *   @b Writes
 *   @n MDIO_CONTROL_REG_ENABLE=0
 *
 *   @b Example
 *   @verbatim
 *
        // Disable MDIO state machine
        CSL_MDIO_disableStateMachine ();

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_disableStateMachine (
)
{
    CSL_FINS (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_ENABLE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isStateMachineIdle
 *
 *   @b Description
 *   @n This function returns the value of the 'IDLE' bit of the MDIO control 
 *      register. It returns 1 to indicate that the MDIO state machine is in idle
 *      state and 0 otherwise.
 *
 *   @b Arguments   None
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
 *   @n MDIO_CONTROL_REG_IDLE
 *
 *   @b Example
 *   @verbatim
        
        if (CSL_MDIO_isStateMachineIdle ())
        {
            // MDIO state machine idle
        }
        else
        {
            // MDIO state machine not idle
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isStateMachineIdle (
    void
)
{
    return CSL_FEXT (hMdioRegs->CONTROL_REG, MDIO_CONTROL_REG_IDLE);
}

/** ============================================================================
 *   @n@b CSL_MDIO_isPhyAlive
 *
 *   @b Description
 *   @n For a given PHY address provided to this function in 'phyAddr',  this function
 *      reads the 'ALIVE' bit corresponding to that PHY; The function returns 1 to
 *      indicate that the most recent access for the address was acknowledged by the 
 *      PHY and 0 otherwise.
 *
 *   @b Arguments
 *   @verbatim
 *      phyAddr             The PHY address number (0-31) for which the alive bit
 *                          status must be checked and returned by this function.
 *   @endverbatim
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
 *   @n MDIO_ALIVE_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      phyAddr = 0;
       
        // Check if PHY Address 0 is alive
        if (CSL_MDIO_isPhyAlive (phyAddr))
        {
            // MDIO PHY 0 access succeeded
        }
        else
        {
            // MDIO PHY 0 access failed
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isPhyAlive (
    Uint32                  phyAddr
)
{
    return CSL_FEXTR (hMdioRegs->ALIVE_REG, phyAddr, phyAddr);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearPhyAliveStatus
 *
 *   @b Description
 *   @n For a given PHY address provided to this function in 'phyAddr',  this function
 *      clears the 'ALIVE' bit corresponding to that PHY.
 *
 *   @b Arguments
 *   @verbatim
 *      phyAddr             The PHY address number (0-31) for which the alive bit
 *                          status must be cleared.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the ALIVE bit for the PHY address specified
 *
 *   @b Writes
 *   @n MDIO_ALIVE_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      phyAddr = 0;
       
        // Clear PHY 0 alive status bit
        CSL_MDIO_clearPhyAliveStatus (phyAddr);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearPhyAliveStatus (
    Uint32                  phyAddr
)
{
    CSL_FINSR (hMdioRegs->ALIVE_REG, phyAddr, phyAddr, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isPhyLinked
 *
 *   @b Description
 *   @n For a given PHY address provided to this function in 'phyAddr',  this function
 *      reads the 'LINK' bit corresponding to that PHY; The function returns 1 to
 *      indicate that the corresponding PHY address has a link.
 *
 *   @b Arguments
 *   @verbatim
 *      phyAddr             The PHY address number (0-31) for which the link bit
 *                          status must be checked and returned by this function.
 *   @endverbatim
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
 *   @n MDIO_LINK_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      phyAddr = 0;
       
        // Check if PHY Address 0 link is up
        if (CSL_MDIO_isPhyLinked (phyAddr))
        {
            // MDIO PHY 0 link up
        }
        else
        {
            // MDIO PHY 0 link not up
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isPhyLinked (
    Uint32                  phyAddr
)
{
    return CSL_FEXTR (hMdioRegs->LINK_REG, phyAddr, phyAddr);
}

/** ============================================================================
 *   @n@b CSL_MDIO_isUnmaskedLinkStatusChangeIntSet
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERPHYSEL register, this function returns the 'LINKINTRAW' register
 *      contents corresponding to it. This functions returns a 1 to indicate that
 *      the link status has changed for the PHY provided and 0 otherwise.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the link status change
 *                          bit must be read. Phy selector value 0 corresponds to the
 *                          PHY address programmed in USERPHYSEL0 register and a Phy
 *                          selector value 1 corresponds to the PHY address in 
 *                          USERPHYSEL1 register.
 *   @endverbatim
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
 *   @n MDIO_ALIVE_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        if (CSL_MDIO_isUnmaskedLinkStatusChangeIntSet (index))
        {
            // Link status has changed
        }
        else
        {
            // Link status has not changed
        }
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isUnmaskedLinkStatusChangeIntSet (
    Uint32                  index
)
{
    return CSL_FEXTR (hMdioRegs->LINK_INT_RAW_REG, index, index);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearUnmaskedLinkStatusChangeInt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERPHYSEL register, this function clears the 'LINKINTRAW' register
 *      contents corresponding to it. 
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the link status change
 *                          bit must be read. Phy selector value 0 corresponds to the
 *                          PHY address programmed in USERPHYSEL0 register and a Phy
 *                          selector value 1 corresponds to the PHY address in 
 *                          USERPHYSEL1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the Link change interrupt.
 *
 *   @b Writes
 *   @n MDIO_LINK_INT_RAW_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearUnmaskedLinkStatusChangeInt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->LINK_INT_RAW_REG, index, index, 1);
}

/** ============================================================================
 *   @n@b CSL_MDIO_isMaskedLinkStatusChangeIntSet
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERPHYSEL register, this function returns the 'LINKINTMASKED' register
 *      contents corresponding to it. This functions returns a 1 to indicate that
 *      the link status has changed for the PHY provided and 0 otherwise.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the link status change
 *                          bit must be read. Phy selector value 0 corresponds to the
 *                          PHY address programmed in USERPHYSEL0 register and a Phy
 *                          selector value 1 corresponds to the PHY address in 
 *                          USERPHYSEL1 register.
 *   @endverbatim
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
 *   @n MDIO_LINK_INT_MASKED_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        if (CSL_MDIO_isMaskedLinkStatusChangeIntSet (index))
        {
            // Link status has changed
        }
        else
        {
            // Link status has not changed
        }
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isMaskedLinkStatusChangeIntSet (
    Uint32                  index
)
{
    return CSL_FEXTR (hMdioRegs->LINK_INT_MASKED_REG, index, index);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearMaskedLinkStatusChangeInt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERPHYSEL register, this function clears the 'LINKINTMASKED' register
 *      contents corresponding to it. 
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the link status change
 *                          bit must be read. Phy selector value 0 corresponds to the
 *                          PHY address programmed in USERPHYSEL0 register and a Phy
 *                          selector value 1 corresponds to the PHY address in 
 *                          USERPHYSEL1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the Link change interrupt.
 *
 *   @b Writes
 *   @n MDIO_LINK_INT_MASKED_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_clearMaskedLinkStatusChangeInt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearMaskedLinkStatusChangeInt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->LINK_INT_MASKED_REG, index, index, 1);
}


/** ============================================================================
 *   @n@b CSL_MDIO_isUnmaskedUserCmdCompleteIntSet
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function returns the 'USERINTRAW' register
 *      contents corresponding to it. This functions returns a 1 to indicate that
 *      the previously scheduled PHY read/write command using that particular
 *      USERACCESS register has completed and returns 0 otherwise.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the unmasked user 
 *                          command complete interrupt bit must be read. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
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
 *   @n MDIO_USER_INT_RAW_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        if (CSL_MDIO_isUnmaskedUserCmdCompleteIntSet (index))
        {
            // User command completed
        }
        else
        {
            // User command not done
        }
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isUnmaskedUserCmdCompleteIntSet (
    Uint32                  index
)
{
    return CSL_FEXTR (hMdioRegs->USER_INT_RAW_REG, index, index);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearUnmaskedUserCmdCompleteInt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function clears the 'USERINTRAW' register
 *      contents corresponding to it. 
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the unmasked user 
 *                          command complete interrupt bit must be cleared. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the User command complete interrupt.
 *
 *   @b Writes
 *   @n MDIO_USER_INT_RAW_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_clearUnmaskedUserCmdCompleteInt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearUnmaskedUserCmdCompleteInt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->USER_INT_RAW_REG, index, index, 1);
}

/** ============================================================================
 *   @n@b CSL_MDIO_isMaskedUserCmdCompleteIntSet
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function returns the 'USERINTMASKED' register
 *      contents corresponding to it. This functions returns a 1 to indicate that
 *      the previously scheduled PHY read/write command using that particular
 *      USERACCESS register has completed and corresponding USERINTMASKSET bit is
 *      set to 1 and returns 0 otherwise.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the masked user 
 *                          command complete interrupt bit must be read. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
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
 *   @n MDIO_USER_INT_MASKED_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        if (CSL_MDIO_isMaskedUserCmdCompleteIntSet (index))
        {
            // User command completed
        }
        else
        {
            // User command not done
        }
        }

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isMaskedUserCmdCompleteIntSet (
    Uint32                  index
)
{
    return CSL_FEXTR (hMdioRegs->USER_INT_MASKED_REG, index, index);
}

/** ============================================================================
 *   @n@b CSL_MDIO_clearMaskedUserCmdCompleteInt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function clears the 'USERINTMASKED' register
 *      contents corresponding to it. 
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the masked user 
 *                          command complete interrupt bit must be cleared. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears the User command complete interrupt.
 *
 *   @b Writes
 *   @n MDIO_USER_INT_MASKED_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_clearMaskedUserCmdCompleteInt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_clearMaskedUserCmdCompleteInt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->USER_INT_MASKED_REG, index, index, 1);
}

/** ============================================================================
 *   @n@b CSL_MDIO_enableUserCmdCompleteInterrupt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function enables the user command complete
 *      interrupts for it.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the masked user 
 *                          command complete interrupt bit must be enabled. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables the interrupt for User command complete events posted using USERACCESS
 *       register.
 *
 *   @b Writes
 *   @n MDIO_USER_INT_MASK_SET_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_enableUserCmdCompleteInterrupt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_enableUserCmdCompleteInterrupt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->USER_INT_MASK_SET_REG, index, index, 1);
}

/** ============================================================================
 *   @n@b CSL_MDIO_disableUserCmdCompleteInterrupt
 *
 *   @b Description
 *   @n For a given PHY selector(0-1) provided corresponding to the PHY address 
 *      in USERACCESS register, this function disables the user command complete
 *      interrupts for it.
 *
 *   @b Arguments
 *   @verbatim
 *      index              The PHY selector (0-1) for which the masked user 
 *                          command complete interrupt bit must be enabled. Phy selector 
 *                          value 0 corresponds to the PHY address used in USERACCESS0
 *                          register and a Phy selector value 1 corresponds to the PHY 
 *                          address in USERACCESS1 register.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Disables the interrupt for User command complete events posted using USERACCESS
 *       register.
 *
 *   @b Writes
 *   @n MDIO_USER_INT_MASK_CLEAR_REG
 *
 *   @b Example
 *   @verbatim
 *      Uint32      index = 0;
       
        CSL_MDIO_disableUserCmdCompleteInterrupt (index);
	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_disableUserCmdCompleteInterrupt (
    Uint32                  index
)
{
    CSL_FINSR (hMdioRegs->USER_INT_MASK_CLEAR_REG, index, index, 1);
}

/** ============================================================================
 *   @n@b CSL_MDIO_getUserAccessRegister
 *
 *   @b Description
 *   @n This function retrieves the contents of the USERACCESS register 
 *      corresponding to the 'index' (0-1) provided.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERACCESS register contents
 *                          must be read by this function. Valid values are 0-1,
 *                          both inclusive.
 *      pUserAccessReg      CSL_MDIO_USERACCESS structure that needs to be 
 *                          populated with the User Access register contents.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n MDIO_USER_ACCESS_REG_DATA,
 *      MDIO_USER_ACCESS_REG_PHYADR,
 *      MDIO_USER_ACCESS_REG_REGADR,
 *      MDIO_USER_ACCESS_REG_ACK,
 *      MDIO_USER_ACCESS_REG_GO
 *
 *   @b Example
 *   @verbatim
 *      CSL_MDIO_USERACCESS      userAccReg;

        // Get User Access Reg 0 contents
        CSL_MDIO_getUserAccessRegister (0, &userAccReg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_getUserAccessRegister (
    Uint32                  index,                        
    CSL_MDIO_USERACCESS*    pUserAccessReg
)
{
    Uint32                  regVal;

    regVal  =   hMdioRegs->USER_GROUP [index].USER_ACCESS_REG;
    pUserAccessReg->data    =   CSL_FEXT (regVal, MDIO_USER_ACCESS_REG_DATA);
    pUserAccessReg->phyAddr =   CSL_FEXT (regVal, MDIO_USER_ACCESS_REG_PHYADR);
    pUserAccessReg->regAddr =   CSL_FEXT (regVal, MDIO_USER_ACCESS_REG_REGADR);
    pUserAccessReg->ack     =   CSL_FEXT (regVal, MDIO_USER_ACCESS_REG_ACK);
    pUserAccessReg->go      =   CSL_FEXT (regVal, MDIO_USER_ACCESS_REG_GO);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_setUserAccessRegister
 *
 *   @b Description
 *   @n This function configures the contents of the USERACCESS register 
 *      corresponding to the 'index' (0-1) provided with the values provided
 *      in 'pUserAccessReg' input parameter.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERACCESS register contents
 *                          must be configured by this function. Valid values are 0-1,
 *                          both inclusive.
 *      pUserAccessReg      CSL_MDIO_USERACCESS structure that specifies the values
 *                          that need to be configured into the User Access register 
 *                          contents.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  User Access Register(USERACCESS) corresponding to the index specified is 
 *       configured.
 *
 *   @b Reads
 *   @n MDIO_USER_ACCESS_REG_DATA,
 *      MDIO_USER_ACCESS_REG_PHYADR,
 *      MDIO_USER_ACCESS_REG_REGADR,
 *      MDIO_USER_ACCESS_REG_WRITE=1,
 *      MDIO_USER_ACCESS_REG_GO
 *
 *   @b Example
 *   @verbatim
 *      CSL_MDIO_USERACCESS      userAccReg;
 
        userAccessReg.data      =   1;
        userAccessReg.phyAddr   =   0;
        ...
        userAccessReg.go        =   1;

        // Set User Access Reg 0 contents
        CSL_MDIO_setUserAccessRegister (0, &userAccReg);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_setUserAccessRegister (
    Uint32                  index,                        
    CSL_MDIO_USERACCESS*    pUserAccessReg
)
{
    hMdioRegs->USER_GROUP [index].USER_ACCESS_REG   =   CSL_FMK (MDIO_USER_ACCESS_REG_DATA, pUserAccessReg->data) |
                                                        CSL_FMK (MDIO_USER_ACCESS_REG_PHYADR, pUserAccessReg->phyAddr) |
                                                        CSL_FMK (MDIO_USER_ACCESS_REG_REGADR, pUserAccessReg->regAddr) |
                                                        CSL_FMK (MDIO_USER_ACCESS_REG_WRITE, 1) |
                                                        CSL_FMK (MDIO_USER_ACCESS_REG_GO, pUserAccessReg->go);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_isUserAccessPending
 *
 *   @b Description
 *   @n This function reads the 'GO' bit of the USERACCESS register corresponding
 *      to the 'index' specified and returns its value. When 1 returned, indicates
 *      that the user access transaction (read/write) performed earlier is still
 *      pending and has not yet completed. 0 indicates that the transaction has 
 *      completed.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERACCESS register's GO bit
 *                          must be read by this function. Valid values are 0-1,
 *                          both inclusive.
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None.
 *
 *   @b Reads
 *   @n MDIO_USER_ACCESS_REG_GO
 *
 *   @b Example
 *   @verbatim
 *      CSL_MDIO_USERACCESS      userAccReg;
 
        userAccessReg.data      =   1;
        userAccessReg.phyAddr   =   0;
        ...
        userAccessReg.go        =   1;

        // Set User Access Reg 0 contents
        CSL_MDIO_setUserAccessRegister (0, &userAccReg);

        // Wait till the set completes
        while (CSL_MDIO_isUserAccessPending (0));

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE Uint32 CSL_MDIO_isUserAccessPending (
    Uint32                  index                        
)
{
    return CSL_FEXT (hMdioRegs->USER_GROUP [index].USER_ACCESS_REG, MDIO_USER_ACCESS_REG_GO);
}


/** ============================================================================
 *   @n@b CSL_MDIO_enableLinkStatusChangeInterrupt
 *
 *   @b Description
 *   @n This function sets the 'LINKINTENB' bit to 1 for a PHY address 'phyAddr'
 *      being monitored by the MDIO module. This configuration is done for
 *      the 'USERPHYSEL' register corresponding to the index specified here.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERPHYSEL register contents
 *                          must be configured by this function. Valid values are 0-1,
 *                          both inclusive.
 *      phyAddr             PHY Address for which the Link Status change interrupt 
 *                          must be enabled.                         
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  USERPHYSEL register's 'PHYADDRMON' and 'LINKINTENB' bits are configured for 
 *       the index specified. Link Status Change interrupt enabled for PHY address
 *       specified.
 *
 *   @b Writes
 *   @n MDIO_USER_PHY_SEL_REG_PHYADR_MON,
 *      MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE=1
 *
 *   @b Example
 *   @verbatim
 *      
        // Enable PHY 0's interrupt
        CSL_MDIO_enableLinkStatusChangeInterrupt (0, 0);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_enableLinkStatusChangeInterrupt (
    Uint32                  index,                        
    Uint32                  phyAddr
)
{
    hMdioRegs->USER_GROUP [index].USER_PHY_SEL_REG  =   CSL_FMK (MDIO_USER_PHY_SEL_REG_PHYADR_MON, phyAddr) |
                                                        CSL_FMK (MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE, 1);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_disableLinkStatusChangeInterrupt
 *
 *   @b Description
 *   @n This function sets the 'LINKINTENB' bit to 0 for a PHY address 'phyAddr'
 *      being monitored by the MDIO module. This configuration is done for
 *      the 'USERPHYSEL' register corresponding to the index specified here.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERPHYSEL register contents
 *                          must be configured by this function. Valid values are 0-1,
 *                          both inclusive.
 *      phyAddr             PHY Address for which the Link Status change interrupt 
 *                          must be disabled.                         
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  USERPHYSEL register's 'PHYADDRMON' and 'LINKINTENB' bits are configured for 
 *       the index specified.Link Status Change interrupt disabled for PHY address
 *       specified.
 *
 *   @b Writes
 *   @n MDIO_USER_PHY_SEL_REG_PHYADR_MON,
 *      MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE=0
 *
 *   @b Example
 *   @verbatim
 *      
        // Disable PHY 0's interrupt
        CSL_MDIO_disableLinkStatusChangeInterrupt (0, 0);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_disableLinkStatusChangeInterrupt (
    Uint32                  index,                        
    Uint32                  phyAddr
)
{
    hMdioRegs->USER_GROUP [index].USER_PHY_SEL_REG  =   CSL_FMK (MDIO_USER_PHY_SEL_REG_PHYADR_MON, phyAddr) |
                                                        CSL_FMK (MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE, 0);

    return;
}

/** ============================================================================
 *   @n@b CSL_MDIO_getUserPhySelect
 *
 *   @b Description
 *   @n This function reads the 'USERPHYSEL' register contents corresponding to 
 *      the index specified here and returns its contents in the output param
 *      'pUserPhySelect'.
 *
 *   @b Arguments
 *   @verbatim
 *      index               Specifies which of the USERPHYSEL register contents
 *                          must be configured by this function. Valid values are 0-1,
 *                          both inclusive.
 *      pUserPhySelect      CSL_MDIO_USERPHYSEL structure that needs to be populated
 *                          with register contents
 *   @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None.
 *
 *   @b Reads
 *   @n MDIO_USER_PHY_SEL_REG_PHYADR_MON,
 *      MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE,
 *      MDIO_USER_PHY_SEL_REG_LINKSEL
 *
 *   @b Example
 *   @verbatim
 *      CSL_MDIO_USERPHYSEL    userPhySelect; 
 
        // Get USERPHYSEL0 contents
        CSL_MDIO_getUserPhySelect (0, &userPhySelect);

	 @endverbatim
 * =============================================================================
 */
CSL_IDEF_INLINE void CSL_MDIO_getUserPhySelect (
    Uint32                  index,                        
    CSL_MDIO_USERPHYSEL*    pUserPhySelect
)
{
    Uint32                  regVal;

    regVal  =   hMdioRegs->USER_GROUP [index].USER_PHY_SEL_REG;
    pUserPhySelect->linkSel         =   CSL_FEXT (regVal, MDIO_USER_PHY_SEL_REG_LINKSEL);
    pUserPhySelect->phyAddr         =   CSL_FEXT (regVal, MDIO_USER_PHY_SEL_REG_PHYADR_MON);
    pUserPhySelect->bLinkIntEnable  =   CSL_FEXT (regVal, MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE);

    return;
}


#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
