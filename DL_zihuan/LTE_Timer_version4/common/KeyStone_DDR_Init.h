
#ifndef _KEYSTONE_DDR_INIT_H_
#define _KEYSTONE_DDR_INIT_H_
#include "cslr_emif4f.h"

typedef enum
{
	DDR_TERM_DISABLED= 0, 						
	DDR_TERM_RZQ_OVER_4, 						
	DDR_TERM_RZQ_OVER_2, 						
	DDR_TERM_RZQ_OVER_6, 						
	DDR_TERM_RZQ_OVER_12, 						
	DDR_TERM_RZQ_OVER_8 						
}T_DDR_TERM;

typedef enum
{
	DDR_DYN_ODT_DISABLED= 0, 						
	DDR_DYN_ODT_OVER_4, 						
	DDR_DYN_ODT_OVER_2 						
}T_DDR_DYN_ODT;

typedef enum
{
	SDRAM_DRIVE_RZQ_OVER_6= 0, 						
	SDRAM_DRIVE_RZQ_OVER_7 						
}T_SDRAM_DRIVE;

typedef enum
{
	DDR_CWL_5= 0, 						
	DDR_CWL_6, 						
	DDR_CWL_7, 						
	DDR_CWL_8 						
}T_DDR_CWL;

typedef enum
{
	DDR_CL_5= 2, 						
	DDR_CL_6= 4, 						
	DDR_CL_7= 6, 						
	DDR_CL_8= 8, 						
	DDR_CL_9= 10, 						
	DDR_CL_10= 12, 						
	DDR_CL_11= 14 						
}T_DDR_CL;

typedef enum
{
	DDR_BUS_WIDTH_64= 0, 						
	DDR_BUS_WIDTH_32, 						
	DDR_BUS_WIDTH_16					
}T_DDR_BUS_WIDTH;

typedef enum
{
	DDR_ROW_SIZE_9_BIT = 0,
	DDR_ROW_SIZE_10_BIT,
	DDR_ROW_SIZE_11_BIT,
	DDR_ROW_SIZE_12_BIT,
	DDR_ROW_SIZE_13_BIT,
	DDR_ROW_SIZE_14_BIT,
	DDR_ROW_SIZE_15_BIT,
	DDR_ROW_SIZE_16_BIT
}T_DDR_ROW_SIZE;

typedef enum
{
	DDR_BANK_NUM_1= 0, 						
	DDR_BANK_NUM_2, 						
	DDR_BANK_NUM_4, 						
	DDR_BANK_NUM_8					
}T_DDR_BANK_NUM;

typedef enum
{
	DDR_PAGE_SIZE_8_BIT_256_WORD = 0,
	DDR_PAGE_SIZE_9_BIT_512_WORD,
	DDR_PAGE_SIZE_10_BIT_1024_WORD,
	DDR_PAGE_SIZE_11_BIT_2048_WORD
}T_DDR_PAGE_SIZE;

extern CSL_Emif4fRegs * DDR_Regs;

extern void Shannon_EVM_DDR_Init(float clock_MHz);
extern void Nyquist_EVM_DDR_Init(float clock_MHz);

#endif

