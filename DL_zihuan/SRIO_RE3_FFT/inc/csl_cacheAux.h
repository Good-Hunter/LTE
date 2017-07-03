

#ifndef _CSL_CACHEAUX_H_
#define _CSL_CACHEAUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_cache.h"


CSL_IDEF_INLINE void CACHE_enableCaching  (unsigned char mar)
{
    CSL_FINS(hCache->MAR[mar], CGEM_MAR0_PC, 1);
}


CSL_IDEF_INLINE void CACHE_disableCaching (unsigned char mar)
{
    CSL_FINS(hCache->MAR[mar], CGEM_MAR0_PC, 0);
}


CSL_IDEF_INLINE void CACHE_getMemRegionInfo (unsigned char mar, unsigned char* pcx, unsigned char* pfx)
{
    unsigned int value = hCache->MAR[mar];

    *pcx = CSL_FEXT (value, CGEM_MAR0_PCX);
    *pfx = CSL_FEXT (value, CGEM_MAR0_PFX);
}


CSL_IDEF_INLINE void CACHE_setMemRegionInfo (unsigned char mar, unsigned char pcx, unsigned char pfx)
{
    CSL_FINS (hCache->MAR[mar], CGEM_MAR0_PCX, pcx);
    CSL_FINS (hCache->MAR[mar], CGEM_MAR0_PFX, pfx);
}


CSL_IDEF_INLINE void CACHE_setL1DSize (CACHE_L1Size newSize)
{
    CSL_FINS (hCache->L1DCFG, CGEM_L1DCFG_L1DMODE, newSize);
}


CSL_IDEF_INLINE CACHE_L1Size CACHE_getL1DSize (void)
{
    return (CACHE_L1Size)CSL_FEXT (hCache->L1DCFG, CGEM_L1DCFG_L1DMODE);
}


CSL_IDEF_INLINE void CACHE_freezeL1D(void)
{
    /* Set the Freeze Mode Enabled bit. */
    CSL_FINS (hCache->L1DCC, CGEM_L1DCC_OPER, 1);
}


CSL_IDEF_INLINE void CACHE_unfreezeL1D(void)
{
    /* Reset the Freeze Mode Enabled bit. */
    CSL_FINS (hCache->L1DCC, CGEM_L1DCC_OPER, 0);
}


CSL_IDEF_INLINE unsigned int CACHE_getPrevL1DMode(void)
{
    return CSL_FEXT (hCache->L1DCC, CGEM_L1DCC_POPER);    
}


CSL_IDEF_INLINE void CACHE_invAllL1dWait (void)
{
    /* Wait for the Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L1DINV, CGEM_L1DINV_I) == 1);
}


CSL_IDEF_INLINE void CACHE_invAllL1d (CACHE_Wait wait)
{
    /* Invalidate the Cache Line. */
    CSL_FINS (hCache->L1DINV, CGEM_L1DINV_I, 1); 
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_invAllL1dWait();
}


CSL_IDEF_INLINE void CACHE_wbAllL1dWait (void)
{
    /* Wait for the Writeback operation to complete. */
    while (CSL_FEXT(hCache->L1DWB, CGEM_L1DWB_C) == 1);
}


CSL_IDEF_INLINE void CACHE_wbAllL1d (CACHE_Wait wait)
{
    /* Writeback the Cache Line. */
    CSL_FINS (hCache->L1DWB, CGEM_L1DWB_C, 1); 
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_wbAllL1dWait();
}


CSL_IDEF_INLINE void CACHE_wbInvAllL1dWait (void)
{
    /* Wait for the Invalidate Writeback operation to complete. */
    while (CSL_FEXT(hCache->L1DWBINV, CGEM_L1DWBINV_C) == 1);
}


CSL_IDEF_INLINE void  CACHE_wbInvAllL1d (CACHE_Wait wait)
{
    /* Invalidate and writeback the cache line. */
    CSL_FINS (hCache->L1DWBINV, CGEM_L1DWBINV_C, 1); 
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_wbInvAllL1dWait();     
}


