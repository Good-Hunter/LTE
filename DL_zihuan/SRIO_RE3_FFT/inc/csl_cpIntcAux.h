
#ifndef _CSL_CPINTC_AUX_H_
#define _CSL_CPINTC_AUX_H_

#include "csl_cpIntc.h"

#ifdef __cplusplus
 extern "C" {
#endif


CSL_IDEF_INLINE CSL_CPINTC_setNestingMode 
(
    CSL_CPINTC_Handle       hnd, 
    CSL_CPINTCNestingMode   nestMode
)
{
    /* Write the CONTROL_REG with the specified nesting mode. */        
    ((CSL_CPINTC_RegsOvly)hnd)->CONTROL_REG = CSL_FMK(CPINTC_CONTROL_REG_NEST_MODE, nestMode);
}


CSL_IDEF_INLINE CSL_CPINTCNestingMode CSL_CPINTC_getNestingMode (CSL_CPINTC_Handle hnd)
{
    return (CSL_CPINTCNestingMode)CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->CONTROL_REG, 
                                            CPINTC_CONTROL_REG_NEST_MODE);
}


CSL_IDEF_INLINE CSL_CPINTC_setNestingLevel 
(
    CSL_CPINTC_Handle       hnd, 
    CSL_CPINTCNestingLevel  nestLevel
)
{
    unsigned int writeVal;

    writeVal = nestLevel & 0x1ff;
    writeVal |= 0x80000000;   /* set bit 31 auto_override */
    /* Write the GLB_NEST_LEVEL_REG with the specified nesting mode. */        
    ((CSL_CPINTC_RegsOvly)hnd)->GLB_NEST_LEVEL_REG = writeVal;
}


CSL_IDEF_INLINE CSL_CPINTCNestingLevel CSL_CPINTC_getNestingLevel (CSL_CPINTC_Handle hnd)
{
    return (CSL_CPINTCNestingLevel)CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->GLB_NEST_LEVEL_REG, 
                                            CPINTC_GLB_NEST_LEVEL_REG_GLB_NEST_LEVEL);
}


CSL_IDEF_INLINE void CSL_CPINTC_enableAllHostInterrupt (CSL_CPINTC_Handle hnd)
{
    /* Enable all host interrupts by writing 1 to the register. */
    ((CSL_CPINTC_RegsOvly)hnd)->GLOBAL_ENABLE_HINT_REG = 
            CSL_FMK(CPINTC_GLOBAL_ENABLE_HINT_REG_ENABLE_HINT_ANY, 1);
}


CSL_IDEF_INLINE void CSL_CPINTC_disableAllHostInterrupt (CSL_CPINTC_Handle hnd)
{
    /* Disable all host interrupts by writing 0 to the register. */
    ((CSL_CPINTC_RegsOvly)hnd)->GLOBAL_ENABLE_HINT_REG = 
            CSL_FMK(CPINTC_GLOBAL_ENABLE_HINT_REG_ENABLE_HINT_ANY, 0);
}


CSL_IDEF_INLINE void CSL_CPINTC_enableSysInterrupt 
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCSystemInterrupt   sysIntr
)
{
    /* Write the sysIntr to the ENABLE_SET_INDEX_REG to enable the interrupt. */
    ((CSL_CPINTC_RegsOvly)hnd)->ENABLE_SET_INDEX_REG = 
            CSL_FMK(CPINTC_ENABLE_SET_INDEX_REG_ENABLE_SET_INDEX, sysIntr);
}


CSL_IDEF_INLINE void CSL_CPINTC_disableSysInterrupt 
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCSystemInterrupt   sysIntr
)
{
    /* Write the sysIntr to the ENABLE_CLR_INDEX_REG to disable the interrupt. */
    ((CSL_CPINTC_RegsOvly)hnd)->ENABLE_CLR_INDEX_REG = 
            CSL_FMK(CPINTC_ENABLE_CLR_INDEX_REG_ENABLE_CLR_INDEX, sysIntr);
}


CSL_IDEF_INLINE void CSL_CPINTC_clearSysInterrupt 
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCSystemInterrupt   sysIntr
)
{
    /* Write the sysIntr to the STATUS_CLR_INDEX_REG to clear the interrupt. */
    ((CSL_CPINTC_RegsOvly)hnd)->STATUS_CLR_INDEX_REG = 
            CSL_FMK(CPINTC_STATUS_CLR_INDEX_REG_STATUS_CLR_INDEX, sysIntr);
}


CSL_IDEF_INLINE void CSL_CPINTC_enableHostInterrupt 
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCHostInterrupt     hostIntr
)
{
    /* Write the hostIntr to the HINT_ENABLE_SET_INDEX_REG to enable the interrupt. */
    ((CSL_CPINTC_RegsOvly)hnd)->HINT_ENABLE_SET_INDEX_REG = 
            CSL_FMK(CPINTC_HINT_ENABLE_SET_INDEX_REG_HINT_ENABLE_SET_INDEX, hostIntr);
}


