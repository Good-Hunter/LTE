
#ifndef _KEYSTONE_COMMON_H_
#define _KEYSTONE_COMMON_H_

#include <c6x.h>
#include "tistdtypes.h"
#include "cslr_xmc.h"
#include "cslr_cgem.h"
#include "cslr_bootcfg.h"
#include "cslr_tpcc.h"
#include "cslr_device.h"
#include "cslr_cpintc.h"
#include "cslr_tmr.h"
#include "cslr_srio.h"
#include "cslr_vusr.h"
#include "cslr_i2c.h"
#include "csl_psc.h"
#include "csl_cache.h"
#include "csl_cacheAux.h"
#include "cppi_desc.h"

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint8 RSVD0[228];
    volatile Uint32 RSTYPE;
    volatile Uint32 RSTCTRL;
    volatile Uint32 RSTCFG;
    volatile Uint32 RSISO;
    volatile Uint8 RSVD1[12];
    volatile Uint32 PLLCTL;
    volatile Uint8 RSVD2[4];
    volatile Uint32 SECCTL;
    volatile Uint8 RSVD3[4];
    volatile Uint32 PLLM;
    volatile Uint32 PREDIV;
    volatile Uint32 PLLDIV1;
    volatile Uint32 PLLDIV2;
    volatile Uint32 PLLDIV3;
    volatile Uint8 RSVD4[4];
    volatile Uint32 POSTDIV;
    volatile Uint8 RSVD5[12];
    volatile Uint32 PLLCMD;
    volatile Uint32 PLLSTAT;
    volatile Uint32 ALNCTL;
    volatile Uint32 DCHANGE;
    volatile Uint32 CKEN;
    volatile Uint32 CKSTAT;
    volatile Uint32 SYSTAT;
    volatile Uint8 RSVD6[12];
    volatile Uint32 PLLDIV4;
    volatile Uint32 PLLDIV5;
    volatile Uint32 PLLDIV6;
    volatile Uint32 PLLDIV7;
    volatile Uint32 PLLDIV8;
    volatile Uint32 PLLDIV9;
    volatile Uint32 PLLDIV10;
} CSL_PllcRegs;

typedef enum
{
	NYQUIST = 0, 						
	TRUBO_NYQUIST, 						
	SHANNON,
	UNKNOWN
}TDSP_Type;

/*convert local address to global address for EDMA on multi-core DSP*/
#define GLOBAL_ADDR(addr) (Uint32)addr<0x1000000?\
						(Uint32)addr+(0x10000000+DNUM*0x1000000):\
						(Uint32)addr

extern CSL_XmcRegs * XMC_regs;
extern CSL_CgemRegs * CGEM_regs;
extern CSL_BootcfgRegs * boot_cfg_regs;
extern CSL_PllcRegs * pllc_regs;
extern CSL_PscRegs *  pscRegs;
extern CSL_SrioRegs *  srioRegs;
extern CSL_VusrRegs * hyperLinkRegs;
extern CSL_I2cRegs * i2cRegs;

extern CSL_TpccRegs*  gEDMACC0Regs;
extern CSL_TpccRegs*  gEDMACC1Regs;
extern CSL_TpccRegs*  gEDMACC2Regs;
extern CSL_TpccRegs*  gEDMACCRegs[3];
extern CSL_CPINTCRegs* cpIntc0Regs;
extern CSL_CPINTCRegs* cpIntc1Regs;
extern CSL_TmrRegs * timer0Regs;
extern CSL_TmrRegs * timer1Regs;
extern CSL_TmrRegs * timer2Regs;
extern CSL_TmrRegs * timer3Regs;
extern CSL_TmrRegs * timer4Regs;
extern CSL_TmrRegs * timer5Regs;
extern CSL_TmrRegs * timer6Regs;
extern CSL_TmrRegs * timer7Regs;
extern CSL_TmrRegs * timer8Regs;
extern CSL_TmrRegs * timerRegs[9];

static inline unsigned int SWAP_ENDIAN(unsigned int byte_index)
{
#ifdef _BIG_ENDIAN
	return ((byte_index&0xFFFFFFFC)+(3-byte_index&3));
#else
	return byte_index;
#endif
}

static inline void InvalidPrefetchBuffer()
{
	//invalid prefetch buffer
	XMC_regs->XPFCMD = 1<<CSL_XMC_XPFCMD_INV_SHIFT;
}
 
