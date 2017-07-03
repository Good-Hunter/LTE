/***********************************************/
/*pl_comm.h                                    */
/*功能：公共头文件                             */
/***********************************************/
#ifndef PL_COMM_H_
#define PL_COMM_H_
#include "cpsw_singlecore.h"

#define MAX_TB_BLOCK_SIZE       75376 /*空分复用的最大TBsize149776*/
#define LARGE_DELAY_CDD  0
#define INV_SQRT_2_Q15_PP   0x5a825a82   /*1/sqrt(2),1/sqrt(2) P表示正号 Q(16,1)*/
#define INV_SQRT_2_Q15_NP   0xa57e5a82   /*-1/sqrt(2),1/sqrt(2) N表示负号 Q(16,1)*/
#define INV_SQRT_2_Q15_PN   0x5a82a57e   /*1/sqrt(2),-1/sqrt(2) N表示负号 Q(16,1)*/
#define PDSCH_PSEUDO_LENGTH    86400   /*下行子帧最多支持的扰码长度，即速率匹配后的有效物理资源*/

/*公共参数相关*/
#define N_MAX_DL_RB       110/*最大下行RB个数*/
#define N_DL_RB       100/*最大下行RB个数*/
#define N_SYM_PER_SLOT     7/*normal模式下每个时隙的OFDM符号个数*/
#define NUM_CELL_ID       504/*cellid总个数*/
#define N_SC_PER_RB       12/*每个prb使用的子载波个数*/
#define NORMAL_CP         1/*普通CP*/
#define EXTENDED_CP       0/*扩展CP*/
#define DL                3/*下行*/
#define SPECIAL           1/*特殊*/
#define UL                2/*上行*/
#define NUM_MAX_ANTENNA   2/*天线个数*/
#define SUBFRAME_LEN      30720/*1ms的点数*/
#define EFFECTIVE_NC      50   /*Nc=1600 as per 3GPP,Set to 1600/32, as we are */
#define CRS_SYMB_NUM_NORMCP_1MS  4    //2天线端口时导频占四个符号（1ms）
#define CRS_SUBCARR_NUM_20M     2 * N_DL_RB     //每个端口每列符号的导频re数目
#define SUBCARR_NUM_PRB_DATA 8
#define MAX_SUBFN_NUM 10
#define CXX_NRB_DL_NUM                N_DL_RB
#define CXX_NRB_UL_NUM                100
#define CXX_PRB_MAX_NUM               100     /* 最大PRB个数：100 */
#define CXX_LOCALIZED_MAPPING_TYPE    0
#define CCC_NGAP1_VALUE                0

#define PARA_LEN 5//参数缓存buffer长度:5-2015.3.25增加

/*小区参考信号相关*/
#define MAX_C_SEQ_LENGTH  56/*55=110(下行prb最大个数)*2*2/8(一个byte=8bit)，56表示便于为4的整数倍*/

/*同步信号相关*/
#define PSC_SSC_SC_NUM   62/*同步信号子载波个数*/
#define TOTAL_PSC_SSC_SC_NUM  72/*同步信号子载波个数含保护带宽10个子载波*/
#define TOTAL_PBCH_SC_NUM     72/*PBCH子载波个数含保护带宽10个子载波*/

/*PBCH信号相关*/
#define N_PBCH_SYM_7_8 48     /*symbol 7 and 8 PBCH占用的prb个数*/
#define N_PBCH_DATA_SC 72     /*symbol 9 PBCH占用的prb个数 */
#define N_PBCH_SYMBOL  ((N_PBCH_SYM_7_8 + N_PBCH_DATA_SC) * 2)
#define PBCH_TBSIZE  24

/*用户参数相关*/
#define NUM_USER     1/*支持的用户个数*/
#define QPSK         2
#define QAM16        4
#define QAM64        6

#define  MAX_COV_TBS 100
/*扰码相关*/
#define MAX_DESCRAMBLE_LENGTH 12000
#define NEGATE_LLR_LUT_SIZE 32

/*PDCCH相关*/
#define MAX_CCH_SYMBOL_NUM 4

/*其他*/
#define N_SYM_PER_CORE 4//每个core支持的最多符号数
typedef enum
{
    PDSCH = 0,
    PBCH,
    PDCCH,
    PCFICH,
    PHICH
}CHAN_TYPE;


typedef enum
{
    NUM_LAYER_1 = 1,
    NUM_LAYER_2,
    NUM_LAYER_3,
    NUM_LAYER_4
    
}NUM_LAYER;

