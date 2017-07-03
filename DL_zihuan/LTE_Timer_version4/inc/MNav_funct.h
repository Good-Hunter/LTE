#ifndef MNAV_FUNCT_H_
#define MNAV_FUNCT_H_

/******************************************************************/
/******************** CPPI / QMSS Configuration *******************/
/******************************************************************/

/******************** Macros ********************/

/* Define Descriptor type */
#define MNAV_DESC_TYPE_HOST          0
#define MNAV_DESC_TYPE_MONO          2
#define MNAV_DESC_TYPE_DEFAULT       3      /* for stream use, not flow config */

/* Accumulator command type */
#define QMSS_ACC_CMD_DISABLE         0x80
#define QMSS_ACC_CMD_ENABLE          0x81

/* Accumulator pacing mode */
#define QMSS_ACC_PAC_MOD_NONE              0  /* signed interrupt on entry threshold count only */
#define QMSS_ACC_PAC_MOD_LAST_INTERRUPT    1  /* Time delay since last interrupt */
#define QMSS_ACC_PAC_MOD_FIRST_NEW_PACKET  2  /* Time delay since first new packet */
#define QMSS_ACC_PAC_MOD_LAST_NEW_PACKET   3  /* Time delay since last new packet */

/* Accumulator entry size */          
#define QMSS_ACC_ENT_SIZE_REG_D            0  /* 'D' register only (4 byte entries) */
#define QMSS_ACC_ENT_SIZE_REG_CD           1  /* 'C,D' registers (8 byte entries) */
#define QMSS_ACC_ENT_SIZE_REG_ABCD         2  /* 'A,B,C,D' registers (16 byte entries) */

/* Accumulator queue mode */           
#define QMSS_ACC_QUE_MOD_SINGLE_QUEUE      0  /* Single Queue Mode - The channel monitors a single queue. */
#define QMSS_ACC_QUE_MOD_MULTI_QUEUE       1  /* Multi-Queue Mode - The channel monitors up to 32 queues starting at the supplied base queue index. */

/* Accumulator count mode */                    
#define QMSS_ACC_CNT_MOD_NULL_TERMINATE    0  /* NULL Terminate Mode */
#define QMSS_ACC_CNT_MOD_ENTRY_COUNT       1  /* Entry Count Mode */

/******************** CDMA QMSS config****************************************/
#define QMSS_CFG_BASE                   (0x02a00000u)
#define QMSS_VBUSM_BASE                 (0x34000000u)
#define SRIO_CFG_BASE                   (0x02900000u)
#define PASS_CFG_BASE                   (0x02000000u)
#define FFTCA_CFG_BASE                  (0x021f0000u)
#define FFTCB_CFG_BASE                  (0x021f4000u)
#define AIF_CFG_BASE                    (0x01f00000u)
#define BCP_CFG_BASE                    (0x35200000u)

/* Define QMSS Register block regions. */
#define QM_CTRL_REGION                  (QMSS_CFG_BASE + 0x00068000u)
#define QM_DESC_REGION                  (QMSS_CFG_BASE + 0x0006a000u)
#define QM_QMAN_REGION                  (QMSS_CFG_BASE + 0x00020000u)
#define QM_QMAN_VBUSM_REGION            (QMSS_VBUSM_BASE + 0x00020000u)
#define QM_PEEK_REGION                  (QMSS_CFG_BASE + 0x00000000u)
#define QM_PROXY_REGION                 (QMSS_CFG_BASE + 0x00040000u)
#define QM_LRAM_REGION                  (                0x00080000u)
#define QM_INTD_REGION                  (QMSS_CFG_BASE + 0x000A0000u)
#define QM_THRESH_REGION                (QMSS_CFG_BASE + 0x00062000u)

#define PDSP1_CMD_REGION                (QMSS_CFG_BASE + 0x000b8000u)
#define PDSP1_REG_REGION                (QMSS_CFG_BASE + 0x0006E000u)
#define PDSP1_IRAM_REGION               (QMSS_CFG_BASE + 0x00060000u)
#define PDSP2_CMD_REGION                (QMSS_CFG_BASE + 0x000bc000u)
#define PDSP2_REG_REGION                (QMSS_CFG_BASE + 0x0006F000u)
#define PDSP2_IRAM_REGION               (QMSS_CFG_BASE + 0x00061000u)

