/*
 * Interrupt_Config_.h
 *
 *	本程序想构建一个可以独立运行的CIC程序
 *
 *  Created on: 2014-7-20
 *      Author: castMemory
 */

#ifndef INTERRUPT_CONFIG_H_
#define INTERRUPT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
					#include <ti/csl/src/intc/_csl_intc.h>
					#include <ti/csl/src/intc/csl_intc.h>
					#include <ti/csl/src/intc/csl_intcAux.h>
					#include <ti/csl/src/intc/cslr_intc.h>

					#include <ti/csl/csl_cpintc.h>
					#include <ti/csl/csl_cpintcAux.h>
*/

/*  Include files
 */
//加入intc模块的头文件
#include <ti\csl\src\intc\_csl_intc.h>
#include <ti\csl\src\intc\csl_intcAux.h>

//加入CIC模块的头文件
#include <ti\csl\csl_cpintc.h>
#include <ti\csl\csl_cpintcAux.h>

//加入自定义头文件
#include <SRIO_error.h>

#define INTC_MAX_EVENT_NUM						128				//intc模块能接受的 event 个数

//CIC0和intc的映射
#define CIC0_OUT_8_PLUS_16_MUL_N(n) 	(0x8 + (16* n))
#define CIC0_OUT_9_PLUS_16_MUL_N(n) 	(0x9 + (16* n))
#define CIC0_OUT_10_PLUS_16_MUL_N(n) 	(0xA + (16* n))
#define CIC0_OUT_11_PLUS_16_MUL_N(n) 	(0xB + (16* n))
#define CIC0_OUT_12_PLUS_16_MUL_N(n) 	(0xC + (16* n))
#define CIC0_OUT_13_PLUS_16_MUL_N(n) 	(0xD + (16* n))
#define CIC0_OUT_14_PLUS_16_MUL_N(n) 	(0xE + (16* n))
#define CIC0_OUT_15_PLUS_16_MUL_N(n) 	(0xF + (16* n))

#define CIC0_OUT_64_PLUS_10_MUL_N(n) (64 + 0 + (n*10))
#define CIC0_OUT_65_PLUS_10_MUL_N(n) (64 + 1 + (n*10))
#define CIC0_OUT_66_PLUS_10_MUL_N(n) (64 + 2 + (n*10))
#define CIC0_OUT_67_PLUS_10_MUL_N(n) (64 + 3 + (n*10))
#define CIC0_OUT_68_PLUS_10_MUL_N(n) (64 + 4 + (n*10))
#define CIC0_OUT_69_PLUS_10_MUL_N(n) (64 + 5 + (n*10))
#define CIC0_OUT_70_PLUS_10_MUL_N(n) (64 + 6 + (n*10))
#define CIC0_OUT_71_PLUS_10_MUL_N(n) (64 + 7 + (n*10))
#define CIC0_OUT_72_PLUS_10_MUL_N(n) (64 + 8 + (n*10))
#define CIC0_OUT_73_PLUS_10_MUL_N(n) (64 + 9 + (n*10))

#define CIC0_TO_HOST_INTR_MAP					\
{												\
	CIC0_OUT_8_PLUS_16_MUL_N(0),				\
	CIC0_OUT_8_PLUS_16_MUL_N(1),				\
	CIC0_OUT_8_PLUS_16_MUL_N(2),				\
	CIC0_OUT_8_PLUS_16_MUL_N(3),				\
												\
	CIC0_OUT_9_PLUS_16_MUL_N(0),				\
	CIC0_OUT_9_PLUS_16_MUL_N(1),				\
	CIC0_OUT_9_PLUS_16_MUL_N(2),				\
	CIC0_OUT_9_PLUS_16_MUL_N(3),				\
												\
	CIC0_OUT_10_PLUS_16_MUL_N(0),				\
	CIC0_OUT_10_PLUS_16_MUL_N(1),				\
	CIC0_OUT_10_PLUS_16_MUL_N(2),				\
	CIC0_OUT_10_PLUS_16_MUL_N(3),				\
												\
	CIC0_OUT_11_PLUS_16_MUL_N(0),				\
	CIC0_OUT_11_PLUS_16_MUL_N(1),				\
	CIC0_OUT_11_PLUS_16_MUL_N(2),				\
	CIC0_OUT_11_PLUS_16_MUL_N(3),				\
}

/** Application Callback Function pointer typedef */
typedef void (* AppCallBackFunction ) (void *);

typedef struct
{
    /** Pointer to callback function */
    AppCallBackFunction pCallbackFunc;
    /** Pointer to Argumnet to be passed to the callback function when it is
     * invoked
     */
    void *pArg;
}AppCallback_Func;

//全局变量，CIC模块的句柄
CSL_CPINTC_Handle cpIntc;

extern Uint32 IntcInitialise ( void );
extern Uint32 CPIntc_init ( void );
extern Uint32 CPIntc_for_Srio ( void );
extern Uint32 CPIntc_for_Timer ( void );

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_CONFIG_H_ */
