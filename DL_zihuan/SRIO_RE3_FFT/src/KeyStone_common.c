
#include <stdio.h>
#include "csl_bootcfgAux.h"
#include "csl_pscAux.h"
#include "KeyStone_common.h"

CSL_XmcRegs * XMC_regs = (CSL_XmcRegs *) CSL_XMC_CONFIG_REGS;
//CSL_CgemRegs * CGEM_regs = (CSL_CgemRegs *)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS;  //CS 11/14/05
CSL_BootcfgRegs * boot_cfg_regs = (CSL_BootcfgRegs *)CSL_BOOT_CFG_REGS;
CSL_CPINTCRegs* cpIntc0Regs = (CSL_CPINTCRegs*)CSL_CP_INTC_0_REGS;
CSL_CPINTCRegs* cpIntc1Regs = (CSL_CPINTCRegs*)CSL_CP_INTC_0_REGS;
CSL_PllcRegs * pllc_regs = (CSL_PllcRegs * )0x02310000;
CSL_PscRegs *  pscRegs =   (CSL_PscRegs *)CSL_PSC_REGS;
CSL_SrioRegs *  srioRegs =   (CSL_SrioRegs *)CSL_SRIO_CONFIG_REGS;
CSL_VusrRegs * hyperLinkRegs = (CSL_VusrRegs *)0x21400000;
CSL_I2cRegs * i2cRegs = (CSL_I2cRegs *)CSL_I2C_DATA_CONTROL_REGS;

CSL_TpccRegs*  gEDMACC0Regs  = (CSL_TpccRegs*)CSL_EDMA0CC_REGS;
CSL_TpccRegs*  gEDMACC1Regs  = (CSL_TpccRegs*)CSL_EDMA1CC_REGS;
CSL_TpccRegs*  gEDMACC2Regs  = (CSL_TpccRegs*)CSL_EDMA2CC_REGS;
CSL_TpccRegs*  gEDMACCRegs[3]  = {
	(CSL_TpccRegs*)CSL_EDMA0CC_REGS,
	(CSL_TpccRegs*)CSL_EDMA1CC_REGS,
	(CSL_TpccRegs*)CSL_EDMA2CC_REGS
};

CSL_TmrRegs * timer0Regs = (CSL_TmrRegs *)CSL_TIMER_0_REGS;
CSL_TmrRegs * timer1Regs = (CSL_TmrRegs *)CSL_TIMER_1_REGS;
CSL_TmrRegs * timer2Regs = (CSL_TmrRegs *)CSL_TIMER_2_REGS;
CSL_TmrRegs * timer3Regs = (CSL_TmrRegs *)CSL_TIMER_3_REGS;
CSL_TmrRegs * timer4Regs = (CSL_TmrRegs *)CSL_TIMER_4_REGS;
CSL_TmrRegs * timer5Regs = (CSL_TmrRegs *)CSL_TIMER_5_REGS;
CSL_TmrRegs * timer6Regs = (CSL_TmrRegs *)CSL_TIMER_6_REGS;
CSL_TmrRegs * timer7Regs = (CSL_TmrRegs *)CSL_TIMER_7_REGS;
CSL_TmrRegs * timer8Regs = (CSL_TmrRegs *)(CSL_TIMER_7_REGS+(CSL_TIMER_7_REGS-CSL_TIMER_6_REGS));
CSL_TmrRegs * timerRegs[9] = {
	(CSL_TmrRegs *)CSL_TIMER_0_REGS,
	(CSL_TmrRegs *)CSL_TIMER_1_REGS,
	(CSL_TmrRegs *)CSL_TIMER_2_REGS,
	(CSL_TmrRegs *)CSL_TIMER_3_REGS,
	(CSL_TmrRegs *)CSL_TIMER_4_REGS,
	(CSL_TmrRegs *)CSL_TIMER_5_REGS,
	(CSL_TmrRegs *)CSL_TIMER_6_REGS,
	(CSL_TmrRegs *)CSL_TIMER_7_REGS,
	(CSL_TmrRegs *)(CSL_TIMER_7_REGS+(CSL_TIMER_7_REGS-CSL_TIMER_6_REGS))
};