typedef enum
{
    SINGLE_ANTENNA_PORT = 0,
    SPATIAL_MULTIPLEXING,
    TRANSMIT_DIVERSITY
    
}DIVERSITY_MODE;

typedef enum
{
    NUM_CODEWORD_1 = 1,
    NUM_CODEWORD_2
    
}NUM_CODEWORD;

typedef enum
{
    NUM_ANTENNA_PORT_1 = 1,
    NUM_ANTENNA_PORT_2,
    NUM_ANTENNA_PORT_3,
    NUM_ANTENNA_PORT_4
    
}NUM_ANTENNA_PORT;



typedef enum
{
    LTE_CRC16 = 0,
    LTE_CRC24A,
    LTE_CRC24B
}LTE_CRC_TYPE;

enum DCI_format
{
Dci0=0,
Dci1,
Dci1A,
Dci01A,
Dci1B,
Dci1C,
Dci1D,
Dci2,
Dci2A,
Dci2B,
Dci3,
Dci3A
} ;

typedef struct CONFIG_PARA
{
  unsigned char     subframe_direction[10];
  unsigned short    rnti;
  //2015.3.25添加
  unsigned char     ueCategory;
  unsigned char     delay_mod;
  unsigned char     P_A;

  unsigned char     ueTransMode;
  unsigned char     modulation;
  unsigned char     num_PRB;
  unsigned char     rvIdx;

  unsigned int      num_code_word;

  unsigned char    phich_Durtion;
  unsigned char    g_phich_Ng;
  unsigned char    frame_config;
  unsigned char    TM_mode;

  unsigned int      tbSize;
  unsigned int      tbSize_subframe0;
  unsigned int      tbSize_subframe1;
  unsigned char     pdcch_format;
  unsigned char     DCI_format;
  unsigned char     start_CCE;
  unsigned char     common_pdcch;
  unsigned short    dci_bit_len;
  unsigned short    common_rnti;
  unsigned int      DCI_data[18];//最大576bit
  
  unsigned int      bitmap;
  
  unsigned char     n_group;
  unsigned char     n_seq;
  unsigned short    pad;

}CONFIG_PARA;
typedef struct USER_PARA
{
    unsigned int      num_code_word;
    
    
    unsigned short    rnti;
    unsigned char     ueCategory;
    unsigned char     ueTransMode;
    
    unsigned char     modulation[NUM_CODEWORD_2];
    unsigned char     numLayers;
    unsigned char     delay_mod;
    unsigned char     P_A;
    unsigned char 		ndi;
    unsigned int      tbSize[NUM_CODEWORD_2];
    unsigned int      Nir;
    unsigned int      numBitsG[NUM_CODEWORD_2];
    unsigned int      tb_ptr[NUM_CODEWORD_2];
    unsigned int      rmdematch_data_ptr[NUM_CODEWORD_2];
    unsigned int      descramble_data_ptr[NUM_CODEWORD_2];
    unsigned int      m_sybol;
    unsigned int      m_ap_symbol;
    unsigned int      laryer_symbol;
    unsigned int      bitmap;
    
    unsigned char     maxNumHarqProcess;
    unsigned char     rvIdx[NUM_CODEWORD_2];
    unsigned char     codeWordIdx[NUM_CODEWORD_2];
    
    unsigned short    num_of_cb[NUM_CODEWORD_2];
    unsigned char     num_PRB;
    unsigned char     prb_start;
    
    unsigned short    cb_len[13];
    unsigned short    common_rnti;
    
    signed char     fft_agc[NUM_MAX_ANTENNA][2 * N_SYM_PER_SLOT];
    
    signed int subcarrier_demap_num[N_SYM_PER_SLOT * 2];
    
    signed short symbol_start[N_SYM_PER_SLOT * 2];
    //lycan 为了分布式prb而新加的disindex_PRB
    unsigned char disindex_PRB[100];
}USER_PARA;

typedef struct CELL_PARA
{
    unsigned short      cellID;
    unsigned short      PBCHSyncSCStart;

    unsigned short      PBCHSyncSCEnd;
    unsigned char       num_antenna_port;
    unsigned char       RBG_size;

    
    unsigned char       CFI;
    unsigned char       N_cp;
    unsigned char       subframe_direction[10];
    
    unsigned char       num_PRB_per_bw_config;
    unsigned char       P_B;
    unsigned char       dwpts_symbol;
    unsigned char       ul_rb_num;

    unsigned char       frame_config;
    unsigned char       phich_Durtion;
    unsigned char       g_phich_Ng;
    unsigned char       TM_mode;

    unsigned char       dl_Bandwidth;
    unsigned char       pad[3];
 
}CELL_PARA;




