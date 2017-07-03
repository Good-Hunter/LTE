/****************************************************************************\
 *           Copyright (C) 2000 Texas Instruments Incorporated.             *
 *                           All Rights Reserved                            *
 *                                                                          *
 * GENERAL DISCLAIMER                                                       *
 * ------------------                                                       *
 * All software and related documentation is provided "AS IS" and without   *
 * warranty or support of any kind and Texas Instruments expressly disclaims*
 * all other warranties, express or implied, including, but not limited to, *
 * the implied warranties of merchantability and fitness for a particular   *
 * purpose.  Under no circumstances shall Texas Instruments be liable for   *
 * any incidental, special or consequential damages that result from the    *
 * use or inability to use the software or related documentation, even if   *
 * Texas Instruments has been advised of the liability.                     *
 ****************************************************************************
 *                                                                          *
 * Written by :                                                             *
 *            Sebastien Tomas  (VCP2/TCP2)                                  *
 *            Texas Instruments                                             * 
 *            12 May, 2005                                                  *
 *                                                                          *
 * The purpose of this test is to evaluate 
 * VCP2 and TCP2 co-processors in the TCI6482 (Himalaya)               *
 *									                                        *
 * This test should not be redistributed for any reason without             *
 * permission.                                                              *
 *                                                                          *
 Modified by Brighton Feng 12 July, 2006 for VCP2/TCP2 channel density test.
\***************************************************************************/
#ifndef _CHANNEL_MODEL_H
#define _CHANNEL_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <tistdtypes.h>

void AddAWGN_fixpt(Uint32 *in,Uint32 *out, const Uint8 sigma_fixpt, const Uint32 length);

void MapOnBaseband(unsigned int* in,int len, unsigned int* out);

Uint32 GenerateBits(Uint32* packedData, Uint32 len);

void little2big(Int8 *input,Int8 *output,Uint32 byteLength);

int	addCyclic(	char *output,
				char *input,
				int length,
				int prefixSize,
				int postfixSize,
				int rate);

#ifdef __cplusplus
}
#endif

#endif

