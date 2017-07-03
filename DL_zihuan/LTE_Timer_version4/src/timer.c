/*2015.5.21编辑
 * 用于定时器的初始化，定时器中断服务子程序
 * Author:mjt
 */

#include <stdio.h>
#include <string.h>
#include <Timer_module.h>
#include "system_init.h"
CSL_TmrRegsOvly temp_reg;//测试存在bug，使用临时变量保存寄存器的地址。
void Timer_start()
{
	hTmr->regs = temp_reg;
	//将定时器配置成连续工作模式
	CSL_TmrStart64( hTmr,CSL_TMR_ENAMODE_CONT);	//等价于CSL_TmrStart64( hTmr,2)

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

	//开启Timer模块的句柄
	/*hTmr = CSL_tmrOpen(&tmrObj,tmrNum,&tmrParam,&status);//开启句柄
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





