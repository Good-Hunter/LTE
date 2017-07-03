/***********************************************/
/*pl_comm.h                                    */
/*���ܣ�����ͷ�ļ�                             */
/***********************************************/
#ifndef PL_COMM_H_
#define PL_COMM_H_

#define LARGE_DELAY_CDD  0
#define INV_SQRT_2_Q15_PP   0x5a825a82   /*1/sqrt(2),1/sqrt(2) P��ʾ���� Q(16,1)*/
#define INV_SQRT_2_Q15_NP   0xa57e5a82   /*-1/sqrt(2),1/sqrt(2) N��ʾ���� Q(16,1)*/
#define INV_SQRT_2_Q15_PN   0x5a82a57e   /*1/sqrt(2),-1/sqrt(2) N��ʾ���� Q(16,1)*/

#define USER_NUM 5//11.19 ��system_init.h�м��й���
#define MAX_TB_BLOCK_SIZE       75376 /*�շָ��õ����TBsize149776*///11.19 ��system_init.h�м��й���
/*�����������*/
#define N_MAX_DL_RB       110/*�������RB����*/
#define N_DL_RB       100/*�������RB����*/
#define N_SYM_PER_SLOT     7/*normalģʽ��ÿ��ʱ϶��OFDM���Ÿ���*/
#define NUM_CELL_ID       504/*cellid�ܸ���*/
#define N_SC_PER_RB       12/*ÿ��prbʹ�õ����ز�����*/
#define NORMAL_CP         1/*��ͨCP*/
#define EXTENDED_CP       0/*��չCP*/
#define DL                3/*����*/
#define SPECIAL           1/*����*/
#define UL                2/*����*/
#define NUM_MAX_ANTENNA   2/*���߸���*/
#define SUBFRAME_LEN      30720/*1ms�ĵ���*/
#define MAX_SUBFN_NUM 10
#define PARA_LEN 5//��������buffer����:5
#define BUF_LEN 6
/*С���ο��ź����*/
#define MAX_C_SEQ_LENGTH  56/*55=110(����prb������)*2*2/8(һ��byte=8bit)��56��ʾ����Ϊ4��������*/

/*ͬ���ź����*/
#define PSC_SSC_SC_NUM   62/*ͬ���ź����ز�����*/
#define TOTAL_PSC_SSC_SC_NUM  72/*ͬ���ź����ز���������������10�����ز�*/
#define TOTAL_PBCH_SC_NUM     72/*PBCH���ز���������������10�����ز�*/

/*PBCH�ź����*/
#define N_PBCH_SYM_7_8 48     /*symbol 7 and 8 PBCHռ�õ�prb����*/
#define N_PBCH_DATA_SC 72     /*symbol 9 PBCHռ�õ�prb���� */

/*�û��������*/
#define NUM_USER     1/*֧�ֵ��û�����*/
#define QPSK         2
#define QAM16        4
#define QAM64        6

/*PCFICH�ŵ����*/
#define PCFICH_BIT_LEN  32

/*PDCCH�ŵ����*/
#define MAX_CCH_SYMBOL_NUM 4

/*�������*/
#define MAX_DESCRAMBLE_LENGTH 12000
#define NEGATE_LLR_LUT_SIZE 4
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

typedef struct
{
  unsigned char pdcch_format;
  unsigned char DCI_format;
  unsigned char start_CCE;
  unsigned char common_pdcch;
  unsigned short dci_bit_len;
  unsigned short rnti;
  unsigned int DCI_data[18];//���576bit
}DL_pdcch_para_struct;

typedef struct
{
  unsigned char ACK_NACK;
  unsigned char n_group;
  unsigned char n_seq;
  unsigned char pad;
  unsigned int I_lowest_index_PRB_RA;
  unsigned int NDmrs;
}DL_phich_para_struct;