typedef struct
{
    unsigned short n0;//symbol0 reg数
    unsigned short n1;//symbol0 reg数
    unsigned short n2;//symbol0 reg数
    unsigned short pcfich_res[4 * 4];//PCFICH位置；
    //unsigned short phich_map[MAX_SUBFN_NUM][50][3];//PHICH
    unsigned short symbol_pdcch_reg_num[MAX_SUBFN_NUM][MAX_CCH_SYMBOL_NUM];//PDCCH在符号0的reg数目
    unsigned char  reg_unused_flag[MAX_SUBFN_NUM][300];//PDCCH资源位置
    //unsigned short pdcch_mapping_carrier[MAX_SUBFN_NUM][900 * 4];//PDCCH映射的子载波位置
    unsigned short symbol0_reg_k[200][4];
    unsigned short symbol1_reg_k[300][4];
    unsigned short symbol2_reg_k[300][4];
    unsigned short phich_reg[MAX_SUBFN_NUM][50][3];//PHICHREG位置
    unsigned short phich_mapping[MAX_SUBFN_NUM][50 * 12];/*phich映射的载波*/
}DL_resource_mapping_struct;

typedef struct
{
  unsigned char L;//Aggregation level L
  unsigned char M;//候选PDCCH数
  unsigned char pad[2];
}DL_pdcch_search_describe;

typedef struct
{
  unsigned char ACK_NACK;
  unsigned char n_group;
  unsigned char n_seq;
  unsigned char pad;
}DL_phich_para_struct;

typedef struct
{
    unsigned int pdcch_data_decode_bits[(100 + 31)/32];
    int pcfich_freq_data[NUM_MAX_ANTENNA][4 * 4];//PCFICH的频域数据
    int pcfich_H_data[NUM_ANTENNA_PORT_2][NUM_MAX_ANTENNA][4 * 4];//PCFICH的信道估计
    int  pcfich_data_after_decoding[4 * 4];//pcfich解预编码后数据
    char  pcfich_data_after_dem[4 * 4 * 2];//pcfich解调后数据
    char  pcfich_data_after_descram[4 * 4 * 2];//pcfich解扰后数据
    //int pcfich_pseudo[2];//PCFICH扰码

    int pdcch_freq_data[NUM_MAX_ANTENNA][800 * 4];//PDCCH的频域数据,2(最大2天线) * 8(每个PDCCH最大8cce) * 9(reg) * 4
    int H_freq_data[NUM_ANTENNA_PORT_2][NUM_MAX_ANTENNA][800 * 4];//PDCCH的频域数据,2(最大2端口) * 8(每个PDCCH最大8cce) * 9(reg) * 4
    unsigned short pdcch_data_k[800 * 4];
    int  pdcch_data_after_decoding[800 * 4];//pcfich解预编码后数据
    char pdcch_data_after_demod[800 * 8];
    //unsigned int pdcch_pseudo[200];//pdcch扰码


    unsigned char pdcch_num;
    unsigned char dci_format[40];
    unsigned short buffer1[3600];
    unsigned short buffer2[3600];
    char buffer3[3][12800];
}DL_cch_data_struct;

typedef union DEMODU_RM_BLOCK
{

  signed int predecoding_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_CORE];/*均衡后的PDSCH,每个core最多4个符号*/
  //signed char demodulation_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2 * QAM64];/*解调*/
  //signed char rmdematch_data[NUM_CODEWORD_2][MAX_TB_BLOCK_SIZE * 4];/*解速率匹配*/
}DEMODU_RM_BLOCK;

typedef union SCRAM_TB_BLOCK
{
  signed int prbdemapping_freq_data[NUM_MAX_ANTENNA][N_SYM_PER_CORE][N_SC_PER_RB * N_DL_RB];/*pdsch数据解资源映射,必须是128byte的整数倍，否则cachewriteback会有问题*/
  //signed short laydemapping_llr_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2];/*解llr层映射*/
  //signed char descramble_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2 * QAM64];/*解扰*/
}SCRAM_TB_BLOCK;

typedef union CHE_CRS_BLOCK
{
  signed short llr_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_CORE];/*均衡后的PDSCH的LLR*/
}CHE_CRS_BLOCK;

typedef union FFT_LAY_BLOCK
{
  signed int prbdemapping_freq_che[NUM_ANTENNA_PORT_2][NUM_MAX_ANTENNA][N_SYM_PER_CORE][N_SC_PER_RB * N_DL_RB];/*pdsch对应信道估计解资源映射，必须是128byte的整数倍，否则cachewriteback会有问题*/
  //signed int layerdemapping_data[NUM_CODEWORD_2][N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2];/*解层映射*/
  
}FFT_LAY_BLOCK;

