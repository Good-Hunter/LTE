#ifndef _PACKET_DESCRIPTOR_H_
#define _PACKET_DESCRIPTOR_H_



/*******************************************************************/
/* Define the bit and word layouts for the Host Packet Descriptor. */
/* For a Host Packet, this is used for the first descriptor only.  */
/*******************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32 type_id         : 2;  //always 0x0 (Host Packet ID)
  Uint32 packet_type     : 5;
  Uint32 reserved_w0     : 2;
  Uint32 ps_reg_loc      : 1;  //0=PS words in desc, 1=PS words in SOP buff
  Uint32 packet_length   : 22; //in bytes (4M - 1 max)

  /* word 1 */
  Uint32 src_tag_hi      : 8;
  Uint32 src_tag_lo      : 8;
  Uint32 dest_tag_hi     : 8;
  Uint32 dest_tag_lo     : 8;

  /* word 2 */
  Uint32 epib            : 1;  //1=extended packet info block is present
  Uint32 reserved_w2     : 1;
  Uint32 psv_word_count  : 6;  //number of 32-bit PS data words
  Uint32 err_flags       : 4;
  Uint32 ps_flags        : 4;
  Uint32 return_policy   : 1;  //0=linked packet goes to pkt_return_qnum,
                                 //1=each descriptor goes to pkt_return_qnum
  Uint32 ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  Uint32 pkt_return_qmgr : 2;
  Uint32 pkt_return_qnum : 12;

  /* word 3 */
  Uint32 reserved_w3     : 10;
  Uint32 buffer_len      : 22;

  /* word 4 */
  Uint32 buffer_ptr;

  /* word 5 */
  Uint32 next_desc_ptr;

  /* word 6 */
  Uint32 orig_buff0_pool : 4;
  Uint32 orig_buff0_refc : 6;
  Uint32 orig_buff0_len  : 22;

  /* word 7 */
  Uint32 orig_buff0_ptr;

} HostPacketDescriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32 packet_length   : 22; //in bytes (4M - 1 max)
  Uint32 ps_reg_loc      : 1;  //0=PS words in desc, 1=PS words in SOP buff
  Uint32 reserved_w0     : 2;
  Uint32 packet_type     : 5;
  Uint32 type_id         : 2;  //always 0x0 (Host Packet ID)

  /* word 1 */
  Uint32 dest_tag_lo     : 8;
  Uint32 dest_tag_hi     : 8;
  Uint32 src_tag_lo      : 8;
  Uint32 src_tag_hi      : 8;

  /* word 2 */
  Uint32 pkt_return_qnum : 12;
  Uint32 pkt_return_qmgr : 2;
  Uint32 ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  Uint32 return_policy   : 1;  //0=linked packet goes to pkt_return_qnum,
                                 //1=each descriptor goes to pkt_return_qnum
  Uint32 ps_flags        : 4;
  Uint32 err_flags       : 4;
  Uint32 psv_word_count  : 6;  //number of 32-bit PS data words
  Uint32 reserved_w2     : 1;
  Uint32 epib            : 1;  //1=extended packet info block is present


  /* word 3 */
  Uint32 buffer_len      : 22;
  Uint32 reserved_w3     : 10;

  /* word 4 */
  Uint32 buffer_ptr;

  /* word 5 */
  Uint32 next_desc_ptr;

  /* word 6 */
  Uint32 orig_buff0_len  : 22;
  Uint32 orig_buff0_refc : 6;
  Uint32 orig_buff0_pool : 4;

  /* word 7 */
  Uint32 orig_buff0_ptr;

} HostPacketDescriptor;
#endif

/*********************************************************************/
/* Define the bit and word layouts for the Monolithic Pkt Descriptor.*/
/*********************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32  type_id         : 2;  //always 0x2 (Monolithic Packet ID)
  Uint32  packet_type     : 5;
  Uint32  data_offset     : 9;
  Uint32  packet_length   : 16; //in bytes (65535 max)

  /* word 1 */
  Uint32  src_tag_hi      : 8;
  Uint32  src_tag_lo      : 8;
  Uint32  dest_tag_hi     : 8;
  Uint32  dest_tag_lo     : 8;

  /* word 2 */
  Uint32  epib            : 1;  //1=extended packet info block is present
  Uint32  reserved_w2     : 1;
  Uint32  psv_word_count  : 6;  //number of 32-bit PS data words
  Uint32  err_flags       : 4;
  Uint32  ps_flags        : 4;
  Uint32  reserved_w2b    : 1;
  Uint32  ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  Uint32  pkt_return_qmgr : 2;
  Uint32  pkt_return_qnum : 12;

} MonolithicPacketDescriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32  packet_length   : 16; //in bytes (65535 max)
  Uint32  data_offset     : 9;
  Uint32  packet_type     : 5;
  Uint32  type_id         : 2;  //always 0x2 (Monolithic Packet ID)

  /* word 1 */
  Uint32  dest_tag_lo     : 8;
  Uint32  dest_tag_hi     : 8;
  Uint32  src_tag_lo      : 8;
  Uint32  src_tag_hi      : 8;

  /* word 2 */
  Uint32  pkt_return_qnum : 12;
  Uint32  pkt_return_qmgr : 2;
  Uint32  ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  Uint32  reserved_w2b    : 1;
  Uint32  ps_flags        : 4;
  Uint32  err_flags       : 4;
  Uint32  psv_word_count  : 6;  //number of 32-bit PS data words
  Uint32  reserved_w2     : 1;
  Uint32  epib            : 1;  //1=extended packet info block is present

} MonolithicPacketDescriptor;
#endif