typedef struct CONFIG_PARA
{
  unsigned char     subframe_direction[10];
  unsigned short    rnti;

  unsigned char     ueTransMode;
  unsigned char     modulation[NUM_CODEWORD_2];
  unsigned char     num_PRB;
  unsigned char     rvIdx;
  /*2015.1.14 �Բ������кϲ�����*/
  unsigned short 	maxNumHarqProcess;
  unsigned short	numLayers;
  unsigned short 	ueCategory;
  unsigned short 	delay_mod;
  unsigned short	PA;
  //unsigned short 	PB;

  unsigned int      num_code_word;

  unsigned int      bitmap;

  unsigned int      tbSize[NUM_CODEWORD_2];
  unsigned int      tbSize_subframe0[NUM_CODEWORD_2];
  unsigned int      tbSize_subframe1[NUM_CODEWORD_2];

  unsigned char      frame_config;
  unsigned char      pdcch_num;
  unsigned char      pad[2];
  DL_pdcch_para_struct pdcch_para[5];
  DL_phich_para_struct phich_para;
}CONFIG_PARA;
typedef struct USER_PARA
{
    unsigned int      num_code_word;
    unsigned int      tbSize[NUM_CODEWORD_2];
    unsigned int      Nir;
    unsigned int      numBitsG[NUM_CODEWORD_2];
    unsigned int      tb_ptr[NUM_CODEWORD_2];
    unsigned int      modulation_data_ptr[NUM_CODEWORD_2];
    unsigned int      m_sybol;
    unsigned int      m_ap_symbol;
    unsigned int      laryer_symbol;
    unsigned int      bitmap;
    
    
    unsigned short    rnti;
    unsigned char     ueCategory;
    unsigned char     ueTransMode;
    
    unsigned char     maxNumHarqProcess;
    unsigned char     rvIdx;
    unsigned char     codeWordIdx[NUM_CODEWORD_2];
    
    unsigned char     modulation[NUM_CODEWORD_2];
    unsigned char     numLayers;
    unsigned char     delay_mod;
    unsigned char     P_A;
    unsigned char 	  P_B;
    
    unsigned char     num_PRB;
    unsigned char     pad[3];
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
    unsigned char       subframe_direction[MAX_SUBFN_NUM];
    
    //unsigned char       num_PRB_per_bw_config;
    unsigned short		num_PRB_per_bw_config;
    unsigned char       P_B;
    unsigned char       dwpts_symbol;
    unsigned char       dl_Bandwidth;

    unsigned char       phich_Durtion;
    unsigned char       g_phich_Ng;
    unsigned char       pbch_pcfich_pdcch_phich_ueTransMode;

    signed int*         pbch_modulation_data_ptr;
    
    unsigned int        pbch_layer_symbol;
    unsigned int        pbch_m_ap_symbol;

    unsigned short      pbch_m_symbol;
    unsigned char       pcfich_index;
    unsigned char       frame_config;

    unsigned int        pcfich_layer_symbol;
    unsigned int        pcfich_m_ap_symbol;
    
    unsigned int        phich_layer_symbol;
    unsigned int        phich_m_ap_symbol;

    unsigned char      pdcch_total_cce_num[MAX_SUBFN_NUM];
    unsigned char      pad[2];
    unsigned short      reg_unused_num[MAX_SUBFN_NUM];

    unsigned short     pdcch_total_reg_num[MAX_SUBFN_NUM];



 
}CELL_PARA;

typedef struct
{
	unsigned short NewParaFlag;
	unsigned short PareType;
	unsigned short NumType3;
	unsigned short DLVxxUserNum;

	unsigned short RNTI[USER_NUM];
	unsigned int SourcePDSCH[USER_NUM][NUM_CODEWORD_2 *((MAX_TB_BLOCK_SIZE  + 31)/32)];
	unsigned int reserved;
}PG_Type3;

typedef struct
{
  unsigned char L;//Aggregation level L
  unsigned char CCEs;//CCE��
  unsigned char M;//��ѡPDCCH��
  unsigned char pad;
}DL_pdcch_search_describe;





typedef struct
{
  unsigned char pdcch_num;
  unsigned char phich_num;
  unsigned char pad[2];
  unsigned int pdcch_m_ap_symbol;
  unsigned int pdcch_layer_symbol;
  DL_pdcch_para_struct pdcch_para[10];
  DL_phich_para_struct phich_para[10];
}MAC_PARA;

enum DCI_format
{
Dci0=0,
Dci1,
Dci1A,
Dci1B,
Dci1C,
Dci1D,
Dci2,
Dci2A,
Dci3,
Dci3A,
Dci01A
} ;
#if 0
typedef struct {
      int fd;                    /* File descriptor */
      unsigned char* buf;        /* Pointer to start of buffer */
      unsigned char* pos;        /* Position in buffer */
      unsigned char* bufend;     /* Pointer to end of buffer */
      unsigned char* buff_stop;  /* Pointer to last read char in buffer */
      unsigned int   flags;      /* File status flags (see below) */
} FILE;
#endif

