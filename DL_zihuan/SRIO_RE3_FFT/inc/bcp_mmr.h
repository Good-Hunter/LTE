

#ifndef _BCP_MMR_H_
#define _BCP_MMR_H_

#include "bcp_lld.h"


typedef struct _Bcp_PeripheralIdParams
{
    /** Minor Version Number (Y) */
    unsigned char             minorNum;
    /** Custom Version Number */
    unsigned char             customNum;
    /** Major Version Number (X) */
    unsigned char             majorNum;
    /** RTL Version Number (R) */
    unsigned char             rtlVersion;
    /** Fixed module ID. */        
    uint16_t            function;
    /** Scheme. */        
    unsigned char             scheme;
} Bcp_PeripheralIdParams;


typedef struct _Bcp_ModuleIdleStatus
{
    unsigned char             tm_idle_status;        
    unsigned char             dio_idle_status;        
    unsigned char             crc_idle_status;        
    unsigned char             enc_idle_status;        
    unsigned char             rm_idle_status;        
    unsigned char             mod_idle_status;        
    unsigned char             int_idle_status;        
    unsigned char             cor_idle_status;        
    unsigned char             ssl_idle_status;        
    unsigned char             rd_idle_status;        
    unsigned char             dnt_idle_status;        
    unsigned char             all_idle_status;        
} Bcp_ModuleIdleStatus;


typedef struct _Bcp_ModuleCfg
{
    unsigned char             tm;        
    unsigned char             dio;        
    unsigned char             crc;        
    unsigned char             enc;        
    unsigned char             rm;        
    unsigned char             mod;        
    unsigned char             intleaver;        
    unsigned char             cor;        
    unsigned char             ssl;        
    unsigned char             rd;        
    unsigned char             dnt;        
} Bcp_ModuleCfg;


typedef struct _Bcp_TmIntCfg
{
    unsigned char             ppb_error;        
    unsigned char             dlg_done_int;        
    unsigned char             eng_error;        
} Bcp_TmIntCfg;


typedef struct _Bcp_TmFlowEntry
{
    /** Input data endian format */
    Bcp_EndianFormat    endian_in;        

    /** Input data format */
    Bcp_DataFormat      format_in;        

    /** Packet type to put in Info Word 0 on Rx */
    unsigned char             pkt_type;

    /** Which DSP to interrupt */
    unsigned char             dsp_int_sel;

    /** Output data endian format */
    Bcp_EndianFormat    endian_out;        

    /** Output data format */
    Bcp_DataFormat      format_out;        

    /** Output QFIFO to use */
    unsigned char             qfifo_out;

    /** PS flags to configure in packet Info word 0 */
    unsigned char             ps_flags;
} Bcp_TmFlowEntry;

      
typedef enum   
{
    /** Idle Mode.
     *  
     *  Data logger does not capture any information and write pointer
     *  is reset to address 0.
     */
    Bcp_DlgCtrlMode_Idle                = 0,
    /** Errors only Mode.
     *  
     *  Data logger stores information only on errored packets. Write counter
     *  is allowed to wrap so writes continue indefinitely.
     */
    Bcp_DlgCtrlMode_ErrorsOnly          = 1,
    /** Start On Error Mode.
     *  
     *  Data logger starts capturing data on next error and captures info
     *  for all following packets until all memory locations have been written.
     */
    Bcp_DlgCtrlMode_StartOnError        = 2,
    /** Stop On Error Mode.
     *  
     *  Data logger stores data from all packets until an error occurs.
     *  When the error occurs it stores that packets data and stops 
     *  collecting data.
     */
    Bcp_DlgCtrlMode_StopOnError         = 3,
    /** Free Run Mode.
     *  
     *  Data logger stores data from all packets. Write counter is allowed
     *  to wrap so writes continue indefinitely.
     */
    Bcp_DlgCtrlMode_FreeRun             = 4,
    /** One Shot Mode.
     *  
     *  Data logger stores data from all packets starting at location 0
     *  and continues until all memory has been written and then stops.
     */
    Bcp_DlgCtrlMode_OneShot             = 5,
    /** Hold Mode.
     *  
     *  Data logger does not store any more information but does not clear
     *  the write pointer.
     */
    Bcp_DlgCtrlMode_Hold                = 7
} Bcp_DlgCtrlMode;

/** @brief
 * Maximum number of Data logger RAM entries in any BCP Submodule
 */        
#define     BCP_MAX_NUM_DLG_RAM_ENTRIES     (256)                 


typedef struct _Bcp_DlgRamEntry
{
    /** Global Header field contents */
    unsigned int            global_hdr_val;        

    /** Software Timestamp value */
    unsigned int            sw_timestamp_val;        

    /** Hardware Timestamp value */
    unsigned char             hw_timestamp_val;        

    /** Engine error field value */
    unsigned char             engine_error_val;        
} Bcp_DlgRamEntry;



static inline signed int Bcp_getEncPolyCoef1Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->POLY_COEF1, ENC_MMR_POLY_COEF1_POLYCOEF1);
}


static inline signed int Bcp_setEncPolyCoef1Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyCoefVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->POLY_COEF1, ENC_MMR_POLY_COEF1_POLYCOEF1, polyCoefVal);

    /* Return success */
    return 0;
}


static inline signed int Bcp_getEncPolyCoef2Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->POLY_COEF2, ENC_MMR_POLY_COEF2_POLYCOEF2);
}


static inline signed int Bcp_setEncPolyCoef2Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyCoefVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->POLY_COEF2, ENC_MMR_POLY_COEF2_POLYCOEF2, polyCoefVal);

    return 0;
}


static inline signed int Bcp_getEncPolyCoef3Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);


    return CSL_FEXT (pEncRegs->POLY_COEF3, ENC_MMR_POLY_COEF3_POLYCOEF3);
}


static inline signed int Bcp_setEncPolyCoef3Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyCoefVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->POLY_COEF3, ENC_MMR_POLY_COEF3_POLYCOEF3, polyCoefVal);

    return 0;
}


static inline signed int Bcp_getEncScrInit0Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRINIT0, ENC_MMR_SCRINIT0_SCRINIT0);
}