typedef union PBCH_FREQ_DATA_LAY_BLOCK
{
  signed int  pbch_freq_data[NUM_MAX_ANTENNA][N_PBCH_SYMBOL];
  signed int  pbch_lay_data[N_PBCH_SYMBOL];
  signed char  pbch_demodulation_data[N_PBCH_SYMBOL * 2];
  unsigned int decoded_bits;
}PBCH_FREQ_DATA_LAY_BLOCK;

typedef union PBCH_CHE_LLR_BLOCK
{

  signed int  pbch_che_data[NUM_ANTENNA_PORT_2][NUM_MAX_ANTENNA][N_PBCH_SYMBOL];
  unsigned int descrambling_bits[(N_PBCH_SYMBOL*4*2/32)];//解扰用的bit，1920个bit用int为单位表示
}PBCH_CHE_LLR_BLOCK;

typedef union PBCH_PRECODING_DEMOD_BLOCK
{
  signed int  pbch_precoding_data[N_PBCH_SYMBOL];//4是支持最大层数
  signed char  descramble_data[N_PBCH_SYMBOL * 2];
}PBCH_PRECODING_DEMOD_BLOCK;

/*dci结构体*/
typedef struct
{
    unsigned char format_01a_flag;
    unsigned char hopping_flag;
    unsigned char    mod_cod_rv;/*调制、编码和rv参数*/
    unsigned char    ndi;/*新数据指示*/
    unsigned char    tpc_pusch;
    unsigned char    cyclic_shift_dmrs;
    unsigned char    ul_index;
    unsigned char    cqi;/*cqi指示*/
    unsigned char    n_ulhop;/*ul_hop bit*/
    unsigned char    vrb_start;/*rb开始位置，从前面资源分配参数中得到*/
    unsigned char    vrb_length;/*连续的资源块，从前面资源分配参数中得到*/
    unsigned char    pad;
    unsigned int     res_block;/*资源块分配*/
}Ycc_dci_format0;

typedef struct
{
    unsigned char    res_alloc_type;/*资源块分配类型*/
    unsigned char    qm;/*调制*/
    unsigned char    harq_pro_num;/*HARQ的进程数目*/
    unsigned char    ndi;/*新数据指示*/
    unsigned char    rv;/*冗余版本号*/
    unsigned char    tpc_pucch;
    unsigned char    down_assign_index;
    unsigned char    res_blocks_subset;/*资源块子集*/
    unsigned char    shift_res_span;/*资源分配宽度指示*/
    unsigned char    vrb_start; /*rb开始位置，从前面资源分配参数中得到*/
    unsigned char    vrb_length;/*连续的资源块，从前面资源分配参数中得到*/
    unsigned char    layer_num;
    unsigned char    ts0_prb_index[100];
    unsigned char    ts1_prb_index[100];
    unsigned int     res_block;/*资源块分配*/
    unsigned int     tb_size;/*码块大小*/
}Ycc_dci_format1;

typedef struct
{
  unsigned char format_01a_flag;/*format 0或者1a的标志*/
  unsigned char dis_tran_flag; /*distributed transmission flag*/
    unsigned char    qm;/*调制*/
    unsigned char    harq_pro_num;/*HARQ的进程数目*/
    unsigned char    ndi;/*新数据指示*/
    unsigned char    rv;/*冗余版本号*/
    unsigned char    tpc_pucch;
    unsigned char    down_assign_index;
    unsigned char    n_gap;/*distributed n_rb_dl >50时参数*/
    unsigned char    vrb_start; /*rb开始位置，从前面资源分配参数中得到*/
    unsigned char    vrb_length;/*连续的资源块，从前面资源分配参数中得到*/
    unsigned char    layer_num;
    unsigned char    ts0_prb_index[100];
    unsigned char    ts1_prb_index[100];
    unsigned int     res_block;/*资源块分配*/
    unsigned int     tb_size;/*码块大小*/
}Ycc_dci_format1a;

typedef struct
{
    unsigned char    qm;/*调制*/
    unsigned char    rv;/*冗余版本号*/
    unsigned char    layer_num;
    unsigned char    pad;
    unsigned char    vrb_start; /*rb开始位置，从前面资源分配参数中得到*/
    unsigned char    vrb_length;/*连续的资源块，从前面资源分配参数中得到*/
    unsigned short   res_block;/*资源块分配*/
    unsigned int     tb_size;/*码块大小*/
    unsigned char    gap_type;  //indicate the type of gap, Ngap= Ngap,1 or Ngap,2
    unsigned char    ts0_prb_index[100];
    unsigned char    ts1_prb_index[100];
}Ycc_dci_format1c;

