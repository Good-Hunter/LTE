#ifndef MNAV_STRUCT_H_
#define MNAV_STRUCT_H_

/*******************************************************************/
/* Define the bit and word layouts for the Accumulator Command     */
/* signed interface Structure only.                                       */
/*******************************************************************/

#ifdef _BIG_ENDIAN
typedef struct
{
  unsigned int retrn_code      :8; //0=idle, 1=success, 2-6=error
  unsigned int un1             :8;
  unsigned int command         :8; //0x80=disable, 0x81=enable, 0=firmware response
  unsigned int channel         :8; //0 to 47 or 0 to 15
  unsigned int queue_mask;         //(multi-mode only) bit 0=qm_index queue
  unsigned int list_address;       //address of Host ping-pong buffer
  unsigned int max_entries     :16;//max entries per list
  unsigned int qm_index        :16; //qnum to monitor (multiple of 32 for multimode)
  unsigned int un2             :8;
  unsigned int cfg_un          :2;
  unsigned int cfg_multi_q     :1; //0=single queue mode, 1=multi queue mode
  unsigned int cfg_list_mode   :1;//0=NULL terminate, 1=entry count mode
  unsigned int cfg_list_size   :2;//0="D" Reg, 1="C+D" regs, 2="A+B+C+D"
  unsigned int cfg_int_delay   :2;//0=none, 1=last int, 2=1st new, 3=last new
  unsigned int timer_count     :16;//number of 25us timer ticks to delay int
} MNav_AccCmd;
#else
typedef struct
{
  unsigned int channel         :8; //0 to 47 or 0 to 15
  unsigned int command         :8; //0x80=disable, 0x81=enable, 0=firmware response
  unsigned int un1             :8;
  unsigned int retrn_code      :8; //0=idle, 1=success, 2-6=error
  unsigned int queue_mask;         //(multi-mode only) bit 0=qm_index queue
  unsigned int list_address;       //address of Host ping-pong buffer
  unsigned int qm_index        :16; //qnum to monitor (multiple of 32 for multimode)
  unsigned int max_entries     :16;//max entries per list
  unsigned int timer_count     :16;//number of 25us timer ticks to delay int
  unsigned int cfg_int_delay   :2;//0=none, 1=last int, 2=1st new, 3=last new
  unsigned int cfg_list_size   :2;//0="D" Reg, 1="C+D" regs, 2="A+B+C+D"
  unsigned int cfg_list_mode   :1;//0=NULL terminate, 1=entry count mode
  unsigned int cfg_multi_q     :1; //0=single queue mode, 1=multi queue mode
  unsigned int cfg_un          :2;
  unsigned int un2             :8;
} MNav_AccCmd;
#endif


/*******************************************************************/
/* Define the bit and word layouts for the Rx Flow Configuration     */
/* signed interface Structure only.  Big endian only                               */
/*******************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
/** Rx flow configuration register A */

   unsigned short  un1					: 1;
   /** This field controls whether or not the Extended Packet Info Block will be present in the Rx Packet Descriptor.  
    * 0 - The port DMA will clear the Extended Packet Info Block Present bit in the PD and will drop any extended 
    * packet info words that are presented from the back end application. 
    * 1 - The port DMA will set the Extended Packet Info Block Present bit in the PD and will copy any extended packet
    * info words that are presented across the Rx streaming interface into the extended packet info words in the descriptor.
    * If no extended packet info words are presented from the back end application, the port DMA will overwrite the fields with zeroes.
    */
   unsigned short  rx_einfo_present		: 1;
   /** This field controls whether or not the Protocol Specific words will be present in the Rx Packet Descriptor 
    * 0 - The port DMA will set the PS word count to 0 in the PD and will drop any PS words that are presented 
    * from the back end application.
    * 1 - The port DMA will set the PS word count to the value given by the back end application and will copy 
    * the PS words from the back end application to the location 
    */
   unsigned short  rx_psinfo_present	: 1;
   /** This field controls the error handling mode for the flow and is only used when channel errors occurs 
    * 0 = Starvation errors result in dropping packet and reclaiming any used descriptor or buffer resources 
    * back to the original queues/pools they were allocated to
    * 1 = Starvation errors result in subsequent re-try of the descriptor allocation operation.  
    */
   unsigned short  rx_error_handling	: 1;
   /** This field indicates the descriptor type to use 1 = Host, 2 = Monolithic */
   unsigned char   rx_desc_type			: 2;
   /** This field controls where the Protocol Specific words will be placed in the Host Mode CPPI data structure 
    * 0 - protocol specific information is located in descriptor 
    * 1 - protocol specific information is located in SOP buffer */
   unsigned short  rx_ps_location		: 1;
   /** This field specifies the number of bytes that are to be skipped in the SOP buffer before beginning 
    * to write the payload or protocol specific bytes(if they are in the sop buffer).  This value must
    * be less than the minimum size of a buffer in the system */
   unsigned short  rx_sop_offset		: 9;
   unsigned short  un2					: 2;
   /** This field indicates the default receive queue manager that this channel should use */
   unsigned short  rx_dest_qmgr			: 2;
   /** This field indicates the default receive queue that this channel should use */
   unsigned short  rx_dest_qnum			: 12;
} reg_a;