/****************************************************************************/
/* Define the bit and word layouts for the SRIO Type 9 message TX descriptor*/
/****************************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.

  /* word 1 */
  Uint32  StreamID  : 16; 	//Stream ID for the transaction
  Uint32  reserved0 : 5;
  Uint32  TT        : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  reserved1 : 1;
  Uint32  COS       : 8; 	//Class of Service

} SRIO_Type9_Message_TX_Descriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.

  /* word 1 */
  Uint32  COS       : 8; 	//Class of Service
  Uint32  reserved1 : 1;
  Uint32  TT        : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  reserved0 : 5;
  Uint32  StreamID  : 16; 	//Stream ID for the transaction

} SRIO_Type9_Message_TX_Descriptor;
#endif

/****************************************************************************/
/* Define the bit and word layouts for the SRIO Type 9 message RX descriptor*/
/****************************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.

  /* word 1 */
  Uint32  StreamID  : 16; 	//Stream ID for the transaction
  Uint32  reserved0 : 1;
  Uint32  PRI       : 4; 	//Message Priority (VC||PRIO||CRF)
  Uint32  T         : 1; 	//This is TT [0] bit only.
  Uint32  CC        : 2; 	//Completion Code
  Uint32  COS       : 8; 	//Class of Service

} SRIO_Type9_Message_RX_Descriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.

  /* word 1 */
  Uint32  COS       : 8; 	//Class of Service
  Uint32  CC        : 2; 	//Completion Code
  Uint32  T         : 1; 	//This is TT [0] bit only.
  Uint32  PRI       : 4; 	//Message Priority (VC||PRIO||CRF)
  Uint32  reserved0 : 1;
  Uint32  StreamID  : 16; 	//Stream ID for the transaction

} SRIO_Type9_Message_RX_Descriptor;
#endif

/****************************************************************************/
/* Define the bit and word layouts for the SRIO Type 11 message RX descriptor*/
/****************************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.

  /* word 1 */
  Uint32  reserved0 : 15; 	
  Uint32  CC        : 2; 	//Completion Code
  Uint32  PRI       : 4; 	//Message Priority (VC||PRIO||CRF)
  Uint32  TT        : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  LTR       : 3; 	//Destination Letter
  Uint32  MailBox   : 6; 	//Destination Mailbox

} SRIO_Type11_Message_RX_Descriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.

  /* word 1 */
  Uint32  MailBox   : 6; 	//Destination Mailbox
  Uint32  LTR       : 3; 	//Destination Letter
  Uint32  TT        : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  PRI       : 4; 	//Message Priority (VC||PRIO||CRF)
  Uint32  CC        : 2; 	//Completion Code
  Uint32  reserved0 : 15; 	

} SRIO_Type11_Message_RX_Descriptor;
#endif

/****************************************************************************/
/* Define the bit and word layouts for the SRIO Type 11 message TX descriptor*/
/****************************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  Uint32  SRC_ID      : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.
  Uint32  Dest_ID     : 16; //Destination Node ID the message was sent to.

  /* word 1 */
  Uint32  reserved0   : 5; 	
  Uint32  Retry_Count : 6; 	//Message Retry Count
  Uint32  SSIZE       : 4; 	//standard message payload size.
  Uint32  reserved1   : 6; 	
  Uint32  TT          : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  LTR         : 3; 	//Destination Letter
  Uint32  MailBox     : 6; 	//Destination Mailbox

} SRIO_Type11_Message_TX_Descriptor;
#else
typedef struct
{
  /* word 0 */
  Uint32  Dest_ID   : 16; //Destination Node ID the message was sent to.
  Uint32  SRC_ID    : 16;  //Source Node Id ！ Unique Node identifier of the Source of the message.

  /* word 1 */
  Uint32  MailBox     : 6; 	//Destination Mailbox
  Uint32  LTR         : 3; 	//Destination Letter
  Uint32  TT          : 2; 	//RapidIO tt field specifying 8 or 16bit DeviceIDs
  Uint32  reserved1   : 6; 	
  Uint32  SSIZE       : 4; 	//standard message payload size.
  Uint32  Retry_Count : 6; 	//Message Retry Count
  Uint32  reserved0   : 5; 	

} SRIO_Type11_Message_TX_Descriptor;
#endif


#endif 