CSL_IDEF_INLINE void CACHE_invL1dWait (void)
{
    /* Wait for the Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L1DIWC, CGEM_L1DIWC_WC) != 0);
}


CSL_IDEF_INLINE void CACHE_invL1d 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L1DIBAR = CSL_FMK(CGEM_L1DIBAR_ADDR, (unsigned int)blockPtr);
    hCache->L1DIWC  = CSL_FMK(CGEM_L1DIWC_WC,    (unsigned int)((byteCnt+3)>>2));

    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_invL1dWait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_wbL1dWait (void)
{
    /* Wait for the Writeback operation to complete. */
    while (CSL_FEXT(hCache->L1DWWC, CGEM_L1DWWC_WC) != 0);    
}


CSL_IDEF_INLINE void CACHE_wbL1d 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L1DWBAR = CSL_FMK (CGEM_L1DWBAR_ADDR, (unsigned int)blockPtr);
    hCache->L1DWWC  = CSL_FMK (CGEM_L1DWWC_WC,    (unsigned int)((byteCnt+3)>>2));

    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_wbL1dWait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_wbInvL1dWait (void)
{
    /* Wait for the Block Writeback/Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L1DWIWC, CGEM_L1DWIWC_WC) != 0);
}


CSL_IDEF_INLINE void CACHE_wbInvL1d 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L1DWIBAR = CSL_FMK(CGEM_L1DWIBAR_ADDR, (unsigned int)blockPtr);
    hCache->L1DWIWC  = CSL_FMK(CGEM_L1DWIWC_WC,    (unsigned int)((byteCnt+3)>>2));
 
    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_wbInvL1dWait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_setL1PSize (CACHE_L1Size newSize)
{
    CSL_FINS (hCache->L1PCFG, CGEM_L1PCFG_L1PMODE, newSize);
}


CSL_IDEF_INLINE CACHE_L1Size CACHE_getL1PSize (void)
{
    return (CACHE_L1Size)CSL_FEXT (hCache->L1PCFG, CGEM_L1PCFG_L1PMODE);
}


CSL_IDEF_INLINE void CACHE_freezeL1P(void)
{
    /* Set the Freeze Mode Enabled bit. */
    CSL_FINS (hCache->L1PCC, CGEM_L1PCC_OPER, 1);
}


CSL_IDEF_INLINE void CACHE_unfreezeL1P(void)
{
    /* Reset the Freeze Mode Enabled bit. */
    CSL_FINS (hCache->L1PCC, CGEM_L1PCC_OPER, 0);
}


CSL_IDEF_INLINE unsigned int CACHE_getPrevL1PMode(void)
{
    return CSL_FEXT (hCache->L1PCC, CGEM_L1PCC_POPER);    
}


CSL_IDEF_INLINE void CACHE_invL1pWait (void)
{
    /* Wait for the Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L1PIWC, CGEM_L1PIWC_WC) != 0);    
}


CSL_IDEF_INLINE void CACHE_invL1p 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length which is to be invalidated */
    hCache->L1PIBAR = CSL_FMK(CGEM_L1PIBAR_ADDR, (unsigned int)blockPtr);
    hCache->L1PIWC  = CSL_FMK(CGEM_L1PIWC_WC,    (unsigned int)((byteCnt+3)>>2));

    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_invL1pWait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_invAllL1pWait (void)
{
    /* Wait for the Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L1PINV, CGEM_L1PINV_I) == 1);    
}


CSL_IDEF_INLINE void CACHE_invAllL1p (CACHE_Wait wait)
{
    /* Invalidate the L1P Cache. */
    CSL_FINS (hCache->L1PINV, CGEM_L1PINV_I, 1);    

    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_invAllL1pWait();
}


CSL_IDEF_INLINE void CACHE_setL2Size (CACHE_L2Size newSize)
{
    /* Set the new L2 cache size. */
    CSL_FINS (hCache->L2CFG, CGEM_L2CFG_L2MODE, newSize);
}


CSL_IDEF_INLINE CACHE_L2Size CACHE_getL2Size (void)
{    
    return (CACHE_L2Size) CSL_FEXT (hCache->L2CFG, CGEM_L2CFG_L2MODE);
}


CSL_IDEF_INLINE void CACHE_freezeL2 (void)
{
    /* The RL File does not define the L2CC bit so we used the RAW macro to 
     * configure the corresponding bit. */
    CSL_FINSR(hCache->L2CFG, 3, 3, 1);
}


CSL_IDEF_INLINE void CACHE_unfreezeL2 (void)
{
    /* The RL File does not define the L2CC bit so we used the RAW macro to 
     * configure the corresponding bit. */
    CSL_FINSR(hCache->L2CFG, 3, 3, 0);
}


CSL_IDEF_INLINE void CACHE_wbL2Wait (void)
{
    /* Wait for the Writeback operation to complete. */
    while (CSL_FEXT(hCache->L2WWC, CGEM_L2WWC_WC) != 0);        
}


CSL_IDEF_INLINE void CACHE_wbL2 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L2WBAR = CSL_FMK (CGEM_L2WBAR_ADDR, (unsigned int)blockPtr);    
    hCache->L2WWC  = CSL_FMK (CGEM_L2WWC_WC,    (unsigned int)((byteCnt+3)>>2));

    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_wbL2Wait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}

