#ifndef _TCP3D_DRV_H_
#define _TCP3D_DRV_H_

#include "csl_edma3.h"

#define LLR0_LINK_INDEX     (4)
#define LLR1_LINK_INDEX     (7)
#define HD0_LINK_INDEX      (9)
#define HD1_LINK_INDEX      (11)
#define TRIGGER_CH_INDEX    (2)


typedef struct EDMA_PARAM_SET
{
#ifdef _LITTLE_ENDIAN
    /*OPT register*/
    unsigned int sam          :  1;
    unsigned int dam          :  1;   
    unsigned int syncDim      :  1;   
    unsigned int stat         :  1;   
    unsigned int rsvd0        :  4;
    unsigned int fwid         :  3;
    unsigned int tccMode      :  1;
    unsigned int tcc          :  6;
    unsigned int rsvd1        :  1;
    unsigned int rsvd2        :  1;
    unsigned int tcintEn      :  1;
    unsigned int itcintEn     :  1;
    unsigned int tcchEn       :  1;
    unsigned int itcchEn      :  1;
    unsigned int privId       :  4;
    unsigned int rsvd3        :  3;
    unsigned int priv         :  1;

    
    unsigned int src          : 32;
    
    unsigned int aCnt         : 16;
    unsigned int bCnt         : 16;
    
    unsigned int dst          : 32;
    
    unsigned int srcBidx      : 16;
    unsigned int dstBidx      : 16;
    
    unsigned int link         : 16;
    unsigned int bCntRld      : 16;
    
    unsigned int srcCidx      : 16;
    unsigned int dstCidx      : 16;
    
    unsigned int cCnt         : 16;
    unsigned int rsvd         : 16;
#else
    /*OPT register*/
    unsigned int priv         :  1;
    unsigned int rsvd3        :  3;
    unsigned int privId       :  4;
    unsigned int itcchEn      :  1; 
    unsigned int tcchEn       :  1;
    unsigned int itcintEn     :  1;
    unsigned int tcintEn      :  1;
    unsigned int rsvd2        :  1;
    unsigned int rsvd1        :  1;
    unsigned int tcc          :  6;
    unsigned int tccMode      :  1;
    unsigned int fwid         :  3;
    unsigned int rsvd0        :  4;
    unsigned int stat         :  1;
    unsigned int syncDim      :  1;
    unsigned int dam          :  1;
    unsigned int sam          :  1;
    
    unsigned int src          : 32;
    
    unsigned int bCnt         : 16;
    unsigned int aCnt         : 16;
    
    unsigned int dst          : 32;
    
    unsigned int dstBidx      : 16;
    unsigned int srcBidx      : 16;
    
    unsigned int bCntRld      : 16;
    unsigned int link         : 16;
    
    unsigned int dstCidx      : 16;
    unsigned int srcCidx      : 16;
    
    unsigned int rsvd         : 16;
    unsigned int cCnt         : 16;
#endif  
} EDMA_PARAM_SET;


/*
  0         OPT
  1         SRC
  2  BCNT    | ACNT
  3         DST
  4  DSTBIDX | SRCBIDX
  5  BCNTRLD | LINK
  6  DSTCIDX | SRCCIDX
  7  Rsvd    | CCNT
*/
typedef struct EDMA_PARAM_SET_WORD{
    unsigned int opt  ;
    unsigned int src  ;
    unsigned int abCnt;
    unsigned int dst  ;
    unsigned int bIdx ;
    unsigned int link ;
    unsigned int cIdx ;
    unsigned int cCnt ;
} EDMA_PARAM_SET_WORD;

typedef struct DEMO_SEdmaTbl
{
    unsigned short  ChIdx;
    unsigned short  link;
    unsigned char   syncDim;
    unsigned char   tcchEn;
    unsigned char   tcc;
    unsigned short  aCnt;
    unsigned short  bCnt;
    unsigned short  cCnt;
    unsigned int  srcAddr;
    unsigned int  dstAddr;
    unsigned short  srcBidx;
    unsigned short  dstBidx;
    unsigned short  srcCidx;
    unsigned short  dstCidx;
    unsigned int  chHandler;
    unsigned int  PaRAM_Addr;
} DEMO_SEdmaTbl;