/* Define QMSS PKTDMA Register block regions. */
#define QMSS_PKTDMA_GBL_CFG_REGION      (QMSS_CFG_BASE + 0x0006c000u)
#define QMSS_PKTDMA_TX_CHAN_REGION      (QMSS_CFG_BASE + 0x0006c400u)
#define QMSS_PKTDMA_RX_CHAN_REGION      (QMSS_CFG_BASE + 0x0006c800u)
#define QMSS_PKTDMA_TX_SCHD_REGION      (QMSS_CFG_BASE + 0x0006cc00u)
#define QMSS_PKTDMA_RX_FLOW_REGION      (QMSS_CFG_BASE + 0x0006d000u)

/* Define PASS PKTDMA Register block regions. */
#define PASS_PKTDMA_GBL_CFG_REGION      (PASS_CFG_BASE + 0x00004000u)
#define PASS_PKTDMA_TX_CHAN_REGION      (PASS_CFG_BASE + 0x00004400u)
#define PASS_PKTDMA_RX_CHAN_REGION      (PASS_CFG_BASE + 0x00004800u)
#define PASS_PKTDMA_TX_SCHD_REGION      (PASS_CFG_BASE + 0x00004C00u)
#define PASS_PKTDMA_RX_FLOW_REGION      (PASS_CFG_BASE + 0x00005000u)

/* Define SRIO PKTDMA Register block regions. */
#define SRIO_PKTDMA_GBL_CFG_REGION      (SRIO_CFG_BASE + 0x00001000u)
#define SRIO_PKTDMA_TX_CHAN_REGION      (SRIO_CFG_BASE + 0x00001400u)
#define SRIO_PKTDMA_RX_CHAN_REGION      (SRIO_CFG_BASE + 0x00001800u)
#define SRIO_PKTDMA_TX_SCHD_REGION      (SRIO_CFG_BASE + 0x00001c00u)
#define SRIO_PKTDMA_RX_FLOW_REGION      (SRIO_CFG_BASE + 0x00002000u)

/* Define FFTC A PKTDMA Register block regions. */
#define FFTCA_PKTDMA_GBL_CFG_REGION     (FFTCA_CFG_BASE + 0x00000200u)
#define FFTCA_PKTDMA_TX_CHAN_REGION     (FFTCA_CFG_BASE + 0x00000400u)
#define FFTCA_PKTDMA_RX_CHAN_REGION     (FFTCA_CFG_BASE + 0x00000500u)
#define FFTCA_PKTDMA_TX_SCHD_REGION     (FFTCA_CFG_BASE + 0x00000300u)
#define FFTCA_PKTDMA_RX_FLOW_REGION     (FFTCA_CFG_BASE + 0x00000600u)      

/* Define FFTC B PKTDMA Register block regions. */                
#define FFTCB_PKTDMA_GBL_CFG_REGION     (FFTCB_CFG_BASE + 0x00000200u)
#define FFTCB_PKTDMA_TX_CHAN_REGION     (FFTCB_CFG_BASE + 0x00000400u)
#define FFTCB_PKTDMA_RX_CHAN_REGION     (FFTCB_CFG_BASE + 0x00000500u)
#define FFTCB_PKTDMA_TX_SCHD_REGION     (FFTCB_CFG_BASE + 0x00000300u)
#define FFTCB_PKTDMA_RX_FLOW_REGION     (FFTCB_CFG_BASE + 0x00000600u)

/* Define AIF PKTDMA Register block regions. */                   
#define AIF_PKTDMA_GBL_CFG_REGION       (AIF_CFG_BASE + 0x00014000u)    
#define AIF_PKTDMA_TX_CHAN_REGION       (AIF_CFG_BASE + 0x00016000u)    
#define AIF_PKTDMA_RX_CHAN_REGION       (AIF_CFG_BASE + 0x00018000u)         
#define AIF_PKTDMA_TX_SCHD_REGION       (AIF_CFG_BASE + 0x00000000u)  
#define AIF_PKTDMA_RX_FLOW_REGION       (AIF_CFG_BASE + 0x0001a000u)    

/**********************************************************************
 * Define offsets to individual QM registers within an address region.
 */
/* Queue Manager Region */
#define QM_REG_QUE_REVISION       0x000
#define QM_REG_QUE_DIVERSION      0x008
#define QM_REG_LINKRAM_0_BASE     0x00C
#define QM_REG_LINKRAM_0_SIZE     0x010
#define QM_REG_LINKRAM_1_BASE     0x014
#define QM_REG_STARVATION_CNT     0x020

/* Descriptor Memory Region */
#define QM_REG_MEM_REGION_BASE    0x000
#define QM_REG_MEM_REGION_INDEX   0x004
#define QM_REG_MEM_REGION_SETUP   0x008

