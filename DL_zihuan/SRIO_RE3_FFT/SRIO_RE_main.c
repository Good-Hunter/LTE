/*
 * SRIO_RE_main.c
 *
 *  Created on: 2014-11-5
 *      Author: cmonroes
 */
/***********************************************/
/*机箱部分                                         */

/***********************************************/
#include <SRIO_drv.h>
#include "PHY_PHYADP_interfaceV3.h"
#define SRIO_DATA_BUFFER_SIZE		32768				//32KB

/*	SRIO数据区	*/
#pragma DATA_SECTION (Srio_DataBuffer_L2, "SrioDataBuffer_in_L2");
unsigned int Srio_DataBuffer_L2[SRIO_DATA_BUFFER_SIZE] = {0};
#pragma DATA_SECTION (Srio_DataBuffer_SL2, "SrioDataBuffer_in_SL2");
unsigned int Srio_DataBuffer_SL2[SRIO_DATA_BUFFER_SIZE] = {0};
#pragma DATA_SECTION (Srio_DataBuffer_DDR3, "SrioDataBuffer_in_DDR3");
unsigned int Srio_DataBuffer_DDR3[SRIO_DATA_BUFFER_SIZE] = {0};
extern volatile UEPHYADPtoPHYType2 phytomactype2;
#define SRIO_PHYTO719_SIZE             16
SRIO_Trans_Config transParameter[]=
{
	/*远端高地址*/			/*远端低地址*/						/*本地地址*/					/*字节数*/			/*端口号*/			/*LSU编号*/
{		0,			(Uint32)0x10870000,		(Uint32)(&phytomactype2),		SRIO_DATA_BUFFER_SIZE,		SRIO_PORT_0,	SRIO_LSU_NUM_0},
{		0,			(Uint32)0xfc400000,		(Uint32)0x0c0f4500 ,		SRIO_PHYTO719_SIZE ,		SRIO_PORT_0,	SRIO_LSU_NUM_0}
};

extern void SrioDevice_init (void);
extern void LTE_Test(char doorbellvalid,unsigned char event );
extern void get_flag();
/***********************************************/
/*机箱部分                                         */

/***********************************************/
/******************vcp2 part cs 15/05/13*************************/
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_chip.h>
//#include "stdio.h"

#include "vcp2_common.h"
#include "vcp2_testInit.h"
//#include "TSC.h"
#include "KeyStone_common.h"
#include "VCP2_Channel_Density.h"

//volatile Uint32* powerOnFlagPtr = (volatile Uint32*)0x0C000000;
/******************vcp2 part cs 15/05/13*************************/
#include <stdio.h>
#include <string.h>
#include "Keystone_SRIO_init.h"
#include "Keystone_DDR_init.h"
#include "SRIO_test.h"
//#include "SRIO_Interrupts.h"   CS 14/11/05
#include "SRIO_PktDMA_init.h"
#include "SRIO_internal_loopback_test.h"
#include "SRIO_external_line_loopback_test.h"
#include "SRIO_external_forward_back_test.h"
#include "SRIO_2DSP_test.h"
#include "TSC.h"
#include "system_init.h"
#include "platform.h"
#include "cpsw_singlecore.h"

extern volatile int vec_flag;
#define TIME_GAP 1200000
extern void passPowerUp (void);
extern unsigned volatile int used_index;
extern unsigned int ethernet_packet_num;


#pragma DATA_ALIGN (gxx_core0_init_complete_flag, CACHE_L2_LINESIZE)
#pragma DATA_SECTION(gxx_core0_init_complete_flag, ".shareddata")
unsigned int   gxx_core0_init_complete_flag = 1;
unsigned short gxx_half_sfn = 0;
unsigned short gxx_subfn = 0;
unsigned int gxx_time_ant_data[10][30720*2];
#pragma DATA_ALIGN (gxx_time_ant_data, 32)
#pragma DATA_SECTION(gxx_time_ant_data,"PacketData.buffer_DDR")
unsigned int gtest_cycle_record[100];
unsigned int gxx_time = 100000;
unsigned int llr0_link_index_abCnt;
unsigned int llr0_link_index_bIdx;
unsigned int llr0_link_index_cIdx;
unsigned int llr1_link_index_abCnt;
unsigned int llr1_link_index_bIdx;
unsigned int llr1_link_index_cIdx;
unsigned int hd0_link_index_abCnt;
unsigned int hd1_link_index_abCnt;

unsigned int previous_RNTI;
unsigned int current_RNTI;