/*
 * TCP3D configuration parameters. These parameters are used to construct the user info field passed to BCP. 
 * These value will be read by the EDMA from the BCP packet to program TCP3D.
 */
typedef struct  {
#if 0    
    /* CB length */
    unsigned short cblength;
    /* CB length+4 */
    unsigned short cblengthplus4;
    /* Beta state are located at the end of code block = llrOffset * 3; */
    unsigned short betaStateLoc;
    /* HD ouput size = (CB length/8) */
    unsigned short cblengthover8;
    /* The next 3 field MUST be located at an offset of 2 bytes */
    /* CB length/2 Populate ACNT */
    unsigned short cblengthover2;
    /* CB length/2 Populate SRCBIDX */
    unsigned short cblengthover2_1;
    /* LLR Offset to Parity bits. Populate SRCCIDX */
    unsigned short llroffset;
    /* Padding */
    unsigned short reserved;
#endif
    /* Number of SW0 and block size */
    unsigned int TCP3D_IC_CFG0;
    /* SW0, SW1, SW2 length */
    unsigned int TCP3D_IC_CFG1;
    /* Interleaver table load, Max star, output status register, 
     * swap HD bit ordering, extrinsic scaling, soft output flag, 
     * soft output format, number of iterations, SNR reporting,
     * stopping criteria, CRC selection */
    unsigned int TCP3D_IC_CFG2;
    /* Max start threshold and value */
    unsigned int TCP3D_IC_CFG3;
    /* Beta state 0-3 for MAP0 */
    unsigned int TCP3D_IC_CFG4;
    /* Beta state 5-7 for MAP0 */
    unsigned int TCP3D_IC_CFG5;
    /* Beta state 0-3 for MAP1 */
    unsigned int TCP3D_IC_CFG6;
    /* Beta state 5-7 for MAP1 */
    unsigned int TCP3D_IC_CFG7;
    /* Extrinsic values 0-3 */
    unsigned int TCP3D_IC_CFG8;
    /* Extrinsic values 4-7 */
    unsigned int TCP3D_IC_CFG9;
    /* Extrinsic values 8-11 */
    unsigned int TCP3D_IC_CFG10;
    /* Extrinsic values 12-15 */
    unsigned int TCP3D_IC_CFG11;
    /* ITG param 0-1 */
    unsigned int TCP3D_IC_CFG12;
    /* ITG param 2-3 */
    unsigned int TCP3D_IC_CFG13;
    /* ITG param 4 */
    unsigned int TCP3D_IC_CFG14;
    /* Trigger to start new decode */
    unsigned int TCP3D_TRIG;
} BCP_userInfo;

/*
 * TCP3D configuration parameters. These parameters are used to construct the user info field passed to BCP. 
 * These value will be read by the EDMA from the BCP packet to program TCP3D.
 */
typedef struct cbConfig
{
    /* Control */
    signed int Mode;         //TCP3_MODE

    /* Input */
    signed int NumInfoBits;      //CFG0
    unsigned char sw0Length;       //CFG1
    signed int maxStar;         //CFG2
    signed int outStsRead;
    signed int extScale;
    signed int softOutRead;
    signed int softOutFrmtSel;
    signed int minIter;
    signed int maxIter;
    signed int snrVal;
    signed int snrReport;
    signed int stopSel;
    signed int crcIterSel;
    signed int crcPolySel;
    signed int maxStarThres;        //CFG3
    signed int maxStarValue;
    signed int extrScale_0;        //CFG8
    signed int extrScale_1;
    signed int extrScale_2;
    signed int extrScale_3;
    signed int extrScale_4;        //CFG9
    signed int extrScale_5;
    signed int extrScale_6;
    signed int extrScale_7;
    signed int extrScale_8;        //CFG10
    signed int extrScale_9;
    signed int extrScale_10;
    signed int extrScale_11;
    signed int extrScale_12;       //CFG11
    signed int extrScale_13;
    signed int extrScale_14;
    signed int extrScale_15;
} cbConfig;