void KeyStone_main_PLL_init ( unsigned int main_PLLM, unsigned int main_PLLD)
{
	unsigned int i;

	if(0<main_PLLM&&main_PLLM<=4096&&0<main_PLLD&&main_PLLD<=64)
	{
		CSL_BootCfgUnlockKicker();

		boot_cfg_regs->CORE_PLL_CTL1 |= 0x00000040; //Set ENSAT = 1

		printf("Initialize main PLL = x%d/%d\n", main_PLLM, main_PLLD);

		//Bypass the PLL, PLL OUTPUT_DIVIDER = 2
		pllc_regs->SECCTL |= 0x00880000;

		/*Clear the PLLENSRC bit in PLLCTL to 0 to allow PLLCTL.PLLEN to take effect*/
		pllc_regs->PLLCTL= pllc_regs->PLLCTL&(~(1<<5));

		/*Clear the PLLEN bit in PLLCTL to 0 (select PLL bypass mode)*/
		pllc_regs->PLLCTL= pllc_regs->PLLCTL&(~(1<<0));

		/*Wait for 4 input clock cycles to ensure PLLC switches to bypass mode properly*/
		for(i=0; i< 4*8192/5+1; i++)
			asm(" nop 5");

		/*The PLL has a bandgap generator circuit driving the PLL voltage regulator. In order
		to ensure proper PLL startup, the PLL power_down pin needs to be toggled. This is
		accomplished by toggling the ¡°PLLPWRDN¡± bit in the PLLCTL register. This needs to
		be done before the main PLL initialization sequence. */
		pllc_regs->PLLCTL |= (1<<1);	//Power down the PLL
		// wait for 5 us (min)
		for(i=0; i< 1000; i++)
			asm(" nop 5");

		// Verify if pll is in power down
		if ((pllc_regs->PLLCTL & (1<<1)) !=0 )
		{
			pllc_regs->PLLCTL &= ~(1<<1);         // Power up PLL
			// Wait PLL Stabilization time that is 150 usec
			for(i=0; i< 30000; i++)
				asm(" nop 5");
		}

		/*In PLLCTL, write PLLRST = 1 (PLL is reset)*/
		pllc_regs->PLLCTL= pllc_regs->PLLCTL|(1<<3);

		/*Program the required multiplier and divider values*/
		//pllc_regs->PREDIV= 0x8000;

		/*PLLM[5:0] bits of the multiplier is controlled by the PLLM 
		register inside the PLL controller and PLLM[12:6] bits are 
		controlled by the chip level register. MAINPLLCTL0 register 
		PLLM[12:6] bits should be written just before writing to PLLM 
		register PLLM[5:0] bits in the controller to have the complete 
		13 bit value latched when the GO operation is initiated in the 
		PLL controller.*/
		boot_cfg_regs->CORE_PLL_CTL0 = ((main_PLLM-1)<<24)| 	/*BWADJ[7:0]*/
			(((main_PLLM*2-1)&0x1FC0)<<6)|(main_PLLD-1);
		pllc_regs->PLLM= (main_PLLM*2-1)&0x3F;

		boot_cfg_regs->CORE_PLL_CTL1 = 0x00000040      /*Set ENSAT bit = 1*/
			|(main_PLLM-1)>>8;	/*BWADJ[11:8]*/
		//pllc_regs->POSTDIV= 0x8000;

		/*Wait 1000 ns for PLL reset*/
		for(i=0; i< 1500/5+1; i++)
			asm(" nop 5");
			
		/*In PLLCTL, write PLLRST = 0 to bring PLL out of reset*/
		pllc_regs->PLLCTL= pllc_regs->PLLCTL&(~(1<<3));

		/*Wait 2000 input clock cycles for PLL to lock*/
		for(i=0; i< 2000*8192/5+1; i++)
			asm(" nop 5");
			
		//clear the bypass bit, enable PLL
		pllc_regs->SECCTL &= ~(0x00800000);
		
		/*Set the PLLEN bit in PLLCTL to 1 to enable PLL mode*/
		pllc_regs->PLLCTL= pllc_regs->PLLCTL|(1<<0);

	}

}


void KeyStone_DDR_PLL_init (unsigned int DDR_PLLM, unsigned int DDR_PLLD)
{
	int i;
	
	if(0<DDR_PLLM&&DDR_PLLM<=8192&&0<DDR_PLLD&&DDR_PLLD<=64)
	{
		CSL_BootCfgUnlockKicker();

		printf("Initialize DDR PLL = x%d/%d\n", DDR_PLLM, DDR_PLLD);
		boot_cfg_regs->DDR3_PLL_CTL1 = 0x00000040      /*Set ENSAT bit = 1*/
			|((DDR_PLLM-1)/2)>>8;	/*BWADJ[11:8]*/
		boot_cfg_regs->DDR3_PLL_CTL1 |= 0x00002000;      //Set RESET bit = 1
		boot_cfg_regs->DDR3_PLL_CTL0 = (boot_cfg_regs->DDR3_PLL_CTL0&0x00780000)|
			(((DDR_PLLM-1)/2)<<24)|((DDR_PLLM-1)<<6)|(DDR_PLLD-1);
		for(i=0;i<1000;i++)
			asm(" nop");            //Wait >1000ns for reset to complete
		boot_cfg_regs->DDR3_PLL_CTL1 &= ~(0x00002000);   //Clear RESET bit
		for(i=0;i<10000;i++)
			asm(" nop");          //Wait >2000 clocks (>30us) for PLL lock

	}
}

