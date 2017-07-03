

#ifndef _BCPCOMMON_H_
#define _BCPCOMMON_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include <c6x.h>
#include "csl_chip.h"
#include "csl_psc.h"
#include "csl_pscAux.h"

#include "bcp_lld.h"
#include "bcp_mmr.h"
#include "qmss_types.h"
#include "qmss_qm.h"
#include "cppi_types.h"
#include "cppi_desc.h"
#include "cppi_drv.h"



#define L2GLBMAP(addr, coreID)   \
    ( ( ((unsigned int)(addr) >= 0x00800000) && ((unsigned int)(addr) < 0x00900000) ) ? \
      ( (unsigned int)(addr) | (unsigned int)((0x10 | (coreID & 0x3)) << 24) ) : \
      (unsigned int)(addr) ) 


#define BCP_DESC_TYPE_HOST      0
#define BCP_DESC_TYPE_MONO      2
#define BCP_DESC_TYPE_DEFAULT   3 /* for stream use, not flow config */

#define INBUF_SIZE             (16384)
#define OUTBUF_SIZE            (16384)
#define BCP_DESC_SIZE          (48)
#define BCP_MAX_NUM_FD         (16)

#define BCP_TEST_NUM_PACKETS   (1)

#define BCP_QUETYPE_HIPRI      (3)
#define BCP_QUETYPE_FDQ        (2)
#define BCP_QUETYPE_TXQ        (1)
#define BCP_QUETYPE_OTHER      (0)

#define BCP_CDMACHTYPE_TX      (0)
#define BCP_CDMACHTYPE_RX      (1)

#define BCP_NUM_QUEUES         (4)
#define BCP_NUM_CDMA_CHANNELS  (2)
#define BCP_NUM_CDMA_FLOWS     (1)

#define FLOW0                   (0)

#define BCP_Tx_QUE             (868)
#define BCP_Tx_FDQ             (896)
#define BCP_Rx_FDQ             (897)
#define BCP_Rx_QUE             (898)
                                  
typedef struct{
    int queue;
    int thresh;
    int open;
    int type;
    int descNum;
}BCP_QmQueueTable;

typedef struct
{
  Cppi_ChHnd   handle;    //LLD channel handle
  unsigned char        cdma;      //see BCP_EnumCdmaType
  unsigned char        channel;   //channel number (0 to N-1) for the specific CDMA
  unsigned char        tx_pri;    //see BCP_EnumTxPriority
  unsigned char        chan_type; //see BCP_EnumChanType
} BCP_CdmaChanTable;

typedef struct
{
  Cppi_FlowHnd handle;   //LLD Rx flow handle
  unsigned char        cdma;     //see BCP_EnumCdmaType
  unsigned char        flow_id;  //flow ID for the specific CDMA
  uint16_t       dest_q;   //RX queue name (see BCP_EnumQueues)
  uint16_t       fdq_0;    //FDQ names 
} BCP_CdmaFlowTable;


#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  unsigned int type_id         : 2;  //always 0x0 (Host Packet ID)
  unsigned int packet_type     : 5;
  unsigned int reserved_w0     : 2;
  unsigned int ps_reg_loc      : 1;  //0=PS words in desc, 1=PS words in SOP buff
  unsigned int packet_length   : 22; //in bytes (4M - 1 max)

  /* word 1 */
  unsigned int src_tag_hi      : 8;
  unsigned int src_tag_lo      : 8;
  unsigned int dest_tag_hi     : 8;
  unsigned int dest_tag_lo     : 8;

  /* word 2 */
  unsigned int epib            : 1;  //1=extended packet info block is present
  unsigned int reserved_w2     : 1;
  unsigned int psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int err_flags       : 4;
  unsigned int ps_flags        : 4;
  unsigned int return_policy   : 1;  //0=linked packet goes to pkt_return_qnum,
                                 //1=each descriptor goes to pkt_return_qnum
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int pkt_return_qmgr : 2;
  unsigned int pkt_return_qnum : 12;

  /* word 3 */
  unsigned int reserved_w3     : 10;
  unsigned int buffer_len      : 22;

  /* word 4 */
  unsigned int buffer_ptr;

  /* word 5 */
  unsigned int next_desc_ptr;

  /* word 6 */
  unsigned int completion_tag  : 1;
  unsigned int reserved_w6     : 9;
  unsigned int orig_buff0_len  : 22;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} BCP_HostPacketDescriptor;
