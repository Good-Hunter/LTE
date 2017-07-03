

#ifndef _TSC_H
#define _TSC_H

#include <c6x.h>

#define 	CPU_CLK_KHZ 		(1000000)

extern unsigned int cycle_measure_overhead;

/*get the delay between current and prvious TSC
The delay must be less than 32 bit
enable the TSC with "TSCL= 0;" before use this macro*/
#define TSC_getDelay(preTSC) 	((unsigned int)((0xFFFFFFFFl+TSCL)- (unsigned long long)preTSC)+ 1)
#define TSC_delay_cycle(preTSC) 	(TSC_getDelay(preTSC)- cycle_measure_overhead)

/* Initialize Time stame counter to measure cycles*/
extern void TSC_init();
extern void CycleDelay(int count);
#endif

