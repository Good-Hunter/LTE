/*2015.5.21�༭
 * ���ڶ�ʱ���ĳ�ʼ������ʱ���жϷ����ӳ���
 * Author:mjt
 */

#include <stdio.h>
#include <string.h>
#include <Timer_module.h>
#include "system_init.h"
CSL_TmrRegsOvly temp_reg;//���Դ���bug��ʹ����ʱ��������Ĵ����ĵ�ַ��
void Timer_start()
{
	hTmr->regs = temp_reg;
	//����ʱ�����ó���������ģʽ
	CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//�ȼ���CSL_TmrStart64( hTmr,2)

	/* Close opened handle */
	//CSL_tmrClose( hTmr );
}

void Timer_init()
{
	Uint32 				Err;
	CSL_InstNum 		tmrNum = 4;
	CSL_TmrObj 			tmrObj;
	CSL_Status 			status;
	CSL_TmrParam 		tmrParam;

	//CPIntc_for_Srio();
	//CPIntc_for_Timer();

	//����Timerģ��ľ��
	/*hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//�������
	if( CSL_SOK!=status )
	{
		puts("CSL_tmrOpen() is failed!");
		return ;
	}*/

	Err = Timer64bit_config( tmrNum,1000 );

	if( FUNC_FAIL==Err )
	{
		puts("Timer64bit_config() is failed!");
		return;
	}
	//Timer_start();
	temp_reg = hTmr->regs;


}