static inline signed int Bcp_setEncScrInit0Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                initVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRINIT0, ENC_MMR_SCRINIT0_SCRINIT0, initVal);

    return 0;
}

/**
 * ============================================================================
 *  @n@b Bcp_getEncScrPoly0Reg
 *
 *  @b  brief
 *  @n  This API returns the contents of SCRPOLY0 register. The SCRPOLY0 
 *      register contains the WiMax randomizer polynomial value.
 *
 *  @param[in]    
        pBcpLldObj          BCP LLD instance object.

 *  @return     signed int
 *  @li                     -1  -   Invalid BCP instance handle passed.
 *  @li                     >=0 -   Contents of SCRPOLY0 register.
 *
 *  @pre
 *  @n  @a Bcp_lldOpen () must be called to obtain the register overlay handle for 
 *      BCP instance before calling this API. 
 *
 *  @post
 *  @n  None.
 * ============================================================================
 */
static inline signed int Bcp_getEncScrPoly0Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRPOLY0, ENC_MMR_SCRPOLY0_SCRPOLY0);
}


static inline signed int Bcp_setEncScrPoly0Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRPOLY0, ENC_MMR_SCRPOLY0_SCRPOLY0, polyVal);

    return 0;
}


static inline signed int Bcp_getEncCrc24Init0Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->CRC24_INIT0, ENC_MMR_CRC24_INIT0_CRC24INIT0);
}


static inline signed int Bcp_setEncCrc24Init0Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                initVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->CRC24_INIT0, ENC_MMR_CRC24_INIT0_CRC24INIT0, initVal);

    return 0;
}

/**
 * ============================================================================
 *  @n@b Bcp_getEncCrc24Poly0Reg
 *
 *  @b  brief
 *  @n  This API returns the contents of CRC24_POLY0 register. The CRC24_POLY0 
 *      register contains the CRC-24 polynomial value.
 *
 *  @param[in]    
        pBcpLldObj          BCP LLD instance object.

 *  @return     signed int
 *  @li                     -1  -   Invalid BCP instance handle passed.
 *  @li                     >=0 -   Contents of CRC24_POLY0 register.
 *
 *  @pre
 *  @n  @a Bcp_lldOpen () must be called to obtain the register overlay handle for 
 *      BCP instance before calling this API. 
 *
 *  @post
 *  @n  None.
 * ============================================================================
 */
static inline signed int Bcp_getEncCrc24Poly0Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->CRC24_POLY0, ENC_MMR_CRC24_POLY0_CRC24POLY0);
}


static inline signed int Bcp_setEncCrc24Poly0Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->CRC24_POLY0, ENC_MMR_CRC24_POLY0_CRC24POLY0, polyVal);

    return 0;
}


static inline signed int Bcp_getEncScrInit1Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRINIT1, ENC_MMR_SCRINIT1_SCRINIT1);
}


static inline signed int Bcp_setEncScrInit1Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                initVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRINIT1, ENC_MMR_SCRINIT1_SCRINIT1, initVal);

    return 0;
}


static inline signed int Bcp_getEncScrPoly1Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRPOLY1, ENC_MMR_SCRPOLY1_SCRPOLY1);
}


static inline signed int Bcp_setEncScrPoly1Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRPOLY1, ENC_MMR_SCRPOLY1_SCRPOLY1, polyVal);

    return 0;
}


static inline signed int Bcp_getEncCrc16Init1Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->CRC16_INIT1, ENC_MMR_CRC16_INIT1_CRC16INIT1);
}


static inline signed int Bcp_setEncCrc16Init1Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                initVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->CRC16_INIT1, ENC_MMR_CRC16_INIT1_CRC16INIT1, initVal);

    return 0;
}


static inline signed int Bcp_getEncCrc16Poly1Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->CRC16_POLY1, ENC_MMR_CRC16_POLY1_CRC16POLY1);
}


static inline signed int Bcp_setEncCrc16Poly1Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->CRC16_POLY1, ENC_MMR_CRC16_POLY1_CRC16POLY1, polyVal);

    return 0;
}


static inline signed int Bcp_getEncScrInit2Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRINIT2, ENC_MMR_SCRINIT2_SCRINIT2);
}


static inline signed int Bcp_setEncScrInit2Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                initVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRINIT2, ENC_MMR_SCRINIT2_SCRINIT2, initVal);

    return 0;
}


static inline signed int Bcp_getEncScrPoly2Reg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    return CSL_FEXT (pEncRegs->SCRPOLY2, ENC_MMR_SCRPOLY2_SCRPOLY2);
}


static inline signed int Bcp_setEncScrPoly2Reg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                polyVal
)
{
    CSL_Enc_mmrRegs*        pEncRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pEncRegs    =   (CSL_Enc_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_ENC]);

    CSL_FINS (pEncRegs->SCRPOLY2, ENC_MMR_SCRPOLY2_SCRPOLY2, polyVal);

    return 0;
}


static inline signed int Bcp_getCorReedMullerTableColumn
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                colNum
)
{
    CSL_Cor_mmrRegs*        pCorRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pCorRegs    =   (CSL_Cor_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_COR]);

    return CSL_FEXT (pCorRegs->M[colNum], COR_MMR_M0_MOD_EN);
}


static inline signed int Bcp_setCorReedMullerTableColumn
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                colNum,
    unsigned int                mVal
)
{
    CSL_Cor_mmrRegs*        pCorRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pCorRegs    =   (CSL_Cor_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_COR]);

    CSL_FINS (pCorRegs->M[colNum], COR_MMR_M0_MOD_EN, mVal);

    return 0;
}


static inline signed int Bcp_getCorControlReg
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned char*                pQpskMap00,
    unsigned char*                pQpskMap01,
    unsigned char*                pQpskMap10,
    unsigned char*                pQpskMap11
)
{
    CSL_Cor_mmrRegs*        pCorRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pQpskMap00 || !pQpskMap01 || !pQpskMap10 || !pQpskMap11)
        return -1;

    pCorRegs    =   (CSL_Cor_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_COR]);

    *pQpskMap00 =   CSL_FEXT (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_00);
    *pQpskMap01 =   CSL_FEXT (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_01);
    *pQpskMap10 =   CSL_FEXT (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_10);
    *pQpskMap11 =   CSL_FEXT (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_11);

    return 0;
}