void KeyStone_PA_PLL_init (unsigned int PA_PLLM,	unsigned int PA_PLLD)
{
	if(0<PA_PLLM&&PA_PLLM<=8192&&0<PA_PLLD&&PA_PLLD<=64)
	{
		CSL_BootCfgUnlockKicker();

		printf("Initialize PA PLL = x%d/%d\n", PA_PLLM, PA_PLLD);
		boot_cfg_regs->PA_PLL_CTL0 = (boot_cfg_regs->PA_PLL_CTL0&0x00F80000)|
			(((PA_PLLM-1)/2)<<24)|((PA_PLLM-1)<<6)|(PA_PLLD-1);
		boot_cfg_regs->PA_PLL_CTL1 = 0x00000040      /*Set ENSAT bit = 1*/
			|((PA_PLLM-1)/2)>>8;	/*BWADJ[11:8]*/

	}
}

unsigned int mem_prot_key[4] = {1, 2, 3, 4};

/*lock memory protection registers*/
void lock_mem_prot_regs()
{
	int i;
	
	/*1. Write a 1 to the KEYR field of the MPLKCMD register. This resets some internal
	status for the MPLK0 through MPLK3 registers.*/
	CGEM_regs->MPLKCMD = (1<<CSL_CGEM_MPLKCMD_KEYR_SHIFT);
	
	/*2. Write the key to MPLK0 through MPLK3. All four registers must be written
	exactly once. They may be written in any order.*/
	for(i=0; i<4; i++)
	{
		CGEM_regs->MPLK[i] = mem_prot_key[i];
	}

	/*3. Write a 1 to the LOCK field of the MPLKCMD register. This engages the lock.*/
	CGEM_regs->MPLKCMD = (1<<CSL_CGEM_MPLKCMD_LOCK_SHIFT);

	/*wait to make sure it is locked*/
	while(0==(CGEM_regs->MPLKSTAT&CSL_CGEM_MPLKSTAT_LK_MASK));
}

/*unlock memory protection registers*/
void unlock_mem_prot_regs()
{
	int i;
	
	/*1. Write a 1 to the KEYR field in the MPLKCMD register. This resets some internal
	status for the MPLK0 through the MPLK3 registers.*/
	CGEM_regs->MPLKCMD = (1<<CSL_CGEM_MPLKCMD_KEYR_SHIFT);

	/*2. Write the unlock key to MPLK0 through the MPLK3 registers. The hardware
	compares the written value with the stored key value. Software must write to all
	four registers exactly once. The writes can arrive in any order.*/
	for(i=0; i<4; i++)
	{
		CGEM_regs->MPLK[i] = mem_prot_key[i];
	}

	/*3. Write a 1 to the UNLOCK field in the MPLKCMD register. If the key written in
	step 2 matches the stored key, the hardware disengages the lock. If the key written
	in step 2 does not match, the hardware signals an exception. The hardware
	reports the fault address as the address of the MPLKCMD register.*/
	CGEM_regs->MPLKCMD = (1<<CSL_CGEM_MPLKCMD_UNLOCK_SHIFT);

	/*wait to make sure it is unlocked*/
	while(1==(CGEM_regs->MPLKSTAT&CSL_CGEM_MPLKSTAT_LK_MASK));
}

