#include <pl_comm.h>
/*
 * ics.h
 *
 *  Created on: 2011-12-6
 *      Author: Administrator
 */

#ifndef ICS_H_
#define ICS_H_
#endif /* ICS_H_ */
#define CS_SEARCH_TIME 5
#define CS_LENGTH_5MS 153600
#define CS_LENGTH_SUBFN 30720
#define CS_ORDER_FIR  127
#define CS_CELL_ID2   3
#define CS_CELL_ID1_MAX_NUM   168
#define NULL  0
#define CS_FFT_LENGTH 6144
#define CS_EVEN_HALFSFN 0
#define CS_ODD_HALFSFN 1
#define CS_SEQ_LENGTH 62
#define CS_DFT_MODE 1
#define CS_IDFT_MODE 0
#define CS_MAX_ANTE_NUM 2
#define CS_16DOWN_5MS (CS_LENGTH_5MS >> 4)
#define CS_PSC_OFFSET 0
#define CS_FFT_AOTU_AGC 0x10000000
#define CS_MACHFILTER_REF 13
#define CS_SSC_OBTAIN_FFT_REF 4
typedef signed int Sint;
typedef unsigned int Uint;
typedef signed short Sshort;
typedef unsigned short Ushort;
typedef unsigned char Uchar;
typedef signed char Schar;
typedef unsigned int Uint32;
typedef signed int Int32;
typedef unsigned short Uint16;
typedef unsigned char Uint8;
extern Uint gcs_cell_id2;/*CELL_ID2*/
extern Sint gcs_time_data_rx_buffer[CS_LENGTH_5MS * CS_MAX_ANTE_NUM];
extern Sint gcs_time_data[(CS_LENGTH_5MS + 128) * 2];
extern Sint gcs_fir_output_buffer[9600 * 2];
extern Sint gcs_coff_buffer[127];
extern Sint FFT_EXP_BUFFER[CS_FFT_LENGTH];
extern Sint IFFT_EXP_BUFFER[CS_FFT_LENGTH];
extern Sint gcs_match_filter_buffer[9600 * 2];
extern const Sint PSC_FREQ_LOCAL[CS_CELL_ID2 * CS_FFT_LENGTH];
extern Sint gcs_match_filter_out[(CS_FFT_LENGTH + 4) * 2 * CS_MAX_ANTE_NUM];/*dft输出数据要偏移4个int才是真正的结果*/

extern Uint gcs_peak_pos[CS_CELL_ID2];
extern Uint gcs_peak[CS_CELL_ID2];
extern Sshort gcs_foe_step;
extern Sshort gcs_ssc_pos_map[2];
extern Uint gcs_cor_max[2];/*记录常规CP和扩展CP假设下的相关结果*/
extern Uint gcs_even_odd_flag[2];/*记录常规CP和扩展CP假设下的偶数或者奇数半帧判断结果*/
extern Uint gcs_id1_index[2];/*记录常规CP和扩展CP假设下的cell_id1判断结果*/
extern Sint gcs_ssc_seq[62];
extern Sint gcs_ssc_seq_even[(CS_SEQ_LENGTH >> 1) * CS_MAX_ANTE_NUM];
extern Sint gcs_ssc_seq_odd[(CS_SEQ_LENGTH >> 1) * CS_MAX_ANTE_NUM];
extern Uint gcs_psc_start_pos;/*主同步信号位置*/
extern Uint gcs_frame_start_pos;/*帧头起始位置*/
extern Uint gcs_c0_seq[(CS_SEQ_LENGTH >> 1)];
extern Uint gcs_c1_seq[(CS_SEQ_LENGTH >> 1)];
extern Sint gcs_freq_offset_buffer[10];
extern Sint gcs_s0_seq[(CS_SEQ_LENGTH >> 1)];
extern Sint gcs_s1_seq[(CS_SEQ_LENGTH >> 1)];
extern Sint gcs_zm0_seq[(CS_SEQ_LENGTH >> 1)];
extern Sint gcs_zm1_seq[(CS_SEQ_LENGTH >> 1)];
extern Sint gcs_ssc_freq_data_buffer[10];
extern Uchar gcs_m0m1_map[168][2];
extern const Sint PSC_TIME_LOCAL[CS_CELL_ID2 * 128];
