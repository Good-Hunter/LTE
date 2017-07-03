/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2006
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *   ==========================================================================
 */


/** ===========================================================================
 * VCP2_Channel_Density.h
 * @desc  header file for VCP2 channel density test.
 * ============================================================================
 */
/* ============================================================================
 *  Revision History
 *  ===============
 *  11-July-2006   Brighton     File Created.
 * ============================================================================
 */
 
 
#ifndef _VCP2_CHANNEL_DENSITY_H_
#define _VCP2_CHANNEL_DENSITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl_VCP2.h>

#define MAX_VCP2_FRAME_LEN 	(10*1024)
#define MAX_VCP2_TAIL_LEN 	8
#define FMAX 	120

/* Polynomials for different constraint lengths and
   code rates
   - All these are octal numbers
   - Default values
*/
#define VCP2_POLY0_FOR_CONST9_RATE_1_2 0561
#define VCP2_POLY1_FOR_CONST9_RATE_1_2 0753

#define VCP2_POLY0_FOR_CONST8_RATE_1_2 0712
#define VCP2_POLY1_FOR_CONST8_RATE_1_2 0476

#define VCP2_POLY0_FOR_CONST7_RATE_1_2 0554
#define VCP2_POLY1_FOR_CONST7_RATE_1_2 0744

#define VCP2_POLY0_FOR_CONST6_RATE_1_2 0650
#define VCP2_POLY1_FOR_CONST6_RATE_1_2 0570

#define VCP2_POLY0_FOR_CONST5_RATE_1_2 0460
#define VCP2_POLY1_FOR_CONST5_RATE_1_2 0720

//polys for 1/3
#define VCP2_POLY0_FOR_CONST9_RATE_1_3 0557
#define VCP2_POLY1_FOR_CONST9_RATE_1_3 0663
#define VCP2_POLY2_FOR_CONST9_RATE_1_3 0711

#define VCP2_POLY0_FOR_CONST8_RATE_1_3 0452
#define VCP2_POLY1_FOR_CONST8_RATE_1_3 0662
#define VCP2_POLY2_FOR_CONST8_RATE_1_3 0756

#define VCP2_POLY0_FOR_CONST7_RATE_1_3 0544
#define VCP2_POLY1_FOR_CONST7_RATE_1_3 0624
#define VCP2_POLY2_FOR_CONST7_RATE_1_3 0764

#define VCP2_POLY0_FOR_CONST6_RATE_1_3 0470
#define VCP2_POLY1_FOR_CONST6_RATE_1_3 0530
#define VCP2_POLY2_FOR_CONST6_RATE_1_3 0750

#define VCP2_POLY0_FOR_CONST5_RATE_1_3 0520
#define VCP2_POLY1_FOR_CONST5_RATE_1_3 0660
#define VCP2_POLY2_FOR_CONST5_RATE_1_3 0760

//polys for 1/4
#define VCP2_POLY0_FOR_CONST9_RATE_1_4 0765
#define VCP2_POLY1_FOR_CONST9_RATE_1_4 0671
#define VCP2_POLY2_FOR_CONST9_RATE_1_4 0513
#define VCP2_POLY3_FOR_CONST9_RATE_1_4 0473

#define VCP2_POLY0_FOR_CONST8_RATE_1_4 0452
#define VCP2_POLY1_FOR_CONST8_RATE_1_4 0662
#define VCP2_POLY2_FOR_CONST8_RATE_1_4 0756
#define VCP2_POLY3_FOR_CONST8_RATE_1_4 0452

#define VCP2_POLY0_FOR_CONST7_RATE_1_4 0544
#define VCP2_POLY1_FOR_CONST7_RATE_1_4 0624
#define VCP2_POLY2_FOR_CONST7_RATE_1_4 0764
#define VCP2_POLY3_FOR_CONST7_RATE_1_4 0544

#define VCP2_POLY0_FOR_CONST6_RATE_1_4 0470
#define VCP2_POLY1_FOR_CONST6_RATE_1_4 0530
#define VCP2_POLY2_FOR_CONST6_RATE_1_4 0750
#define VCP2_POLY3_FOR_CONST6_RATE_1_4 0470

