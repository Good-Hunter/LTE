/*
 * Timer_module.c
 *
 *	�ֶ��޸���cslr_tmr.h �ļ��е� CSL_TmrRegs
 *  Created on: 2014-5-28
 *      Author: castMemory
 */

#include <Timer_module.h>
#include "system_init.h"
unsigned long long gDSP_Core_Speed_MHz = 1200;
//#pragma DATA_SECTION(hTmr, ".shareddata")



/* �������ܣ����64bitͨ�ö�ʱ��������
 *
 * ����������tmrNum		��ʾʹ���ĸ���ʱ��
 * 			periodValue	��ʾ��ʱ�����ڣ���λus---->ע�⣺clockģʽʱ�����ڼĴ�����ֵ����
 *
 * �������䣺���ｫ64bitͨ�ö�ʱ�����ó� clock��������ģʽ��������Ҫ�Լ��Ķ�
 * 				��1GHz�ڲ�CPUʱ��Ƶ���£�����ȡֵ���޶���64bit
 * */
#pragma CODE_SECTION (Timer64bit_config, ".text:Timer_section");
Uint32 Timer64bit_config( CSL_InstNum tmrNum, unsigned long long periodValue )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	//CSL_TmrHwSetup			hwSetup = {0};

	CSL_TmrContext 		context;	//���������������contextInfo��������չ����������Ҫͨ��������ֵʱ����NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//���������������ģ����Ϣ��������չ����������Ҫͨ��������ֵʱ����NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//�������
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//��ʱ����λ
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//ʹ�ܷ������
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);
	{
	//���Դ��룬��Timer����ʱ����CPUʱ�ӵķ�Ƶ��
		Uint8 temp;
		temp = ( (hTmr->regs->EMUMGT_CLKSPD)&CSL_TMR_EMUMGT_CLKSPD_CLKDIV_MASK)       \
											>>CSL_TMR_EMUMGT_CLKSPD_CLKDIV_SHIFT;
		printf("In Tmr_init(), CLKSPD = %d\n",temp);
	}

//���������ö�ʱ��ǰ���ö�ʱ����λ���Ҳ�����
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//�������ڼĴ�����ֵ���������ڵ�λus
	period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/6/14+1;
	//period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/6;

	hwSetup.tmrTimerMode = CSL_TMR_TIMMODE_GPT;				//���ù���ģʽ������ѡ 64bitͨ�ö�ʱ��ģʽ
    hwSetup.tmrClksrcLo = CSL_TMR_CLKSRC_INTERNAL;			//ʱ��Դѡȡ
    hwSetup.tmrIpGateLo = CSL_TMR_CLOCK_INP_NOGATE;    		//��ѡȡ�ڲ�ʱ��Դʱ���Ƿ���TINPL�źŵ�gate
	hwSetup.tmrClockPulseLo = CSL_TMR_CP_PULSE;				//clock ���� pulseģʽ
	hwSetup.tmrPulseWidthLo = CSL_TMR_PWID_FOURCLKS;		//pulseģʽʱ���������
	hwSetup.tmrInvInpLo = CSL_TMR_INVINP_UNINVERTED;		//�Ƿ� TINPL�źŷ�ת
	hwSetup.tmrInvOutpLo = CSL_TMR_INVOUTP_UNINVERTED;		//�Ƿ� TOUTL�źŷ�ת

	hwSetup.tmrClksrcHi = CSL_TMR_CLKSRC_INTERNAL;
	hwSetup.tmrIpGateHi = CSL_TMR_CLOCK_INP_NOGATE;
	hwSetup.tmrClockPulseHi = CSL_TMR_CP_PULSE;
	hwSetup.tmrPulseWidthHi = CSL_TMR_PWID_FOURCLKS;
	hwSetup.tmrInvInpHi = CSL_TMR_INVINP_UNINVERTED;
	hwSetup.tmrInvOutpHi = CSL_TMR_INVOUTP_UNINVERTED;

	if( CSL_TMR_CP_CLOCK==(hwSetup.tmrClockPulseLo) )
		period /= 2;			//������clockģʽʱ�����ڽ�Ϊԭ����һ��

	hwSetup.tmrTimerPeriodLo = _loll(period-1);			//����Period�Ĵ�����ע���1
	hwSetup.tmrTimerPeriodHi = _hill(period-1);
	hwSetup.tmrTimerCounterLo = 0;						//����counter�Ĵ���
	hwSetup.tmrTimerCounterHi = 0;
	hwSetup.tmrPreScalarCounterHi = 0;