extern unsigned volatile short  sfn;
extern unsigned volatile short  slot_idx;
extern unsigned volatile short  subframeN;

unsigned volatile int receivenumber = 0;
unsigned int p_start,p_end;
void main()
{
  int i,j,k,cInit;
  int count=0;
  unsigned int uiDspNum;
  TDSP_Type dspType;
  unsigned int cycles,nn,m,srio_cycle4,cycle_sub;
  EDMA_PARAM_SET_WORD * par;
//  printf("%s start............................................\n", test_mode_str[loopback_mode]); cs 14/11/06

  glbCoreId = CSL_chipReadReg (CSL_CHIP_DNUM);


#ifndef SIMULATOR
    CACHE_setL1PSize(CACHE_L1_32KCACHE);
    CACHE_setL1DSize(CACHE_L1_32KCACHE);
    CACHE_setL2Size(CACHE_256KCACHE);
    CACHE_invAllL1p(CACHE_WAIT);
    CACHE_wbInvAllL1d(CACHE_WAIT);
    //CACHE_invAllL2(CACHE_NOWAIT);
  /*make other cores local memory cacheable and prefetchable*/

    for(i=16; i<24; i++)
      CGEM_regs->MAR[i]=1|(1<<CSL_CGEM_MAR0_PFX_SHIFT);

    /*make DDR cacheable and non-prefetchable*/
    for(i=128; i<256; i++)
      CGEM_regs->MAR[i]=1|(1<<CSL_CGEM_MAR0_PFX_SHIFT);

    /*make other space non-cacheable and non-prefetchable*/
    for(i=24; i<128; i++)
      CGEM_regs->MAR[i]=0;

#endif

  TSC_init();
#ifndef SIMULATOR
  dspType= Get_dsp_type();
  if(NYQUIST==dspType||TRUBO_NYQUIST==dspType)
  {
    //DSP core speed
    //KeyStone_main_PLL_init (236, 29);   //for 122.88MHz input clock
    //DDR speed = 66.67*20/1= 1333
    KeyStone_DDR_PLL_init (20, 1);

    Nyquist_EVM_DDR_Init(666.667);  //for 1333Mbps

    //serdes_cfg.commonSetup.inputRefClock_MHz = 250;//156.25;       //cs  14/11/06
  }
  else
  {
    puts("Unknown DSP type!");
    return;
  }
#endif



  if(0 == glbCoreId)//core0配置SRIO
  {
#if 1
	Device_Interrupt_init();
	SrioDevice_init ();
    /****************小区参数配置*************************/
    para_init_f(&vxx_config_para_init,&vxx_cell_para);
    cell_para_update(&vxx_cell_para);/*小区参数配置后不会改变*/
    rs_sequences_gen(&rsgen_output[0][0][0],/*最多17slot*3symbol*220char,存储的是实虚部共同索引*/
                     vxx_cell_para.cellID,
                     vxx_cell_para.N_cp );
   // unsigned char * restrict rsgen_output_buffer   &rsgen_output[0][0][0]
    f_dl_database_updata(&vxx_cell_para);
    user_para_update(vxx_user_para,0,&vxx_cell_para,&vxx_phich_para);
    CACHE_wbL2((void *)&vxx_cell_para, sizeof(vxx_cell_para), CACHE_NOWAIT);// Wait for the writeback operation to complete.
    //CACHE_wbL2((void *)&vxx_config_para_init, sizeof(vxx_config_para_init), CACHE_NOWAIT);// Wait for the writeback operation to complete.
    //CACHE_wbL2((void *)rsgen_output, sizeof(rsgen_output), CACHE_NOWAIT);// Wait for the writeback operation to complete.
    CACHE_wbL2(vxx_user_para, sizeof(vxx_user_para), CACHE_NOWAIT);// Wait for the writeback operation to complete.
    for(i = 0;i < 20;i+=2)//遍历生成该用户所有时隙的所有码字的扰码
    {
	   f_dl_phich_mapping(i >> 1,
						  g_N_group_phich[i >> 1],
						  g_dl_resource_mapping.phich_mapping[i >> 1]);
    }
    for(j=0;j<5;j++)
    {
        for(i = 0;i < 20;i+=2)//遍历生成该用户所有时隙的所有码字的扰码
        {
            cInit = (61+j) * (1 << 14) + \
                0 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
            Descrambling_data_pdsch_gen(x1_sequence_table,
                                       cInit,
                                       PDSCH_PSEUDO_LENGTH,
                                       pdsch_pseudo_init[j][i/2][0]);
            cInit = (61+j) * (1 << 14) + \
                1 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
            Descrambling_data_pdsch_gen(x1_sequence_table,
                                       cInit,
                                       PDSCH_PSEUDO_LENGTH,
                                       pdsch_pseudo_init[j][i/2][1]);
       }
    }
    //memcpy(pdsch_pseudo_ddr,pdsch_pseudo_init[current_RNTI-61],sizeof(pdsch_pseudo_ddr));
    CACHE_wbL2((void *)pdsch_pseudo_init, sizeof(pdsch_pseudo_init), CACHE_WAIT);
#if 0
    for(i = 0;i < 20;i+=2)//遍历生成该用户所有时隙的所有码字的扰码
    {
        f_dl_phich_mapping(i >> 1,
                           g_N_group_phich[i >> 1],
                           g_dl_resource_mapping.phich_mapping[i >> 1]);
        cInit = vxx_user_para[0].rnti * (1 << 14) + \
            0 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
        Descrambling_data_pdsch_gen(x1_sequence_table,
                                    cInit,
                                    PDSCH_PSEUDO_LENGTH,
                                    pdsch_pseudo_ddr[i/2][0]);
        cInit = vxx_user_para[0].rnti * (1 << 14) + \
            1 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
        Descrambling_data_pdsch_gen(x1_sequence_table,
                                    cInit,
                                    PDSCH_PSEUDO_LENGTH,
                                    pdsch_pseudo_ddr[i/2][1]);
    }
#endif
    /****************SRIO配置*************************/
    //uiDspNum= 1;
    //fxx_srio_init(uiDspNum);

#endif
   // Device_Interrupt_init();
   // SrioDevice_init ();
    /****************FFT配置*************************/

    enable_fftc ();
    fftc_system_init(CSL_FFTC_A);
    /*Setup TX free queue*/
    if(Setup_Tx_FDQ() != 0)
    {
        printf ("[Core %d]: FFTC setup TX FDQ failed \n", glbCoreId);
    }
      /*Setup RX free queue*/
    if(Setup_Rx_FDQ() != 0)
    {
        printf ("[Core %d]: FFTC setup RX FDQ failed \n", glbCoreId);
    }

    /*CSL_FFTC_A:fftc_cfgRegs = (CSL_FftcRegs*)(0x021F0000)
    CSL_FFTC_B:fftc_cfgRegs = (CSL_FftcRegs*)(0x021F4000)*/
    /*FFTC Global config*/
    if(fftc_global_config((CSL_FftcRegs*)0x021F0000) != 0)
    {
        printf ("[Core %d]: FFTC Global config failed \n", glbCoreId);
    }

  }
  else if(1 == glbCoreId)//core1配置dsp计算
  {

#if 1
	  	   printf("\n");

	  	   for(i = CSL_VCP2_A; i <= CSL_VCP2_D; i++)
	  	   {
	  	        printf("[Core%d][do VCP2 BER Test]: Enabling VCP2-%d \n", DNUM, i);

	  	        if( enableVcp2(i) != SUCCESS )
	  	        {
	  	             printf("\n[Core%d][do VCP2 BER Test]: Error, Couldn't enable VCP2-%d", DNUM, i);
	  	            // return(FAILURE);
	  	        }
	  	   }


	  	   edma3Init(0,0);  //edma3Init(i%4,i%4);

#endif

  }
  else if(2 == glbCoreId)//core2配置dsp计算
  {
    //enable_bcp();
    //system_init();

  }
  else if(3 == glbCoreId)//core3配置以太
  {
	  while(gxx_core0_init_complete_flag)
      {
      	CACHE_invL2((void *)&gxx_core0_init_complete_flag, sizeof(gxx_core0_init_complete_flag), CACHE_WAIT);
      }

    enable_bcp();
    BCP_initBcp();
    BCP_initQm();
    BCP_initCppi();
    BCP_setupQm();
    BCP_setupCppi();
    BCP_setupDesc();
    enable_tcp3dA();
    tcp3d_config (0);
    DEMO_initEDMA();
    DEMO_setupEDMA();
    par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[LLR0_LINK_INDEX].PaRAM_Addr;
    llr0_link_index_abCnt = par->abCnt;
    llr0_link_index_bIdx = par->bIdx;
    llr0_link_index_cIdx = par->cIdx;

    par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[LLR1_LINK_INDEX].PaRAM_Addr;
    llr1_link_index_abCnt = par->abCnt;
    llr1_link_index_bIdx = par->bIdx;
    llr1_link_index_cIdx = par->cIdx;

    par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[HD0_LINK_INDEX].PaRAM_Addr;
    hd0_link_index_abCnt = par->abCnt;

    par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[HD1_LINK_INDEX].PaRAM_Addr;
    hd1_link_index_abCnt = par->abCnt;

  }

  vxx_user_num = 1;
  //for(k = 0;k < 20;k+=2)
  while(1)
  {
    if(0 == glbCoreId)//core0配置SRIO
    {
        gxx_core0_init_complete_flag = 0;
        CACHE_wbL2((void *)&gxx_core0_init_complete_flag, sizeof(gxx_core0_init_complete_flag), CACHE_WAIT);
        //used_index = 0;
        for(; ;)
        {
         /*   if(receivenumber==20000)
        	{
        	    vxx_config_para_init.rnti=65;
        	    CACHE_wbL2((void *)&vxx_config_para_init.rnti, sizeof(vxx_config_para_init.rnti), CACHE_NOWAIT);
        	}
        	*/

#if 1
            /****************小区参数配置*************************/
        	previous_RNTI=current_RNTI;
            para_init(&vxx_config_para_init,&vxx_cell_para);
            cell_para_update(&vxx_cell_para);/*小区参数配置后不会改变*/
            f_dl_database_updata(&vxx_cell_para);
            user_para_update(vxx_user_para,0,&vxx_cell_para,&vxx_phich_para);
            CACHE_wbL2((void *)&vxx_cell_para, sizeof(vxx_cell_para), CACHE_NOWAIT);// Wait for the writeback operation to complete.
            //CACHE_wbL2((void *)&vxx_config_para_init, sizeof(vxx_config_para_init), CACHE_NOWAIT);// Wait for the writeback operation to complete.
            //CACHE_wbL2((void *)rsgen_output, sizeof(rsgen_output), CACHE_NOWAIT);// Wait for the writeback operation to complete.
            CACHE_wbL2(vxx_user_para, sizeof(vxx_user_para), CACHE_NOWAIT);// Wait for the writeback operation to complete.
#if 0
            if(previous_RNTI!=current_RNTI)
            {
            	p_start = TSCL;
                for(i = 0;i < 20;i+=2)//遍历生成该用户所有时隙的所有码字的扰码
            	{
            	    f_dl_phich_mapping(i >> 1,
            	                       g_N_group_phich[i >> 1],
            	                       g_dl_resource_mapping.phich_mapping[i >> 1]);
            	    cInit = vxx_user_para[0].rnti * (1 << 14) + \
            	            0 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
            	    Descrambling_data_pdsch_gen(x1_sequence_table,
            	                                cInit,
            	                                PDSCH_PSEUDO_LENGTH,
            	                                pdsch_pseudo_ddr[i/2][0]);
            	    cInit = vxx_user_para[0].rnti * (1 << 14) + \
            	            1 * (1 << 13) + i/2  * (1 << 9) + vxx_cell_para.cellID;
            	    Descrambling_data_pdsch_gen(x1_sequence_table,
            	                                cInit,
            	                                PDSCH_PSEUDO_LENGTH,
            	                                pdsch_pseudo_ddr[i/2][1]);
            	}
                slot_idx=(slot_idx+6)%20;
                CACHE_invL2((void *)&used_index, sizeof(used_index), CACHE_WAIT);
                used_index=used_index+3;
                CACHE_wbL2((void *)&used_index, sizeof(used_index), CACHE_WAIT);
                p_end = TSC_delay_cycle(p_start);
                continue;



            }
#endif
#endif

        //  cycles = TSCL;
          receivenumber++;
          /*gxx_symbol0_flag = 0;
          gxx_symbol6_flag = 0;
          gxx_symbol11_flag = 0;
          gxx_symbol13_flag = 0;*/
          lte_dl_receiver (slot_idx);

          slot_idx = (slot_idx+2)%20;
          CACHE_wbL2((void *)&slot_idx, sizeof(slot_idx), CACHE_NOWAIT);

        //  cycle_sub = TSC_delay_cycle(cycles);
         srio_cycle4 = TIME_GAP - cycle_sub;
          if(srio_cycle4 > 0)
          {
            //CycleDelay(srio_cycle4);
          }
          //printf("frame %d received\n",i>>1);
          if(18==i)
          {
        	 count++;
          }
        }

    }
    else
    {
      lte_dl_receiver (0xff);
    }
  }
  //puts("SRIO test complete.\n");

}






