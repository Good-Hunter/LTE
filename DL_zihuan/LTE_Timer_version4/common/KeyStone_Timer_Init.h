
#ifndef _KEYSTONE_TIMER_INIT_H_
#define _KEYSTONE_TIMER_INIT_H_

#include <ti\csl\tistdtypes.h>

/*period is in DSP core clock/6*/
extern void Timer_Init(int timer_num, Uint32 period);

/*delay is in DSP core clock/6*/
extern void Timer_One_Shot_Init(int timer_num, Uint32 delay);

#endif