static inline signed int Bcp_setCorControlReg
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned char                 qpskMap00,
    unsigned char                 qpskMap01,
    unsigned char                 qpskMap10,
    unsigned char                 qpskMap11
)
{
    CSL_Cor_mmrRegs*        pCorRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pCorRegs    =   (CSL_Cor_mmrRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_COR]);

    CSL_FINS (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_00, qpskMap00);
    CSL_FINS (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_01, qpskMap01);
    CSL_FINS (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_10, qpskMap10);
    CSL_FINS (pCorRegs->CONTROL, COR_MMR_CONTROL_QPSK_MAP_11, qpskMap11);

    return 0;
}


static inline signed int Bcp_getPIDReg 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_PeripheralIdParams* pPIDParams
)
{
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pPIDParams)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    pPIDParams->minorNum    =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_MINOR);
    pPIDParams->customNum   =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_CUSTOM);
    pPIDParams->majorNum    =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_MAJOR);
    pPIDParams->rtlVersion  =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_RTL);
    pPIDParams->function    =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_FUNC);
    pPIDParams->scheme      =   CSL_FEXT (pTmRegs->BCP_PID, BCP_TM_BCP_PID_BCP_PID_SCHEME);

    return 0;
}


static inline signed int Bcp_doSoftwareReset 
(
    Bcp_LldObj*                pBcpLldObj
)
{
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    CSL_FINS (pTmRegs->BCP_SOFT_RESET, BCP_TM_BCP_SOFT_RESET_SOFT_RESET, 1);          

    return 0;
}


static inline signed int Bcp_clearTMHalt 
(
    Bcp_LldObj*                 pBcpLldObj,
    Bcp_QueueId                 txQNum
)
{
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    CSL_FINSR (pTmRegs->TM_HALT_CTRL, txQNum, txQNum, 1);          

    return 0;
}


static inline signed int Bcp_getTMHaltStatus 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    return CSL_FEXT (pTmRegs->TM_HALT_STATUS, BCP_TM_TM_HALT_STATUS_HALT_STATUS);
}


static inline signed int Bcp_getEmulationClockStopStatus 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleIdleStatus*   pIdleStatus
)
{
    unsigned int                word;
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pIdleStatus || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    word    =   pTmRegs->BCP_EMU_CLKSTOP_STATUS;

    pIdleStatus->tm_idle_status     =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_TM_ENG_IDLE);
    pIdleStatus->dio_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_DIO_ENG_IDLE);
    pIdleStatus->crc_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_CRC_ENG_IDLE);
    pIdleStatus->enc_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_ENC_ENG_IDLE);
    pIdleStatus->rm_idle_status     =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_RM_ENG_IDLE);
    pIdleStatus->mod_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_MOD_ENG_IDLE);
    pIdleStatus->int_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_INT_ENG_IDLE);
    pIdleStatus->cor_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_COR_ENG_IDLE);
    pIdleStatus->ssl_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_SSL_ENG_IDLE);
    pIdleStatus->rd_idle_status     =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_RD_ENG_IDLE);
    pIdleStatus->dnt_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_DNT_ENG_IDLE);
    pIdleStatus->all_idle_status    =   CSL_FEXT (word, BCP_TM_BCP_EMU_CLKSTOP_STATUS_ALL_ENG_IDLE);

    return 0;
}


static inline signed int Bcp_getEmulationClockStopControlReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char*                        pEmuRtSel,
    unsigned char*                        pEmuFreeRun
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pEmuRtSel || !pEmuFreeRun)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    *pEmuRtSel  =   CSL_FEXT (pTmRegs->BCP_EMU_CLKSTOP_CTRL, BCP_TM_BCP_EMU_CLKSTOP_CTRL_BCP_EMU_RT_SEL);
    *pEmuFreeRun=   CSL_FEXT (pTmRegs->BCP_EMU_CLKSTOP_CTRL, BCP_TM_BCP_EMU_CLKSTOP_CTRL_BCP_EMU_FREERUN);


    return 0;
}


static inline signed int Bcp_setEmulationClockStopControlReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         emuRtSel,
    unsigned char                         emuFreeRun
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->BCP_EMU_CLKSTOP_CTRL   =   CSL_FMK (BCP_TM_BCP_EMU_CLKSTOP_CTRL_BCP_EMU_RT_SEL, emuRtSel) |
                                        CSL_FMK (BCP_TM_BCP_EMU_CLKSTOP_CTRL_BCP_EMU_FREERUN, emuFreeRun);

    return 0;
}



static inline signed int Bcp_getDlgSwTimeStampReg 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXT (pTmRegs->BCP_DLG_SW_TIMESTAMP, BCP_TM_BCP_DLG_SW_TIMESTAMP_BCP_DLG_SW_TIMESTAMP);
}


static inline signed int Bcp_setDlgSwTimeStampReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned int                        tsVal
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->BCP_DLG_SW_TIMESTAMP, BCP_TM_BCP_DLG_SW_TIMESTAMP_BCP_DLG_SW_TIMESTAMP, tsVal);

    return 0;
}


static inline signed int Bcp_getDlgHwTimeStampReg 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXT (pTmRegs->BCP_DLG_HW_TIMESTAMP, BCP_TM_BCP_DLG_HW_TIMESTAMP_BCP_DLG_HW_TIMESTAMP);
}


static inline signed int Bcp_resetDlgHwTimeStampReg 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->BCP_DLG_HW_TIMESTAMP_CTRL, BCP_TM_BCP_DLG_HW_TIMESTAMP_CTRL_BCP_DLG_HW_TIMESTAMP_RUN, 1);

    return 0;
}


static inline signed int Bcp_setDlgHold 
(
    Bcp_LldObj*                     pBcpLldObj,
    Bcp_ModuleId                    moduleId
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINSR (pTmRegs->BCP_DLG_HOLD_RST, 
               CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_HOLD_SHIFT+moduleId, 
               CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_HOLD_SHIFT+moduleId, 1);

    return 0;
}