CSL_IDEF_INLINE void CSL_CPINTC_disableHostInterrupt
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCHostInterrupt     hostIntr
)
{
    /* Write the hostIntr to the HINT_ENABLE_CLR_INDEX_REG to disable the interrupt. */
    ((CSL_CPINTC_RegsOvly)hnd)->HINT_ENABLE_CLR_INDEX_REG = 
            CSL_FMK(CPINTC_HINT_ENABLE_CLR_INDEX_REG_HINT_ENABLE_CLR_INDEX, hostIntr);        
}


CSL_IDEF_INLINE unsigned short CSL_CPINTC_isInterruptPending (CSL_CPINTC_Handle hnd)
{
    signed int   pendStatus;
    
    /* Read the 'GLB_PRI_INTR_REG' to determine if there is a pending interrupt or not? */
    pendStatus = CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->GLB_PRI_INTR_REG, 
                          CPINTC_GLB_PRI_INTR_REG_GLB_NONE);
    return (pendStatus == 1) ? FALSE : TRUE; 
}


CSL_IDEF_INLINE CSL_CPINTCSystemInterrupt CSL_CPINTC_getPendingInterrupt (CSL_CPINTC_Handle hnd)
{
    return (CSL_CPINTCHostInterrupt)(CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->GLB_PRI_INTR_REG, 
                      CPINTC_GLB_PRI_INTR_REG_GLB_PRI_INTR));
}

CSL_IDEF_INLINE unsigned short CSL_CPINTC_isHostInterruptPending 
(
  CSL_CPINTC_Handle hnd, 
    CSL_CPINTCHostInterrupt     hostIntr
)
{
    signed int   pendStatus;
    
    /* Read the 'PRI_HINT_REG' to determine if there is a pending interrupt for this host */
    pendStatus = CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->PRI_HINT_REG[hostIntr], 
                          CPINTC_PRI_HINT_REG0_NONE_HINT_0);
    return (pendStatus == 1) ? FALSE : TRUE; 
}


CSL_IDEF_INLINE CSL_CPINTCSystemInterrupt CSL_CPINTC_getPendingHostInterrupt 
(
  CSL_CPINTC_Handle hnd,
    CSL_CPINTCHostInterrupt     hostIntr
)
{
    return (CSL_CPINTCSystemInterrupt)(CSL_FEXT(((CSL_CPINTC_RegsOvly)hnd)->PRI_HINT_REG[hostIntr], 
                      CPINTC_PRI_HINT_REG0_PRI_HINT_0));
}


CSL_IDEF_INLINE void CSL_CPINTC_getRawInterruptStatus 
(
    CSL_CPINTC_Handle   hnd,
    unsigned char               index,
    unsigned int*             rawStatus
)
{
    *rawStatus = ((CSL_CPINTC_RegsOvly)hnd)->RAW_STATUS_REG[index];
}


CSL_IDEF_INLINE void CSL_CPINTC_mapSystemIntrToChannel 
(
    CSL_CPINTC_Handle           hnd,
    CSL_CPINTCSystemInterrupt   sysIntr,
    CSL_CPINTCChannel           channel
)
{
    unsigned int  reg_index;
    signed int   bitLow;
    unsigned int* ch_map_start;

    /* There is one register per 4 system interrupts */    
    reg_index = sysIntr / 4;

    /* Get the bit position which has to be updated. */
    bitLow = (sysIntr % 4) * 8;

    /*CH_MAP is declared as byte array. 
      In order to make this write endian independent
      it will be treated as a 32bit word array*/
    ch_map_start = (unsigned int *) &((CSL_CPINTC_RegsOvly)hnd)->CH_MAP[0];

    /* Set the corresponding bits in the Host Interrupt Map Register. */
    CSL_FINSR ( *(ch_map_start + reg_index), bitLow+7, bitLow, channel);
    return;
}


CSL_IDEF_INLINE void CSL_CPINTC_mapChannelToHostInterrupt 
(
    CSL_CPINTC_Handle         hnd,
    CSL_CPINTCChannel         channel,
    CSL_CPINTCHostInterrupt   hostIntr
)
{
    unsigned int  host_reg_index;
    signed int   bitLow;  

    /* There is one register per 4 channels */    
    host_reg_index = channel / 4;

    /* Get the bit position which has to be updated. */
    bitLow = (channel % 4) * 8;

    /* Set the corresponding bits in the Host Interrupt Map Register. */
    CSL_FINSR (((CSL_CPINTC_RegsOvly)hnd)->HINT_MAP[host_reg_index], bitLow+7, bitLow, hostIntr);
    return;
}

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* _CSL_CPINTC_AUX_H_ */