//	hTmr->regs->RELLO = _loll(period-1);				//����reload�Ĵ���
//	hTmr->regs->RELHI = _hill(period-1);


    status = CSL_tmrHwSetup( hTmr,&hwSetup );
    if( CSL_SOK!=status )
    {
    	puts("CSL_tmrHwSetup() is failed!");
    	return FUNC_FAIL;
    }

#if 0
//����ʱ�����ó���������ģʽ
    CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//�ȼ���CSL_TmrStart64( hTmr,2)

    /* Close opened handle */
    CSL_tmrClose( hTmr );
#endif

    return FUNC_SUCCESS;
}



/* �������ܣ����32bit��ʱ��������
 *
 * ����������tmrNum		��ʾʹ���ĸ���ʱ��
 * 			periodValue	��ʾ��ʱ�����ڣ���λus---->ע�⣺clockģʽʱ�����ڼĴ�����ֵ����
 *
 * �������䣺32bit��ʱ�������ֹ���ģʽ��chained ��   unchained�����ݾ�����Ҫ�Ķ����������⣬��Ҫע���ж�ӳ��
 * 				��1GHz�ڲ�CPUʱ��Ƶ���£�����ȡֵ���޶���32bit
 * 				�������ó�chained����ʱ�ӹ���ģʽ
 * */
#pragma CODE_SECTION (Timer32bit_config, ".text:Timer_section");
Uint32 Timer32bit_config( CSL_InstNum tmrNum, Uint32 periodValue )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	CSL_TmrHwSetup			hwSetup = {0};
	//CSL_TmrHandle 			hTmr;
	CSL_TmrContext 		context;	//���������������contextInfo��������չ����������Ҫͨ��������ֵʱ����NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//���������������ģ����Ϣ��������չ����������Ҫͨ��������ֵʱ����NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//�������
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//��ʱ����λ
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//ʹ�ܷ������
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);

//���������ö�ʱ��ǰ���ö�ʱ����λ���Ҳ�����
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//�������ڼĴ�����ֵ���������ڵ�λus
	period = ( (unsigned long long)periodValue )*gDSP_Core_Speed_MHz/14;

	hwSetup.tmrTimerMode = CSL_TMR_TIMMODE_GPT;				//���ù���ģʽ������ѡ 64bitͨ�ö�ʱ��ģʽ
    hwSetup.tmrClksrcLo = CSL_TMR_CLKSRC_INTERNAL;			//ʱ��Դѡȡ
    hwSetup.tmrIpGateLo = CSL_TMR_CLOCK_INP_NOGATE;    		//��ѡȡ�ڲ�ʱ��Դʱ���Ƿ���TINPL�źŵ�gate
	hwSetup.tmrClockPulseLo = CSL_TMR_CP_CLOCK;				//clock ���� pulseģʽ
	hwSetup.tmrPulseWidthLo = CSL_TMR_PWID_FOURCLKS;		//pulseģʽʱ���������
	hwSetup.tmrInvInpLo = CSL_TMR_INVINP_UNINVERTED;		//�Ƿ� TINPL�źŷ�ת
	hwSetup.tmrInvOutpLo = CSL_TMR_INVOUTP_UNINVERTED;		//�Ƿ� TOUTL�źŷ�ת

	hwSetup.tmrClksrcHi = CSL_TMR_CLKSRC_INTERNAL;
	hwSetup.tmrIpGateHi = CSL_TMR_CLOCK_INP_NOGATE;
	hwSetup.tmrClockPulseHi = CSL_TMR_CP_CLOCK;
	hwSetup.tmrPulseWidthHi = CSL_TMR_PWID_FOURCLKS;
	hwSetup.tmrInvInpHi = CSL_TMR_INVINP_UNINVERTED;
	hwSetup.tmrInvOutpHi = CSL_TMR_INVOUTP_UNINVERTED;

	if( CSL_TMR_CP_CLOCK==(hwSetup.tmrClockPulseLo) )
		period /= 2;			//������clockģʽʱ�����ڽ�Ϊԭ����һ��

	hwSetup.tmrTimerPeriodLo = _loll(period-1);			//����Period�Ĵ�����ע���1
	hwSetup.tmrTimerPeriodHi = 1;						// ��chained����ģʽ�£���32bit�Ķ�ʱ����Ϊ��Ƶ��ʹ�ã��������÷�Ƶ����2
	hwSetup.tmrTimerCounterLo = 0;						//����counter�Ĵ���
	hwSetup.tmrTimerCounterHi = 0;
	hwSetup.tmrPreScalarCounterHi = 0;