static inline signed int Bcp_getDlgHoldStatus 
(
    Bcp_LldObj*                     pBcpLldObj,
    Bcp_ModuleId                    moduleId
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXTR (pTmRegs->BCP_DLG_HOLD_RST, 
                      CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_HOLD_SHIFT+moduleId, 
                      CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_HOLD_SHIFT+moduleId);
}


static inline signed int Bcp_setDlgIdle 
(
    Bcp_LldObj*                     pBcpLldObj,
    Bcp_ModuleId                    moduleId
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINSR (pTmRegs->BCP_DLG_HOLD_RST, 
               CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_RST_SHIFT+moduleId, 
               CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_RST_SHIFT+moduleId, 1);

    return 0;
}


static inline signed int Bcp_getDlgIdleStatus 
(
    Bcp_LldObj*                     pBcpLldObj,
    Bcp_ModuleId                    moduleId
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXTR (pTmRegs->BCP_DLG_HOLD_RST, 
                      CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_RST_SHIFT+moduleId, 
                      CSL_BCP_TM_BCP_DLG_HOLD_RST_TM_DLG_RST_SHIFT+moduleId);
}


static inline signed int Bcp_setDlgHoldResetReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned int                        holdResetVal
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->BCP_DLG_HOLD_RST    =   holdResetVal;

    return 0;
}


static inline signed int Bcp_getDlgHoldResetReg 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return pTmRegs->BCP_DLG_HOLD_RST;
}


static inline signed int Bcp_setCdmaHpSrcId 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         srcId
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_CDMAHP_SRC_ID, srcId);

    return 0;
}


static inline signed int Bcp_getCdmaHpSrcId 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXT (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_CDMAHP_SRC_ID);
}


static inline signed int Bcp_enableCdmaHp 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_CDMAHP_DISABLE, 0);

    return 0;
}


static inline signed int Bcp_disableCdmaHp 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_CDMAHP_DISABLE, 1);

    return 0;
}


static inline signed int Bcp_isCdmaHpEnabled 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    unsigned char                         bIsCdmaHpDisabled;        
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    bIsCdmaHpDisabled = CSL_FEXT (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_CDMAHP_DISABLE);

    return (bIsCdmaHpDisabled == 0 ? 1 : 0);
}


static inline signed int Bcp_enableRxCdmaHpWriteArbPrio 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_RX_CDMAHP_WR_ARB_HPRIORITY, 1);

    return 0;
}


static inline signed int Bcp_disableRxCdmaHpWriteArbPrio 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_RX_CDMAHP_WR_ARB_HPRIORITY, 0);

    return 0;
}


static inline signed int Bcp_isRxCdmaHpWriteArbPrioEnabled 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXT (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_RX_CDMAHP_WR_ARB_HPRIORITY);
}


static inline signed int Bcp_enableTxCdmaHpReadArbPrio 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_TX_CDMAHP_RD_ARB_HPRIORITY, 1);

    return 0;
}


static inline signed int Bcp_disableTxCdmaHpReadArbPrio 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    CSL_FINS (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_TX_CDMAHP_RD_ARB_HPRIORITY, 0);

    return 0;
}


static inline signed int Bcp_isTxCdmaHpReadArbPrioEnabled 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    return CSL_FEXT (pTmRegs->TM_CONTROL, BCP_TM_TM_CONTROL_TX_CDMAHP_RD_ARB_HPRIORITY);
}


static inline signed int Bcp_setTMControlReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         srcId,
    unsigned char                         bCdmaDisable,
    unsigned char                         bRxWrArbPrioEnable,
    unsigned char                         bTxRdArbPrioEnable
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_CONTROL     =   CSL_FMK (BCP_TM_TM_CONTROL_CDMAHP_SRC_ID, srcId) |
                                CSL_FMK (BCP_TM_TM_CONTROL_CDMAHP_DISABLE, bCdmaDisable) |
                                CSL_FMK (BCP_TM_TM_CONTROL_RX_CDMAHP_WR_ARB_HPRIORITY, bRxWrArbPrioEnable) |
                                CSL_FMK (BCP_TM_TM_CONTROL_TX_CDMAHP_RD_ARB_HPRIORITY, bTxRdArbPrioEnable);

    return 0;
}


static inline signed int Bcp_getTMControlReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char*                        pSrcId,
    unsigned char*                        pIsCdmaDisabled,
    unsigned char*                        pIsRxWrArbPrioEnabled,
    unsigned char*                        pIsTxRdArbPrioEnabled
)
{
    unsigned int                        tmpWord;        
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pSrcId || !pIsCdmaDisabled || !pIsRxWrArbPrioEnabled || !pIsTxRdArbPrioEnabled)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_CONTROL;
    
    *pSrcId                 =   CSL_FEXT (tmpWord, BCP_TM_TM_CONTROL_CDMAHP_SRC_ID);
    *pIsCdmaDisabled        =   CSL_FEXT (tmpWord, BCP_TM_TM_CONTROL_CDMAHP_DISABLE);
    *pIsRxWrArbPrioEnabled  =   CSL_FEXT (tmpWord, BCP_TM_TM_CONTROL_RX_CDMAHP_WR_ARB_HPRIORITY);
    *pIsTxRdArbPrioEnabled  =   CSL_FEXT (tmpWord, BCP_TM_TM_CONTROL_TX_CDMAHP_RD_ARB_HPRIORITY);

    return 0;
}


static inline signed int Bcp_setTxCdmaHpReadPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         prioVal[8]
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_TX_CDMAHP_READ_PRIORITY  =    
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_0, prioVal[0]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_1, prioVal[1]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_2, prioVal[2]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_3, prioVal[3]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_4, prioVal[4]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_5, prioVal[5]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_6, prioVal[6]) |
                    CSL_FMK (BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_7, prioVal[7]);

    return 0;
}


static inline signed int Bcp_getTxCdmaHpReadPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         prioVal[8]
)
{
    unsigned int                        tmpWord;
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_TX_CDMAHP_READ_PRIORITY;

    prioVal[0]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_0);
    prioVal[1]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_1);
    prioVal[2]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_2);
    prioVal[3]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_3);
    prioVal[4]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_4);
    prioVal[5]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_5);
    prioVal[6]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_6);
    prioVal[7]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_CDMAHP_READ_PRIORITY_TX_CDMAHP_RD_ARB_PRIORITY_7);

    return 0;
}