typedef struct
{
/** Rx flow configuration register B */

   /** This is the value to insert into bits 15:8 of the source tag if the rx_src_tag_hi_sel is set to 1 */
   unsigned char   rx_src_tag_hi		: 8;    
   /** This is the value to insert into bits 7:0 of the source tag if the rx_src_tag_lo_sel is set to 1 */
   unsigned char   rx_src_tag_lo		: 8;
   /** This is the value to insert into bits 15:8 of the destination tag if the rx_dest_tag_hi_sel is set to 1 */
   unsigned char   rx_dest_tag_hi		: 8;
   /** This is the value to insert into bits 7:0 of the destination tag if the rx_dest_tag_lo_sel is set to 1 */
   unsigned char   rx_dest_tag_lo		: 8;
} reg_b;

typedef struct
{
/** Rx flow configuration register C */

   unsigned char   un1					: 1;
   /** This field specifies the source for bits 15:8 of the source tag field in the output packet descriptor.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_src_tag_hi
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with src_tag[7:0] from back end application
     * 5 = RESERVED
     * 6-7 = RESERVED
     */
   unsigned char   rx_src_tag_hi_sel	: 3;    
   unsigned char   un2					: 1;
   /** This field specifies the source for bits 7:0 of the source tag field in the output packet descriptor.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_src_tag_lo
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with src_tag[7:0] from back end application
     * 5 = RESERVED
     * 6-7 = RESERVED
     */
   unsigned char   rx_src_tag_lo_sel	: 3;
   unsigned char   un3					: 1;
   /** This field specifies the source for bits 15:8 of the source tag field in the word 1 of the output PD.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_dest_tag_hi
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with dest_tag[7:0] from back end application
     * 5 = overwrite with dest_tag[15:8] from back end application
     * 6-7 = RESERVED
     */
   unsigned char   rx_dest_tag_hi_sel	: 3;
   unsigned char   un4					: 1;
   /** This field specifies the source for bits 7:0 of the source tag field in word 1 of the output PD.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_dest_tag_lo
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with dest_tag[7:0] from back end application
     * 5 = overwrite with dest_tag[15:8] from back end application
     * 6-7 = RESERVED
     */    
   unsigned char   rx_dest_tag_lo_sel	: 3;
   unsigned short   un5					: 13;
   /** This bits control whether or not the flow will compare the packet size received from the back end application 
     * against the rx_size_thresh2 fields to determine which FDQ to allocate the SOP buffer from.  
     * The bits in this field is encoded as follows:
     * 0 = Do not use the threshold.
     * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz2_qnum/rx_fdq0_sz2_qmgr.
     */
   unsigned char   rx_size_thresh2_en	: 1;
   /** This bits control whether or not the flow will compare the packet size received from the back end application 
    * against the rx_size_thresh1 fields to determine which FDQ to allocate the SOP buffer from.  
    * The bits in this field is encoded as follows:
    * 0 = Do not use the threshold.
    * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz1_qnum/rx_fdq0_sz1_qmgr.
    */
   unsigned char   rx_size_thresh1_en	: 1;
   /** This bits control whether or not the flow will compare the packet size received from the back end application 
    * against the rx_size_thresh0 fields to determine which FDQ to allocate the SOP buffer from.  
    * The bits in this field is encoded as follows:
    * 0 = Do not use the threshold.
    * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz0_qnum/rx_fdq0_sz0_qmgr.
    */
   unsigned char   rx_size_thresh0_en	: 1;
} reg_c;

