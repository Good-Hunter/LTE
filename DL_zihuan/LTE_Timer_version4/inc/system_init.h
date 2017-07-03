/***********************************************/
/*system_init                                  */
/*功能：系统初始化头文件                       */
/***********************************************/
#include <string.h>

/* BCP application include */
#include "BCP_common.h"
#include "bcp_lte.h"
#include "pl_comm.h"

/* FFTC application include */
#include "fftc_cfg_funct.h"

/* CSL INTC include */
#include "csl_intc.h"

/* CSL Cache Include */
#include "csl_cache.h"
#include "csl_cacheAux.h"
#include "csl_xmcAux.h"
#include "KeyStone_common.h"
//#include "SRIO_Test.h"
#include "KeyStone_DDR_Init.h"
#include "SRIO_PktDMA_Init.h"
#include <Timer_module.h>

#include <SRIO_drv.h>

#define SIZE_HOST_DESC      64
#define SIZE_MONO_DESC      64
#define TOTAL_DESC        128
#define TX_HOST_DESC      TOTAL_DESC / 2
#define RX_HOST_DESC      TOTAL_DESC / 2


#define NULL                    0
#define IDFT_SIZE               2048
#define IDFT_MODE               0
#define ZERO_PAD_VALUE          0x034c0100 /*844*/
#define VARIABLE_SHIFT_INPUT    0x02d40000 /*724*/
#define DFT_MODE                1
#define MAX_DFT_SIZE_LIST_LENGTH  26
//#define MAX_TB_BLOCK_SIZE       75376 /*空分复用的最大TBsize149776*///mjt//11.19剪切到pl_common.h
#define TIME_GAP  1000000
#define TIME_ALL 71428
#define BUF_LEN 6//接收端数据数组大小

#define PBCH_TBSIZE  24
#define PBCH_BIT_LEN 1920//NORMAL CP配置
#define PBCH_SYMBOL_LEN_SLOT0 (PBCH_BIT_LEN >> 3)//NORMAL CP配置
#define USER_NUM1 10
//#define USER_NUM 5//mjt//11.19剪切到pl_common.h

#define VERSION_719    //16.2.25jx
extern Cppi_Handle     glbBCPCdmaHdl;
extern Bcp_LldObj      glbBcpLldObj;
extern int             glbCoreId;

extern unsigned char glbDescBuf[BCP_MAX_NUM_FD*2][BCP_DESC_SIZE]; //each memory region should have at least 32 descriptors
extern unsigned int srcBuf[BCP_MAX_NUM_FD][INBUF_SIZE];
extern unsigned int dstBuf[BCP_MAX_NUM_FD][OUTBUF_SIZE];

extern Bcp_LldObj    * pBcpLldObj;
extern unsigned int fftcInstNum;
extern unsigned int used_len;/*记录IFFT后的计算的chip个数*/
extern unsigned int vxx_tb_buffer[NUM_CODEWORD_2 *((MAX_TB_BLOCK_SIZE + 31)/32)];
extern unsigned int gRxBuffer[NUM_RX_DESC][CPPI_FFTC_BUF_LEN_RX];
extern unsigned int vxx_output_data[NUM_ANTENNA_PORT_2][2 * N_SYM_PER_SLOT * N_DL_RB * N_SC_PER_RB];
extern unsigned int DFT_sz_list[50];
extern CELL_PARA vxx_cell_para;
extern USER_PARA vxx_user_para[USER_NUM];
extern CONFIG_PARA vxx_config_para_init[USER_NUM];
extern LAYERMAPPING_FFT_DATA vxx_layermapping_fft_data;
extern unsigned int vxx_precoding_data[NUM_ANTENNA_PORT_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2];
extern unsigned char rsgen_output[20][3][220];
extern unsigned char vxx_user_num;
extern unsigned int* ssch_data_subframe0_buffer;
extern unsigned int* ssch_data_subframe5_buffer;
extern unsigned int* psch_data_ptr;
extern unsigned int vxx_pbch_modulation_data[960];
extern unsigned int vxx_pbch_layermapping_data[PBCH_SYMBOL_LEN_SLOT0];
extern unsigned int vxx_pbch_precoding_data[NUM_ANTENNA_PORT_2][PBCH_SYMBOL_LEN_SLOT0];
extern unsigned int vxx_pbch_data;
extern char vxx_shift_factor[NUM_ANTENNA_PORT_2][2*N_SYM_PER_SLOT];
extern unsigned int g_pcfich_data_origin;//pcfich编码原始BIT
extern unsigned int g_pcfich_data_after_scram;//pcfich加扰后BIT
extern unsigned int g_pcfich_data_after_modulation[16];
extern unsigned int g_pcfich_data_after_layermapping[16];
extern signed int g_pcfich_data_after_precoding[NUM_ANTENNA_PORT_2][16];
extern unsigned int g_pdcch_pseudo[100];//PCFICH扰码
extern unsigned char g_N_group_phich[MAX_SUBFN_NUM];
extern unsigned int g_pdcch_bit_data_after_scram[3200];
extern int g_pdcch_data_after_layermapping[3200];
extern int g_pdcch_data_after_precoding[NUM_ANTENNA_PORT_2][3200];
extern unsigned int g_phich_data_after_precoding[NUM_ANTENNA_PORT_2][50 * 12];
extern int g_pdcch_data_after_interleaver[NUM_ANTENNA_PORT_2][3200];
extern MAC_PARA vxx_mac_para;
extern DL_resource_mapping_struct g_dl_resource_mapping;
extern DL_phich_data_struct g_phich_data;
extern unsigned char pdcch_cce_unuse_flag[100];
extern unsigned short g_symbol0_reg_k[200][4];
extern unsigned short g_symbol1_reg_k[300][4];
extern unsigned short g_symbol2_reg_k[300][4];
extern void cell_para_update(CELL_PARA* cell_para_ptr,
                      unsigned char * restrict rsgen_output_buffer,
                      unsigned int * PSCH_data_buffer,
                      unsigned int * SSCH_data_subframe0_buffer,
                      unsigned int * SSCH_data_subframe5_buffer);

