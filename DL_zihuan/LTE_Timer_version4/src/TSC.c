

#include "TSC.h"

unsigned int cycle_measure_overhead=50;

void calc_cycle_measure_overhead()
{
	unsigned int cycle_cold, cycle_warm;
	cycle_cold= TSCL;
	cycle_cold = TSC_getDelay(cycle_cold);
	cycle_warm= TSCL;
	cycle_warm = TSC_getDelay(cycle_warm);
	cycle_measure_overhead = (cycle_cold + cycle_warm)/2;
}

/* Initialize Time stame counter to measure cycles*/
void TSC_init()
{
	TSCL = 0; 	/* Enable the TSC */
	calc_cycle_measure_overhead();
}

void CycleDelay(int count)
{
	unsigned int  sat;
	if(count < 0)
	return;
	sat = TSCL + (unsigned int)count;
	while(TSCL < sat);
}