static inline signed int Bcp_setTxQfifoReadDestSelReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         destSel[8],
    unsigned char                         prioVal[8]
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_TX_QFIFO_RD_DEST_SEL  =    
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_0, destSel[0]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_1, destSel[1]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_2, destSel[2]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_3, destSel[3]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_4, destSel[4]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_5, destSel[5]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_6, destSel[6]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_7, destSel[7]) |

                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_0, prioVal[0]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_1, prioVal[1]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_2, prioVal[2]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_3, prioVal[3]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_4, prioVal[4]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_5, prioVal[5]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_6, prioVal[6]) |
                    CSL_FMK (BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_7, prioVal[7]);

    return 0;
}


static inline signed int Bcp_getTxQfifoReadDestSelReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         destSel[8],
    unsigned char                         prioVal[8]
)
{
    unsigned int                        tmpWord;
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_TX_QFIFO_RD_DEST_SEL;

    destSel[0]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_0);
    destSel[1]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_1);
    destSel[2]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_2);
    destSel[3]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_3);
    destSel[4]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_4);
    destSel[5]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_5);
    destSel[6]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_6);
    destSel[7]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_DEST_SEL_7);

    prioVal[0]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_0);
    prioVal[1]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_1);
    prioVal[2]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_2);
    prioVal[3]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_3);
    prioVal[4]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_4);
    prioVal[5]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_5);
    prioVal[6]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_6);
    prioVal[7]  =   CSL_FEXT (tmpWord, BCP_TM_TM_TX_QFIFO_RD_DEST_SEL_TX_QFIFO_RD_ARB_PRIORITY_7);

    return 0;
}


static inline signed int Bcp_setRxQFifoWriteArbPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         prioVal[4][8]
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_RX_QFIFO_WR_ARB_PRIORITY_3_0  =    
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_0, prioVal[0][0]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_1, prioVal[1][0]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_2, prioVal[2][0]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_3, prioVal[3][0]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_0, prioVal[0][1]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_1, prioVal[1][1]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_2, prioVal[2][1]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_3, prioVal[3][1]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_0, prioVal[0][2]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_1, prioVal[1][2]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_2, prioVal[2][2]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_3, prioVal[3][2]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_0, prioVal[0][3]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_1, prioVal[1][3]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_2, prioVal[2][3]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_3, prioVal[3][3]);

    pTmRegs->TM_RX_QFIFO_WR_ARB_PRIORITY_7_4  =    
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_0, prioVal[0][4]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_1, prioVal[1][4]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_2, prioVal[2][4]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_3, prioVal[3][4]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_0, prioVal[0][5]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_1, prioVal[1][5]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_2, prioVal[2][5]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_3, prioVal[3][5]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_0, prioVal[0][6]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_1, prioVal[1][6]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_2, prioVal[2][6]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_3, prioVal[3][6]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_0, prioVal[0][7]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_1, prioVal[1][7]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_2, prioVal[2][7]) |
                    CSL_FMK (BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_3, prioVal[3][7]);

    return 0;
}


static inline signed int Bcp_getRxQFifoWriteArbPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         prioVal[4][8]
)
{
    unsigned int                        tmpWord [2];        
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs         =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord [0]     =   pTmRegs->TM_RX_QFIFO_WR_ARB_PRIORITY_3_0;
    tmpWord [1]     =   pTmRegs->TM_RX_QFIFO_WR_ARB_PRIORITY_7_4;

    prioVal[0][0]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_0);
    prioVal[1][0]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_1);
    prioVal[2][0]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_2);
    prioVal[3][0]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_0_WR_ARB_PRIORITY_3);
    prioVal[0][1]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_0);
    prioVal[1][1]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_1);
    prioVal[2][1]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_2);
    prioVal[3][1]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_1_WR_ARB_PRIORITY_3);
    prioVal[0][2]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_0);
    prioVal[1][2]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_1);
    prioVal[2][2]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_2);
    prioVal[3][2]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_2_WR_ARB_PRIORITY_3);
    prioVal[0][3]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_0);
    prioVal[1][3]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_1);
    prioVal[2][3]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_2);
    prioVal[3][3]   =   CSL_FEXT (tmpWord [0], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_3_0_TX_QFIFO_3_WR_ARB_PRIORITY_3);

    prioVal[0][4]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_0);
    prioVal[1][4]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_1);
    prioVal[2][4]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_2);
    prioVal[3][4]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_4_WR_ARB_PRIORITY_3);
    prioVal[0][5]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_0);
    prioVal[1][5]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_1);
    prioVal[2][5]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_2);
    prioVal[3][5]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_5_WR_ARB_PRIORITY_3);
    prioVal[0][6]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_0);
    prioVal[1][6]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_1);
    prioVal[2][6]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_2);
    prioVal[3][6]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_6_WR_ARB_PRIORITY_3);
    prioVal[0][7]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_0);
    prioVal[1][7]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_1);
    prioVal[2][7]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_2);
    prioVal[3][7]   =   CSL_FEXT (tmpWord [1], BCP_TM_TM_RX_QFIFO_WR_ARB_PRIORITY_7_4_TX_QFIFO_7_WR_ARB_PRIORITY_3);

    return 0;
}


static inline signed int Bcp_setRxCdmaWriteArbPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         prioVal[8]
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_RX_CDMAHP_WR_ARB_PRIORITY  =    
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_0, prioVal[0]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_1, prioVal[1]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_2, prioVal[2]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_3, prioVal[3]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_4, prioVal[4]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_5, prioVal[5]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_6, prioVal[6]) |
                    CSL_FMK (BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_7, prioVal[7]);

    return 0;
}


static inline signed int Bcp_getRxCdmaWriteArbPrioReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         destSel[8],
    unsigned char                         prioVal[8]
)
{
    unsigned int                        tmpWord;
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_RX_CDMAHP_WR_ARB_PRIORITY;

    prioVal[0]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_0);
    prioVal[1]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_1);
    prioVal[2]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_2);
    prioVal[3]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_3);
    prioVal[4]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_4);
    prioVal[5]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_5);
    prioVal[6]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_6);
    prioVal[7]  =   CSL_FEXT (tmpWord, BCP_TM_TM_RX_CDMAHP_WR_ARBPRIORITY_RX_CDMAHP_WR_ARB_PRIORITY_7);

    return 0;
}