typedef struct
{
    unsigned char    res_alloc_type;/*资源块分配类型*/
    unsigned char    tpc_pucch;
    unsigned char    down_assign_index;
    unsigned char    harq_pro_num;/*HARQ的进程数目*/

    unsigned char    codeword_swap_flag;
    unsigned char    codeword0_qm;/*调制*/
    unsigned char    codeword0_ndi;/*新数据指示*/
    unsigned char    codeword0_rv;/*冗余版本号*/

    unsigned char    codeword1_qm;/*调制*/
    unsigned char    codeword1_ndi;/*新数据指示*/
    unsigned char    codeword1_rv;/*冗余版本号*/
    unsigned char    res_blocks_subset;/*资源块子集*/

    unsigned char    shift_res_span;/*资源分配宽度指示*/
    unsigned char    codeword0_enable_flag;/*codeword有效的标志*/
    unsigned char    codeword1_enable_flag;/*codeword有效的标志*/
    unsigned char    precoding_index;
    
    unsigned char    vrb_start; /*rb开始位置，从前面资源分配参数中得到*/
    unsigned char    vrb_length;/*连续的资源块，从前面资源分配参数中得到*/
    unsigned char    layer_num;
    unsigned char    mod_tbs_index;

    unsigned char    ts0_prb_index[100];
    unsigned char    ts1_prb_index[100];
    unsigned int     res_block;/*资源块分配*/
    unsigned int     codeword0_tb_size;/*码块大小*/
    unsigned int     codeword1_tb_size;/*码块大小*/
}Ycc_dci_format2;

typedef struct
{
    unsigned int tpc_user;  /*2bit表示pucch或者pusch的功率调整，表示的用户数目为 (format0)/2*/
}Ycc_dci_format3;

typedef struct
{
    unsigned int tpc_user;  /*1bit表示pucch或者pusch的功率调整，表示的用户数目为 format0*/
}Ycc_dci_format3a;

union dci_data_format
{
   Ycc_dci_format0       dci_format_0;
   Ycc_dci_format1       dci_format_1;
   Ycc_dci_format1a      dci_format_1a;
   Ycc_dci_format1c      dci_format_1c;
   Ycc_dci_format2       dci_format_2;
   Ycc_dci_format3       dci_format_3;
   Ycc_dci_format3a      dci_format_3a;
};

typedef struct
{
  unsigned int Flag_for_format0_1A     :1;  //格式0和格式1A区分的标志——1比特，其中：“0”表示格式0，“1”表示格式1A
  unsigned int Hopping_flag           :1;  //跳频标志位——1比特
  unsigned int Resource_block_assignment  :13;  //资源块分配和跳频资源分配
  unsigned int Modulation_and_coding_scheme           :5;  //调制编码方案和冗余版本
  unsigned int ndi                    :1;  //新数据指示
  unsigned int TPC_command           :2;  //被调度的PUSCH的传输功率控制命令
  unsigned int Cyclic_shift_DMRS     :3;  //解调参考信号循环移位
  unsigned int UI_or_DAI             :2;  //TD-LTE的上下行配置0:上行索引号;TD-LTE上下行配置1-6情况中:下行分配索引
  unsigned int CQI_request           :1;   //CQI请求
  unsigned int Pad                   :3;
}DCI0_20M_TDD_struct;
typedef struct
{
  unsigned int Resource_allocation_header     :1;  //资源分配头:resource allocation type 0 / type 1
  unsigned int Resource_block_assignment      :25;  //资源块分配
  unsigned int Modulation_and_coding_scheme   :5;  //调制与编码方案
  unsigned int HARQ_process_number            :4;  //HARQ进程数
  unsigned int ndi                            :1;  //新数据指示
  unsigned int Redundancy_version             :2;  //冗余版本——2比特
  unsigned int TPC_command_PUCCH              :2;  //PUCCH传输功率控制命令——2比特
  unsigned int DAI                            :2;  //下行分配索引
}DCI1_20M_TDD_struct;
typedef struct
{
  unsigned int Flag_for_format0_1A     :1;  //格式0和格式1A区分的标志——1比特，其中：“0”表示格式0，“1”表示格式1A
  unsigned int Localized_Distributed_assignment_flag           :1;  //集中式和分布式VRB分配标志——1比特，设置为0
  unsigned int Resource_block_assignment  :13;  //资源块分配和跳频资源分配
  unsigned int Preamble_Index             :6;//随机接入导频序列号——6比特
  unsigned int PRACH_Mask_Index           :4;//PRACH掩码号——4比特，[4]
  unsigned int Modulation_and_coding_scheme           :5;  //调制编码方案和冗余版本
  unsigned int ndi                    :1;  //新数据指示
  unsigned int TPC_command           :2;  //被调度的PUSCH的传输功率控制命令
  unsigned int Cyclic_shift_DMRS     :3;  //解调参考信号循环移位
  unsigned int UI_or_DAI             :2;  //TD-LTE的上下行配置0:上行索引号;TD-LTE上下行配置1-6情况中:下行分配索引
  unsigned int CQI_request           :1;   //CQI请求
  unsigned int Pad                   :3;
}DCI1a_20M_TDD_struct;
/*const globel var*/