#define MAXNUM_EDMA_PHYCH       (3)
#define MAXNUM_EDMA_LINKCH      (10)
#define A_SYMC                  (0)
#define AB_SYMC                 (1)
#define CFG                     (0)
#define TCP3D                   (0)
void DEMO_initEDMA();
void DEMO_setupEDMA();

extern unsigned int tcp3d_cfg[2][16];
extern signed char   tcp3d_llr[2][6144*3 + 12];
extern unsigned int tcp3d_hd [2][6144/32+1];

extern signed short    TCP3_LteInterleaverTable [220][7];
extern signed short    TCP3_WimaxInterleaverTable[17][4];
extern signed int    WIMAX_FRAME_LENGTHS[17];
extern DEMO_SEdmaTbl DEMO_EdmaTbl[MAXNUM_EDMA_PHYCH + MAXNUM_EDMA_LINKCH];

extern void prepare_user_info (BCP_userInfo *userInfo, signed char *tailBits, unsigned short cblen,signed short total_cbs);

extern void Tcp3d_betaStates(
                    signed char    * restrict tailBits,
                    signed int   signChange,
                    signed int   Kt,
                    signed char    * restrict beta0Ptr,
                    signed char    * restrict beta1Ptr);

extern signed int LTE_interleaver_index(signed int K);

extern signed int WIMAX_interleaver_index(signed int K);

extern void tcp3d_config (unsigned char instanceNum);

extern signed int enable_tcp3dA();
extern signed int enable_tcp3dB();
#if 0
#define hTCP3dACfgReg               ((CSL_Tcp3d_cfgRegs *) CSL_TCP3D_A_CFG_REGS)
#define hTCP3dADmaReg               ((CSL_Tcp3d_dmaRegs *) CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_IC_CFG0_P0_OFFSET)

#define hTCP3dALLR0Reg              (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)
#define hTCP3dALLR1Reg              (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P1_OFFSET)

#define hTCP3dAHD0Reg               (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD0_OFFSET)
#define hTCP3dAHD1Reg               (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD1_OFFSET)

#define hTCP3dBCfgReg               ((CSL_Tcp3d_cfgRegs *) CSL_TCP3D_B_CFG_REGS
#define hTCP3dBDmaReg               ((CSL_Tcp3d_dmaRegs *) CSL_TCP3D_B_DATA_REGS + CSL_TCP3D_DMA_TCP3D_IC_CFG0_P0_OFFSET)

#define hTCP3dBLLR0Reg              (CSL_TCP3D_B_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)
#define hTCP3dBLLR1Reg              (CSL_TCP3D_B_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P1_OFFSET)

#define hTCP3dBHD0Reg               (CSL_TCP3D_B_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD0_OFFSET)
#define hTCP3dBHD1Reg               (CSL_TCP3D_B_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD1_OFFSET)

#define hTCP3dCCfgReg               ((CSL_Tcp3d_cfgRegs *) CSL_TCP3D_C_CFG_REGS)
#define hTCP3dCDmaReg               ((CSL_Tcp3d_dmaRegs *) CSL_TCP3D_C_DATA_REGS + CSL_TCP3D_DMA_TCP3D_IC_CFG0_P0_OFFSET)

#define hTCP3dCLLR0Reg              (CSL_TCP3D_C_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)
#define hTCP3dCLLR1Reg              (CSL_TCP3D_C_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P1_OFFSET)

#define hTCP3dCHD0Reg               (CSL_TCP3D_C_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD0_OFFSET)
#define hTCP3dCHD1Reg               (CSL_TCP3D_C_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD1_OFFSET)
#endif

/* Handle to access QMSS INTD Register overlay structure */
#define hQmssIntdReg                ((CSL_Qm_intdRegs *) CSL_QM_SS_CFG_INTD_REGS)

/* Handle to access CPINTC Register overlay structure */
#define hCpIntcReg                  ((CSL_CPINTCRegs *) CSL_CP_INTC_1_REGS)

#endif

