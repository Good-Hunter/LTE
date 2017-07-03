#include <stdio.h>
#include <string.h>
#include <c6x.h>
#include "csl_psc.h"
#include "csl_pscAux.h"
#include "csl_edma3.h"
#include "csl_edma3Aux.h"
#include "cslr_tcp3d_cfg.h"
#include "cslr_tcp3d_dma.h"
#include "cslr_tcp3d_dma_offsets.h"

#include "tcp3dDrv.h"
#include "tcp3d_utils.h"
#include "system_init.h"
#include "vcp2_testInit.h"   //cs 15/07/07
#define MAP_L2toGLB(a,coreId)  (0x10000000+a+(coreId<<24))

/*
#define CSL_TCP3D_A_DATA_REGS      (0x20800000)
// Input Config Register Memory offsets
#define CSL_TCP3D_DMA_TCP3D_IC_CFG0_P0_OFFSET       (0x000000u)
#define CSL_TCP3D_DMA_TCP3D_OUT_STS0_P0_OFFSET      (0x000100u)

#define CSL_TCP3D_DMA_TCP3D_IC_CFG0_P1_OFFSET       (0x000200u)
#define CSL_TCP3D_DMA_TCP3D_OUT_STS0_P1_OFFSET      (0x000300u)

// Input Memory offsets
#define CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET           (0x010000u)
#define CSL_TCP3D_DMA_TCP3D_PAR0_P0_OFFSET          (0x012000u)
#define CSL_TCP3D_DMA_TCP3D_PAR1_P0_OFFSET          (0x014000u)
#define CSL_TCP3D_DMA_TCP3D_INTER_P0_OFFSET         (0x020000u)

#define CSL_TCP3D_DMA_TCP3D_SYS_P1_OFFSET           (0x016000u)
#define CSL_TCP3D_DMA_TCP3D_PAR0_P1_OFFSET          (0x018000u)
#define CSL_TCP3D_DMA_TCP3D_PAR1_P1_OFFSET          (0x01A000u)
#define CSL_TCP3D_DMA_TCP3D_INTER_P1_OFFSET         (0x024000u)

// Output Memory offsets
#define CSL_TCP3D_DMA_TCP3D_OUT_HD0_OFFSET          (0x030000u)
#define CSL_TCP3D_DMA_TCP3D_OUT_HD1_OFFSET          (0x031000u)

#define CSL_TCP3D_DMA_TCP3D_OUT_SO0_OFFSET          (0x080000u)
#define CSL_TCP3D_DMA_TCP3D_OUT_SO1_OFFSET          (0x082000u)
#define CSL_TCP3D_DMA_TCP3D_OUT_SO2_OFFSET          (0x084000u)
*/
#define TCP3D_CFG0  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_IC_CFG0_P0_OFFSET)
#define TCP3D_LLR0  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)
#define TCP3D_OUT0  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD0_OFFSET)

#define TCP3D_CFG1  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_IC_CFG0_P1_OFFSET)
#define TCP3D_LLR1  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_SYS_P1_OFFSET)
#define TCP3D_OUT1  (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_HD1_OFFSET)

#define LLR_DSTCIDX (CSL_TCP3D_DMA_TCP3D_PAR0_P0_OFFSET - CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)
#define LLR_DSTBIDX ((CSL_TCP3D_DMA_TCP3D_PAR0_P0_OFFSET - CSL_TCP3D_DMA_TCP3D_SYS_P0_OFFSET)/2)

unsigned int tcp3d_cfg[2][16];
signed char   tcp3d_llr[2][6144*3 + 12];
unsigned int tcp3d_hd [2][6144/32+1];
/*
  dummy TRIGGER -> CFG0 -> LLR0(sys stream) -> LLR0(par1 stream) -> LLR0(par2 stream) -> dummy
  dummy TRIGGER -> CFG1 -> LLR1(sys stream) -> LLR1(par1 stream) -> LLR1(par2 stream) -> dummy
  REVT0 -> HD0
  REVT1 -> HD1
 */
