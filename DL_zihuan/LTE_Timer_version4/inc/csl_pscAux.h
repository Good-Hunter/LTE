
#ifndef _CSL_PSCAUX_H_
#define _CSL_PSCAUX_H_

#include "csl_psc.h"

#ifdef __cplusplus
extern "C" {
#endif


CSL_IDEF_INLINE unsigned int CSL_PSC_getVersionInfo (void)
{

    return hPscRegs->PID;
}



CSL_IDEF_INLINE unsigned char CSL_PSC_getVoltageControl (void)
{

    return CSL_FEXT (hPscRegs->VCNTLID, PSC_VCNTLID_VCNTL);
}
        


CSL_IDEF_INLINE void CSL_PSC_setModuleNextState (
    unsigned int                  moduleNum,
    CSL_PSC_MODSTATE        state
)
{
    CSL_FINS (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_NEXT, state);
    
    return;
}


CSL_IDEF_INLINE CSL_PSC_MODSTATE CSL_PSC_getModuleNextState  (
    unsigned int                  moduleNum
)
{
    return (CSL_PSC_MODSTATE) CSL_FEXT (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_NEXT);
}


CSL_IDEF_INLINE void CSL_PSC_setModuleLocalReset  (
    unsigned int                  moduleNum,
    CSL_PSC_MDLRST          resetState
)
{
    CSL_FINS (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_LRST, resetState);

    return;
}


CSL_IDEF_INLINE CSL_PSC_MDLRST CSL_PSC_getModuleLocalReset  (
    unsigned int                  moduleNum
)
{
    return (CSL_PSC_MDLRST) CSL_FEXT (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_LRST);
}



CSL_IDEF_INLINE void CSL_PSC_enableModuleResetIsolation  (
    unsigned int                  moduleNum
)
{
    CSL_FINST (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_RSTISO, ENABLE);

    return;
}



CSL_IDEF_INLINE void CSL_PSC_disableModuleResetIsolation  (
    unsigned int                  moduleNum
)
{
    CSL_FINST (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_RSTISO, DISABLE);

    return;
}



CSL_IDEF_INLINE unsigned short CSL_PSC_isModuleResetIsolationEnabled  (
    unsigned int                  moduleNum
)
{
    return CSL_FEXT (hPscRegs->MDCTL[moduleNum], PSC_MDCTL_RSTISO);
}



CSL_IDEF_INLINE CSL_PSC_MODSTATE CSL_PSC_getModuleState  (
    unsigned int                  moduleNum
)
{
    return (CSL_PSC_MODSTATE) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_STATE);
}



CSL_IDEF_INLINE CSL_PSC_MDLRST CSL_PSC_getModuleLocalResetStatus  (
    unsigned int                  moduleNum
)
{
    return (CSL_PSC_MDLRST) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_LRST);
}



CSL_IDEF_INLINE unsigned short CSL_PSC_isModuleLocalResetDone  (
    unsigned int                  moduleNum
)
{
    return (unsigned short) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_LRSTDONE);
}



CSL_IDEF_INLINE CSL_PSC_MDRST CSL_PSC_getModuleResetStatus  (
    unsigned int                  moduleNum
)
{
    return (CSL_PSC_MDRST) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_MRST);
}



CSL_IDEF_INLINE unsigned short CSL_PSC_isModuleResetDone  (
    unsigned int                  moduleNum
)
{
    return (unsigned short) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_MRSTDONE);
}



CSL_IDEF_INLINE unsigned short CSL_PSC_isModuleClockOn  (
    unsigned int                  moduleNum
)
{
    return (unsigned short) CSL_FEXT(hPscRegs->MDSTAT[moduleNum], PSC_MDSTAT_MCKOUT);
}



CSL_IDEF_INLINE void CSL_PSC_enablePowerDomain (
    unsigned int                  pwrDmnNum
)
{
    CSL_FINST (hPscRegs->PDCTL[pwrDmnNum], PSC_PDCTL_NEXT, ON);
    
    return;
}



CSL_IDEF_INLINE void CSL_PSC_disablePowerDomain (
    unsigned int                  pwrDmnNum
)
{
    CSL_FINST (hPscRegs->PDCTL[pwrDmnNum], PSC_PDCTL_NEXT, OFF);
    
    return;
}




CSL_IDEF_INLINE CSL_PSC_PDSTATE CSL_PSC_getPowerDomainState  (
    unsigned int                  pwrDmnNum
)
{
    return (CSL_PSC_PDSTATE) CSL_FEXT(hPscRegs->PDSTAT[pwrDmnNum], PSC_PDSTAT_STATE);
}


CSL_IDEF_INLINE void CSL_PSC_startStateTransition (
    unsigned int                  pwrDmnNum
)
{
    hPscRegs->PTCMD =   (1 << pwrDmnNum);
    
    return;
}


CSL_IDEF_INLINE unsigned int CSL_PSC_isStateTransitionDone (
    unsigned int                  pwrDmnNum
)
{
    unsigned int  pdTransStatus;

    pdTransStatus =   CSL_FEXTR (hPscRegs->PTSTAT, pwrDmnNum, pwrDmnNum);

    if (pdTransStatus)
    {
        /* Power domain transition is in progress. Return 0 to indicate not yet done. */            
        return 0;
    }
    else
    {
        /* Power domain transition is done. */            
        return 1;
    }
}

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* CSL_PSCAUX_H_ */