static inline void Invalid(void * blockPtr, Uint32 byteCnt)
{
	if(byteCnt>0xFFFF*4)
		byteCnt= 0xFFFF*4;
	if((Uint32)blockPtr>=0x0C000000&&(Uint32)blockPtr<0x0C400000)	//SL2
	{
		CACHE_invL1d((void *)blockPtr, byteCnt, CACHE_WAIT);
		InvalidPrefetchBuffer();
	}
	else if((Uint32)blockPtr>=0x10000000) 	//DDR or other cores
	{
		CACHE_invL2((void *)blockPtr, byteCnt, CACHE_WAIT);
		InvalidPrefetchBuffer();
	}
}

static inline void WritebackInvalidAll(void * blockPtr)
{
	if((Uint32)blockPtr>=0x0C000000&&(Uint32)blockPtr<0x0C400000)	//SL2
	{
		CACHE_wbInvAllL1d(CACHE_WAIT);//CACHE_invL1d((void *)blockPtr, byteCnt, CACHE_WAIT);
		InvalidPrefetchBuffer();
	}
	else if((Uint32)blockPtr>=0x10000000) 	//DDR or other cores
	{
		//CACHE_invL2((void *)blockPtr, byteCnt, CACHE_WAIT);
		CACHE_wbInvAllL2(CACHE_WAIT);
		InvalidPrefetchBuffer();
	}
}


static inline void Writeback(void * blockPtr, Uint32 byteCnt)
{
	if(byteCnt>0xFFFF*4)
		byteCnt= 0xFFFF*4;
	if((Uint32)blockPtr>=0x0C000000&&(Uint32)blockPtr<0x0C400000)	//SL2
	{
		CACHE_wbL1d((void *)blockPtr, byteCnt, CACHE_WAIT);
	}
	else if((Uint32)blockPtr>=0x10000000) 	//DDR or other cores
	{
		CACHE_wbL2((void *)blockPtr, byteCnt, CACHE_WAIT);
	}
}
 
static inline void WritebackAll(void * blockPtr)
{
	if((Uint32)blockPtr>=0x0C000000&&(Uint32)blockPtr<0x0C400000)	//SL2
	{
		CACHE_wbAllL1d(CACHE_WAIT);;
	}
	else if((Uint32)blockPtr>=0x10000000) 	//DDR or other cores
	{
		CACHE_wbAllL2(CACHE_WAIT);
	}
}
static inline TDSP_Type Get_dsp_type()
{
	if(0x0009D02F == (boot_cfg_regs->DEVICE_ID_REG0&0xFFFFFFF))
		return NYQUIST;
	if(0x0B94102F == (boot_cfg_regs->DEVICE_ID_REG0&0xFFFFFFF))
		return TRUBO_NYQUIST;
	if(0x0009E02F == (boot_cfg_regs->DEVICE_ID_REG0&0xFFFFFFF))
		return SHANNON;
	return UNKNOWN;
}

static inline Uint32 Get_DSP_Number()
{
	TDSP_Type dspType= Get_dsp_type();

	if(NYQUIST==dspType||TRUBO_NYQUIST==dspType)
	{
		/*Nyquist EVM use boot configure pin 4 and 5 to identify DSPs*/
		return (( boot_cfg_regs->BOOT_REG0 >> (3+1) ) & 0x03);
	}

	/*if use DNUM to differentiate DSPs for two DSPs test,
	the program must be run on core 0 of DSP 0 and core 1 of DSP1*/
	return DNUM;
}


extern void KeyStone_main_PLL_init ( unsigned int main_PLLM, unsigned int main_PLLD);
extern void KeyStone_DDR_PLL_init (unsigned int DDR_PLLM, unsigned int DDR_PLLD);
extern void KeyStone_PA_PLL_init (unsigned int PA_PLLM,	unsigned int PA_PLLD);

extern void lock_mem_prot_regs();
extern void unlock_mem_prot_regs();
extern void KeyStone_XM_cfg();
extern void KeyStone_L1_cfg();
extern void KeyStone_L2_cfg();

extern void edma_Init ();

extern Int32 Keystone_enable_PSC_module (Uint32 pwrDmnNum, Uint32 moduleNum);
extern Int32 Keystone_disable_PSC_module (Uint32 pwrDmnNum, Uint32 moduleNum);
extern Int32 Keystone_disable_PSC_Power_Domain (Uint32 pwrDmnNum);

#endif

