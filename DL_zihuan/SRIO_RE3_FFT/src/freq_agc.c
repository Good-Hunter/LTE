/*
 * 1、symb0是控制区，区分不了用户？
 * 2、是用数据re，还是用导频re？					---应该是一起做；
 * 3、1个用户1个因子，对prb不连续分配的如何处理？
 * 4、一次移几个符号？							---一个，以agc_mode区分
 * 5、使用_norm()，注意异常保护（比如实部为0）           ---norm结果不能大于15
 */
#include "pl_comm.h"

//局部宏
#define	AGC_TARGET				3

void freq_agc(signed int* e_freq_data_ptr,    //In & Out，所在符号的频域数据的首地址(ant0)，要求频域数据按照kaR*14*1200的方式排列
              signed int  prb_start,          //Para
              signed int  prb_num,            //Para
              signed int  agc_mode,           //Para, 0---以symb为单位，计算+移位，1---只移位
              signed int* agc_coef)           //In & Out, agc_mode=0时为输出；=1时为输入
{
	signed int i, j, agc_temp = 0;
	signed int* freq_data_ptr;
	signed int  agc_coef_temp;

	agc_coef_temp = *agc_coef;
	//agc因子计算部分
	if(0 == agc_mode) //计算agc因子
	{
		//计算agc_temp
		freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + prb_start * N_SC_PER_RB];
		for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			agc_temp += _norm(freq_data_ptr[j]);
		}
		agc_temp /= (prb_num * N_SC_PER_RB);

		//agc_temp与AGC_TARGET相比较，得出agc_coef
		agc_coef_temp = AGC_TARGET - agc_temp;
		*agc_coef = agc_coef_temp;
	}


	//移位部分
	if(0 < agc_coef_temp) 	//agc_coef>0,右移
	{
		freq_data_ptr = (signed int*)&e_freq_data_ptr[prb_start * N_SC_PER_RB];
	    for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			freq_data_ptr[j] = _shr2(freq_data_ptr[j], agc_coef_temp);	//优化时可考虑用dshr2()
		}

	}
	else			  	//agc_coef<0,左移
	{
		freq_data_ptr = (signed int*)&e_freq_data_ptr[prb_start * N_SC_PER_RB];
		for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			freq_data_ptr[j] = _shl2(freq_data_ptr[j], (agc_coef_temp*(-1)));
		}

	}
}