extern unsigned int user_para_update(USER_PARA* user_para_ptr,
        unsigned char slot_idx,
        CELL_PARA* cell_para_ptr,
        CONFIG_PARA* config_para_ptr);
extern void f_dl_database_updata(CELL_PARA* cell_para_ptr,volatile DL_resource_mapping_struct* g_dl_resource_mapping_ptr);
extern void pdcch_database_updata(CELL_PARA* cell_para_ptr,
                          unsigned  char subfn_index);                          
extern void system_init();
extern signed int enable_bcp (void);
extern signed int enable_fftc (void);
extern unsigned int ethernet_receive_process(unsigned int uiDspNum,char slot_index,unsigned int start);
extern void prepare_lte_enchdr_cfg
(
    Bcp_EncHdrCfg*              pEncHdrCfg,
    Bcp_RadioStd                radioStd,
    unsigned int                      lteChanType,
    BcpTest_LteCBParams*        pCodeBlkParams
);

extern void prepare_lte_rmhdr_cfg 
(
    Bcp_RmHdr_LteCfg*           pLteRmHdrCfg, 
    unsigned int                lteChanType,
    BcpTest_LteCBParams*        pCodeBlkParams, 
    BcpTest_RateMatchParams*    pRmParams
);
extern void prepare_tmhdr_cfg (Bcp_TmHdrCfg* pTmHdrCfg);

extern void prepare_modhdr_cfg 
(
    Bcp_ModHdrCfg*        pModHdrCfg,
    Bcp_RadioStd          radioStd,
    unsigned int          Rprime, 
    unsigned char         mod,   
    unsigned char         operationMode, 
    unsigned short        rms,
    unsigned char         numOFDMsymPerSubfrm,
    unsigned short        numCqiSubcs,
    unsigned short        numRiSubcs,
    unsigned short        numAckSubcs,
    unsigned int          cinit,
    unsigned char         qFmt
);

extern void prepare_crchdr_cfg 
(
    Bcp_CrcHdrCfg*      pCrcHdrCfg, 
    Bcp_RadioStd        radioStd,
    unsigned int        tbSize,
    unsigned int        numFillerBits,
    unsigned int        method2Id,
    unsigned int        dtxFormat,
    signed char         scrambFlag,
    unsigned int        crcSize, 
    unsigned int        numTb, 
    unsigned int        dataFormatIn, 
    unsigned int        numTrCh, 
    unsigned int*       trChLen
);
extern unsigned short f_pdcch_cce_start(unsigned char pdcch_format,
                   unsigned short rnti,
                   unsigned short N_cce,
                   unsigned char subfn,
                   unsigned char common_pdcch_flag
                     );
extern void f_dl_phich_mapping(char subfn_index,
                      char N_group,
                      unsigned short *phich_mapping_ptr);
extern void f_dl_pdcch_mapping_k(char pdcch_symbol_num,
                                    char subfn_index,
                                    CELL_PARA *cell_para_ptr);

extern void srio_discovery();

extern unsigned volatile short slot_idx;
extern unsigned volatile short subframeN;
extern unsigned volatile short sfn;
extern unsigned volatile int dl_pdcch_num;
extern unsigned volatile int dl_phich_num;

extern unsigned int total_output_data[10][NUM_MAX_ANTENNA][SUBFRAME_LEN];
extern CSL_TmrHandle restrict hTmr;		//定时器句柄
extern CSL_TmrHwSetup hwSetup;
extern SRIO_Trans_Config transParameter[];//SRIO传输结构体
extern unsigned volatile int mactophytype1[PARA_LEN][1000];
extern unsigned volatile int mactophytype3[100];
extern unsigned int subframe_0_0[30720];
extern unsigned int subframe_1_0[30720];
extern unsigned int subframe_5_0[30720];
extern unsigned int subframe_6_0[30720];

extern void Device_Interrupt_init();//中断初始化
extern void SrioDevice_init ();
extern void Timer_init();//定时器初始化及配置
extern void LTE_Test(unsigned char slot_index,Uint8 antport_num,unsigned char event);//SRIO发送函数
extern void cfg_change(int ii,int port_num,int slot);//按符号发送transParameter结构体修改
extern Uint32 IntcInitialise ( void );
extern void Timer_start();
//0428刘源修改
extern void BS_DSP0_Core0_PDCCH();//PDCCH信道处理模块
extern void BS_DSP0_Core0_PBCH(signed int * precoding_pbch_data_buffer[]);//利用BCP PBCH信道处理
extern void BS_DSP0_Core0_PDSCH(unsigned int,signed int * precoding_pbch_data_buffer[]);//BCP PDSCH信道处理
extern void BS_DSP0_Core0_PCFICH(char slot_idx,
		signed int *precoding_pcfich_data_buffer[NUM_ANTENNA_PORT_4]);
extern void BS_DSP0_Core0_PHICH(char slot_idx,
		signed int *precoding_phich_data_buffer[NUM_ANTENNA_PORT_4]);
extern void BS_DSP0_Core0_Para_Update(void);
extern void BS_DSP0_Core0_OFDM(unsigned char total_symbol,
		unsigned int * fftc_output_data_address);
extern int  BS_DSP0_Core0_SRIOSEND(unsigned short slot_idx,
								int buf_len,
								unsigned int loc_add);
extern void get_flag();

extern unsigned volatile int gxx_send_flag[8];

extern unsigned volatile int mactophyflag[20];

