/***********************************************/
/*f_qpsk_modulation                              */
/*���ܣ����qpsk���ƹ���                        */
/***********************************************/
#include "pl_comm.h"



void f_qpsk_modulation(unsigned  char *restrict input_data_ptr,//����BIT����
		               unsigned  int *restrict output_data_ptr,//���ƺ�
                       unsigned  int  length)//bit��
{
    long long *p_out_64;
    long long *lut_64;
    p_out_64 = (long long *)output_data_ptr;
    unsigned  int  symbol_num = 0;
    unsigned  int  i = 0;
    unsigned  char *p_in;
    unsigned  char temp;
    unsigned  char index3,index4;
    p_in = input_data_ptr;
    if((length & 0x1) != 0)//��鳤��
    {
        //ALARM
    }
    else
    {
      symbol_num = length >> 3;
    }
    lut_64 = (long long *)g_const_modu_table_QPSK;//QPSKӳ���
    for(i = 0;i < symbol_num;i++)
    {
        temp = *p_in++;
        index3 = (temp >> 4) & 0xf;
        *p_out_64++ = lut_64[index3];
        index4 = (temp) & 0xf;
        *p_out_64++ = lut_64[index4];
    }
}
