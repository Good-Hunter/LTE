/*
 * Timer_module.c
 *
 *	手动修改了cslr_tmr.h 文件中的 CSL_TmrRegs
 *  Created on: 2014-5-28
 *      Author: castMemory
 */

#include <Timer_module.h>
#include "system_init.h"
unsigned long long gDSP_Core_Speed_MHz = 1200;
//#pragma DATA_SECTION(hTmr, ".shareddata")



/* 函数功能：完成64bit通用定时器的配置
 *
 * 函数参数：tmrNum		表示使用哪个定时器
 * 			periodValue	表示定时器周期，单位us---->注意：clock模式时让周期寄存器的值减半
 *
 * 函数补充：这里将64bit通用定时器配置成 clock连续工作模式，根据需要自己改动
 * 				在1GHz内部CPU时钟频率下，周期取值的限定在64bit
 * */
#pragma CODE_SECTION (Timer64bit_config, ".text:Timer_section");
Uint32 Timer64bit_config( CSL_InstNum tmrNum, unsigned long long periodValue )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	//CSL_TmrHwSetup			hwSetup = {0};

	CSL_TmrContext 		context;	//这个变量用来保存contextInfo，属于扩展变量；不需要通过它传递值时，赋NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//这个变量用来传递模块信息，属于扩展变量；不需要通过它传递值时，赋NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//开启句柄
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//定时器软复位
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//使能仿真控制
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);
	{
	//测试代码，看Timer工作时钟与CPU时钟的分频比
		Uint8 temp;
		temp = ( (hTmr->regs->EMUMGT_CLKSPD)&CSL_TMR_EMUMGT_CLKSPD_CLKDIV_MASK)       \
											>>CSL_TMR_EMUMGT_CLKSPD_CLKDIV_SHIFT;
		printf("In Tmr_init(), CLKSPD = %d\n",temp);
	}

//在重新配置定时器前，让定时器复位，且不工作
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//计算周期寄存器的值，给的周期单位us
	period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/6/14+1;
	//period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/6;

	hwSetup.tmrTimerMode = CSL_TMR_TIMMODE_GPT;				//配置工作模式，这里选 64bit通用定时器模式
    hwSetup.tmrClksrcLo = CSL_TMR_CLKSRC_INTERNAL;			//时钟源选取
    hwSetup.tmrIpGateLo = CSL_TMR_CLOCK_INP_NOGATE;    		//当选取内部时钟源时，是否受TINPL信号的gate
	hwSetup.tmrClockPulseLo = CSL_TMR_CP_PULSE;				//clock 或者 pulse模式
	hwSetup.tmrPulseWidthLo = CSL_TMR_PWID_FOURCLKS;		//pulse模式时，脉宽多少
	hwSetup.tmrInvInpLo = CSL_TMR_INVINP_UNINVERTED;		//是否将 TINPL信号反转
	hwSetup.tmrInvOutpLo = CSL_TMR_INVOUTP_UNINVERTED;		//是否将 TOUTL信号反转

	hwSetup.tmrClksrcHi = CSL_TMR_CLKSRC_INTERNAL;
	hwSetup.tmrIpGateHi = CSL_TMR_CLOCK_INP_NOGATE;
	hwSetup.tmrClockPulseHi = CSL_TMR_CP_PULSE;
	hwSetup.tmrPulseWidthHi = CSL_TMR_PWID_FOURCLKS;
	hwSetup.tmrInvInpHi = CSL_TMR_INVINP_UNINVERTED;
	hwSetup.tmrInvOutpHi = CSL_TMR_INVOUTP_UNINVERTED;

	if( CSL_TMR_CP_CLOCK==(hwSetup.tmrClockPulseLo) )
		period /= 2;			//当采用clock模式时，周期降为原来的一半

	hwSetup.tmrTimerPeriodLo = _loll(period-1);			//配置Period寄存器，注意减1
	hwSetup.tmrTimerPeriodHi = _hill(period-1);
	hwSetup.tmrTimerCounterLo = 0;						//配置counter寄存器
	hwSetup.tmrTimerCounterHi = 0;
	hwSetup.tmrPreScalarCounterHi = 0;
//	hTmr->regs->RELLO = _loll(period-1);				//配置reload寄存器
//	hTmr->regs->RELHI = _hill(period-1);


    status = CSL_tmrHwSetup( hTmr,&hwSetup );
    if( CSL_SOK!=status )
    {
    	puts("CSL_tmrHwSetup() is failed!");
    	return FUNC_FAIL;
    }

#if 0
//将定时器配置成连续工作模式
    CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//等价于CSL_TmrStart64( hTmr,2)

    /* Close opened handle */
    CSL_tmrClose( hTmr );
#endif

    return FUNC_SUCCESS;
}



/* 函数功能：完成32bit定时器的配置
 *
 * 函数参数：tmrNum		表示使用哪个定时器
 * 			periodValue	表示定时器周期，单位us---->注意：clock模式时让周期寄存器的值减半
 *
 * 函数补充：32bit定时器有两种工作模式：chained 与   unchained；根据具体需要改动，除配置外，还要注意中断映射
 * 				在1GHz内部CPU时钟频率下，周期取值的限定在32bit
 * 				这里配置成chained连续时钟工作模式
 * */