// In the following table, CFG means configurable, 
DEMO_SEdmaTbl DEMO_EdmaTbl[MAXNUM_EDMA_PHYCH + MAXNUM_EDMA_LINKCH]={
                     /* ChIdx    link  syncDim  tcchEn    tcc     aCnt    bCnt    cCnt    srcAddr             dstAddr            srcBidx dstBidx      srcCidx dstCidx       chHandler PaRAM_Addr*/
/* 0 dummy REVT0    */  { 0,      70,   A_SYMC,     1,      0,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/* 1 dummy REVT1    */  { 1,      72,   A_SYMC,     1,      1,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/* 2 dummy TRIGGER  */  { 2,      64,   A_SYMC,     1,      2,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/* 3 LINK_CFG0      */  {64,      65,   A_SYMC,     1,      2,      60,     1,      1,  (unsigned int)tcp3d_cfg[0],   TCP3D_CFG0,           0,      0,           0,      0,             NULL,      NULL},
/* 4 LINK_LLR0      */  {65,      66,   AB_SYMC,    0,      2,      CFG,    2,      3,  (unsigned int)tcp3d_llr[0],   TCP3D_LLR0,          CFG,    LLR_DSTBIDX,  CFG,   LLR_DSTCIDX,    NULL,      NULL},
/* 5 END_LLR0       */  {66,      67,   A_SYMC,     1,      2,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/* 6 LINK_CFG1      */  {67,      68,   A_SYMC,     1,      2,      60,     1,      1,  (unsigned int)tcp3d_cfg[1],   TCP3D_CFG1,           0,      0,           0,      0,             NULL,      NULL},
/* 7 LINK_LLR1      */  {68,      69,   AB_SYMC,    0,      2,      CFG,    2,      3,  (unsigned int)tcp3d_llr[1],   TCP3D_LLR1,          CFG,    LLR_DSTBIDX,  CFG,   LLR_DSTCIDX,    NULL,      NULL},
/* 8 END_LLR1       */  {69,      64,   A_SYMC,     1,      2,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/* 9 LINK_HD0       */  {70,      71,   A_SYMC,     0,      0,      CFG,    1,      1,   TCP3D_OUT0,          (unsigned int)tcp3d_hd[0],    0,      0,           0,      0,             NULL,      NULL},
/*10 END_HD0        */  {71,      70,   A_SYMC,     1,      0,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
/*11 LINK_HD1       */  {72,      73,   A_SYMC,     0,      1,      CFG,    1,      1,   TCP3D_OUT0,          (unsigned int)tcp3d_hd[1],    0,      0,           0,      0,             NULL,      NULL},
/*12 END_HD1        */  {73,      72,   A_SYMC,     1,      1,      0,      1,      1,      NULL,                  NULL,              0,      0,           0,      0,             NULL,      NULL},
};

CSL_Edma3ChannelObj     glbEdmaChObj[MAXNUM_EDMA_PHYCH];

void DEMO_initEDMA()
{
    CSL_Edma3Obj        edmaObj;
    unsigned char               instNum =CSL_TPCC_2;//CSL_TPCC_1;// CSL_TPCC_2;
    CSL_Status          status; 
    CSL_Edma3Handle     hModule;
    CSL_Edma3ChannelHandle  hEdmaCh;
    CSL_Edma3ChannelAttr    chAttr;
    unsigned int i, PaRAM_Addr;
    CSL_Edma3Context        context;
    CSL_Edma3CmdDrae       draeAttr;   //cs 15/06/04
    CSL_Status             modStatus;   //cs 15/06/04
    /***************************************cs 15/07/07************************************************/
    CSL_Edma3HwSetup hwSetup;
    CSL_Edma3HwDmaChannelSetup dmahwSetup[CSL_EDMA3_TPCC2_NUM_DMACH ] =
            CSL_EDMA3_DMA_CHANNELSETUP_DEFAULT;//cs 15/04/23 there're 64 channels by default,each one map to queue 0 and param set 0-64 separately
    /***************************************cs 15/07/07************************************************/
    if (CSL_edma3Init(&context) != CSL_SOK) 
    {
        printf ("EDMA module initialization failed\n");
    }
    
    /* Open EDMA3 */
    hModule = CSL_edma3Open (&edmaObj, instNum, NULL, &status);
    if ((hModule == NULL) || (status != CSL_SOK)) 
    {
        printf("Open EDMA3 failed");
    }
    /***************************************cs 15/06/04************************************************/
#if 0
    //Module Setup
       hwSetup.dmaChaSetup = &dmahwSetup[0];
       hwSetup.qdmaChaSetup = NULL;
       //CSL_edma3HwSetup(hEdma,&hwSetup);  //cs   15/06/24 8354 cycles
       CSL_edma3HwSetup(hModule,&hwSetup);  //cs   15/06/24 8354 cycles
#endif
    // Setup the DRAE Masks
     draeAttr.region = CSL_EDMA3_REGION_6;
     draeAttr.drae = 0x000000ff;//0xffffffff;
     draeAttr.draeh = 0;

     //modStatus = CSL_edma3HwControl(hEdma,CSL_EDMA3_CMD_DMAREGION_ENABLE,&draeAttr);  // cs 15/04/23 DRAE Enable(Bits 0-31) for the Shadow Region 0.
     modStatus = CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_DMAREGION_ENABLE,&draeAttr);
     /*********************************set the shadow region*******************************************/
    for( i = 0; i < MAXNUM_EDMA_PHYCH; i++ )
    {
        /* Open EDMA channel */
        chAttr.regionNum = CSL_EDMA3_REGION_6;//CSL_EDMA3_REGION_GLOBAL;   //cs 15/06/02 global edma region ,the vcp2 uses shadow region number 0,1,2,3
        chAttr.chaNum    = DEMO_EdmaTbl[i].ChIdx;   //cs 15/06/02  /** Channel number */0 ,1 ,2
        hEdmaCh = CSL_edma3ChannelOpen (&glbEdmaChObj[i], instNum, &chAttr, &status); //cs 15/06/02  The API returns a handle for the specified EDMA Channel for use
        if ((hEdmaCh == NULL) || (status != CSL_SOK)) 
        {
             printf("Open EDMA channel failed");
        }
        else
        {
            DEMO_EdmaTbl[i].chHandler = (unsigned int)hEdmaCh;
            printf("EDMA CH obj %08x\n",hEdmaCh);
        }

        /* Enable EDMA channel */
        if ((status = CSL_edma3HwChannelControl (hEdmaCh, CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL)) != CSL_SOK)
        {
            printf("Enable EDMA channel failed");
        }
#if 0
        /* Map EDMA channel to PaRAM set */
        CSL_edma3MapDMAChannelToParamBlock (hModule, DEMO_EdmaTbl[i].ChIdx, DEMO_EdmaTbl[i].ChIdx);
#endif
    }
    //cs 15/07/09 this function get the PaRAM set address which we want to use,both the PaRAM we want to use directly or linked ,the addresses are already allocated,not allocated in this function
    /* Get the PaRAM set address */
    for( i = 0; i < (MAXNUM_EDMA_PHYCH + MAXNUM_EDMA_LINKCH); i++ )
    {
        hEdmaCh = (CSL_Edma3ChannelHandle)DEMO_EdmaTbl[0].chHandler;
        PaRAM_Addr = (unsigned int)CSL_edma3GetParamHandle (hEdmaCh, DEMO_EdmaTbl[i].ChIdx, &status);
        if (PaRAM_Addr == NULL) 
        {
            printf("Get PaRAM failed");
        }
        else
        {
            DEMO_EdmaTbl[i].PaRAM_Addr = PaRAM_Addr;
            printf("PaRAM address %08x\n",PaRAM_Addr);
        }
    }
}

void DEMO_setupEDMA()
{
    unsigned int i, j;
    unsigned int tmpBuf[8];
    unsigned int * wordPtr;
    EDMA_PARAM_SET * par;
    /* Program PaRAM set */
    for( i = 0; i < (MAXNUM_EDMA_PHYCH + MAXNUM_EDMA_LINKCH); i++ )
    {
        //par = (EDMA_PARAM_SET *)DEMO_EdmaTbl[i].PaRAM_Addr;
        for( j = 0; j < 8; j++ )
            tmpBuf[j] = 0;
        par = (EDMA_PARAM_SET *)tmpBuf;
        par->sam      = 0;
        par->dam      = 0; 
        par->syncDim  = DEMO_EdmaTbl[i].syncDim;   
        par->stat     = 0;
        par->fwid     = 0;
        par->tccMode  = 0;
        par->tcc      = DEMO_EdmaTbl[i].tcc;
        par->tcintEn  = 0;
        par->itcintEn = 0;
        par->tcchEn   = DEMO_EdmaTbl[i].tcchEn;
        if( (i==4) || (i==7) )
        {
            par->itcchEn  = 1;
        }
        else
        {
            par->itcchEn  = 0;
        }
        par->privId   = 0;
        par->priv     = 0;
        if( (DEMO_EdmaTbl[i].srcAddr > 0x800000) && (DEMO_EdmaTbl[i].srcAddr < 0x900000) )
        {
            par->src      = MAP_L2toGLB(DEMO_EdmaTbl[i].srcAddr,glbCoreId);
        }
        else
        {
            par->src      = DEMO_EdmaTbl[i].srcAddr;
        }
        par->aCnt     = DEMO_EdmaTbl[i].aCnt;
        par->bCnt     = DEMO_EdmaTbl[i].bCnt;
        if( (DEMO_EdmaTbl[i].dstAddr > 0x800000) && (DEMO_EdmaTbl[i].dstAddr < 0x900000) )
        {
            par->dst      = MAP_L2toGLB(DEMO_EdmaTbl[i].dstAddr,glbCoreId);
        }
        else
        {
            par->dst      = DEMO_EdmaTbl[i].dstAddr;
        }
        par->srcBidx  = DEMO_EdmaTbl[i].srcBidx;
        par->dstBidx  = DEMO_EdmaTbl[i].dstBidx;
        par->link     = DEMO_EdmaTbl[i].link*32+0x4000;
        par->bCntRld  = 0;
        par->srcCidx  = DEMO_EdmaTbl[i].srcCidx;
        par->dstCidx  = DEMO_EdmaTbl[i].dstCidx;
        par->cCnt     = DEMO_EdmaTbl[i].cCnt;
        
        wordPtr = (unsigned int *)DEMO_EdmaTbl[i].PaRAM_Addr;
        for( j = 0; j < 8; j++ )
        {
            wordPtr[j] = tmpBuf[j];
        }
    }
}

void tcp3d_config (unsigned char instanceNum)
{
    unsigned int  reg = 0;
    CSL_Tcp3d_cfgRegs * hTCP3dACfgReg;
    
    if(instanceNum == 0)
      hTCP3dACfgReg = ((CSL_Tcp3d_cfgRegs *) CSL_TCP3D_A_CFG_REGS);
    else
      hTCP3dACfgReg = ((CSL_Tcp3d_cfgRegs *) CSL_TCP3D_B_CFG_REGS);
      
    /* Soft Reset the TCP3D */
    hTCP3dACfgReg->TCP3_SOFT_RESET = 1;
    hTCP3dACfgReg->TCP3_SOFT_RESET = 0;

    /* Mode 1=LTE */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_MODE_SEL, 1);

    /* Input memory, 1 = double buffer for ping/pong */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_IN_MEM_DB_EN, 1);

    /* 1 = Internal Generation of Interleaver table enabled */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_ITG_EN, CSL_TCP3D_CFG_TCP3_MODE_ITG_EN_ENABLE);

    /* Error Ignore bit, 1 = Don't stop TCP3D on enabled errors */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_ERROR_IGNORE_EN, CSL_TCP3D_CFG_TCP3_MODE_ERROR_IGNORE_EN_DONT_STOP);

    /* 1 = Auto trigger enabled */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_AUTO_TRIG_EN, CSL_TCP3D_CFG_TCP3_MODE_AUTO_TRIG_EN_ENABLE);

    /* LTE CRC value selection - 0 for LTE */
    CSL_FINS (reg, TCP3D_CFG_TCP3_MODE_LTE_CRC_ISEL, 0);

    hTCP3dACfgReg->TCP3_MODE = reg;

    /* Set ENDIAN register parameters */
    reg = 0;
#ifdef _BIG_ENDIAN
    CSL_FINS (reg, TCP3D_CFG_TCP3_END_ENDIAN_INTR, CSL_TCP3D_CFG_TCP3_END_ENDIAN_INTR_16_BIT_NATIVE);
    CSL_FINS (reg, TCP3D_CFG_TCP3_END_ENDIAN_INDATA, CSL_TCP3D_CFG_TCP3_END_ENDIAN_INDATA_8_BIT_NATIVE);
#else
    CSL_FINS (reg, TCP3D_CFG_TCP3_END_ENDIAN_INTR, CSL_TCP3D_CFG_TCP3_END_ENDIAN_INTR_32_BIT_PACKED);
    CSL_FINS (reg, TCP3D_CFG_TCP3_END_ENDIAN_INDATA, CSL_TCP3D_CFG_TCP3_END_ENDIAN_INDATA_32_BIT_PACKED);
#endif

    hTCP3dACfgReg->TCP3_END = reg;

    /* Set EXECUTE P0 register parameters 1 = Normal mode */
    CSL_FINS (hTCP3dACfgReg->TCP3_EXE_P0, TCP3D_CFG_TCP3_EXE_P0_EXE_CMD, CSL_TCP3D_CFG_TCP3_EXE_P0_EXE_CMD_ENABLE);

    /* Set EXECUTE P1 register parameters 1 = Normal mode */
    CSL_FINS (hTCP3dACfgReg->TCP3_EXE_P1, TCP3D_CFG_TCP3_EXE_P1_EXE_CMD, CSL_TCP3D_CFG_TCP3_EXE_P1_EXE_CMD_ENABLE);

    return;
}