/*XMC memory address extension/mapping and memory protection*/
void KeyStone_XM_cfg()
{
	if(CGEM_regs->MPLKSTAT&CSL_CGEM_MPLKSTAT_LK_MASK)
		unlock_mem_prot_regs();
	
	/*change memory attribute of the default memory segement 0, 1 
	to read/write, not for execution, to catch runaway code*/
	XMC_regs->XMPAX[0].XMPAXH = 
		((0)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((30)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(30+1)=2GB*/
	XMC_regs->XMPAX[0].XMPAXL = 
		((0)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((1)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UX_SHIFT));
	XMC_regs->XMPAX[1].XMPAXH = 
		((0x80000000)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((30)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(30+1)=2GB*/
	XMC_regs->XMPAX[1].XMPAXL = 
		((0x80000000)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((1)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UX_SHIFT));

	/*map DDR2 configuration registers at physical address 0x1:00000000 to logical address 0x21000000*/
	XMC_regs->XMPAX[2].XMPAXH = 
		((0x21000000)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((11)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(11+1)=4KB*/
	XMC_regs->XMPAX[2].XMPAXL = 
		((0x10000000)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((1)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UX_SHIFT));

	/*remap Shared L2 to 0x18000000 for read/write*/
	XMC_regs->XMPAX[3].XMPAXH = 
		((0x18000000)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((23)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(23+1)=16MB*/
	XMC_regs->XMPAX[3].XMPAXL = 
		((0x00C00000)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((1)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UX_SHIFT));
#if 0
	/*remap the first 16MB DDR for code*/
	XMC_regs->XMPAX[4].XMPAXH = 
		((0x80000000)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((23)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(23+1)=16MB*/
	XMC_regs->XMPAX[4].XMPAXL = 
		((0x80000000)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((0)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UX_SHIFT));

	/*remap the second 16MB DDR for read only*/
	XMC_regs->XMPAX[5].XMPAXH = 
		((0x81000000)&(CSL_XMC_XMPAXH_BADDR_MASK))|
		((23)<<(CSL_XMC_XMPAXH_SEGSZ_SHIFT)); 	/*2^(23+1)=16MB*/
	XMC_regs->XMPAX[5].XMPAXL = 
		((0x80100000)&(CSL_XMC_XMPAXL_RADDR_MASK))|
		((1)<<(CSL_XMC_XMPAXL_SR_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_SX_SHIFT))|
		((1)<<(CSL_XMC_XMPAXL_UR_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UW_SHIFT))|
		((0)<<(CSL_XMC_XMPAXL_UX_SHIFT));
#endif
	lock_mem_prot_regs();
}

/*----configure the memory protection for local L1 memory----*/
void KeyStone_L1_cfg()
{
	int i;

	if(CGEM_regs->MPLKSTAT&CSL_CGEM_MPLKSTAT_LK_MASK)
		unlock_mem_prot_regs();

	/*clear any memory protection fault*/
	CGEM_regs->L1PMPFCR = 1;
	CGEM_regs->L1DMPFCR = 1;
	CGEM_regs->L2MPFCR = 1;

	/*set L1P only for local execution*/
	for(i=0; i<16; i++)
	{
		CGEM_regs->L1PMPPA[i] = 
			((0)<<CSL_CGEM_L1PMPPA_AID5_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AID4_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AID3_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AID2_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AID1_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AID0_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_AIDX_SHIFT)|
			((1)<<CSL_CGEM_L1PMPPA_LOCAL_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_SR_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_SW_SHIFT)|
			((1)<<CSL_CGEM_L1PMPPA_SX_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_UR_SHIFT)|
			((0)<<CSL_CGEM_L1PMPPA_UW_SHIFT)|
			((1)<<CSL_CGEM_L1PMPPA_UX_SHIFT);
	}
	
	/*set L1D only for local read/write*/
	for(i=0; i<16; i++)
	{
		CGEM_regs->L1DMPPA[i] = 
			((0)<<CSL_CGEM_L1DMPPA_AID5_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AID4_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AID3_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AID2_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AID1_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AID0_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_AIDX_SHIFT)|
			((1)<<CSL_CGEM_L1DMPPA_LOCAL_SHIFT)|
			((1)<<CSL_CGEM_L1DMPPA_SR_SHIFT)|
			((1)<<CSL_CGEM_L1DMPPA_SW_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_SX_SHIFT)|
			((1)<<CSL_CGEM_L1DMPPA_UR_SHIFT)|
			((1)<<CSL_CGEM_L1DMPPA_UW_SHIFT)|
			((0)<<CSL_CGEM_L1DMPPA_UX_SHIFT);
	}
	lock_mem_prot_regs();
}

/*----configure the memory protection for local L2 memory----*/
void KeyStone_L2_cfg()
{
	int i;
	TDSP_Type dspType;

	if(CGEM_regs->MPLKSTAT&CSL_CGEM_MPLKSTAT_LK_MASK)
		unlock_mem_prot_regs();
		
	dspType= Get_dsp_type();
	if(NYQUIST==dspType||TRUBO_NYQUIST==dspType)
	{
		/*set first 64KB LL2 only for local execution*/
		for(i=0; i<2; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((0)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}

		/*set 32KB LL2 only for read only*/
		for(i=2; i<3; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((1)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}
		/*set the remaining LL2 for read/write*/
		for(i=3; i<32; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((1)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}
	}
	else if(SHANNON==dspType)
	{
		/*set first 64KB LL2 only for local execution*/
		for(i=0; i<4; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((0)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}

		/*set 32KB LL2 only for read only*/
		for(i=4; i<6; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((1)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}
		/*set the remaining LL2 for read/write*/
		for(i=6; i<32; i++)
		{
			CGEM_regs->L2MPPA[i] = 
				((1)<<CSL_CGEM_L2MPPA_AID5_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID4_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID3_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID2_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID1_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AID0_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_AIDX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_LOCAL_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SR_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_SW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_SX_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UR_SHIFT)|
				((1)<<CSL_CGEM_L2MPPA_UW_SHIFT)|
				((0)<<CSL_CGEM_L2MPPA_UX_SHIFT);
		}
	}

	lock_mem_prot_regs();
}

void edma_Init ()
{
	int i;
	unsigned int * uipPaRAM;

	/*clear PaRAM*/
	uipPaRAM= (unsigned int *)&(gEDMACC0Regs->PARAMSET[0]);
	for(i=0; i<8*CSL_EDMA3_TPCC0_NUM_PARAMSETS; i++) 	
		*uipPaRAM++=0;

	uipPaRAM= (unsigned int *)&(gEDMACC1Regs->PARAMSET[0]);
	for(i=0; i<8*CSL_EDMA3_TPCC1_NUM_PARAMSETS; i++) 	
		*uipPaRAM++=0;

	uipPaRAM= (unsigned int *)&(gEDMACC2Regs->PARAMSET[0]);
	for(i=0; i<8*CSL_EDMA3_TPCC2_NUM_PARAMSETS; i++) 	
		*uipPaRAM++=0;
		
	/*Assign PaRAM for different channels*/
	for(i=0; i<CSL_EDMA3_TPCC0_NUM_DMACH; i++) 	
		gEDMACC0Regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

	for(i=0; i<CSL_EDMA3_TPCC1_NUM_DMACH; i++) 	
		gEDMACC1Regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

	for(i=0; i<CSL_EDMA3_TPCC2_NUM_DMACH; i++) 	
		gEDMACC2Regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

	/*Assign TC/Queue for different channels*/
	gEDMACC0Regs->TPCC_DMAQNUM[0]= 0x10101010;
	gEDMACC0Regs->TPCC_DMAQNUM[1]= 0x10101010;

	gEDMACC1Regs->TPCC_DMAQNUM[0]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[1]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[2]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[3]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[4]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[5]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[6]= 0x32103210;
	gEDMACC1Regs->TPCC_DMAQNUM[7]= 0x32103210;

	gEDMACC2Regs->TPCC_DMAQNUM[0]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[1]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[2]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[3]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[4]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[5]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[6]= 0x32103210;
	gEDMACC2Regs->TPCC_DMAQNUM[7]= 0x32103210;

	/*clear any events and status*/
	gEDMACC0Regs->TPCC_ECR= 0xFFFFFFFF;
	gEDMACC0Regs->TPCC_EECR= 0xFFFFFFFF;
	gEDMACC0Regs->TPCC_ICR= 0xFFFFFFFF;
	gEDMACC0Regs->TPCC_IECR= 0xFFFFFFFF;
	gEDMACC0Regs->TPCC_EMCR= 0xFFFFFFFF;
	gEDMACC0Regs->TPCC_SECR= 0xFFFFFFFF;

	gEDMACC1Regs->TPCC_ECR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_ECRH= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_EECR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_EECRH= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_ICR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_ICRH= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_IECR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_IECRH= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_EMCR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_EMCRH= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_SECR= 0xFFFFFFFF;
	gEDMACC1Regs->TPCC_SECRH= 0xFFFFFFFF;

	gEDMACC2Regs->TPCC_ECR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_ECRH= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_EECR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_EECRH= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_ICR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_ICRH= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_IECR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_IECRH= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_EMCR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_EMCRH= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_SECR= 0xFFFFFFFF;
	gEDMACC2Regs->TPCC_SECRH= 0xFFFFFFFF;

}

signed int Keystone_enable_PSC_module (unsigned int pwrDmnNum, unsigned int moduleNum)
{
    /* Set Power domain to ON */
    CSL_PSC_enablePowerDomain (pwrDmnNum);

    /* Enable the clocks too*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if ((CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_ENABLE))
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }
}

signed int Keystone_disable_PSC_module (unsigned int pwrDmnNum, unsigned int moduleNum)
{
    /* disable the clocks*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_SWRSTDISABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_SWRSTDISABLE)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}

signed int Keystone_disable_PSC_Power_Domain (unsigned int pwrDmnNum)
{
    /* Set Power domain to OFF */
    CSL_PSC_disablePowerDomain (pwrDmnNum);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if (CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_OFF)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}