#else
typedef struct
{
  /* word 0 */
  unsigned int packet_length   : 22; //in bytes (4M - 1 max)
  unsigned int ps_reg_loc      : 1;  //0=PS words in desc, 1=PS words in SOP buff
  unsigned int reserved_w0     : 2;
  unsigned int packet_type     : 5;
  unsigned int type_id         : 2;  //always 0x0 (Host Packet ID)

  /* word 1 */
  unsigned int dest_tag_lo     : 8;
  unsigned int dest_tag_hi     : 8;
  unsigned int src_tag_lo      : 8;
  unsigned int src_tag_hi      : 8;

  /* word 2 */
  unsigned int pkt_return_qnum : 12;
  unsigned int pkt_return_qmgr : 2;
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int return_policy   : 1;  //0=linked packet goes to pkt_return_qnum,
                                 //1=each descriptor goes to pkt_return_qnum
  unsigned int ps_flags        : 4;
  unsigned int err_flags       : 4;
  unsigned int psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int reserved_w2     : 1;
  unsigned int epib            : 1;  //1=extended packet info block is present


  /* word 3 */
  unsigned int buffer_len      : 22;
  unsigned int reserved_w3     : 10;

  /* word 4 */
  unsigned int buffer_ptr;

  /* word 5 */
  unsigned int next_desc_ptr;

  /* word 6 */
  unsigned int orig_buff0_len  : 22;
  unsigned int reserved_w6     : 9;
  unsigned int completion_tag  : 1;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} BCP_HostPacketDescriptor;
#endif



#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  unsigned int reserved_w0;
  /* word 1 */
  unsigned int reserved_w1;

  /* word 2 */
  unsigned int reserved_w2     : 17;
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int pkt_return_qmgr : 2;
  unsigned int pkt_return_qnum : 12;

  /* word 3 */
  unsigned int reserved_w3     : 10;
  unsigned int buffer_len      : 22;

  /* word 4 */
  unsigned int buffer_ptr;

  /* word 5 */
  unsigned int next_desc_ptr;

  /* word 6 */
  unsigned int reserved_w6     : 10;
  unsigned int orig_buff0_len  : 22;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} BCP_HostBufferDescriptor;
#else
typedef struct
{
  /* word 0 */
  unsigned int reserved_w0;
  /* word 1 */
  unsigned int reserved_w1;

  /* word 2 */
  unsigned int pkt_return_qnum : 12;
  unsigned int pkt_return_qmgr : 2;
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int reserved_w2     : 17;

  /* word 3 */
  unsigned int buffer_len      : 22;
  unsigned int reserved_w3     : 10;

  /* word 4 */
  unsigned int buffer_ptr;

  /* word 5 */
  unsigned int next_desc_ptr;

  /* word 6 */
  unsigned int orig_buff0_len  : 22;
  unsigned int reserved_w6     : 10;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} BCP_HostBufferDescriptor;
#endif


