
#ifndef _BCP_LLD_H_
#define _BCP_LLD_H_

#include <stdint.h>
#include <stdlib.h>

/* BCP CSL Register file and CSL definitions include */
#include "csl.h"
#include "cslr_bcp.h"
#include "soc.h"

/**
@defgroup BCP_LLD_SYMBOL  BCP LLD Symbols Defined
@ingroup BCP_LLD_API
*/



#ifdef __cplusplus
extern "C"
{
#endif

       
#define     BCP_MAX_NUM_TXQUEUES                    (8)

       
#define     BCP_MAX_NUM_TM_FLOWS                    (64)

     
#define     BCP_MAX_NUM_PS_WORDS                    (8)                 

       
#define     BCP_MAX_NUM_USER_INFO_WORDS             (124)                 

/** Well know submodule engine header lengths. */        
#define     BCP_TM_HDR_LEN                          (1)

/** Number of Submodules in BCP */
#define     BCP_NUM_SUBMODULES                      (14)        

/**
@}
*/

/** @addtogroup BCP_LLD_DATASTRUCT
 @{ */

typedef     CSL_Enc_mmrRegs*                        Bcp_EncRegsOvly;
typedef     CSL_Cor_mmrRegs*                        Bcp_CorRegsOvly;
typedef     CSL_Bcp_tmRegs*                         Bcp_TmRegsOvly;
typedef     CSL_Dio_mmrRegs*                        Bcp_DioRegsOvly;
typedef     CSL_Bcp_IntRegs*                        Bcp_IntRegsOvly;
typedef     CSL_Bcp_DataLoggerRegs*                 Bcp_DlgRegsOvly;
typedef     CSL_Bcp_DataLoggerRAM*                  Bcp_DlgRamRegsOvly;


typedef struct _Bcp_LldObj
{
    /** BCP Peripheral instance number */
    unsigned char                     instNum;

    /** Config Registers Base address for each of submodules */
    void*                       modCfgRegs [BCP_NUM_SUBMODULES]; 

    /** signed interrupt Registers Base address for each of submodules */
    Bcp_IntRegsOvly             modIntRegs [BCP_NUM_SUBMODULES]; 

    /** Data Logger Registers Base address for each of submodules */
    Bcp_DlgRegsOvly             modDlgRegs [BCP_NUM_SUBMODULES];

    /** Data Logger RAM Registers Base address for each of submodules */
    Bcp_DlgRamRegsOvly          modDlgRamRegs [BCP_NUM_SUBMODULES];
} Bcp_LldObj;

      
typedef enum   
{
    /** BCP Tx Queue 0  */
    Bcp_QueueId_0                                  = 0,
    /** BCP Tx Queue 1  */
    Bcp_QueueId_1                                  = 1,
    /** BCP Tx Queue 2  */
    Bcp_QueueId_2                                  = 2,
    /** BCP Tx Queue 3  */
    Bcp_QueueId_3                                  = 3,
    /** BCP Tx Queue 4  */
    Bcp_QueueId_4                                  = 4,
    /** BCP Tx Queue 5  */
    Bcp_QueueId_5                                  = 5,
    /** BCP Tx Queue 6  */
    Bcp_QueueId_6                                  = 6,
    /** BCP Tx Queue 7  */
    Bcp_QueueId_7                                  = 7
} Bcp_QueueId;

       
typedef enum   
{
    /** TM */
    Bcp_ModuleId_TM                                 = 0,
    /** signed interleaver engine (4) */
    Bcp_ModuleId_INT                                = 4,
    /** Rate Modulation engine (5) */
    Bcp_ModuleId_RM                                 = 5,
    /** Encoder engine (6) */
    Bcp_ModuleId_ENC                                = 6,
    /** Modulation engine (7) */
    Bcp_ModuleId_MOD                                = 7,
    /** CRC engine (8) */
    Bcp_ModuleId_CRC                                = 8,
    /** Soft Slicer engine (9) */
    Bcp_ModuleId_SSL                                = 9,
    /** Rate de-matcher engine (10) */
    Bcp_ModuleId_RD                                 = 10,
    /** Correlation engine (11) */
    Bcp_ModuleId_COR                                = 11,
    /** De-interleaver engine (12) */
    Bcp_ModuleId_DNT                                = 12,
    /** Direct Input Output (13) */
    Bcp_ModuleId_DIO                                = 13
} Bcp_ModuleId;

      
typedef enum   
{
    /** Normal Packet  */
    Bcp_PacketType_Normal               = 0,
    /** Start of multi-packet packet  */
    Bcp_PacketType_SOM                  = 1,
    /** Middle of multi-packet packet  */
    Bcp_PacketType_MOM                  = 2,
    /** End of multi-packet packet  */
    Bcp_PacketType_EOM                  = 3
} Bcp_PacketType;

       
typedef enum   
{
    /** LTE  */
    Bcp_RadioStd_LTE                    = 0,
    /** WiMax 802.16e  */
    Bcp_RadioStd_WIMAX_802_16E          = 2,
    /** WCDMA R99  */
    Bcp_RadioStd_WCDMA_R99              = 4,
    /** HSPA  */
    Bcp_RadioStd_HSPA                   = 5
} Bcp_RadioStd;

     
typedef enum   
{
    /** 128 bit words */
    Bcp_EndianFormat_128                = 0,
    /** 32 bit words */
    Bcp_EndianFormat_32                 = 1,
    /** 16 bit words */
    Bcp_EndianFormat_16                 = 2,
    /** 8 bit words */
    Bcp_EndianFormat_8                  = 3
} Bcp_EndianFormat;

     
typedef enum   
{
    /** No change */
    Bcp_DataFormat_NoChange             = 0,
    /** Reverse 32 bit words */
    Bcp_DataFormat_Reverse32            = 1,
    /** Reverse 16 bit words */
    Bcp_DataFormat_Reverse16            = 2,
    /** Reverse 8 bit words */
    Bcp_DataFormat_Reverse8             = 3,
    /** Swap 32 bit words */
    Bcp_DataFormat_Swap32               = 5,
    /** Swap 16 bit words */
    Bcp_DataFormat_Swap16               = 6,
    /** Swap 8 bit words */
    Bcp_DataFormat_Swap8                = 7
} Bcp_DataFormat;

     
typedef struct _Bcp_GlobalHdrCfg
{
    /** Global Header Word 0 */        
    
    /** Packet type */        
    Bcp_PacketType              pkt_type;

    /** Flush bit. When enabled stops Tx arbitration of 
     *  QFIFO until the CDMAHP finishes writing the resulting
     *  packet back into memory.
     */
    unsigned char                     flush;

    /** Drop bit. Used to fill CDMAHP_FIFO with unused data 
     *  so flush does not end with stale data in CDMAHP_FIFO.
     */
    unsigned char                     drop;

    /** Halt bit. When set, generates a soft stop with 
     *  an interrupt when BCP goes idle.
     */
    unsigned char                     halt;

    /** Radio standard to use for the flow */
    Bcp_RadioStd                radio_standard;

    /** Number of 32 bit words with header information. */
    unsigned char                     hdr_end_ptr;

    /** Flow Id */
    unsigned char                     flow_id;

    /** Global Header Word 1 */        
    
    /** Application defined destination tag info to be
     *  passed to Rx end. 
     *
     *  Can be set to a unique sequence number to identify 
     *  the packet.
     */
    uint16_t                    destn_tag;
} Bcp_GlobalHdrCfg;

    
typedef struct _Bcp_TmHdrCfg
{
    /** Number of 32 bit words of PS data to be streamed across 
     *
     *  Valid values are: 0 - 8
     */        
    unsigned char                     ps_data_size;

    /** Number of 128 bit words of information. 
     *
     *  Valid values are: 0 - 31
     */
    unsigned char                     info_data_size;
} Bcp_TmHdrCfg;
            
        
typedef enum   
{
    /** CRC 0  */
    Bcp_CrcFormat_Crc0                  = 0,
    /** CRC 8  */
    Bcp_CrcFormat_Crc8                  = 1,
    /** CRC 12  */
    Bcp_CrcFormat_Crc12                 = 2,
    /** CRC 16  */
    Bcp_CrcFormat_Crc16                 = 3,
    /** CRC 16w  */
    Bcp_CrcFormat_Crc16w                = 4,
    /** CRC 24a  */
    Bcp_CrcFormat_Crc24a                = 5,
    /** CRC 24b  */
    Bcp_CrcFormat_Crc24b                = 6,
    /** CRC 32  */
    Bcp_CrcFormat_Crc32                 = 7
} Bcp_CrcFormat;

    
typedef struct _Bcp_CrcHdrCfg
{
    /** CRC Header Word 0 */        

    /** Local header length (Not including Header word 0). 
     *
     *  Set to 8 for Long header format and all the 8 words of
     *  configuration will be used in CRC header. Set to 3 for
     *  Short header format and only the first 3 words of configuration
     *  are used in CRC header. 
     */
    unsigned char                     local_hdr_len;        

    /** Number of filler bits to use.
     *
     *  Number of filler bits (zeros) to insert at beginning of 
     *  each packet payload. Can range between 0-255.
     */
    unsigned char                     filler_bits;

    /** unsigned shortean flag to indicate if CRC bits must be appended
     *  in LTE / WCDMA order.
     *
     *  Set to 1 for LTE order and 0 for WCDMA.
     */
    unsigned char                     bit_order;

    /** DTX Data format.
     *
     *  Set to 1 for 2-bit data (DTX:data) and 0 for 1-bit
     *  data (data only).
     */
    unsigned char                     dtx_format;

    /** CRC Header Word 1 */        

    /** Scrambler initialization value for 8bits/clock. */
    unsigned int                    num_scramble_sys;

    /** CRC Header Word 2 */        

    /** CRC Method 2 Id. */
    unsigned int                    method2_id;

    /** CRC Header Word 3 */        

    /** Voice A Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    va_blk_len;

    /** Voice A CRC format. */
    Bcp_CrcFormat               va_crc;

    /** Voice A Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     va_blks;

    /** CRC Header Word 4 */        

    /** Voice B Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    vb_blk_len;

    /** Voice B CRC format. */
    Bcp_CrcFormat               vb_crc;

    /** Voice B Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     vb_blks;

    /** CRC Header Word 5 */        

    /** Voice C Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    vc_blk_len;

    /** Voice C CRC format. */
    Bcp_CrcFormat               vc_crc;

    /** Voice C Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     vc_blks;

    /** CRC Header Word 6 */        

    /** Data 1 Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    d1_blk_len;

    /** Data 1 CRC format. */
    Bcp_CrcFormat               d1_crc;

    /** Data 1 Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     d1_blks;

    /** CRC Header Word 7 */        

    /** Data 2 Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    d2_blk_len;

    /** Data 2 CRC format. */
    Bcp_CrcFormat               d2_crc;

    /** Data 2 Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     d2_blks;

    /** CRC Header Word 8 */        

    /** DCCH Transport Block Length. 
     *
     *  Must be set to number of data bits in each transport block
     *  of the transport channel.
     */
    unsigned int                    dc_blk_len;

    /** DCCH CRC format. */
    Bcp_CrcFormat               dc_crc;

    /** DCCH Blocks - Number of transport blocks in this transport
     *  channel.
     *
     *  Can be any value between 0-255.
     */
    unsigned char                     dc_blks;
} Bcp_CrcHdrCfg;

   
typedef struct _Bcp_RmHdr_ChannelCfg
{
    /** Channel 'n' length before rate matching */
    uint16_t                    sys_len;

    /** Channel 'n' initial value for 2nd rate matching loop */
    uint16_t                    sys_init2;

    /** Channel 'n' minus value for 2nd rate matching loop */
    uint16_t                    sys_minus2;

    /** Channel 'n' plus value for 2nd rate matching loop */
    uint16_t                    sys_plus2;

    /** Channel 'n' TD-SCDMA alpha value for bit separation */
    unsigned char                     sys_aplha;

    /** Channel 'n' TD-SCDMA beta value for bit separation */
    unsigned char                     sys_beta;

    /** Puncture flag 
     *
     *  Set to:
     *      0   -   Perform repeat in second rate matching loop
     *      1   -   Perform puncture in second rate matching loop
     */
    unsigned char                     sys_puncture;

    /** Channel type.
     *
     *  Set to:
     *      0   -   Convolutional or Rel-99 Turbo Repeat
     *      1   -   Rel-99 style Turbo puncture, use p0 parity params
     *      2   -   Rel-99 style Turbo puncture, use p1 parity params
     */
    unsigned char                     sys_turbo;
}Bcp_RmHdr_ChannelCfg;

  
typedef struct _Bcp_RmHdr_xCdmaCfg
{
    /** RM Header Word 0 */        

    /** Local header length (Not including Header word 0). 
     *
     *  Can be 20, 23, 26, 29, 32, 35, 39.
     */
    unsigned char                     local_hdr_len;        

    /** Input ordering mode.
     *
     *  Set to:
     *      0   - Bit interleaved from signed interleaver module.
     *      1   - Quad word interleaved from Encoder module.
     */
    unsigned char                     input_order;

    /** Input encoding.
     *
     *  Set to:
     *      0   - 1/3 rate convolution or turbo encoded.
     *      1   - 1/2 rate convolution encoded.
     */
    unsigned char                     half_rate;

    /** RM Header Word 1 */

    /** Collect columns - Number of columns in bit collection table. */
    unsigned int                    collect_cols;

    /** Collect rows - Number of rows in bit collection table. */
    unsigned char                     collect_rows;

    /** RM Header Word 2 */

    /** Scrambler initialization value for TD-SCDMA systematic HSDPA. */
    uint16_t                    num_scram;

    /** RM Header Word 3 */
    /** Sys0 length before rate-matching. */
    unsigned int                    sys0_len;

    /** RM Header Word 4 */
    /** Sys0 initial value for 2nd rate matching loop. */
    unsigned int                    sys0_init2;

    /** RM Header Word 5 */
    /** Sys0 minus value for 2nd rate matching loop. */
    unsigned int                    sys0_minus2;

    /** RM Header Word 6 */
    /** Sys0 plus value for 2nd rate matching loop. */
    unsigned int                    sys0_plus2;

    /** Sys0 TD-SDCDMA aplha value for bit separation. */
    unsigned char                     sys0_alpha;

    /** Sys0 TD-SDCDMA beta value for bit separation. */
    unsigned char                     sys0_beta;

    /** Puncture flag 
     *
     *  Set to:
     *      0   -   Perform repeat in second rate matching loop
     *      1   -   Perform puncture in second rate matching loop
     */
    unsigned char                     sys0_puncture;

    /** Channel type.
     *
     *  Set to:
     *      0   -   Convolutional or Rel-99 Turbo Repeat
     *      1   -   Rel-99 style Turbo puncture, use p0 parity params
     *      2   -   Rel-99 style Turbo puncture, use p1 parity params
     *      3   -   HSDPA style turbo channel, use p0 params.
     */
    unsigned char                     sys0_turbo;

    /** RM Header Word 7 */
    /** p0 Parity1 length before rate matching */
    unsigned int                    p0_par1_len;

    /** RM Header Word 8 */
    /** p0 Parity1 initial value for 1st rate matching loop */
    unsigned int                    p0_par1_init1;

    /** RM Header Word 9 */
    /** p0 Parity1 minus value for 1st rate matching loop */
    unsigned int                    p0_par1_minus1;

    /** RM Header Word 10 */
    /** p0 Parity1 plus value for 1st rate matching loop */
    unsigned int                    p0_par1_plus1;

    /** RM Header Word 11 */
    /** p0 Parity1 initial value for 2nd rate matching loop */
    unsigned int                    p0_par1_init2;

    /** RM Header Word 12 */
    /** p0 Parity1 minus value for 2nd rate matching loop */
    unsigned int                    p0_par1_minus2;

    /** RM Header Word 13 */
    /** p0 Parity1 plus value for 2nd rate matching loop */
    unsigned int                    p0_par1_plus2;

    /** RM Header Word 14 */
    /** p0 Parity2 length before rate matching */
    unsigned int                    p0_par2_len;

    /** RM Header Word 15 */
    /** p0 Parity2 initial value for 1st rate matching loop */
    unsigned int                    p0_par2_init1;

    /** RM Header Word 16 */
    /** p0 Parity2 minus value for 1st rate matching loop */
    unsigned int                    p0_par2_minus1;

    /** RM Header Word 17 */
    /** p0 Parity2 plus value for 1st rate matching loop */
    unsigned int                    p0_par2_plus1;

    /** RM Header Word 18 */
    /** p0 Parity2 initial value for 2nd rate matching loop */
    unsigned int                    p0_par2_init2;

    /** RM Header Word 19 */
    /** p0 Parity2 minus value for 2nd rate matching loop */
    unsigned int                    p0_par2_minus2;

    /** RM Header Word 20 */
    /** p0 Parity2 plus value for 2nd rate matching loop */
    unsigned int                    p0_par2_plus2;

    /** RM Header Word 21-35 */
    Bcp_RmHdr_ChannelCfg        channelCfg [5];

    /** RM Header Word 36 */
    /** p1 Parity1 length before rate matching */
    uint16_t                    p1_par1_len;

    /** p1 Parity1 initial value for 2nd rate matching loop */
    uint16_t                    p1_par1_init2;

    /** RM Header Word 37 */
    /** p1 Parity1 minus value for 2nd rate matching loop */
    uint16_t                    p1_par1_minus2;

    /** p1 Parity1 plus value for 2nd rate matching loop */
    uint16_t                    p1_par1_plus2;

    /** RM Header Word 38 */
    /** p1 Parity2 length before rate matching */
    uint16_t                    p1_par2_len;

    /** p1 Parity2 initial value for 2nd rate matching loop */
    uint16_t                    p1_par2_init2;

    /** RM Header Word 39 */
    /** p1 Parity2 minus value for 2nd rate matching loop */
    uint16_t                    p1_par2_minus2;

    /** p1 Parity2 plus value for 2nd rate matching loop */
    uint16_t                    p1_par2_plus2;
} Bcp_RmHdr_xCdmaCfg;
    
typedef struct _Bcp_RmHdr_LteCfg
{    
    /** RM Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** RM Header Word 1 */        

    /** LTE channel type.
     *
     *  Set to:
     *      0   -   Shared channel
     *      1   -   Control channel
     */
    unsigned char                     channel_type;

    /** Input bit format.
     *
     *  Set to:
     *      0   -   Input bits are hard bits stored in packet
     *              format as one bit per stream bit.
     *      1   -   Input bits are soft bits stored as one soft
     *              bit per stream byte.
     */
    unsigned char                     input_bit_format;

    /** Output bit format. */
    unsigned char                     output_bit_format;

    /** Number of filler bits (F) in the first code block. 
     *
     *  Valid values: 0-63
     */
    unsigned char                     num_filler_bits_f;

    unsigned char                     rv_start_column1;
    unsigned char                     rv_start_column2;

    /** RM Header word 2 */

    unsigned char                     param_ncb1_column;
    uint16_t                    param_ncb1_row;
    unsigned char                     param_ncb2_column;
    uint16_t                    param_ncb2_row;

    /** RM Header word 3 */

    unsigned char                     num_code_blocks_c1;
    uint16_t                    block_size_k1;

    /** RM Header word 4 */

    unsigned char                     num_code_blocks_ce1;
    uint16_t                    block_size_e1;

    /** RM Header word 5 */

    unsigned char                     num_code_blocks_c2;
    uint16_t                    block_size_k2;

    /** RM Header word 6 */

    unsigned char                     num_code_blocks_ce2;
    uint16_t                    block_size_e2;
} Bcp_RmHdr_LteCfg;

   
typedef struct _Bcp_RmHdr_WiMaxCfg
{    
    /** RM Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** RM Header Word 1 */        

    /** Number of code blocks C1 of size K1 */
    unsigned char                     num_code_blocks_c1;

    /** Code block size K1 */
    uint16_t                    block_size_k1;

    /** RM Header Word 2 */        

    /** Sub-block interleaver parameter 'm' for codeblock of size K1 */
    unsigned char                     param_m1;

    /** Sub-block interleaver parameter 'j' for codeblock of size K1 */
    unsigned char                     param_j1;

    /** Output size E1 */
    uint16_t                    block_size_e1;

    /** RM Header Word 3 */        

    /** Number of code blocks C2 of size K2 */
    unsigned char                     num_code_blocks_c2;

    /** Code block size K2 */
    uint16_t                    block_size_k2;

    /** RM Header Word 4 */        

    /** Sub-block interleaver parameter 'm' for codeblock of size K2 */
    unsigned char                     param_m2;

    /** Sub-block interleaver parameter 'j' for codeblock of size K2 */
    unsigned char                     param_j2;

    /** Output size E2 */
    uint16_t                    block_size_e2;

    /** RM Header Word 5 */        

    /** Number of code blocks C3 of size K3 */
    unsigned char                     num_code_blocks_c3;

    /** Code block size K3 */
    uint16_t                    block_size_k3;

    /** RM Header Word 6 */        

    /** Sub-block interleaver parameter 'm' for codeblock of size K3 */
    unsigned char                     param_m3;

    /** Sub-block interleaver parameter 'j' for codeblock of size K3 */
    unsigned char                     param_j3;

    /** Output size E3 */
    uint16_t                    block_size_e3;
} Bcp_RmHdr_WiMaxCfg;

typedef struct _Bcp_EncBlockCfg
{
    /** Size of this block in bits - upto 8192 bits */        
    uint16_t                    block_size;

    /** Number of code blocks of this size block */
    uint16_t                    num_code_blks;

    /** signed interleaver generator parameter
     *  
     *  WCDMA: Set to 0
     *  LTE: signed interleaver param f1
     *  WIMAX: signed interleaver param P0
     */
    uint16_t                    intvpar0;

    /** signed interleaver generator parameter
     *  
     *  WCDMA: Set to 0
     *  LTE: signed interleaver param f2
     *  WIMAX: signed interleaver param P1
     */
    uint16_t                    intvpar1;

    /** signed interleaver generator parameter
     *  
     *  WCDMA: Set to 0
     *  LTE: Set to 0
     *  WIMAX: signed interleaver param P2
     */
    uint16_t                    intvpar2;

    /** signed interleaver generator parameter
     *  
     *  WCDMA: Set to 0
     *  LTE: Set to 0
     *  WIMAX: signed interleaver param P3
     */
    uint16_t                    intvpar3;
} Bcp_EncBlockCfg;        

   
typedef struct _Bcp_EncHdrCfg
{    
    /** ENC Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Specify if data needs to be turbo on convolutional
     *  encoded.
     *
     *  Set to 0 for convolutional and 1 for turbo encoding
     */
    unsigned char                     turbo_conv_sel;

    /** Scrambler/CRC configuration. Set to:
     *  0 - Do not generate Scrambler/CRC for code blocks
     *  1 - Generate scrambler/CRC-24
     *  2 - Generate scrambler/CRC-16
     *  3 - Generate scrambler
     */
    unsigned char                     scr_crc_en;

    /** Convolutional code rate. Valid only for convolutional encoding.
     *
     *  Set to 0 for convolutional code rate 1/3
     *  and 1 for convolutional code rate 1/2
     */
    unsigned char                     code_rate_flag;

    /** ENC Header Word 1-9 */
    Bcp_EncBlockCfg             blockCfg [3];
} Bcp_EncHdrCfg;

   
typedef enum   
{
    /** Hard modulation (uncompressed) */
    Bcp_ModulationMode_HardUncompressed = 0,
    /** Hard modulation (compressed)  */
    Bcp_ModulationMode_HardCompressed   = 1,
    /** Soft modulation  */
    Bcp_ModulationMode_Soft             = 2
} Bcp_ModulationMode;

     
typedef enum   
{
    /** BPSK */
    Bcp_ModulationType_BPSK             = 0,
    /** QPSK  */
    Bcp_ModulationType_QPSK             = 1,
    /** 16 QAM  */
    Bcp_ModulationType_16QAM            = 2,
    /** 64 QAM  */
    Bcp_ModulationType_64QAM            = 3,
    /** 256 QAM  */
    Bcp_ModulationType_256QAM           = 4,
    /** 4 PAM  */
    Bcp_ModulationType_4PAM             = 5
} Bcp_ModulationType;

     
typedef enum   
{
    /** 9 columns (Extended w/ SRS) */
    Bcp_CmuxLength_9                    = 0,
    /** 10 columns (Extended w/o SRS) */
    Bcp_CmuxLength_10                   = 1,
    /** 11 columns (Normal w/ SRS)  */
    Bcp_CmuxLength_11                   = 2,
    /** 12 columns (Normal w/o SRS)  */
    Bcp_CmuxLength_12                   = 3
} Bcp_CmuxLength;

       
typedef enum   
{
    /** Q0 */
    Bcp_QFormat_Q0                      = 0,
    /** Q1 */
    Bcp_QFormat_Q1                      = 1,
    /** Q2 */
    Bcp_QFormat_Q2                      = 2,
    /** Q3 */
    Bcp_QFormat_Q3                      = 3
} Bcp_QFormat;

  
typedef struct _Bcp_ModHdrCfg
{    
    /** MOD Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Unit value 'a' for soft and hard modulation */
    uint16_t                    uva_val;
   
    /** MOD Header Word 1 */

    /** Modulation mode select */
    Bcp_ModulationMode          sh_mod_sel;

    /** Transport block 2-layer split.
     *
     *  Valid for soft modulation mode only.
     *
     *  Set to:
     *      0   -   Disable split mode
     *      1   -   Enable split mode
     */
    unsigned char                     split_mode_en;

    /** Scrambling enable.
     *
     *  Valid for soft and hard modulation LTE.
     *
     *  Set to:
     *      0   -   Bypass scrambling
     *      1   -   Enable scrambling
     */
    unsigned char                     scr_en;

    /** Modulation type select. */
    Bcp_ModulationType          mod_type_sel;

    /** Cmux length select. Valid for soft modulation only */
    Bcp_CmuxLength              cmux_ln;

    /** LLR Q format select. Valid for soft modulation only */
    Bcp_QFormat                 q_format;

    /** Index into 'B' tables of initial vectors to create 'B' matrices 
     *
     *  Valid for soft modulation only.
     */
    unsigned char                     b_table_index;

    /** IQ Output reverse.
     *
     *  Set to:
     *      0   -   Output Q as LSB then I
     *      1   -   Output I as LSB then Q
     */
    unsigned char                     jack_bit;

    /** MOD Header Word 2 */

    /** Cinit value for scrambling P2 sequence generator. */
    unsigned int                    cinit_p2;

    /** MOD Header Word 3 */

    /** Rmux length. 
     *
     *  Valid values are (increments of 12):
     *      12-1320 -   Transport block in one layer 
     *      12-2640 -   Transport block in two layers
     */
    uint16_t                    rmux_ln;

    /** Number of CQI symbols to insert. */
    uint16_t                    cqi_ln;

    /** MOD Header Word 4 */

    /** Number of RI symbols to insert. */
    uint16_t                    ri_ln;

    /** Number of ACK symbols to overwrite. */
    uint16_t                    ack_ln;
} Bcp_ModHdrCfg;

  
typedef struct _Bcp_IntDntTableCfg
{
    /** Number of rows in interleaver table per frame */
    uint16_t                    num_r2_length;

    /** Number of dummy symbols per frame in last Row */
    unsigned char                     num_dummy;

    /** Input data format */
    unsigned char                     num_data_format_in;
}Bcp_IntDntTableCfg;

    
typedef struct _Bcp_IntHdrCfg
{    
    /** INT Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** INT Header Word 1 */

    /** Constellation rearrangement format for 4/6 bit output symbol */
    unsigned char                     num_constellation;

    /** Flag to indicate if the input is coming directly from the encoder
     *  instead of rate matcher or CRC submodules.
     */
    unsigned char                     flag_in_order;

    /** Flag to indicate if the data is 1/2 rate convolution data. */
    unsigned char                     flag_half_rate;

    /** Output data format. */
    unsigned char                     num_data_format_out;

    /** Frame count. */
    unsigned char                     num_frame_count;

    /** Number of columns in interleaver matrix. 
     *
     *  1,2,4, or 8 selects 1st interleaving operation and refers to number of
     *  columns in the matrix. 30 selects 2nd interleaving operation and always
     *  has 30 fixed columns.
     */
    unsigned char                     num_int_ways;

    /** INT header Word 2 */

    /** Number of DTX to be appended to end of last frame */
    uint16_t                    num_add_dtx;

    /** INT header Words 3-8 */
    Bcp_IntDntTableCfg          tblCfg [6];
} Bcp_IntHdrCfg;

      
typedef enum   
{
    /** PUCCH */
    Bcp_CorPuchhDespread_Sel_PUCHH      = 0,
    /** De-spreading */
    Bcp_CorPucchDespread_Sel_DESPREAD   = 1
} Bcp_CorPuchhDespread_Sel;

       
typedef enum   
{
    /** Inputs are 16-bit real numbers */
    Bcp_CorDespread_Cplx_16REAL     = 0,
    /** Inputs are 32-bit complex numbers */
    Bcp_CorDespread_Cplx_32COMPLEX  = 1
}Bcp_CorDespread_Cplx;

  
typedef struct _Bcp_CorBlockParams
{    
    /** Number of samples given for de-spreading. Largest possible
     *  for is 2^16 = 65,536 samples.
     */
    uint16_t                    despreading_length;

    /** De-spreading ratio. */
    unsigned char                     sf_ratio;
} Bcp_CorBlockParams;

    
typedef struct _Bcp_CorHdrCfg
{    
    /** COR Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Mode flag to select between Pucch correlation packet
     *  or De-spreading packet.
     */
    Bcp_CorPuchhDespread_Sel    pucch_despread_sel;

    /** Indicates if de-spreading inputs are 16-bit real or 32-bit
     *  complex numbers.
     */
    Bcp_CorDespread_Cplx        despread_flag_cplx;
   
    /** COR Header Word 1 */

    /** Number of user bits. */
    unsigned char                     pucch_a;

    /** Scrambling code. */
    unsigned int                    pucch_scr_seq;

    /** Number of Antennas */
    unsigned char                     pucch_nr;

    /** Operational mode. */
    unsigned char                     pucch_mode;

    /** COR Header Word 2-7 */

    /** Correlation parameters for each physical block */
    Bcp_CorBlockParams          block_params [6];
} Bcp_CorHdrCfg;

      
typedef enum   
{
    /** FDD */
    Bcp_SslFddTdd_Sel_FDD       = 0,
    /** TDD */
    Bcp_SslFddTdd_Sel_TDD       = 1
}Bcp_SslFddTdd_Sel;

      
typedef enum   
{
    /** 2ms TTI */
    Bcp_Tti_Sel_2ms             = 0,
    /** 10ms TTI */
    Bcp_Tti_Sel_10ms            = 1
}Bcp_Tti_Sel;

    
typedef struct _Bcp_SslHdr_ModeSelCfg
{    
    /** SSL Header Word 1 */

    /** Mode flag to select FDD/TDD. */
    Bcp_SslFddTdd_Sel           fdd_tdd_sel;

    /** Transport block 2-layer split 
     *  
     *  Set to:
     *      0   -   Split mode disabled
     *      1   -   Split mode enabled
     */
    unsigned char                     split_mode_en;

    /** IQ Output reverse.
     *
     *  Set to:
     *      0   -   Output Q as LSB then I
     *      1   -   Output I as LSB then Q
     */
    unsigned char                     jack_bit;

    /** LTE descrambling enable.
     *
     *  Set to:
     *      0   -   Descrambling bypassed
     *      1   -   Descrambling enabled
     */
    unsigned char                     lte_descrambler_en;

    /** Modulation type select */
    Bcp_ModulationType          mod_type_sel;

    /** WCDMA output from SSL slot packing mode.
     *
     *  Set to:
     *      0   -   slots aligned on 128 bit boundaries
     *      1   -   slots not aligned on 128 bit boundaries, i.e., packed
     */
    unsigned char                     tdd_output_pack;

    /** Cmux length select. Valid for soft modulation only */
    Bcp_CmuxLength              cmux_ln;

    /** LLR Q format select. Valid for soft modulation only */
    Bcp_QFormat                 q_format;

    /** Index into 'B' tables of initial vectors to create 'B' matrices */
    unsigned char                     b_matrix_sel;

    /** TTI selection. Valid only for WCDMA(FDD) only. */
    Bcp_Tti_Sel                 tti_2ms_10ms_sel;

    /** Number of WCMDA (TD-SCDMA) slots. */
    unsigned char                     wcdma_num_slots;

    /** Number of physical channels for WCDMA (FDD) */
    unsigned char                     wcmda_num_phy_ch;

    /** Rmux length index. 
     *
     *  Index into rmux length tables. Valid for LTE only.
     */
    uint16_t                    rmux_ln_index;        

} Bcp_SslHdr_ModeSelCfg;

    
typedef struct _Bcp_SslHdr_LteCfg
{
    /** SSL Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Soft slicer Unit Value.
     *
     *  Valid values are:
     *      2-PAM:  0-16383 (QPSK & BPSK)
     *      4-PAM:  0-10922 (16 QAM & 4-PAM)
     *      6-PAM:  0-4681  (64 QAM)
     *      8-PAM:  0-2184  (256 QAM)
     */
    uint16_t                    uva;

    /** SSL Header Word 1 */        

    /** SSL Header Mode selection Configuration */        
    Bcp_SslHdr_ModeSelCfg       modeSelCfg;        

    /** SSL Header Word 2 */        

    /** Number of RI LLRs of Qw soft bits to puncture 
     *
     *  Valid values are:
     *      0-5184: Normal mode
     *      0-10368: Split mode    
     */
    uint16_t                    ri_ln;

    /** Number of ACK LLRs of Qw soft bits to overwrite 
     *
     *  Valid values are:
     *      0-5184: Normal mode
     *      0-10368: Split mode    
     */
    uint16_t                    ack_ln;

    /** SSL Header Word 3 */        

    /** Cinit value for descrambler P2 sequence generator. */
    unsigned int                    cinit_p2;

    /** SSL Header Word 4-15 */        

    /** Noise scaling factor for Column 'n' in LTE single layer mode
     *  if cmux_ln=12 or Column '0,n' in LTE split mode if cmux_ln=12
     */
    unsigned int                    scale_c0[12];

    /** SSL Header Word 16-27 */        

    /** Noise scaling factor for Column '1,n' in LTE split mode if cmux_ln=12 
     *
     *  Valid for LTE split mode only.
     */
    unsigned int                    scale_c1[12];
} Bcp_SslHdr_LteCfg;

    
typedef struct _Bcp_SslHdr_WcdmaFddCfg
{
    /** SSL Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** WCDMA (FDD) Number of symbols per segment */
    uint16_t                    wcdma_symb_seq;

    /** SSL Header Word 1 */        

    /** SSL Header Mode selection Configuration */        
    Bcp_SslHdr_ModeSelCfg       modeSelCfg;        

    /** SSL Header Word 2-20 */        

    /** Soft slicer Unit Value.
     *
     *  Valid values are:
     *      2-PAM:  0-16383 (QPSK & BPSK)
     *      4-PAM:  0-10922 (16 QAM & 4-PAM)
     */
    uint16_t                    uva [12];

    /** Noise scaling factor for Segment 'n' in WCDMA FDD */
    unsigned int                    scale_c0[12];
} Bcp_SslHdr_WcdmaFddCfg;

    
typedef struct _Bcp_SslHdr_WcdmaTdScdmaCfg
{
    /** SSL Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** SSL Header Word 1 */        

    /** SSL Header Mode selection Configuration */        
    Bcp_SslHdr_ModeSelCfg       modeSelCfg;        

    /** SSL Header Word 2-13 */        

    /** Soft slicer Unit Value.
     *
     *  Valid values are:
     *      2-PAM:  0-16383 (QPSK & BPSK)
     *      4-PAM:  0-10922 (16 QAM & 4-PAM)
     *      6-PAM:  0-4681  (64 QAM)
     */
    uint16_t                    uva [6];

    /** Number of symbols for slot 'n' */        
    uint16_t                    wcdma_size_slot [6];        

    /** Noise scaling factor for Slot 'n' in WCDMA TD-SCDMA */
    unsigned int                    scale_c0[6];
} Bcp_SslHdr_WcdmaTdScdmaCfg;


    
typedef struct _Bcp_SslHdr_WiMaxCfg
{
    /** SSL Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Soft slicer Unit Value.
     *
     *  Valid values are:
     *      2-PAM:  0-16383 (QPSK & BPSK)
     *      4-PAM:  0-10922 (16 QAM & 4-PAM)
     *      6-PAM:  0-4681  (64 QAM)
     *      8-PAM:  0-2184  (256 QAM)
     */
    uint16_t                    uva;

    /** SSL Header Word 1 */        

    /** SSL Header Mode selection Configuration */        
    Bcp_SslHdr_ModeSelCfg       modeSelCfg;        
} Bcp_SslHdr_WiMaxCfg;


     
typedef struct _Bcp_DntHdrCfg
{
    /** DNT Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** DNT Header Word 1 */        

    /** Constellation re-arrangement for 16QAM. Valid values are 0-3 */
    unsigned char                     num_constellation;

    unsigned char                     num_frame_count;

    /** Descrambler generator initialization value */
    uint16_t                    num_descramble;

    /** DNT Header Word 2 */        

    /** Number of symbols per packet. */
    unsigned int                    num_data_value;

    /** DNT Header Word 3-8 */        
    Bcp_IntDntTableCfg          tblCfg[6];
} Bcp_DntHdrCfg;

    
typedef struct _Bcp_RdHdr_LteCfg
{
    /** RD Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Enable HARQ input flag. 
     *
     *  Set to:
     *      0   -   To indicate to engine not to use history data in 
     *              LLR combining
     *      1   -   To indicate to engine to use history data
     */
    unsigned char                     enable_harq_input;

    /** Enable HARQ output flag.
     *  
     *  Set to:
     *      0   -   To indicate to engine not to write back updated history 
     *              data.
     *      1   -   To indicate to engine to write back whole TB history data.              
     */
    unsigned char                     enable_harq_output;

    /** Unsigned scale factor in Q4 format. */
    unsigned char                     tcp3d_scale_factor;

    /** Turbo decoder LLR range 
     *
     *  Set to:
     *  0       -   LLRs towards turbo decoder are in range +/-31
     *  1       -   LLRs towards turbo decoder are in range +/-127
     */
    unsigned char                     tcp3d_dyn_range;

    /** RD Header Word 1 */        

    /** Control data flag.
     *  
     *  Set to:
     *      0   -   for CQI bits to be rate dematched
     *      1   -   for bypassing rate dematching of CQI data
     */
    unsigned char                     cqi_pass_through;

    /** Starting column in the writing phase to dual matrix. */
    unsigned char                     rv_start_column;

    /** Number of filler bits F in the first code block. */
    unsigned char                     num_filler_bits_f;

    /** Starting value for flow Id counter. */
    unsigned char                     init_cb_flowId;

    /** Max value for flow Id counter */
    unsigned char                     flowId_hi;

    /** Offset for CQI CB only. */
    unsigned char                     flowId_cqi_offset;

    /** RD Header word 2 */

    /** Number of code blocks C1 of size K1 */
    unsigned char                     num_code_blocks_c1;

    /** Number of code blocks Ce1 of transmitted size E1 */
    unsigned char                     num_code_blocks_ce1;

    /** Number of code blocks C2 of size K2 */
    unsigned char                     num_code_blocks_c2;

    /** Number of code blocks Ce2 with transmitted size E2 */
    unsigned char                     num_code_blocks_ce2;

    /** RD Header word 3 */

    /** Code block size K1 */
    uint16_t                    block_size_k1;

    /** Transmitted size E1 */
    uint16_t                    block_size_e1;

    /** RD Header word 4 */

    /** Code block size K2 */
    uint16_t                    block_size_k2;

    /** Transmitted size E2 */
    uint16_t                    block_size_e2;

    /** RD Header word 5 */

    /** Payload size of CQI data */
    uint16_t                    block_size_outq_cqi;

    /** Transmitted size of CQI data */
    uint16_t                    block_size_inq_cqi;

    /** RD Header word 6 */

    /** Address of HARQ input data */
    unsigned int                    harq_input_address;

    /** RD Header word 7 */

    /** Address of HARQ output data */
    unsigned int                    harq_output_address;
} Bcp_RdHdr_LteCfg;

    
typedef struct _Bcp_RdHdr_ChannelCfg
{
    /** Channel 'n' length after rate de-matching */
    uint16_t                    sys_len;

    /** Channel 'n' initial value for rate de-matching */
    uint16_t                    sys_init2;

    /** Channel 'n' minus value for rate de-matching */
    uint16_t                    sys_minus2;

    /** Channel 'n' plus value for rate de-matching */
    uint16_t                    sys_plus2;

    /** Channel 'n' TD-SCDMA alpha value for bit separation */
    unsigned char                     sys_aplha;

    /** Channel 'n' TD-SCDMA beta value for bit separation */
    unsigned char                     sys_beta;

    /** Puncture flag 
     *
     *  Set to:
     *      0   -   Perform un-repeat during rate de-matching
     *      1   -   Perform un-puncture during rate de-matching
     */
    unsigned char                     sys_puncture;

    /** Channel type.
     *
     *  Set to:
     *      0   -   Convolutional or Rel-99 Turbo Repeat
     *      1   -   Rel-99 style Turbo puncture, use p0 parity params
     *      2   -   Rel-99 style Turbo puncture, use p1 parity params
     */
    unsigned char                     sys_turbo;
}Bcp_RdHdr_ChannelCfg;


    
typedef struct _Bcp_RdHdr_xCdmaCfg
{
    /** RD Header Word 0 */        

    /** Local header length (Not including Header word 0). 
     *
     *  Possible values are 41 for Long header format and 22
     *  for short header format.
     */
    unsigned char                     local_hdr_len;        

    /** Un-collection method for HSUPA channels
     *
     *  Set to:
     *      0   - Perform HSPA un-collection.
     *      1   - Perform Rel-99 un-collection.
     */
    unsigned char                     fdd;

    /** Enable history data for LLR combining
     *
     *  Set to:
     *      0   - Do not use history data.
     *      1   - Use history data.
     */
    unsigned char                     en_harq_in;

    /** Enable storing of history data for future LLR combining
     *
     *  Set to:
     *      0   - Do not store history data.
     *      1   - Store history data.
     */
    unsigned char                     en_harq_out;

    /** Turbo decoder LLR range 
     *
     *  Set to:
     *  0       -   LLRs towards turbo decoder are in range +/-31
     *  1       -   LLRs towards turbo decoder are in range +/-127
     */
    unsigned char                     tcp3d_dyn_range;

    /** Scale factor that determines how 6 bits of LLR data are 
     *  extracted from 8 HARQ bits for sending to TCP3d.
     */
    unsigned char                     tcp3d_scale_factor;

    /** RD Header Word 1 */

    /** Collect columns - Number of columns in bit collection table. */
    unsigned int                    collect_cols;

    /** Collect rows - Number of rows in bit collection table. */
    unsigned char                     collect_rows;

    /** RD Header Word 2 */
    
    /** Length of each turbo code block (after rate de-matching) including
     *  tail bits.
     */
    uint16_t                    turbo_length;

    /** Number of turbo code blocks to output */
    unsigned char                     turbo_count;

    /** Starting value of CB flow Id counter */
    unsigned char                     flow_id_init;

    /** Maximum value of CB Flow id counter */
    unsigned char                     flow_id_max;

    /** RD Header Word 3 */
    /** Starting address of HARQ history data input */
    unsigned int                    harq_in_addr;

    /** RD Header Word 4 */
    /** Starting address of HARQ history data output */
    unsigned int                    harq_out_addr;

    /** RD Header Word 5 */
    /** Channel 0 length after rate de-matching. */
    unsigned int                    sys0_len;

    /** RD Header Word 6 */
    /** Channel 0 initial value for 2nd rate matching loop and rate de-matching . */
    unsigned int                    sys0_init2;

    /** RDD Header Word 7 */
    /** Channel 0 minus value for 2nd rate matching loop and rate de-matching. */
    unsigned int                    sys0_minus2;

    /** RD Header Word 8 */
    /** Channel 0 plus value for 2nd rate matching loop and rate de-matching. */
    unsigned int                    sys0_plus2;

    /** Channel 0 TD-SDCDMA aplha value for bit separation. */
    unsigned char                     sys0_alpha;

    /** Channel 0 TD-SDCDMA beta value for bit separation. */
    unsigned char                     sys0_beta;

    /** Channel 0 Puncture flag 
     *
     *  Set to:
     *      0   -   Perform un-repeat during rate de-matching
     *      1   -   Perform un-puncture during rate de-matching
     */
    unsigned char                     sys0_puncture;

    /** Channel type.
     *
     *  Set to:
     *      0   -   Convolutional or Rel-99 Turbo Repeat
     *      1   -   Rel-99 style Turbo puncture, use p0 parity params
     *      2   -   Rel-99 style Turbo puncture, use p1 parity params
     *      3   -   HSUPA style turbo channel, use p0 params.
     */
    unsigned char                     sys0_turbo;

    /** RD Header Word 9 */
    /** P0 Parity1 length after rate de-matching */
    unsigned int                    p0_par1_len;

    /** RD Header Word 10 */
    /** P0 Parity1 initial value for 1st rate matching loop */
    unsigned int                    p0_par1_init1;

    /** RD Header Word 11 */
    /** P0 Parity1 minus value for 1st rate matching loop */
    unsigned int                    p0_par1_minus1;

    /** RD Header Word 12 */
    /** P0 Parity1 plus value for 1st rate matching loop */
    unsigned int                    p0_par1_plus1;

    /** RD Header Word 13 */
    /** P0 Parity1 initial value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par1_init2;

    /** RD Header Word 14 */
    /** P0 Parity1 minus value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par1_minus2;

    /** RD Header Word 15 */
    /** P0 Parity1 plus value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par1_plus2;

    /** RD Header Word 16 */
    /** P0 Parity2 length after rate de-matching */
    unsigned int                    p0_par2_len;

    /** RD Header Word 17 */
    /** P0 Parity2 initial value for 1st rate matching loop */
    unsigned int                    p0_par2_init1;

    /** RD Header Word 18 */
    /** P0 Parity2 minus value for 1st rate matching loop */
    unsigned int                    p0_par2_minus1;

    /** RD Header Word 19 */
    /** P0 Parity2 plus value for 1st rate matching loop */
    unsigned int                    p0_par2_plus1;

    /** RD Header Word 20 */
    /** P0 Parity2 initial value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par2_init2;

    /** RD Header Word 21 */
    /** P0 Parity2 minus value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par2_minus2;

    /** RD Header Word 22 */
    /** P0 Parity2 plus value for 2nd rate matching loop and rate de-matching */
    unsigned int                    p0_par2_plus2;

    /** RD Header Words 23-37 */
    Bcp_RdHdr_ChannelCfg        channelCfg[5];

    /** RD Header Word 38 */
    /** p1 Parity1 length after rate de-matching */
    uint16_t                    p1_par1_len;

    /** p1 Parity1 initial value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par1_init2;

    /** RD Header Word 39 */
    /** p1 Parity1 minus value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par1_minus2;

    /** p1 Parity1 plus value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par1_plus2;

    /** RM Header Word 40 */
    /** p1 Parity2 length after rate de-matching */
    uint16_t                    p1_par2_len;

    /** p1 Parity2 initial value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par2_init2;

    /** RM Header Word 41 */
    /** p1 Parity2 minus value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par2_minus2;

    /** p1 Parity2 plus value for 2nd rate matching loop and rate de-matching */
    uint16_t                    p1_par2_plus2;
} Bcp_RdHdr_xCdmaCfg;

   
typedef struct _Bcp_RdHdr_WiMaxCfg
{
    /** RD Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        

    /** Enable HARQ input flag. 
     *
     *  Set to:
     *      0   -   To indicate to engine not to use history data in 
     *              LLR combining
     *      1   -   To indicate to engine to use history data
     */
    unsigned char                     enable_harq_input;

    /** Enable HARQ output flag.
     *  
     *  Set to:
     *      0   -   To indicate to engine not to write back updated history 
     *              data.
     *      1   -   To indicate to engine to write back whole TB history data.              
     */
    unsigned char                     enable_harq_output;

    /** Turbo decoder LLR range 
     *
     *  Set to:
     *  0       -   LLRs towards turbo decoder are in range +/-31
     *  1       -   LLRs towards turbo decoder are in range +/-127
     */
    unsigned char                     tcp3d_dyn_range;

    /** Unsigned scale factor in Q4 format. */
    unsigned char                     tcp3d_scale_factor;

    /** RD Header Word 1 */        

    /** Number of code blocks C1 of size K1 */
    unsigned char                     num_code_blocks_c1;

    /** Starting value for flow Id Offset value. */
    unsigned char                     flowId_starting_offset;

    /** Flow Id ending offset */
    unsigned char                     flowId_ending_offset;

    /** Code block size K1 */
    uint16_t                    block_size_k1;

    /** RD Header Word 2 */        

    /** Sub-block interleaver parameter 'm' for CBs of size K1 */
    unsigned char                     param_m1;

    /** Sub-block interleaver parameter 'j' for CBs of size K1 */
    unsigned char                     param_j1;

    /** Transmitted size E1 */
    uint16_t                    block_size_e1;

    /** RD Header Word 3 */        

    /** Number of code blocks C2 of size K2 */
    unsigned char                     num_code_blocks_c2;

    /** Code block size K2 */
    uint16_t                    block_size_k2;

    /** RD Header Word 4 */        

    /** Sub-block interleaver parameter 'm' for CBs of size K2 */
    unsigned char                     param_m2;

    /** Sub-block interleaver parameter 'j' for CBs of size K2 */
    unsigned char                     param_j2;

    /** Transmitted size E2 */
    uint16_t                    block_size_e2;

    /** RD Header Word 5 */        

    /** Number of code blocks C3 of size K3 */
    unsigned char                     num_code_blocks_c3;

    /** Code block size K3 */
    uint16_t                    block_size_k3;

    /** RD Header Word 6 */        

    /** Sub-block interleaver parameter 'm' for CBs of size K3 */
    unsigned char                     param_m3;

    /** Sub-block interleaver parameter 'j' for CBs of size K3 */
    unsigned char                     param_j3;

    /** Transmitted size E3 */
    uint16_t                    block_size_e3;

    /** RD Header word 7 */

    /** Address of HARQ input data */
    unsigned int                    harq_input_address;

    /** RD Header word 8 */

    /** Address of HARQ output data */
    unsigned int                    harq_output_address;
} Bcp_RdHdr_WiMaxCfg;

     
typedef struct _Bcp_DioDmaBlkCfg
{
    /** Address of the DMA block. Must be aligned on 16 byte boundary */
    unsigned int                    dio_addr;

    /** Number of bytes to access. Must be a multiple of 16 bytes */
    unsigned int                    dio_cnt;
} Bcp_DioDmaBlkCfg;

     
typedef struct _Bcp_DioHdrCfg
{
    /** DIO Header Word 0 */        

    /** Local header length (Not including Header word 0). */
    unsigned char                     local_hdr_len;        
    
    /** Endian swap control */        
    Bcp_EndianFormat            dio_endian;

    /** Read/Write control. Set to 1 for Read operation, 0 otherwise */
    unsigned char                     dio_rd_wr;

    /** Number of DMA blocks for the operation */
    unsigned char                     dio_blk_cnt;

    /** DIO Header Words 1-12 */        

    /** DIO DMA block configuration. */
    Bcp_DioDmaBlkCfg            dio_dmablk_cfg [6];
} Bcp_DioHdrCfg;


/**
@}
*/

extern signed int Bcp_addGlobalHeader (
    Bcp_GlobalHdrCfg*                   pBcpGlobalHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addTMHeader (
    Bcp_TmHdrCfg*                       pTmHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addCRCHeader (
    Bcp_CrcHdrCfg*                      pCrcHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addxCdma_RMHeader (
    Bcp_RmHdr_xCdmaCfg*                 pRmHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addLte_RMHeader (
    Bcp_RmHdr_LteCfg*                   pRmHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addWiMax_RMHeader (
    Bcp_RmHdr_WiMaxCfg*                 pRmHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addEncoderHeader (
    Bcp_EncHdrCfg*                      pEncHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addModulatorHeader (
    Bcp_ModHdrCfg*                      pModHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);
        
extern signed int Bcp_addInterleaverHeader (
    Bcp_IntHdrCfg*                      pIntHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addCorrelationHeader (
    Bcp_CorHdrCfg*                      pCorHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addLTE_SSLHeader (
    Bcp_SslHdr_LteCfg*                  pSslHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addWCDMAFDD_SSLHeader (
    Bcp_SslHdr_WcdmaFddCfg*             pSslHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addWCDMATDSCDMA_SSLHeader (
    Bcp_SslHdr_WcdmaTdScdmaCfg*         pSslHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addWiMax_SSLHeader (
    Bcp_SslHdr_WiMaxCfg*                pSslHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addDeinterleaverHeader (
    Bcp_DntHdrCfg*                      pDntHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addLTE_RDHeader (
    Bcp_RdHdr_LteCfg*                   pRdHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addxCdma_RDHeader (
    Bcp_RdHdr_xCdmaCfg*                 pRdHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addWiMax_RDHeader (
    Bcp_RdHdr_WiMaxCfg*                 pRdHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_addDIOHeader (
    Bcp_DioHdrCfg*                      pDioHdrCfg, 
    unsigned char*                            pData,
    unsigned int*                           pLen
);

extern signed int Bcp_lldOpen 
(
    unsigned char                             instNum,
    unsigned int                            cfgRegsBaseAddress,
    Bcp_LldObj*                         pBcpLldObj
);

extern signed int Bcp_lldClose 
(
    Bcp_LldObj*                         pBcpLldObj
);

#ifdef __cplusplus
}
#endif

#endif  /* __BCP_LLD_H__ */