typedef struct
{
/** Rx flow configuration register D */

   unsigned short  un1					: 2;
   /** This field specifies which Queue Manager should be used for the 1st Rx buffer in a packet whose size 
     * is less than or equal to the rx_size0 value */
   unsigned short  rx_fdq0_sz0_qmgr		: 2;
   /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a packet whose 
     * size is less than or equal to the rx_size0 value */
   unsigned short  rx_fdq0_sz0_qnum		: 12;
   unsigned short  un2					: 2;
   /** This field specifies which Queue Manager should be used for the 2nd Rx buffer in a host type packet */
   unsigned short  rx_fdq1_qmgr			: 2;
   /** This field specifies which Free Descriptor Queue should be used for the 2nd Rx buffer in a host type packet */
   unsigned short  rx_fdq1_qnum			: 12;
} reg_d;

typedef struct
{
/** Rx flow configuration register E */

   unsigned short  un1					: 2;
   /** This field specifies which Queue Manager should be used for the 3rd Rx buffer in a host type packet */
   unsigned short  rx_fdq2_qmgr			: 2;
   /** This field specifies which Free Descriptor Queue should be used for the 3rd Rx buffer in a host type packet */
   unsigned short  rx_fdq2_qnum			: 12;
   unsigned short  un2					: 2;
   /** This field specifies which Queue Manager should be used for the 4th or later Rx buffers 
     * in a host type packet */
   unsigned short  rx_fdq3_qmgr			: 2;
   /** This field specifies which Free Descriptor Queue should be used for the 4th or later Rx
     *  buffers in a host type packet */
   unsigned short  rx_fdq3_qnum			: 12;
} reg_e;

typedef struct
{
/** Rx flow configuration register F */

   /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the packet size is less than or equal to the value 
     * given in this threshold, the DMA controller in the port will allocate the SOP buffer from the queue given by 
     * the rx_fdq0_sz0_qmgr and rx_fdq0_sz0_qnum fields. This field is optional.
     */
   unsigned short  rx_size_thresh0		: 16;
   /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the  packet size is greater than the rx_size_thresh0 
     * but is less than or equal to the value given in this threshold, the DMA controller in the port will allocate the 
     * SOP buffer from the queue given by the rx_fdq0_sz1_qmgr and rx_fdq0_sz1_qnum fields. 
     * If enabled, this value must be greater than the value given in the rx_size_thresh0 field. This field is optional.
     */
   unsigned short  rx_size_thresh1		: 16;
} reg_f;

typedef struct
{
/** Rx flow configuration register G */

   /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the  packet size is less than or equal to the value
     * given in this threshold, the DMA controller in the port will allocate the SOP buffer from the queue given by the 
     * rx_fdq0_sz2_qmgr and rx_fdq0_sz2_qnum fields.
     * If enabled, this value must be greater than the value given in the rx_size_thresh1 field. This field is optional.
     */
   unsigned short  rx_size_thresh2		: 16;
   unsigned short  un1					: 2;
   /** This field specifies which Queue Manager should be used for the 1st Rx buffer in a packet whose size 
     * is less than or equal to the rx_size0 value */
   unsigned short  rx_fdq0_sz1_qmgr		: 2;
   /** This field specifies which Queue should be used for the 1st Rx buffer in a packet whose size is 
     * less than or equal to the rx_size0 value */
   unsigned short  rx_fdq0_sz1_qnum		: 12;
} reg_g;

typedef struct
{
/** Rx flow configuration register H */

   unsigned short  un1				: 2;
   /** This field specifies which Free Descriptor Queue Manager should be used for the 1st Rx buffer in a packet 
     * whose size is less than or equal to the rx_size2 value */
   unsigned short  rx_fdq0_sz2_qmgr : 2;
   /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a packet 
     * whose size is less than or equal to the rx_size2 value */
   unsigned short  rx_fdq0_sz2_qnum : 12;
   unsigned short  un2				: 2;
   /** This field specifies which Free Descriptor Queue Manager should be used for the 1st Rx buffer in a 
     * packet whose size is less than or equal to the rx_size3 value */
   unsigned short  rx_fdq0_sz3_qmgr : 2;
   /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a
     * packet whose size is less than or equal to the rx_size3 value */
   unsigned short  rx_fdq0_sz3_qnum : 12;
} reg_h;

