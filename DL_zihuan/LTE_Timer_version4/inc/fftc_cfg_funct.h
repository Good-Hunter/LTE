#ifndef FFTC_CONFIG_FUNCT_H_
#define FFTC_CONFIG_FUNCT_H_


#include "MNav_funct.h"
#include "MNav_struct.h"
/* FFTC register includes */
#include "cslr_fftc.h"
#include "cslr_device.h"


/**************************************************************
***************** DEFINITIONS & GLOBAL VERIABLES********************
***************************************************************/
/* Number of packets to be used for testing the example. */
//#define MAX_NUM_PACKETS         10
#define SIZE_FFTC_HOST_DESC     32
#define NUM_FFTC_HOST_DESC      32
#define CPPI_FFTC_BUF_LEN_RX   2048*7//DSP->FFTC½ÓÊÕ
#define CPPI_FFTC_BUF_LEN_TX   8192*7//FFTC->DSP·¢ËÍ

/* Number of Tx Free descriptors to allocate */
#define     NUM_TX_DESC         2
/* Number of Rx Free descriptors to allocate */
#define     NUM_RX_DESC         2

/******************** Memory Region / Link RAM / Flow Configuration ********************/
#define NUM_FFTC_MEM_REGION     1
#define NUM_FFTC_START_IDX      0
#define NUM_FFTC_LINK_RAM       0
#define NUM_FFTC_RX_FLOW_ID     0

/* Define Queue Number */
#define NUM_FFTC_QUE_GBL_FREE   1000
#define NUM_FFTCA_QUE_TX_Q0     688    /* should map with FFTCA Tx Channel 0 */
#define NUM_FFTCA_QUE_TX_Q1     689    /* should map with FFTCA Tx Channel 1 */
#define NUM_FFTCA_QUE_TX_Q2     690    /* should map with FFTCA Tx Channel 2 */
#define NUM_FFTCA_QUE_TX_Q3     691    /* should map with FFTCA Tx Channel 3 */

#define NUM_FFTCB_QUE_TX_Q0     692    /* should map with FFTCB Tx Channel 0 */
#define NUM_FFTCB_QUE_TX_Q1     693    /* should map with FFTCB Tx Channel 1 */
#define NUM_FFTCB_QUE_TX_Q2     694    /* should map with FFTCB Tx Channel 2 */
#define NUM_FFTCB_QUE_TX_Q3     695    /* should map with FFTCB Tx Channel 3 */

/* CAUTION: WHEN YOU CHANGE THE FOLLOWING RX FDQ AND RX Q, PLEASE CHANGE THE VALUE OF 
* THE RX FLOW CONFIG REGISTER A-H MANUALLY IN "SETUP_RX" FUNCTION, THE CONFIG VALUE ARE NOT WRITED BY MACRO */
#define NUM_FFTC_QUE_RX_FREE    739    /* RX free descriptor queue*/
#define NUM_FFTC_QUE_TX_FREE    738    /* TX FDQ*/
#define NUM_FFTC_QUE_RX_ACC     704    /* RX queue with Accumulator signed interrupt */
#define NUM_FFTC_QUE_RX_POLL  2000   /* RX queue with Poll method*/

#define NUM_FFTC_RX_CHANNELS  4
#define NUM_FFTC_TX_CHANNELS  4

extern CSL_FftcRegs* fftc_cfgRegs;

extern unsigned char gHostDescRegion_fftc[SIZE_FFTC_HOST_DESC * NUM_FFTC_HOST_DESC];
extern unsigned char gTxBuffer[NUM_TX_DESC][CPPI_FFTC_BUF_LEN_TX];


extern unsigned int fftc_system_init(unsigned int fftc_inst);
extern void fftc_system_deinit(void);
extern signed int Setup_Tx_FDQ (void);
extern signed int Setup_Rx_FDQ (unsigned int* pDataBuffer);
extern signed int fftc_global_config(CSL_FftcRegs* fftc_cfgRegs);
extern unsigned int fftc_get_descriptor(unsigned int queue_num);
extern unsigned int fftc_build_header(unsigned int *tx_buffer,
                                unsigned int PSFiledLen,    unsigned int *PSField, 
                                unsigned int DFTSizeListLen,  unsigned int *DFTSizeList, 
                                unsigned int bLocalConfig,  unsigned int *fftLocalCfg);
extern unsigned int fftc_send_pkt(unsigned int pkt_desc, unsigned int buffer_sz, unsigned int header_sz, unsigned int *tx_buffer, unsigned int queue_num);
extern unsigned int fftc_recv_pkt(unsigned int queue_num,unsigned char OFDMA_start,unsigned char OFDMA_end);
extern signed int fftc_get_dft_idx(unsigned int dft_sz);
extern unsigned int fft_config(unsigned int *dft_size_list,
        unsigned int dft_length,
        unsigned int pkt_sz,
        unsigned int dft_idft_flag,
        unsigned int *restrict fftc_output_data_address,
        unsigned int fftc_inst,
        unsigned int zero_pad_value,
        unsigned int variable_shift_input,
        unsigned int cp_len,
        unsigned char start_symbol,
        unsigned char end_symbol);

#endif /*FFTC_CONFIG_FUNCT_H_*/
