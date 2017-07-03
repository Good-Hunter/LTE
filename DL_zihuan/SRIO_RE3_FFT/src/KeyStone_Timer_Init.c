

#include <stdio.h>
#include <c6x.h> 
#include <csl_tmr.h>
#include "KeyStone_common.h"

/*period is in DSP core clock/6*/
void Timer_Init(int timer_num, unsigned int period)
{
	/*Select 32-bit unchained mode (TIMMODE bits in TGCR)*/
	timerRegs[timer_num]->TGCR= (timerRegs[timer_num]->TGCR&
		(~(CSL_TMR_TGCR_TIMMODE_MASK)))|
		(CSL_TMR_TIMMODE_DUAL_UNCHAINED <<CSL_TMR_TGCR_TIMMODE_SHIFT);

	/*Remove the timer from reset (TIMLORS bits in TGCR)*/
	timerRegs[timer_num]->TGCR= timerRegs[timer_num]->TGCR|
		(CSL_TMR_TGCR_TIMLORS_RESET_OFF<<CSL_TMR_TGCR_TIMLORS_SHIFT);

	timerRegs[timer_num]->PRDLO= period;

	/*make timer stop with emulation*/
	timerRegs[timer_num]->EMUMGT_CLKSPD = (timerRegs[timer_num]->EMUMGT_CLKSPD&
		~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK));

	/*Enable the timer (ENAMODE34 bit field in TCR)*/
	timerRegs[timer_num]->TCR= (timerRegs[timer_num]->TCR&
		(~(CSL_TMR_TCR_ENAMODE_LO_MASK)))|
		(CSL_TMR_ENAMODE_CONT<<CSL_TMR_TCR_ENAMODE_LO_SHIFT);

}


/*delay is in DSP core clock/6*/
void Timer_One_Shot_Init(int timer_num, unsigned int delay)
{
	/*Select 32-bit unchained mode (TIMMODE bits in TGCR)*/
	timerRegs[timer_num]->TGCR= (timerRegs[timer_num]->TGCR&
		(~(CSL_TMR_TGCR_TIMMODE_MASK)))|
		(CSL_TMR_TIMMODE_DUAL_UNCHAINED <<CSL_TMR_TGCR_TIMMODE_SHIFT);

	/*Remove the timer from reset (TIMLORS bits in TGCR)*/
	timerRegs[timer_num]->TGCR= timerRegs[timer_num]->TGCR|
		(CSL_TMR_TGCR_TIMLORS_RESET_OFF<<CSL_TMR_TGCR_TIMLORS_SHIFT);

	timerRegs[timer_num]->PRDLO= delay;

	/*make timer stop with emulation*/
	timerRegs[timer_num]->EMUMGT_CLKSPD = (timerRegs[timer_num]->EMUMGT_CLKSPD&
		~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK));

	/*Enable the timer (ENAMODE34 bit field in TCR)*/
	timerRegs[timer_num]->TCR= (timerRegs[timer_num]->TCR&
		(~(CSL_TMR_TCR_ENAMODE_LO_MASK)))|
		(CSL_TMR_ENAMODE_ENABLE<<CSL_TMR_TCR_ENAMODE_LO_SHIFT);

}