static inline signed int Bcp_setHaltOnErrorReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         index,
    Bcp_ModuleCfg*                  pHaltCfg,
    unsigned char                         bForceHalt
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0]|| !pHaltCfg)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->TM_SPECIAL_INTERRUPT_REGISTERS[index].BCP_HALT_ON_ERROR  =    
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_TM_HALT_ON_ERROR, pHaltCfg->tm) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_DIO_HALT_ON_ERROR, pHaltCfg->dio) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_CRC_HALT_ON_ERROR, pHaltCfg->crc) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_ENC_HALT_ON_ERROR, pHaltCfg->enc) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_RM_HALT_ON_ERROR, pHaltCfg->rm) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_MOD_HALT_ON_ERROR, pHaltCfg->mod) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_INT_HALT_ON_ERROR, pHaltCfg->intleaver) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_COR_HALT_ON_ERROR, pHaltCfg->cor) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_SSL_HALT_ON_ERROR, pHaltCfg->ssl) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_RD_HALT_ON_ERROR, pHaltCfg->rd) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_DNT_HALT_ON_ERROR, pHaltCfg->dnt) |
                    CSL_FMK (BCP_TM_BCP_HALT_ON_ERROR_FRC_HALT_ON_ERROR, bForceHalt);

    return 0;
}


static inline signed int Bcp_getHaltOnErrorReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         index,
    Bcp_ModuleCfg*                  pHaltCfg,
    unsigned char*                        pForceHaltEnabled
)
{
    unsigned int                        tmpWord;
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pHaltCfg || !pForceHaltEnabled)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_SPECIAL_INTERRUPT_REGISTERS[index].BCP_HALT_ON_ERROR;

    pHaltCfg->tm        =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_TM_HALT_ON_ERROR);
    pHaltCfg->dio       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_DIO_HALT_ON_ERROR);
    pHaltCfg->crc       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_CRC_HALT_ON_ERROR);
    pHaltCfg->enc       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_ENC_HALT_ON_ERROR);
    pHaltCfg->rm        =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_RM_HALT_ON_ERROR);
    pHaltCfg->mod       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_MOD_HALT_ON_ERROR);
    pHaltCfg->intleaver =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_INT_HALT_ON_ERROR);
    pHaltCfg->cor       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_COR_HALT_ON_ERROR);
    pHaltCfg->ssl       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_SSL_HALT_ON_ERROR);
    pHaltCfg->rd        =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_RD_HALT_ON_ERROR);
    pHaltCfg->dnt       =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_DNT_HALT_ON_ERROR);
    *pForceHaltEnabled  =   CSL_FEXT (tmpWord, BCP_TM_BCP_HALT_ON_ERROR_FRC_HALT_ON_ERROR);

    return 0;
}



static inline signed int Bcp_getInterruptStatusReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         index,
    Bcp_ModuleCfg*                  pIintStatus
)
{
    unsigned int                    tmpWord;
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pIintStatus)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->TM_SPECIAL_INTERRUPT_REGISTERS[index].BCP_INTERRUPT_STATUS;

    pIintStatus->tm          =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_TM_INTR_STATUS);
    pIintStatus->dio         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_DIO_INTR_STATUS);
    pIintStatus->crc         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_CRC_INTR_STATUS);
    pIintStatus->enc         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_ENC_INTR_STATUS);
    pIintStatus->rm          =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_RM_INTR_STATUS);
    pIintStatus->mod         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_MOD_INTR_STATUS);
    pIintStatus->intleaver   =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_INT_INTR_STATUS);
    pIintStatus->cor         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_COR_INTR_STATUS);
    pIintStatus->ssl         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_SSL_INTR_STATUS);
    pIintStatus->rd          =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_RD_INTR_STATUS);
    pIintStatus->dnt         =   CSL_FEXT (tmpWord, BCP_TM_BCP_INTERRUPT_STATUS_DNT_INTR_STATUS);

    return 0;
}


static inline signed int Bcp_setEoiReg 
(
    Bcp_LldObj*                     pBcpLldObj,
    unsigned char                         eoiVal
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    CSL_FINS (pTmRegs->BCP_EOI, BCP_TM_BCP_EOI_BCP_EOI,eoiVal);

    return 0;
}


static inline signed int Bcp_getEoiReg 
(
    Bcp_LldObj*                     pBcpLldObj
)
{
    CSL_Bcp_tmRegs*                 pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);

    return CSL_FEXT (pTmRegs->BCP_EOI, BCP_TM_BCP_EOI_BCP_EOI);
}


static inline signed int Bcp_getTmFlowEntry 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned char                 flow_index,
    Bcp_TmFlowEntry*        pFlowEntry
)
{
    unsigned int                tmpWord;
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pFlowEntry)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
    tmpWord =   pTmRegs->FLOW_ID_TABLE [flow_index];

    pFlowEntry->endian_in   =   (Bcp_EndianFormat) CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_ENDIAN_IN);
    pFlowEntry->format_in   =   (Bcp_DataFormat) CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_FORMAT_IN);
    pFlowEntry->pkt_type    =   CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_PKT_TYPE);
    pFlowEntry->dsp_int_sel =   CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_DSP_INT_SEL);
    pFlowEntry->endian_out  =   (Bcp_EndianFormat) CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_ENDIAN_OUT);
    pFlowEntry->format_out  =   (Bcp_DataFormat) CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_FORMAT_OUT);
    pFlowEntry->qfifo_out   =   CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_QFIFO_OUT);
    pFlowEntry->ps_flags    =   CSL_FEXT (tmpWord, BCP_TM_FLOW_ID_TABLE_PS_FLAGS);

    return 0;
}