typedef union LAYERMAPPING_FFT_DATA
{
  unsigned int fft_output_data[NUM_MAX_ANTENNA][SUBFRAME_LEN];
  unsigned int layermapping_data[N_SC_PER_RB * N_DL_RB * N_SYM_PER_SLOT * 2 * NUM_LAYER_4];

}LAYERMAPPING_FFT_DATA;
/*const globel var*/
typedef struct
{
    unsigned short n0;//symbol0 reg��
    unsigned short n1;//symbol0 reg��
    unsigned short n2;//symbol0 reg��
    unsigned short pcfich_res[16];//PCFICHλ�ã�
    //unsigned short phich_map[MAX_SUBFN_NUM][50][3];//PHICH
    unsigned short symbol_pdcch_reg_num[MAX_SUBFN_NUM][MAX_CCH_SYMBOL_NUM];//PDCCH�ڷ���0��reg��Ŀ
    unsigned char reg_unused_flag[MAX_SUBFN_NUM][300];//PDCCH��Դλ��
    unsigned short pdcch_mapping_carrier[MAX_SUBFN_NUM][900 * 4];//PDCCHӳ������ز�λ��
    unsigned short phich_reg[MAX_SUBFN_NUM][50][3];//PHICHREGλ��
}DL_resource_mapping_struct;

typedef struct
{
    unsigned int phich_freq_data[50 * 12];//��ӳ��Ƶ������ 50=ceil(2*(100/8))*2;2��mi�����ֵ
    unsigned short phich_mapping[MAX_SUBFN_NUM][50 * 12];/*phichӳ����ز�*/
}DL_phich_data_struct;

extern unsigned int psc_data[3 * TOTAL_PSC_SSC_SC_NUM];
extern unsigned int ssc_subframe0_data[NUM_CELL_ID * TOTAL_PSC_SSC_SC_NUM];
extern unsigned int ssc_subframe5_data[NUM_CELL_ID * TOTAL_PSC_SSC_SC_NUM];
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
extern signed int rs_lut[4];
extern unsigned char gold_x1_byte_table[MAX_C_SEQ_LENGTH];
extern unsigned char dl_slot_index[17];
extern unsigned char rs_symbol_index_per_slot[3];
extern signed short ratio_b_lut[4][4];
extern unsigned short TurboInterTable[189][3];
extern unsigned char special_config_normal[9][3];
extern unsigned char up_down_config[7][10];
extern unsigned short rho_a_lut[2][2][16];
extern unsigned char codeword_to_layer_mapping_0[2][4];
extern unsigned char codeword_to_layer_mapping_1[2][4];
extern unsigned char x1_sequence_table[MAX_DESCRAMBLE_LENGTH];
extern unsigned int Negate_LLR_LUT[NEGATE_LLR_LUT_SIZE];
extern unsigned int g_const_cfi_code_table[3];
extern unsigned char g_const_phich_mi[7][10];
extern unsigned char g_const_pdcch_mapping_table[5][9];
extern DL_pdcch_search_describe g_const_pdcch_common_serch_space[2];
extern DL_pdcch_search_describe g_const_pdcch_user_serch_space[4];
extern unsigned char g_const_pdcch_DCI_search[5][2];
extern unsigned int g_const_descramb_table[16];
extern long long  g_const_modu_table_QPSK[16];
extern int g_phich_bpsk_symbol[2][3];
extern unsigned char g_const_pdcch_cce_num_table[4];
extern unsigned char g_const_pdcch_serch_space[4];
extern unsigned short g_const_DCI_bit_num_table[4];
extern unsigned short Doorbellinformation[10];

/*function*/
extern void srio_send(unsigned int slot_index,unsigned int uiDspNum,char flag,int port_num);
extern void layermapping(
        signed int *restrict modulation_data_0,/*����0���ƺ�ķ��� Q(16,3)*/
        signed int *restrict modulation_data_1,/*����1���ƺ�ķ��� Q(16,3)*/
        signed int *restrict layermapping_data,/*��ӳ���ķ��� Q(16,3)*/
        unsigned int m_sybol,/*����0���ƺ�ķ��Ÿ���*/
        unsigned int *m_laryer_symbol,/*��ӳ����ÿ����Ÿ���*/
        unsigned int num_layer,/*����*/
        unsigned int num_codeword,/*���ָ���*/
        unsigned int diversity_mode/*���д���ģʽ*/
        );