//extern unsigned int ifftdata_receive[10][14][61440];
extern unsigned char gold_x1_byte_table[MAX_C_SEQ_LENGTH];
extern unsigned char SymbolWithPilot[4][2 * N_SYM_PER_SLOT];
extern unsigned char SSyncPresent[2 * N_SYM_PER_SLOT];
extern unsigned char PSyncPresent[2 * N_SYM_PER_SLOT];
extern unsigned char PBCHPresent[2 * N_SYM_PER_SLOT];
extern unsigned char PBCH_index[3][4];
extern unsigned char PDSCH_InRB_MultiplePort[3][4];
extern unsigned char PDSCH_InRB_SinglePort_0Symb[6][5];
extern unsigned char PDSCH_InRB_SinglePort_4Symb[6][5];
extern unsigned char PilotInRBK1[6][2];
extern unsigned char PilotInRBK2[6][2];
extern signed int rs_lut_conj[4];
extern unsigned char gold_x1_byte_table[MAX_C_SEQ_LENGTH];
extern unsigned char dl_slot_index[17];
extern unsigned char rs_symbol_index_per_slot[3];
extern signed short ratio_b_lut[4][4];
extern unsigned short TurboInterTable[189][3];
extern unsigned char special_config_normal[9][3];
extern unsigned char up_down_config[7][10];
extern unsigned short rho_a_lut[2][2][16];
extern unsigned char x1_sequence_table[MAX_DESCRAMBLE_LENGTH];
extern unsigned int Negate_LLR_LUT[NEGATE_LLR_LUT_SIZE];
extern unsigned char codeword_to_layer_mapping_0[2][4];
extern unsigned char codeword_to_layer_mapping_1[2][4];
extern unsigned int ifft_subframe[];
extern int g_const_cfi_code_table[3][8];
extern char g_const_phich_mi[7][10];
extern char g_const_pdcch_mapping_table[5][9];
extern DL_pdcch_search_describe g_const_pdcch_common_serch_space[2];
extern DL_pdcch_search_describe g_const_pdcch_user_serch_space[4];
extern char g_const_pdcch_DCI_common_search[2];
extern char g_const_pdcch_DCI_UE_search[9][2];
extern unsigned int  g_const_descramb_table[16];
extern char g_pdcch_dci_bit_length_table[12];
extern char vxx_dl_Bandwidth[6];
extern const unsigned char VCC_MOD_TBS_INDEX_TABLE[96];
extern const unsigned int VCC_BLOCK_SIZE_TABLE[27][100];
extern const unsigned char VCC_DCI_LEN_20M[12];
extern const unsigned char VCC_DCI_LEN_10M[12];
extern const unsigned char VCC_DCI_LEN_5M[12];
extern const unsigned char MCS_MODULATION[29];
/*function*/
extern void func_cs(void);
extern void crs_prb_demapping(signed int* restrict e_freq_data_ptr,   //In, 频域数据首地址
        signed int  recv_ant_num,       //Para
        signed int  cell_id,        //Para,小区ID（0-503）
        signed int  port_no,        //Para,本次demapping的导频的端口号，目前支持0,1
        signed int* restrict e_freq_crs_ptr,      //Out, 导频数据首地址
        unsigned char offset ,//crs的起始位置
        unsigned char crs_length//crs的长度
   );