static inline signed int Bcp_setTmFlowEntry 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned char                 flow_index,
    Bcp_TmFlowEntry*        pFlowEntry
)
{
    CSL_Bcp_tmRegs*         pTmRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pFlowEntry || flow_index > BCP_MAX_NUM_TM_FLOWS)
        return -1;

    pTmRegs =   (CSL_Bcp_tmRegs *) (pBcpLldObj->modCfgRegs[Bcp_ModuleId_TM]);
        
    pTmRegs->FLOW_ID_TABLE [flow_index]  =   
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_ENDIAN_IN, pFlowEntry->endian_in ) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_FORMAT_IN, pFlowEntry->format_in) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_PKT_TYPE, pFlowEntry->pkt_type) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_DSP_INT_SEL, pFlowEntry->dsp_int_sel) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_ENDIAN_OUT, pFlowEntry->endian_out) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_FORMAT_OUT, pFlowEntry->format_out) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_QFIFO_OUT, pFlowEntry->qfifo_out) |
                CSL_FMK (BCP_TM_FLOW_ID_TABLE_PS_FLAGS, pFlowEntry->ps_flags);

    return 0;
}


static inline signed int Bcp_getDioVbusMPriorityReg 
(
    Bcp_LldObj*             pBcpLldObj
)
{
    CSL_Dio_mmrRegs*        pDioRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pDioRegs    =   (CSL_Dio_mmrRegs *) (pBcpLldObj->modDlgRegs[Bcp_ModuleId_DIO]);

    return CSL_FEXT (pDioRegs->VBUSM_PRIORITY, DIO_MMR_VBUSM_PRIORITY);
}


static inline signed int Bcp_setDioVbusMPriorityReg 
(
    Bcp_LldObj*             pBcpLldObj,
    unsigned int                prioVal
)
{
    CSL_Dio_mmrRegs*        pDioRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pDioRegs    =   (CSL_Dio_mmrRegs *) (pBcpLldObj->modDlgRegs[Bcp_ModuleId_DIO]);

    CSL_FINS (pDioRegs->VBUSM_PRIORITY, DIO_MMR_VBUSM_PRIORITY, prioVal);

    return 0;
}


static inline signed int Bcp_getModuleIntRawStatus 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char*                pEngErrStatus,
    unsigned char*                pDlgDoneStatus,
    unsigned char*                pPpbErrStatus
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pEngErrStatus || !pDlgDoneStatus || !pPpbErrStatus)
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);
    regVal      =   pmodIntRegs->INTR_IRS;

    *pEngErrStatus  =   CSL_FEXT (regVal, MMR_INTR_IRS_X_ENG_ERR_RSTAT);
    *pDlgDoneStatus =   CSL_FEXT (regVal, MMR_INTR_IRS_X_DLG_DONE_RSTAT);
    *pPpbErrStatus  =   CSL_FEXT (regVal, MMR_INTR_IRS_X_PPB_ERR_RSTAT);

    return 0;
}


static inline signed int Bcp_setModuleIntRawStatus
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char                 engErrStatus,
    unsigned char                 dlgDoneStatus,
    unsigned char                 ppbErrStatus
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);

    pmodIntRegs->INTR_IRS_SET   =   CSL_FMK (MMR_INTR_IRS_SET_X_ENG_ERR_RSTAT_SET, engErrStatus) |
                                    CSL_FMK (MMR_INTR_IRS_SET_X_DLG_DONE_RSTAT_SET, dlgDoneStatus) |
                                    CSL_FMK (MMR_INTR_IRS_SET_X_PPB_ERR_RSTAT_SET, ppbErrStatus);
    return 0;
}


static inline signed int Bcp_clearModuleIntRawStatus
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char                 engErrClrMask,
    unsigned char                 dlgDoneClrMask,
    unsigned char                 ppbErrClrMask
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);

    pmodIntRegs->INTR_IRS_CLR   =   CSL_FMK (MMR_INTR_IRS_CLR_X_ENG_ERR_RSTAT_CLR, engErrClrMask) | 
                                    CSL_FMK (MMR_INTR_IRS_CLR_X_DLG_DONE_RSTAT_CLR, dlgDoneClrMask) |
                                    CSL_FMK (MMR_INTR_IRS_CLR_X_PPB_ERR_RSTAT_CLR, ppbErrClrMask);

    return 0;
}


static inline signed int Bcp_getModuleIntEnable
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char*                pEngErrEnable,
    unsigned char*                pDlgDoneEnable,
    unsigned char*                pPpbErrEnable         
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pEngErrEnable || !pDlgDoneEnable || !pPpbErrEnable)
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);
    regVal      =   pmodIntRegs->INTR_EN;

    *pEngErrEnable  =   CSL_FEXT (regVal, MMR_INTR_EN_X_ENG_ERR_EN);
    *pDlgDoneEnable =   CSL_FEXT (regVal, MMR_INTR_EN_X_DLG_DONE_EN);
    *pPpbErrEnable  =   CSL_FEXT (regVal, MMR_INTR_EN_X_PPB_ERR_EN);

    return 0;
}


static inline signed int Bcp_setModuleIntEnable
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char                 engErrEnable,
    unsigned char                 dlgDoneEnable,
    unsigned char                 ppbErrEnable
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);

    pmodIntRegs->INTR_EN_SET    =   CSL_FMK (MMR_INTR_EN_SET_X_ENG_ERR_EN_SET, engErrEnable) |
                                    CSL_FMK (MMR_INTR_EN_SET_X_DLG_DONE_EN_SET, dlgDoneEnable) |
                                    CSL_FMK (MMR_INTR_EN_SET_X_PPB_ERR_EN_SET, ppbErrEnable);

    return 0;
}


static inline signed int Bcp_clearModuleIntEnable
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char                 engErrClrMask,
    unsigned char                 dlgDoneClrMask,
    unsigned char                 ppbErrClrMask
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);

    pmodIntRegs->INTR_EN_CLR    =   CSL_FMK (MMR_INTR_EN_CLR_X_ENG_ERR_EN_CLR, engErrClrMask) |
                                    CSL_FMK (MMR_INTR_EN_CLR_X_DLG_DONE_EN_CLR, dlgDoneClrMask) |
                                    CSL_FMK (MMR_INTR_EN_CLR_X_PPB_ERR_EN_CLR, ppbErrClrMask);

    return 0;
}