/* Queue Management Region */
#define QM_REG_QUE_REG_A          0x000
#define QM_REG_QUE_REG_B          0x004
#define QM_REG_QUE_REG_C          0x008
#define QM_REG_QUE_REG_D          0x00c

/* Queue Status Region */
#define QM_REG_QUE_STATUS_REG_A   0x000
#define QM_REG_QUE_STATUS_REG_B   0x004
#define QM_REG_QUE_STATUS_REG_C   0x008
#define QM_REG_QUE_STATUS_REG_D   0x00c

/* signed interrupt Distributor (INTD) Region */
#define QM_REG_INTD_REVISION      0x000
#define QM_REG_INTD_EOI           0x010
#define QM_REG_INTD_STATUS        0x200
#define QM_REG_INTD_STATUS_CLEAR  0x280
#define QM_REG_INTD_INT_COUNT     0x300

/* PDSP(n) Reg Region */
#define QM_REG_PDSP_CONTROL       0x000
#define QM_REG_PDSP_STATUS        0x004
#define QM_REG_PDSP_CYCLE_COUNT   0x00c
#define QM_REG_PDSP_STALL_COUNT   0x010

/**********************************************************************
 * Define offsets to individual PKTDMA registers within an address region.
 */
/* Global Cfg Register Block */
#define PKTDMA_REG_REVISION         0x000
#define PKTDMA_REG_PERFORM_CTRL     0x004
#define PKTDMA_REG_EMULATION_CTRL   0x008
#define PKTDMA_REG_PRIORITY_CTRL    0x00c
#define PKTDMA_REG_QM0_BASE_ADDR    0x010
#define PKTDMA_REG_QM1_BASE_ADDR    0x014
#define PKTDMA_REG_QM2_BASE_ADDR    0x018
#define PKTDMA_REG_QM3_BASE_ADDR    0x01c

/* Tx Chan Cfg Register Block */
#define PKTDMA_REG_TX_CHAN_CFG_A    0x000
#define PKTDMA_REG_TX_CHAN_CFG_B    0x004

/* Rx Chan Cfg Register Block */
#define PKTDMA_REG_RX_CHAN_CFG_A    0x000

/* Rx Flow Cfg Register Block */
#define PKTDMA_REG_RX_FLOW_CFG_A    0x000
#define PKTDMA_REG_RX_FLOW_CFG_B    0x004
#define PKTDMA_REG_RX_FLOW_CFG_C    0x008
#define PKTDMA_REG_RX_FLOW_CFG_D    0x00c
#define PKTDMA_REG_RX_FLOW_CFG_E    0x010
#define PKTDMA_REG_RX_FLOW_CFG_F    0x014
#define PKTDMA_REG_RX_FLOW_CFG_G    0x018
#define PKTDMA_REG_RX_FLOW_CFG_H    0x01c

/* Tx Sched Cfg Register Block */
#define PKTDMA_REG_TX_SCHED_CHAN_CFG 0x000


void set_memory_region(unsigned short regn, unsigned int addr, unsigned int indx, unsigned int setup);
void set_link_ram(unsigned short ram, unsigned int addr, unsigned int count);
void push_queue(unsigned short qn, unsigned char mode, unsigned int c_val, unsigned int d_val);
unsigned int pop_queue(unsigned short qn);
void empty_queue(unsigned short qn, unsigned int *list, unsigned int *listCount);
unsigned int get_descriptor_count(unsigned short qn);
void set_queue_threshold(unsigned short qn, unsigned int value);
void config_pktdma_qm(unsigned int base, unsigned short *physical_qnum);
void config_tx_chan(unsigned int base, unsigned short chan, unsigned int return_q);
void config_tx_sched(unsigned int base, unsigned short chan, unsigned int priority);
void config_rx_flow(unsigned int base, unsigned short flow,
                    unsigned int a, unsigned int b, unsigned int c, unsigned int d,
                    unsigned int e, unsigned int f, unsigned int g, unsigned int h);
void enable_rx_chan(unsigned int base, unsigned short chan, unsigned int value);
void enable_tx_chan(unsigned int base, unsigned short chan, unsigned int value);
void enable_disable_loopback(unsigned int addr, unsigned int value);

#define MNAV_DESC_TYPE_HOST      0
#define MNAV_DESC_TYPE_MONO      2
#define MNAV_DESC_TYPE_DEFAULT   3 /* for stream use, not flow config */

#define FFTC_A__PKTDMA_GBL_CFG_REGION	0x021f0200
#define FFTC_B__PKTDMA_GBL_CFG_REGION	0x021f4200
#define FFTC_C__PKTDMA_GBL_CFG_REGION	0x35240200

#endif /*MNAV_FUNCT_H_*/
