
#include <stdio.h>
#include <c6x.h>
#include "tistdtypes.h"
#include "cslr_chip.h"
#include "KeyStone_common.h"

void Exception_cfg()
{
	puts("Enable Exception handling for memory protection, interrupt drop and bus error");

	/*clear all exception events*/
	CGEM_regs->EVTCLR[0] = 0xFFFFFFFF;
	CGEM_regs->EVTCLR[1] = 0xFFFFFFFF;
	CGEM_regs->EVTCLR[2] = 0xFFFFFFFF;
	CGEM_regs->EVTCLR[3] = 0xFFFFFFFF;

	/*clear memory falut*/
	CGEM_regs->L1PMPFCR = 1;
	CGEM_regs->L1DMPFCR = 1;
	CGEM_regs->L2MPFCR = 1;
	XMC_regs->XMPFCR = 1;

	/*clear interrupt drop staus*/
	CGEM_regs->INTXCLR = 1;

	/*enable events:
	10 MSMC_mpf_error_n Memory protection fault indicators for local CorePac
	96 INTERR Dropped CPU interrupt event
	119 SYS_CMPA CPU Memory Protection Fault
	120 PMC_CMPA CPU memory protection fault
	121 PMC_DMPA DMA memory protection fault
	122 DMC_CMPA CPU memory protection fault
	123 DMC_DMPA DMA memory protection fault
	124 UMC_CMPA CPU memory protection fault
	125 UMC_DMPA DMA memory protection fault
	126 EMC_CMPA CPU memory protection fault
	127 EMC_BUSERR Bus Error Interrupt
	*/
	CGEM_regs->EXPMASK[0]= ~(1<<10);
	CGEM_regs->EXPMASK[1]= 0xFFFFFFFF;
	CGEM_regs->EXPMASK[2]= 0xFFFFFFFF;
	CGEM_regs->EXPMASK[3]= ~(1|
		(1<<(119-96))|
		(1<<(120-96))|
		(1<<(121-96))|
		(1<<(122-96))|
		(1<<(123-96))|
		(1<<(124-96))|
		(1<<(125-96))|
		(1<<(126-96))|
		(1<<(127-96)));

	/*clear exception flag*/
	ECR = EFR;
	IERR = 0; 

	/*Eanble external exception, global exception enable*/
	TSR = TSR|
		(1<<CSL_CHIP_TSR_XEN_SHIFT)|
		(1<<CSL_CHIP_TSR_GEE_SHIFT);

	/*enable NMI exception*/
	IER = IER|(1<<CSL_CHIP_IER_NMI_SHIFT);
	
}

