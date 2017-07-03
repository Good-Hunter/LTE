/*
 * 1��symb0�ǿ����������ֲ����û���
 * 2����������re�������õ�Ƶre��					---Ӧ����һ������
 * 3��1���û�1�����ӣ���prb�������������δ���
 * 4��һ���Ƽ������ţ�							---һ������agc_mode����
 * 5��ʹ��_norm()��ע���쳣����������ʵ��Ϊ0��           ---norm������ܴ���15
 */
#include "pl_comm.h"

//�ֲ���
#define	AGC_TARGET				3

void freq_agc(signed int* e_freq_data_ptr,    //In & Out�����ڷ��ŵ�Ƶ�����ݵ��׵�ַ(ant0)��Ҫ��Ƶ�����ݰ���kaR*14*1200�ķ�ʽ����
              signed int  prb_start,          //Para
              signed int  prb_num,            //Para
              signed int  agc_mode,           //Para, 0---��symbΪ��λ������+��λ��1---ֻ��λ
              signed int* agc_coef)           //In & Out, agc_mode=0ʱΪ�����=1ʱΪ����
{
	signed int i, j, agc_temp = 0;
	signed int* freq_data_ptr;
	signed int  agc_coef_temp;

	agc_coef_temp = *agc_coef;
	//agc���Ӽ��㲿��
	if(0 == agc_mode) //����agc����
	{
		//����agc_temp
		freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + prb_start * N_SC_PER_RB];
		for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			agc_temp += _norm(freq_data_ptr[j]);
		}
		agc_temp /= (prb_num * N_SC_PER_RB);

		//agc_temp��AGC_TARGET��Ƚϣ��ó�agc_coef
		agc_coef_temp = AGC_TARGET - agc_temp;
		*agc_coef = agc_coef_temp;
	}


	//��λ����
	if(0 < agc_coef_temp) 	//agc_coef>0,����
	{
		freq_data_ptr = (signed int*)&e_freq_data_ptr[prb_start * N_SC_PER_RB];
	    for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			freq_data_ptr[j] = _shr2(freq_data_ptr[j], agc_coef_temp);	//�Ż�ʱ�ɿ�����dshr2()
		}

	}
	else			  	//agc_coef<0,����
	{
		freq_data_ptr = (signed int*)&e_freq_data_ptr[prb_start * N_SC_PER_RB];
		for(j = 0; j < prb_num * N_SC_PER_RB; j++)
		{
			freq_data_ptr[j] = _shl2(freq_data_ptr[j], (agc_coef_temp*(-1)));
		}

	}
}