//	hTmr->regs->RELLO = _loll(period-1);				//����reload�Ĵ���
//	hTmr->regs->RELHI = _hill(period-1);

    status = CSL_tmrHwSetup( hTmr,&hwSetup );
    if( CSL_SOK!=status )
    {
    	puts("CSL_tmrHwSetup() is failed!");
    	return FUNC_FAIL;
    }

//����ʱ�����ó���������ģʽ
    CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//�ȼ���CSL_TmrStart64( hTmr,2)

    /* Close opened handle */
    CSL_tmrClose( hTmr );

    return FUNC_SUCCESS;
}

/* �˺�����ɶ�ʱ������������	*/
#pragma CODE_SECTION (Timer_config, ".text:Timer_section");
Uint32 Timer_config( CSL_InstNum tmrNum )
{
	unsigned long long		period;
	Uint32					uiLoop;
	CSL_Status 				status;
	CSL_TmrConfig			config  = {0};
	//CSL_TmrHandle 			hTmr;
	CSL_TmrContext 		context;	//���������������contextInfo��������չ����������Ҫͨ��������ֵʱ����NULL
	CSL_TmrObj 			tmrObj;
	CSL_TmrParam 		tmrParam;	//���������������ģ����Ϣ��������չ����������Ҫͨ��������ֵʱ����NULL

	status = CSL_tmrInit(&context);
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrInit() is failed!");
		return FUNC_FAIL;
	}

	hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//�������
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return FUNC_FAIL;
	}

	//��ʱ����λ
	hTmr->regs->TCR = 0;
	hTmr->regs->TGCR = 0;

	//ʹ�ܷ������
//	hTmr->regs->EMUMGT_CLKSPD &= ~(CSL_TMR_EMUMGT_CLKSPD_FREE_MASK|CSL_TMR_EMUMGT_CLKSPD_SOFT_MASK);
	{
	//���Դ��룬��Timer����ʱ����CPUʱ�ӵķ�Ƶ��
		Uint8 temp;
		temp = ( (hTmr->regs->EMUMGT_CLKSPD)&CSL_TMR_EMUMGT_CLKSPD_CLKDIV_MASK)       \
											>>CSL_TMR_EMUMGT_CLKSPD_CLKDIV_SHIFT;
		printf("In Tmr_init(), CLKSPD = %d\n",temp);
	}

//���������ö�ʱ��ǰ���ö�ʱ����λ���Ҳ�����
	CSL_TmrReset64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

	CSL_TmrStop64( hTmr );
	for( uiLoop = 0;uiLoop<1000;++uiLoop )
		asm(" nop 5");

//�������ڼĴ�����ֵ���������ڵ�λus
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