void memory_protection_exception(unsigned int MPFAR, 
	unsigned int MPFSR)
{
	unsigned int master_id;

	if(0 == MPFSR)
		return;

	master_id = (MPFSR&CSL_CGEM_L2MPFSR_FID_MASK)>>
		CSL_CGEM_L2MPFSR_FID_SHIFT;

	printf("memory protection exception caused by master %d at 0x%x\n", 
		master_id, MPFAR);

	if (MPFSR & CSL_CGEM_L2MPFSR_LOCAL_MASK) {
		puts("  local access violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_UX_MASK) {
		puts("  User Execute violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_UW_MASK) {
		puts("  User Write violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_UR_MASK) {
		puts("  User Read violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_SX_MASK) {
		puts("  Supervisor Execute violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_SW_MASK) {
		puts("  Supervisor Write violation");
	}
	if (MPFSR & CSL_CGEM_L2MPFSR_SR_MASK) {
		puts("  Supervisor Read violation");
	}
	
}

void EXC_external(void)
{
	unsigned int INTXSTAT, flag[4];

	flag[0] = CGEM_regs->MEXPFLAG[0];
	flag[1] = CGEM_regs->MEXPFLAG[1];
	flag[2] = CGEM_regs->MEXPFLAG[2];
	flag[3] = CGEM_regs->MEXPFLAG[3];
	printf("External exception happened with event flag 0x%x 0x%x 0x%x 0x%x",
		flag[3], flag[2], flag[1], flag[0]);

	/*10 MSMC_mpf_error_n Memory protection fault indicators for local CorePac*/
	if(flag[0]&(1<<10))
	{
		memory_protection_exception(XMC_regs->XMPFAR, XMC_regs->XMPFSR);
	}

	/*96 INTERR Dropped CPU interrupt event   */
	if(flag[3]&1)
	{
		INTXSTAT= CGEM_regs->INTXSTAT;
		if(INTXSTAT&CSL_CGEM_INTXSTAT_DROP_MASK)
			printf("DSPINT %d dropped, System Event number = %d\n",
				(INTXSTAT&CSL_CGEM_INTXSTAT_CPUINT_MASK)>>CSL_CGEM_INTXSTAT_CPUINT_SHIFT,
				(INTXSTAT&CSL_CGEM_INTXSTAT_SYSINT_MASK)>>CSL_CGEM_INTXSTAT_SYSINT_SHIFT);
	}

	/*119 SYS_CMPA CPU Memory Protection Fault*/
	if(flag[3]&(3<<(119-96)))
	{
		puts("local registers (INTC, PowerDown...) are accessed illeagely");
	}

	/*120 PMC_CMPA CPU memory protection fault*/
	/*121 PMC_DMPA DMA memory protection fault*/
	if(flag[3]&(3<<(120-96)))
	{
		memory_protection_exception(CGEM_regs->L1PMPFAR, CGEM_regs->L1PMPFSR);
	}
	/*122 DMC_CMPA CPU memory protection fault*/
	/*123 DMC_DMPA DMA memory protection fault*/
	if(flag[3]&(3<<(122-96)))
	{
		memory_protection_exception(CGEM_regs->L1DMPFAR, CGEM_regs->L1DMPFSR);
	}
	/*124 UMC_CMPA CPU memory protection fault*/
	/*125 UMC_DMPA DMA memory protection fault*/
	if(flag[3]&(3<<(124-96)))
	{
		memory_protection_exception(CGEM_regs->L2MPFAR, CGEM_regs->L2MPFSR);
	}

}

/*internal exception handler*/
void EXC_internal(void)
{
	unsigned int ierr;

	/*record IERR */    
	ierr = IERR;
	
	if (ierr & CSL_CHIP_IERR_IFX_MASK) {
		puts("Instruction fetch exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_FPX_MASK) {
		puts("Fetch packet exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_EPX_MASK) {
		puts("Execute patcket exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_OPX_MASK) {
		puts("Opcode exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_RCX_MASK) {
		puts("Resource conflict exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_RAX_MASK) {
		puts("Resource access exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_PRX_MASK) {
		puts("Privilege exception happened");
	}
	else if (ierr & CSL_CHIP_IERR_LBX_MASK) {
		puts("Loop buffer exception happened");
	}
	else {
		printf("internal exception. IERR= 0x%x\n", ierr);
	}
	
}

void EXC_nmi(void)
{
	puts("Legacy NMI exception happened");
}

/*software generated exception handler*/
void EXC_swgen(void)
{	
	puts("Software generated exception happened.");
}

void interrupt Exeception_ISR(void)
{
	unsigned int efr, nrp, ntsr, tscl, tsch;

	/*record timestamp*/
	tscl = TSCL;
	tsch = TSCH;
	
	nrp = NRP;   /*record NRP */
	ntsr = NTSR; /*record NTSR */
	efr = EFR;   /*record EFR */
	
	/*check all possible exceptions*/
	if (efr & CSL_CHIP_EFR_OXF_MASK) {
		/* S/W generated exception */
		EXC_swgen();		
	}
	else if (efr & CSL_CHIP_EFR_IXF_MASK) {
		/* internal exception */
		EXC_internal();		
	}
	else if (efr & CSL_CHIP_EFR_EXF_MASK) {
		/* external exception */
		EXC_external();		
	}
	else if (efr & CSL_CHIP_EFR_NXF_MASK) {
		/* legacy NMI exception */
		EXC_nmi();		
	}
	else {
		printf("Exception happened. EFR = 0x%x\n", efr);
	}

	printf("NRP=0x%x, NTSR=0x%x, TSCH= 0x%x, TSCL= 0x%x\n", 
		nrp, ntsr, tsch, tscl);

	while(1);		//trap
}


