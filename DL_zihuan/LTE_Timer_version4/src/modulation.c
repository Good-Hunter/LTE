/***********************************************/
/*f_qpsk_modulation                              */
/*功能：完成qpsk调制功能                        */
/***********************************************/
#include "pl_comm.h"



void f_qpsk_modulation(unsigned  char *restrict input_data_ptr,//输入BIT数据
		               unsigned  int *restrict output_data_ptr,//调制后
                       unsigned  int  length)//bit数
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
    if((length & 0x1) != 0)//检查长度
    {
        //ALARM
    }
    else
    {
      symbol_num = length >> 3;
    }
    lut_64 = (long long *)g_const_modu_table_QPSK;//QPSK映射表
    for(i = 0;i < symbol_num;i++)
    {
        temp = *p_in++;
        index3 = (temp >> 4) & 0xf;
        *p_out_64++ = lut_64[index3];
        index4 = (temp) & 0xf;
        *p_out_64++ = lut_64[index4];
    }
}