/** TCP3D SW0 nominal values */
signed int tcp3d_sw0_Tab[] = {16, 32, 48,  64, 96, 128};

/** Used for getting the sw0LenSel index values */
signed int TAB[] = {0, 1, 2, 3, 3, 4, 4, 5};

/** Table used for division optimization logic */
signed int shiftValTab [] = {4, 5, 4, 6, 5, 7};

/** Table used for division optimization logic */
unsigned int mulValTab [] = {32768, 32768, 10923, 32768, 10923, 32768};

/** Table used for checking bounds */
unsigned int frameLenTab[2][2] = {40,5114,40,6144};




CSL_PscRegs *    dbghPscRegs = ((CSL_PscRegs *) (CSL_PSC_REGS));

signed int enable_tcp3dA()
{
#ifndef SIMULATOR_SUPPORT

    /* TCP3d A power domain is turned OFF by default. It
     * needs to be turned on before doing any TCP3d A device
     * register access.
     */
    /* Set TCP3d A Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_TCP3D_A);//CSL_PSC_PD_TCP3D_A

    /* Enable the clocks too for TCP3d A */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_TCP3D_A, PSC_MODSTATE_ENABLE);//cs 14/11/13 原来为CSL_PSC_PD_TCP3D_A，改为CSL_PSC_LPSC_TCP3D_A

 
    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_TCP3D_A);//CSL_PSC_PD_TCP3D_A

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_TCP3D_A));//CSL_PSC_PD_TCP3D_A
    /* Return TCP3d A PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_TCP3D_A) == PSC_PDSTATE_ON) &&//CSL_PSC_PD_TCP3D_A
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_TCP3D_A) == PSC_MODSTATE_ENABLE))   //cs 14/11/13 原来为CSL_PSC_PD_TCP3D_A，改为CSL_PSC_LPSC_TCP3D_A
    {
        /* TCP3d A ON. Ready for use */            
        return 0;
    }
    else
    {
        /* TCP3d A Power on failed. Return error */            
        return -1;            
    }
#else
    /* No power up needed on Sim */
    return 0;
#endif
}

CSL_Tcp3d_cfgRegs   *tcp3dA_CfgRegs = (CSL_Tcp3d_cfgRegs *) CSL_TCP3D_A_CFG_REGS;
CSL_Tcp3d_cfgRegs   *tcp3dB_CfgRegs = (CSL_Tcp3d_cfgRegs *) CSL_TCP3D_B_CFG_REGS;

signed int enable_tcp3dB()
{
#ifndef SIMULATOR_SUPPORT

    /* TCP3d B power domain is turned OFF by default. It
     * needs to be turned on before doing any FFTC device
     * register access.
     */
    /* Set TCP3d B Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_TCP3D_B);

    /* Enable the clocks too for TCP3d B */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_TCP3D_B, PSC_MODSTATE_ENABLE);

 
    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_TCP3D_B);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_TCP3D_B));
    /* Return FFTC PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_TCP3D_B) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_TCP3D_B) == PSC_MODSTATE_ENABLE))
    {
        /* TCP3d B ON. Ready for use */            
        return 0;
    }
    else
    {
        /* TCP3d B Power on failed. Return error */            
        return -1;            
    }
#else
    /* No power up needed on Sim */
    return 0;
#endif
}





