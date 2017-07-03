
#ifndef _CPPI_DESCRIPTORS_H
#define _CPPI_DESCRIPTORS_H

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


#define CPPI_DESC_ID_HOST        0
#define CPPI_DESC_ID_MONO        2
#define CPPI_DESC_ID_TEARDOWN    3

#define CPPI_DESC_TYPE_HOST      1
#define CPPI_DESC_TYPE_MONO      2
#define CPPI_DESC_TYPE_DEFAULT   3 /* for stream use, not flow config */


/*******************************************************************/
/* Define the bit and word layouts for the Host Packet Descriptor. */
/* For a Host Packet, this is used for the first descriptor only.  */
/*******************************************************************/
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

} CPPI_HostPacketDescriptor;
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

} CPPI_HostPacketDescriptor;
#endif

#define CPPI_HOST_PACKET_SIZE  sizeof(CPPI_HostPacketDescriptor)


/*******************************************************************/
/* Define the bit and word layouts for the Host Buffer Descriptor. */
/* For a Host Packet, this will used for secondary descriptors.    */
/*******************************************************************/
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

} CPPI_HostBufferDescriptor;
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

} CPPI_HostBufferDescriptor;
#endif

// Host Buffer packet size is always the same as Host Packet size


/*********************************************************************/
/* Define the bit and word layouts for the Monolithic Pkt Descriptor.*/
/*********************************************************************/
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
  unsigned int on_chip         : 1;  //0=descriptor is in off-chip, 1=on-chip
  unsigned int pkt_return_qmgr : 2;
  unsigned int pkt_return_qnum : 12;

} CPPI_MonolithicPacketDescriptor;
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
  unsigned int on_chip         : 1;  //0=descriptor is in off-chip, 1=on-chip
  unsigned int reserved_w2b    : 1;
  unsigned int ps_flags        : 4;
  unsigned int err_flags       : 4;
  unsigned int psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int reserved_w2     : 1;
  unsigned int epib            : 1;  //1=extended packet info block is present

} CPPI_MonolithicPacketDescriptor;
#endif

#define CPPI_MONO_PACKET_SIZE  sizeof(CPPI_MonolithicPacketDescriptor)


/*********************************************************************/
/* Define the word layout of the Extended Packet Info Block.  It     */
/* is optional and may follow Host Packet and Monolithic descriptors.*/
/*********************************************************************/
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

} CPPI_ExtendedPacketInfoBlock;


/****************************************************************/
/* Define the bit and word layouts for the Teardown Descriptor. */
/****************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  unsigned int type_id         : 2;  //always 0x3 (Teardown Packet ID)
  unsigned int sub_type        : 3;
  unsigned int reserved_w0a    : 10;
  unsigned int tx_rx           : 1;  //0=Tx, 1=Rx
  unsigned int dma_num         : 6;
  unsigned int reserved_w0b    : 4;
  unsigned int channel_num     : 6;

  /* words 1 to 3 */
  unsigned int reserved[3];

} CPPI_TeardownDescriptor;
#else
typedef struct
{
  /* word 0 */
  unsigned int channel_num     : 6;
  unsigned int reserved_w0b    : 4;
  unsigned int dma_num         : 6;
  unsigned int tx_rx           : 1;  //0=Tx, 1=Rx
  unsigned int reserved_w0a    : 10;
  unsigned int sub_type        : 3;
  unsigned int type_id         : 2;  //always 0x3 (Teardown Packet ID)

  /* words 1 to 3 */
  unsigned int reserved[3];

} CPPI_TeardownDescriptor;
#endif

#ifdef __cplusplus
}
#endif //__cplusplus 
#endif