CSL_IDEF_INLINE void CACHE_invL2Wait (void)
{
    /* Wait for the Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L2IWC, CGEM_L2IWC_WC) != 0);        
}


CSL_IDEF_INLINE void CACHE_invL2 
(
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L2IBAR = CSL_FMK (CGEM_L2IBAR_ADDR, (unsigned int)blockPtr);    
    hCache->L2IWC  = CSL_FMK (CGEM_L2IWC_WC,    (unsigned int)((byteCnt+3)>>2));

    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_invL2Wait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_wbInvL2Wait (void)
{
    /* Wait for the Writeback & Invalidate operation to complete. */
    while (CSL_FEXT(hCache->L2WIWC, CGEM_L2WIWC_WC) != 0);        
}


CSL_IDEF_INLINE void CACHE_wbInvL2 (
    void*       blockPtr,
    unsigned int      byteCnt,
    CACHE_Wait  wait
)
{
    /* Setup the block address and length */
    hCache->L2WIBAR = CSL_FMK(CGEM_L2WIBAR_ADDR, (unsigned int)blockPtr);
    hCache->L2WIWC  = CSL_FMK(CGEM_L2WIWC_WC,    (unsigned int)((byteCnt+3)>>2));
 
    /* Determine if we need to wait for the operation to complete. */
    if (wait == CACHE_WAIT)
        CACHE_wbInvL2Wait();
    else if (wait == CACHE_FENCE_WAIT)
        _mfence();
}


CSL_IDEF_INLINE void CACHE_wbAllL2Wait (void)
{
    /* Wait for the writeback operation to complete. */
    while (CSL_FEXT(hCache->L2WB, CGEM_L2WB_C) == 1);    
}


CSL_IDEF_INLINE void CACHE_wbAllL2 (CACHE_Wait wait)
{
    CSL_FINS (hCache->L2WB, CGEM_L2WB_C, 1);
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_wbAllL2Wait();
}


CSL_IDEF_INLINE void CACHE_invAllL2Wait (void)
{
    /* Wait for the invalidate operation to complete. */
    while (CSL_FEXT(hCache->L2INV, CGEM_L2INV_I) == 1);    
}


CSL_IDEF_INLINE void CACHE_invAllL2 (CACHE_Wait wait)
{
    CSL_FINS (hCache->L2INV, CGEM_L2INV_I, 1);
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_invAllL2Wait();    
}


CSL_IDEF_INLINE void CACHE_wbInvAllL2Wait (void)
{
    /* Wait for the writeback-invalidate operation to complete. */
    while (CSL_FEXT(hCache->L2WBINV, CGEM_L2WBINV_C) == 1);    
}


CSL_IDEF_INLINE void CACHE_wbInvAllL2 (CACHE_Wait wait)
{
    CSL_FINS (hCache->L2WBINV, CGEM_L2WBINV_C, 1);
    
    /* Determine if we need to wait for the operation to complete. */
    if (wait)
        CACHE_wbInvAllL2Wait();    
}

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /*CSL_CACHEAUX_H_*/