typedef struct
{
  /** Rx flow configuration register A */
  reg_a rx_flow_cfg_reg_A;
  /** Rx flow configuration register B */
  reg_b rx_flow_cfg_reg_B;
  /** Rx flow configuration register C */
  reg_c rx_flow_cfg_reg_C;
  /** Rx flow configuration register D*/
  reg_d rx_flow_cfg_reg_D;
  /** Rx flow configuration register E */
  reg_e rx_flow_cfg_reg_E;
  /** Rx flow configuration register F */
  reg_f rx_flow_cfg_reg_F;
  /** Rx flow configuration register G */
  reg_g rx_flow_cfg_reg_G;
  /** Rx flow configuration register H */
  reg_h rx_flow_cfg_reg_H;
} MNav_RxFlowCfg;
#endif



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
  unsigned int orig_buff0_pool : 4;
  unsigned int orig_buff0_refc : 6;
  unsigned int orig_buff0_len  : 22;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} MNAV_HostPacketDescriptor_fftc;
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
  unsigned int orig_buff0_refc : 6;
  unsigned int orig_buff0_pool : 4;

  /* word 7 */
  unsigned int orig_buff0_ptr;

} MNAV_HostPacketDescriptor_fftc;
#endif


/*********************************************************************/
/* Define the bit and word layouts for the Monolithic Pkt Descriptor.*/
/*********************************************************************/
#ifdef _BIG_ENDIAN
typedef struct
{
  /* word 0 */
  unsigned int  type_id         : 2;  //always 0x2 (Monolithic Packet ID)
  unsigned int  packet_type     : 5;
  unsigned int  data_offset     : 9;
  unsigned int  packet_length   : 16; //in bytes (65535 max)

  /* word 1 */
  unsigned int  src_tag_hi      : 8;
  unsigned int  src_tag_lo      : 8;
  unsigned int  dest_tag_hi     : 8;
  unsigned int  dest_tag_lo     : 8;

  /* word 2 */
  unsigned int  epib            : 1;  //1=extended packet info block is present
  unsigned int  reserved_w2     : 1;
  unsigned int  psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int  err_flags       : 4;
  unsigned int  ps_flags        : 4;
  unsigned int  reserved_w2b    : 1;
  unsigned int  ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int  pkt_return_qmgr : 2;
  unsigned int  pkt_return_qnum : 12;

} MNAV_MonolithicPacketDescriptor;
#else
typedef struct
{
  /* word 0 */
  unsigned int  packet_length   : 16; //in bytes (65535 max)
  unsigned int  data_offset     : 9;
  unsigned int  packet_type     : 5;
  unsigned int  type_id         : 2;  //always 0x2 (Monolithic Packet ID)

  /* word 1 */
  unsigned int  dest_tag_lo     : 8;
  unsigned int  dest_tag_hi     : 8;
  unsigned int  src_tag_lo      : 8;
  unsigned int  src_tag_hi      : 8;

  /* word 2 */
  unsigned int  pkt_return_qnum : 12;
  unsigned int  pkt_return_qmgr : 2;
  unsigned int  ret_push_policy : 1;  //0=return to queue tail, 1=queue head
  unsigned int  reserved_w2b    : 1;
  unsigned int  ps_flags        : 4;
  unsigned int  err_flags       : 4;
  unsigned int  psv_word_count  : 6;  //number of 32-bit PS data words
  unsigned int  reserved_w2     : 1;
  unsigned int  epib            : 1;  //1=extended packet info block is present

} MNAV_MonolithicPacketDescriptor;
#endif

#define MNAV_MONO_PACKET_SIZE  sizeof(MNAV_MonolithicPacketDescriptor)

/*********************************************************************/
/* Define the word layout of the Extended Packet Info Block. It is   */
/* optional and may follow Host Packet and Monolithic descriptors.   */
/*********************************************************************/
typedef struct
{
  /* optional word 0 */
  unsigned int timestamp;
  /* optional word 1 */
  unsigned int sw_info0;
  /* optional word 2 */
  unsigned int sw_info1;
  /* optional word 3 */
  unsigned int sw_info2; 
  /* PS word 0 */
  unsigned int ps_data0;
   
} MNAV_ExtendedPacketInfoBlock;




#endif /*MNAV_STRUCT_H_*/