extern void chan_est(signed int* restrict e_freq_crs_ptr,   //In, 导频接收数据首地址 Q(16,1)
        unsigned char* restrict d_freq_crs_ptr,   //In, 本地导频序列首地址，排列方式同e_freq_crs_ptr；为了方便实现e/d，此处直接存d的共轭
        signed int  recv_ant_num,         //Para
        signed int  prb_start,            //Para,***目前仅适用于连续分配的prb***
        signed int  prb_num,              //Para
        signed int  port_no,          //Para,本次demapping的导频的端口号，目前支持0,1
        signed int  amp_ratio,      //Para,由导频功率/数据功率折算出的二者幅度比；为了方便实现e/ratio，此处直接存数据/导频幅度比；以复数形式存储，高16bit有效（实部），低16bit为0 Q(16,3)
        signed int* restrict h_freq_crs_ptr,        //Out, 频域信道估计首地址（插值前），排列方式同e_freq_crs_ptrQ(16,1)
        unsigned char offset, //crs的起始位置
        unsigned char crs_length//crs的长度
   );
extern void chan_interp(signed int* h_freq_crs_ptr,        //In, 频域信道估计首地址（插值前），排列方式同e_freq_crs_ptr Q(16,3)
        signed int  recv_ant_num,          //Para
        signed int  prb_start,             //Para,***目前仅适用于连续分配的prb***
        signed int  prb_num,               //Para
        signed int  cell_id,         //Para,小区ID（0-503）
        signed int  port_no,         //Para,本次demapping的导频的端口号，目前支持0,1
        signed int  total_num,
        signed int* h_freq_interp_ptr      //Out, 频域信道估计首地址（插值后），排列方式同e_freq_data_ptr，kaV在最外层
   );
extern void freq_agc(signed int* e_freq_data_ptr,    //In & Out，所在符号的频域数据的首地址(ant0)，要求频域数据按照kaR*14*1200的方式排列
        signed int  prb_start,          //Para
        signed int  prb_num,            //Para
        signed int  agc_mode,           //Para, 0---以symb为单位，计算+移位，1---只移位
        signed int* agc_coef);           //In & Out, agc_mode=0时为输出；=1时为输入
extern void layerdemapping(
        signed int *restrict modulation_data_0,/*码字0调制后的符号 Q(16,3)*/
        signed int *restrict modulation_data_1,/*码字1调制后的符号 Q(16,3)*/
        signed int *restrict layermapping_data,/*解预编码后的符号 Q(16,3)*/
        signed int *restrict llr_data_0,/*码字0调制后的llr Q(16,1)*/
        signed int *restrict llr_data_1,/*码字1调制后的llr Q(16,1)*/
        signed int *restrict llr_data,/*解预编码后的llr Q(16,1)*/
        unsigned int m_sybol,/*码字0调制后的符号个数*/
        unsigned int *m_laryer_symbol,/*层映射后的每层符号个数*/
        unsigned int num_layer,/*层数*/
        unsigned int num_codeword,/*码字个数*/
        unsigned int diversity_mode/*下行传输模式*/
        );
extern void demodulation(signed short *detect_data_ptr,//Q(16,3)
        signed short *llr_data_ptr,//Q(16,1)
        signed char *demod_data_ptr,//Q(8,6)
        unsigned char mimo_type,
        unsigned char modulation_type,
        unsigned int data_length
        );
extern void demodulation_descramble(signed short *restrict detect_data_ptr,//Q(16,3)
        signed short *restrict llr_data_ptr,//Q(16,1)
        signed char *restrict demod_data_ptr,//Q(8,6)
        unsigned char mimo_type,
        unsigned char modulation_type,
        unsigned int data_length,
        signed char * scramble_ptr
        );
extern void BitDescrambling(unsigned char *restrict x1_sequence_table,
                     signed char  *restrict input_pointer,
                     signed char  *restrict output_pointer,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length);
extern void  fft_data_copy(unsigned char num_antenna,
                    unsigned char OFDMA_start,
                    unsigned char OFDMA_end,
                    unsigned int* fftc_input_data_ptr1,
                    signed char shift_factor
                   );
extern void tcp3d_data_copy(unsigned int cblen1,
        unsigned int tc_testvector_len1,
        signed char* restrict tc_testvector,
        signed char* restrict tcp3d_llr_ptr);
extern void  fft_input_data_copy(
                unsigned char start_symbol,
                 unsigned char end_symbol,
                     double* fftc_input_data,
                     double* input_data_ptr1,
                     int port
                   );
extern void  fft_input_data_copy_int(
        unsigned char start_symbol,
        unsigned char end_symbol,
        unsigned int* restrict fftc_input_data,
        unsigned int* restrict input_data_ptr1,
        int port
       );
extern void bcp_data_copy(unsigned int len,
        unsigned int* restrict input,
        unsigned int* restrict output);