extern void precoding(
        signed int *restrict precoding_data[NUM_ANTENNA_PORT_4],/*Ԥ�����ķ��� Q(16,3)*/
        unsigned int *m_ap_symbol,/*Ԥ�����ķ��Ÿ���*/
        signed int *restrict layermapping_data0,/*��ӳ���ķ��� Q(16,3)*/
        signed int *restrict layermapping_data1,/*��ӳ���ķ��� Q(16,2)*/
        unsigned int m_laryer_symbol,/*��ӳ����ÿ����Ÿ���,��2�ı�����*/
        unsigned int num_layer,/*����*/
        unsigned int num_antenna_port,/*���߶˿ڸ���*/
        unsigned int diversity_mode,/*���д���ģʽ*/
        unsigned int delay_mod /*��ʱģʽ*/
       );
extern void prbmapping( unsigned char slot_idx,/*һ������֡�е�ʱ϶����������Ϊż��0,2,4~18*/
        USER_PARA *user_para_ptr,
        CELL_PARA *cell_para_ptr,
        unsigned char * restrict rsgen_output_buffer[3],/*3symbol*220char,�洢����ʵ�鲿��ͬ����*/
        signed int * restrict SCH_data_buffer[NUM_ANTENNA_PORT_2],/*SCH_data_buffer[ant_port][���ƺ�����з���]*/
        signed int * restrict output_data[NUM_ANTENNA_PORT_2],/*һ����֡output_data[ant_port][14*100*12]*/
        signed int * restrict PBCH_data_buffer[NUM_ANTENNA_PORT_2],
        signed int * restrict PSCH_data_buffer,
        signed int * restrict SSCH_data_buffer,
        unsigned char total_symbol_num,
        unsigned char user_index
      );
extern void  fft_data_copy(unsigned char num_antenna_port,
        unsigned char OFDMA_start,
        unsigned char OFDMA_end,
        unsigned int* tx_buffer_ptr,
        unsigned int ant_index
       );
extern void  fft_output_data_copy(unsigned char num_antenna_port,
        unsigned int* restrict fftc_output_data_ptr1,
        unsigned int* restrict fftc_output_data_ptr2,
        unsigned int length,
        int factor0,
        int factor1
       );

extern void time_agc_compensate(signed int* restrict e_time_data_in_ptr[NUM_MAX_ANTENNA],  //In & Out
           signed int* restrict e_time_data_out_ptr[NUM_MAX_ANTENNA], 
           signed int  recv_ant_num,       //Para
           signed int num_antenna_port,
           signed int length,
           signed int start_symbol,
           signed int end_symbol,
           signed short* restrict agc_coef);
extern void f_qpsk_modulation(unsigned  char *restrict input_data_ptr,
                          unsigned  int *restrict output_data_ptr,
                              unsigned  int  length);//bit��
extern void f_block_interleaver(long long *input_data_ptr1,//2��64BITΪһ��REG
        long long *input_data_ptr2,
        long long *output_data_ptr1,
        long long *output_data_ptr2,
        unsigned int D,
        unsigned int cell_id);
extern unsigned int bcp_lte_pdcch(CELL_PARA *cell_para_ptr,//ע����Ҫ�ĺ�����������4.28��Դ���޸�
                           unsigned char slot_idx,
                           MAC_PARA *mac_para_ptr,
                           unsigned int *restrict descrambling_bits,
                           unsigned int rxDataBufferLen[],
                           unsigned char * pRxDataBuffer[]
                           ); 
extern void f_scrambling(unsigned int *restrict c,
                  unsigned int *restrict input_data_ptr,
                  unsigned int *restrict output_data_ptr,
                  unsigned int length); 
extern void Bitscrambling(signed int  *restrict input_pointer,
        signed int  *restrict output_pointer,
        unsigned char *descrambling_bit_ptr,
        unsigned int  bit_descrambler_length
                     );
extern void scrambling_data_gen(unsigned char *restrict x1_sequence_table,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length,
                     unsigned int *descrambling_bit_ptr);

extern void f_phich_data_gen(unsigned char n_seq,
                  unsigned char HI,
                  unsigned int phich_pseudo,
                  int *phich_data_ptr);
extern void cfg_change(int ii,int port_num,int slot_idx);
#endif