#pragma CODE_SECTION (Timer32bit_config, ".text:Timer_section");
Uint32 Timer32bit_config( CSL_InstNum tmrNum, Uint32 periodValue )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	CSL_TmrHwSetup			hwSetup = {0};
	//CSL_TmrHandle 			hTmr;
	CSL_TmrContext 		context;	//这个变量用来保存contextInfo，属于扩展变量；不需要通过它传递值时，赋NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//这个变量用来传递模块信息，属于扩展变量；不需要通过它传递值时，赋NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//开启句柄
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//定时器软复位
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//使能仿真控制
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);

//在重新配置定时器前，让定时器复位，且不工作
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//计算周期寄存器的值，给的周期单位us
	period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/14;

	hwSetup.tmrTimerMode = CSL_TMR_TIMMODE_GPT;				//配置工作模式，这里选 64bit通用定时器模式
    hwSetup.tmrClksrcLo = CSL_TMR_CLKSRC_INTERNAL;			//时钟源选取
    hwSetup.tmrIpGateLo = CSL_TMR_CLOCK_INP_NOGATE;    		//当选取内部时钟源时，是否受TINPL信号的gate
	hwSetup.tmrClockPulseLo = CSL_TMR_CP_CLOCK;				//clock 或者 pulse模式
	hwSetup.tmrPulseWidthLo = CSL_TMR_PWID_FOURCLKS;		//pulse模式时，脉宽多少
	hwSetup.tmrInvInpLo = CSL_TMR_INVINP_UNINVERTED;		//是否将 TINPL信号反转
	hwSetup.tmrInvOutpLo = CSL_TMR_INVOUTP_UNINVERTED;		//是否将 TOUTL信号反转

	hwSetup.tmrClksrcHi = CSL_TMR_CLKSRC_INTERNAL;
	hwSetup.tmrIpGateHi = CSL_TMR_CLOCK_INP_NOGATE;
	hwSetup.tmrClockPulseHi = CSL_TMR_CP_CLOCK;
	hwSetup.tmrPulseWidthHi = CSL_TMR_PWID_FOURCLKS;
	hwSetup.tmrInvInpHi = CSL_TMR_INVINP_UNINVERTED;
	hwSetup.tmrInvOutpHi = CSL_TMR_INVOUTP_UNINVERTED;

	if( CSL_TMR_CP_CLOCK==(hwSetup.tmrClockPulseLo) )
		period /= 2;			//当采用clock模式时，周期降为原来的一半

	hwSetup.tmrTimerPeriodLo = _loll(period-1);			//配置Period寄存器，注意减1
	hwSetup.tmrTimerPeriodHi = 1;						// 在chained工作模式下，高32bit的定时器作为分频器使用，这里设置分频比是2
	hwSetup.tmrTimerCounterLo = 0;						//配置counter寄存器
	hwSetup.tmrTimerCounterHi = 0;
	hwSetup.tmrPreScalarCounterHi = 0;
//	hTmr->regs->RELLO = _loll(period-1);				//配置reload寄存器
//	hTmr->regs->RELHI = _hill(period-1);

    status = CSL_tmrHwSetup( hTmr,&hwSetup );
    if( CSL_SOK!=status )
    {
    	puts("CSL_tmrHwSetup() is failed!");
    	return FUNC_FAIL;
    }

//将定时器配置成连续工作模式
    CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//等价于CSL_TmrStart64( hTmr,2)

    /* Close opened handle */
    CSL_tmrClose( hTmr );

    return FUNC_SUCCESS;
}

/* 此函数完成定时器的所有配置	*/
#pragma CODE_SECTION (Timer_config, ".text:Timer_section");
Uint32 Timer_config( CSL_InstNum tmrNum )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	CSL_TmrConfig			config  = {0};
	//CSL_TmrHandle 			hTmr;
	CSL_TmrContext 		context;	//这个变量用来保存contextInfo，属于扩展变量；不需要通过它传递值时，赋NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//这个变量用来传递模块信息，属于扩展变量；不需要通过它传递值时，赋NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//开启句柄
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//定时器软复位
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//使能仿真控制
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);
	{
	//测试代码，看Timer工作时钟与CPU时钟的分频比
		Uint8 temp;
		temp = ( (hTmr->regs->EMUMGT_CLKSPD)&CSL_TMR_EMUMGT_CLKSPD_CLKDIV_MASK)       \
											>>CSL_TMR_EMUMGT_CLKSPD_CLKDIV_SHIFT;
		printf("In Tmr_init(), CLKSPD = %d\n",temp);
	}

//在重新配置定时器前，让定时器复位，且不工作
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//计算周期寄存器的值，给的周期单位us
	period = ( (unsigned long long)100 )*gDSP_Core_Speed_MHz/6000;

	config.CNTHI = 0;
	config.CNTLO = 0;
	config.PRDHI = _loll(period-1);
	config.PRDLO = _hill(period-1);
	config.TCR   = 0x48;
	config.TGCR  = 0;

    status = CSL_tmrHwSetupRaw( hTmr,&config );
    if( CSL_SOK!=status )
    {
    	puts("CSL_tmrHwSetup() is failed!");
    	return FUNC_FAIL;
    }

    /* Close opened handle */
    CSL_tmrClose( hTmr );

    return FUNC_SUCCESS;
}