#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  unsigned int type_id         : 2;  //always 0x2 (Monolithic Packet ID)
  unsigned int packet_type     : 5;
  unsigned int data_offset     : 9;
  unsigned int packet_length   : 16; //in bytes (65535 max)

  /* word 1 */
  unsigned int src_tag_hi      : 8;
  unsigned int src_tag_lo      : 8;
  unsigned int dest_tag_hi     : 8;
  unsigned int dest_tag_lo     : 8;

  /* word 2 */
  unsigned int epib            : 1;  //1=extended packet info block is present
  unsigned int reserved_w2     : 1;
  unsigned int psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int err_flags       : 4;
  unsigned int ps_flags        : 4;
  unsigned int reserved_w2b    : 1;
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int pkt_return_qmgr : 2;
  unsigned int pkt_return_qnum : 12;

} BCP_MonolithicPacketDescriptor;
#else
typedef struct
{
  /* word 0 */
  unsigned int packet_length   : 16; //in bytes (65535 max)
  unsigned int data_offset     : 9;
  unsigned int packet_type     : 5;
  unsigned int type_id         : 2;  //always 0x2 (Monolithic Packet ID)

  /* word 1 */
  unsigned int dest_tag_lo     : 8;
  unsigned int dest_tag_hi     : 8;
  unsigned int src_tag_lo      : 8;
  unsigned int src_tag_hi      : 8;

  /* word 2 */
  unsigned int pkt_return_qnum : 12;
  unsigned int pkt_return_qmgr : 2;
  unsigned int ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int reserved_w2b    : 1;
  unsigned int ps_flags        : 4;
  unsigned int err_flags       : 4;
  unsigned int psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int reserved_w2     : 1;
  unsigned int epib            : 1;  //1=extended packet info block is present

} BCP_MonolithicPacketDescriptor;
#endif

#define BCP_MONO_PACKET_SIZE  sizeof(BCP_MonolithicPacketDescriptor)



typedef struct
{
  /* word 0 */
  unsigned int timestamp;

  /* word 1 */
  unsigned int sw_info0;

  /* word 2 */
  unsigned int sw_info1;

  /* word 3 */
  unsigned int sw_info2;

} BCP_ExtendedPacketInfoBlock;


extern Qmss_Result BCP_initQm();
extern Cppi_Result BCP_initCppi(void);
extern void        BCP_initBcp();
extern void        BCP_setupQm();
extern void        BCP_setupCppi();
extern void        BCP_setupDesc();
extern void        CSL_tscEnable();
extern signed int     Bcp_initDevice (Bcp_LldObj* pBcpLldObj);

extern Qmss_GlobalConfigParams qmssGblCfgParams[];
extern Cppi_GlobalConfigParams cppiGblCfgParams[];

extern BCP_QmQueueTable BCP_QmTable[BCP_NUM_QUEUES];
extern BCP_CdmaChanTable BCP_cdmaChan[BCP_NUM_CDMA_CHANNELS];
extern BCP_CdmaFlowTable BCP_cdmaFlow[BCP_NUM_CDMA_FLOWS];

extern void read_data_from_file 
(
    FILE*                fp, 
    int8_t*              pDataBuffer, 
    unsigned int*            pDataBufferLen
);
extern void write_data_to_file 
(
    int8_t*              pDataBuffer, 
    unsigned int             dataBufferLen, 
    FILE*                fpOut
);
extern int8_t gind_row_index (unsigned int input);
extern signed int validate_rxdata 
(
    int8_t*              pRefDataBuffer,
    unsigned int             refDataBufferLen,
    int8_t*              pRxDataBuffer, 
    unsigned int             rxDataBufferLen, 
    unsigned int             dataOffset
);

extern void prepare_crchdr_cfg 
(
    Bcp_CrcHdrCfg*      pCrcHdrCfg, 
    Bcp_RadioStd        radioStd,
    unsigned int            tbSize,
    unsigned int            numFillerBits,
    unsigned int            method2Id,
    unsigned int            dtxFormat,
    int8_t              scrambFlag,
    unsigned int            crcSize, 
    unsigned int            numTb, 
    unsigned int            dataFormatIn, 
    unsigned int            numTrCh, 
    unsigned int*           trChLen
);

extern void EndianConverter_16b(unsigned int* ptr, unsigned int len);
extern void EndianConverter_8b(unsigned int* ptr, unsigned int len);

#endif /* _BCPCOMMON_H_ */