static inline signed int Bcp_getModuleIntEnableStatus
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned int                index,
    unsigned char*                pEngErrEnable,
    unsigned char*                pDlgDoneEnable,
    unsigned char*                pPpbErrEnable         
)
{
    CSL_Bcp_IntRegs*        pmodIntRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pEngErrEnable || !pDlgDoneEnable || !pPpbErrEnable)
        return -1;

    pmodIntRegs =   (CSL_Bcp_IntRegs *) (pBcpLldObj->modIntRegs[moduleId] + sizeof (CSL_Bcp_IntRegs) * index);
    regVal      =   pmodIntRegs->INTR_EN_STS;

    *pEngErrEnable  =   CSL_FEXT (regVal, MMR_INTR_EN_STS_X_ENG_ERR);
    *pDlgDoneEnable =   CSL_FEXT (regVal, MMR_INTR_EN_STS_X_DLG_DONE_INT);
    *pPpbErrEnable  =   CSL_FEXT (regVal, MMR_INTR_EN_STS_X_PPB_ERR);

    return 0;
}


static inline signed int Bcp_getModuleDlgControlReg 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    Bcp_DlgCtrlMode*        pDlgCtlVal,
    uint16_t*               pDlgErrMaskVal
)
{
    CSL_Bcp_DataLoggerRegs* pModDlgRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pDlgCtlVal || !pDlgErrMaskVal)
        return -1;

    pModDlgRegs =   (CSL_Bcp_DataLoggerRegs *) (pBcpLldObj->modDlgRegs[moduleId]);
    regVal      =   pModDlgRegs->DLG_CONTROL;

    *pDlgCtlVal     =   (Bcp_DlgCtrlMode) CSL_FEXT (regVal, MMR_DATA_LOG_CTL_DLG_CTL);
    *pDlgErrMaskVal =   CSL_FEXT (regVal, MMR_DATA_LOG_CTL_ERR_MASK);

    return 0;
}


static inline signed int Bcp_setModuleDlgControlReg 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    Bcp_DlgCtrlMode         dlgCtlVal,
    uint16_t                dlgErrMaskVal
)
{
    CSL_Bcp_DataLoggerRegs* pModDlgRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pModDlgRegs =   (CSL_Bcp_DataLoggerRegs *) (pBcpLldObj->modDlgRegs[moduleId]);

    pModDlgRegs->DLG_CONTROL    =   CSL_FMK (MMR_DATA_LOG_CTL_DLG_CTL, dlgCtlVal) |
                                    CSL_FMK (MMR_DATA_LOG_CTL_ERR_MASK, dlgErrMaskVal);

    return 0;
}


static inline signed int Bcp_getModuleDlgStatusReg 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned char*                pDlgRunningStatus,
    unsigned char*                pDlgMemErrorStatus,
    unsigned char*                pDlgWrWrapCount,
    unsigned char*                pDlgWrPtr
)
{
    CSL_Bcp_DataLoggerRegs* pModDlgRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pDlgRunningStatus || !pDlgMemErrorStatus || !pDlgWrWrapCount || !pDlgWrPtr)
        return -1;

    pModDlgRegs =   (CSL_Bcp_DataLoggerRegs *) (pBcpLldObj->modDlgRegs[moduleId]);
    regVal      =   pModDlgRegs->DLG_STATUS;

    *pDlgRunningStatus  =   CSL_FEXT (regVal, MMR_DATA_LOG_STS_RUNNING);
    *pDlgMemErrorStatus =   CSL_FEXT (regVal, MMR_DATA_LOG_STS_MEM_HAS_ERR);
    *pDlgWrWrapCount    =   CSL_FEXT (regVal, MMR_DATA_LOG_STS_WR_WRAP);
    *pDlgWrPtr          =   CSL_FEXT (regVal, MMR_DATA_LOG_STS_WR_PTR);

    return 0;
}


static inline signed int Bcp_getModuleDlgGlobalHeaderReg 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId
)
{
    CSL_Bcp_DataLoggerRegs* pModDlgRegs;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0])
        return -1;

    pModDlgRegs =   (CSL_Bcp_DataLoggerRegs *) (pBcpLldObj->modDlgRegs[moduleId]);

    return CSL_FEXT (pModDlgRegs->DLG_GLOBAL_HDR, MMR_GLBL_HDR_GLBL_HDR);
}


static inline signed int Bcp_getModuleDlgRAMEntry 
(
    Bcp_LldObj*             pBcpLldObj,
    Bcp_ModuleId            moduleId,
    unsigned char                 entry_index,
    Bcp_DlgRamEntry*        pRamEntry
)
{
    CSL_Bcp_DataLoggerRAM*  pDlgRamRegs;
    unsigned int                regVal;

    if (!pBcpLldObj || !pBcpLldObj->modCfgRegs[0] || !pRamEntry || entry_index > BCP_MAX_NUM_DLG_RAM_ENTRIES)
        return -1;

    pDlgRamRegs =   (CSL_Bcp_DataLoggerRAM *) (pBcpLldObj->modDlgRamRegs[moduleId]);

    pRamEntry->global_hdr_val   =   CSL_FEXT (pDlgRamRegs->DLG_DATA_LOG [entry_index].DLG_GLOBAL_HDR, 
                                              MMR_DLG_DATA_LOG_GLBL_HDR_GLBL_HDR);

    pRamEntry->sw_timestamp_val =   CSL_FEXT (pDlgRamRegs->DLG_DATA_LOG [entry_index].DLG_TM_SW_TIMESTAMP, 
                                              MMR_DLG_DATA_LOG_TM_SW_TIMESTAMP_TM_SW_TIMESTAMP);

    regVal                      =   pDlgRamRegs->DLG_DATA_LOG [entry_index].DLG_HW_TIMESTAMP_ENG_ERR;
    pRamEntry->hw_timestamp_val =   CSL_FEXT (regVal, MMR_DLG_DATA_LOG_HW_TIMESTAMP_ENG_ERR_TM_HW_TIMESTAMP);
    pRamEntry->engine_error_val =   CSL_FEXT (regVal, MMR_DLG_DATA_LOG_HW_TIMESTAMP_ENG_ERR_ENG_ERR);

    return 0;
}


#endif