#define VCP2_POLY0_FOR_CONST5_RATE_1_4 0520
#define VCP2_POLY1_FOR_CONST5_RATE_1_4 0660
#define VCP2_POLY2_FOR_CONST5_RATE_1_4 0760
#define VCP2_POLY3_FOR_CONST5_RATE_1_4 0520

/* Maxmimum frame sizes, maximum (reliability length +
   convergence distance) and the number of trellis stages
   processed in 2 passes for different constraint
   lengths of soft and hard decisions
*/

/* For Hard Decisions */
#define VCP2_MAX_FRAME_SIZE_FOR_HD_CONST9          120
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST9    124
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST9  4

#define VCP2_MAX_FRAME_SIZE_FOR_HD_CONST8          217
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST8    217
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST8  7

#define VCP2_MAX_FRAME_SIZE_FOR_HD_CONST7          378
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST7    378
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST7  6

#define VCP2_MAX_FRAME_SIZE_FOR_HD_CONST6          635
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST6    635
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST6  5

#define VCP2_MAX_FRAME_SIZE_FOR_HD_CONST5          2044
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST5    2044
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST5  4

/* For Soft Decisions */
#define VCP2_MAX_FRAME_SIZE_FOR_SD_CONST9          56
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST9    60
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST9  4

#define VCP2_MAX_FRAME_SIZE_FOR_SD_CONST8          105
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST8    105
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST8  7

#define VCP2_MAX_FRAME_SIZE_FOR_SD_CONST7          186
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST7    186
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST7  6

#define VCP2_MAX_FRAME_SIZE_FOR_SD_CONST6          315
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST6    315
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST6  5

#define VCP2_MAX_FRAME_SIZE_FOR_SD_CONST5          1020
#define VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST5    1020
#define VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST5  4

/* Limit on sliding window length in case more than 1 sliding
   window is required. (see section 2.4.5.4) */
#define VCP2_REL_LEN_LIMIT_FOR_A_SILDING_WINDOW 248

/* Branch metrics buffer length and branch metrics per
   trellis state for different code rates */
#define VCP2_BM_PER_TRL_FOR_RATE_1_4                8
#define VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_4 3

#define VCP2_BM_PER_TRL_FOR_RATE_1_3                4
#define VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_3 7

#define VCP2_BM_PER_TRL_FOR_RATE_1_2                2
#define VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_2 15

/* Number of bytes transfered to the VCP2 per receive event */
#define VCP2_BYTE_COUNT_PER_RXEVENT  128

/* Maximum hard decisions in out FIFO */
#define VCP2_MAX_HD_IN_OUTFIFO       2048

/* Number of hard decisions per FIFO word */
#define VCP2_HD_PER_OUTFIFO_WORD     64

/* Maximum soft decisions in out FIFO */
#define VCP2_MAX_SD_IN_OUTFIFO       256

/* Number of hard decisions per FIFO word */
#define VCP2_SD_PER_OUTFIFO_WORD     8

/* Number of words in out FIFO */
#define VCP2_OUTFIFO_WORDS           32



int VCP2_Channel_Density_Test(int len,Int8 * restrict deratingdata,unsigned int *restrict decoded_bits);
int TestVCP2(Int8 * restrict deratingdata,VCP2_BaseParams *VcpParameters,int *crc, VCP2Handle hVcp2,unsigned int *restrict decoded_bits);
//int VCP2_Channel_Density_Test(unsigned int *restrict decoded_bits);
//int TestVCP2(VCP2_BaseParams *VcpParameters, Uint8 uiSigma ,int *crc, VCP2Handle hVcp2,int count,unsigned int *restrict decoded_bits);
void	VCP2_ScaleInputData_CalcBM(Int8 * restrict softInput, Int8 * restrict BranchMetrics, Uint32 Rate, Uint32 Length);
void VCP2_genParams_bf (VCP2_BaseParams * restrict pConfigBase,
	VCP2_Params * restrict pConfigParams);
 #ifdef __cplusplus
}
#endif

#endif /*_VCP2_CHANNEL_DENSITY_H_*/