extern void  tcp3d_data_bit_copy(unsigned int cblen1,
                          unsigned int used_cb_len,
                          signed char* restrict input_ptr4,
                          signed char* restrict tb_buffer);
extern void prb_demapping_pdsch(signed int* e_freq_data_ptr,   //In, 频域数据首地址
        signed int  recv_ant_num,       //Para
        signed int  cell_id,        //Para,小区ID（0-503）
        //signed int  port_no,        //Para,本次demapping的导频的端口号，目前支持0,1
        signed int  symb_no,        //Para,本次demapping的符号，这样可以较灵活的匹配时序调整（第几个符号开始处理...）
        signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
        signed int  ns,           //Para,本次处理的时隙号（port2/3时需要）
        signed int* e_freq_data_demap_ptr_0,    //In/Out, demapping后的接收天线0的频域数据首地址（每次传入本符号该存放的首地址，传出下个符号该存放的首地址）
        signed int* e_freq_data_demap_ptr_1    //In/Out, demapping后的接收天线0的频域数据首地址（每次传入本符号该存放的首地址，传出下个符号该存放的首地址）
        );
extern void chan_eq(signed int*  e_c,     //In, 接收天线的数据
             signed int*  h_c,   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
             signed short* symbol_start,
             signed int* out_ptr,  //Out, d1
             signed int ueTransMode, /*下行传输模式*/
             signed int*  subcarrier_demap_num,
             signed int total_num,
             signed int start_symbol_core,
             signed int m_symbol,
             signed int chan_type,
             signed short* LLR_ptr     //Out,LLr结果首地址
            );
extern void BitDescrambling_pbch(signed char  *restrict input_pointer,
                     signed char  *restrict output_pointer,
                     unsigned int *descrambling_bit_ptr,
                     unsigned int  bit_descrambler_length
                     );

extern void Descrambling_data_pbch_gen(unsigned char *restrict x1_sequence_table,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length,
                     unsigned int *descrambling_bit_ptr);
extern void cc_conv_derate_matching(signed char* data_for_derm,
                             int k,
                             int e,
                             char* s,
                             char* y1,
                             char* y2);

extern char crc_decoder(unsigned int *decode_bits,
                 char crc_type,
                 unsigned int crc_in,
                 char length,
                 CELL_PARA *cell_para,
                 char phy_type,
                 short rnti);
extern unsigned int cc_conv_decoder(int K,
                     int E,
                     char* s,
                     char* y1,
                     char* y2,
                     short *state,
                     char *branch,
                     unsigned int *decoded_bits,
                     char crc_length);
extern unsigned char f_pcfich_decode(int* restrict input_data_ptr,
                      int* restrict cfi_code_table);
extern void f_pcfich_freq_data_recover(int* restrict input_data_ptr,
                            int* restrict out_put_ptr,
                            DL_resource_mapping_struct *dl_resource_mapping_ptr,
                            CELL_PARA *cell_para_ptr
                            );
extern void f_gen_interleave_pattern(unsigned short *interleave_pattern_out_ptr,
                              unsigned short *buffer_ptr,
                              unsigned int D,
                              int cell_id,
                              unsigned short * pdcch_data_table_ptr,
                              unsigned short *reg_k_ptr);
extern void fcc_dci_data_distill(unsigned int *data_in,
    unsigned int *data_out,
    unsigned char dci_format,
    unsigned char trans_mode,
    unsigned char subfn_index,
    CELL_PARA *cell_para_ptr,
    USER_PARA *user_para_ptr);
extern void user_para_update_dci(USER_PARA* user_para_ptr,
                              unsigned char slot_idx,
                              CELL_PARA* cell_para_ptr,
                              union dci_data_format  *dci_data_ptr,
                              unsigned char dci_format);
extern void prb_demapping_update(
                         unsigned char  CFI,
                         signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
                         signed short* symbol_start,
                         signed int* subcarrier_demap_num,
                         signed int  prb_start,          //Para,***目前仅适用于连续分配的prb***
                         signed int  prb_num);
extern void f_phich_det(
     int      n_phich_group,
     int      n_phich_seq,
     unsigned  int     *scrambling_code,
     short      *phich_data_ptr,
     short      *phich_llr_ptr,
     int       *demond_data
    );
extern unsigned char f_harq_ind_decode( int *harq_ind_in);
extern void cc_conv_pdcch_table_derate_matching(signed char* restrict data_for_derm,
                             int K,
                             int E,
                             char* restrict s,
                             char* restrict y1,
                             char* restrict y2,
                             char  output[]
                             );
#endif


