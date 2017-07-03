/***********************************************/
/*f_block_interleaver                              */
/*功能：完成pdcch交织功能                        */
/***********************************************/
#include "pl_comm.h"


void f_block_interleaver(long long *input_data_ptr1,//2个64BIT为一个REG
                 long long *input_data_ptr2,
                 long long *output_data_ptr1,
                 long long *output_data_ptr2,
                 unsigned int D,
                 unsigned int cell_id)
{
    unsigned char m = 0;
    unsigned char P_table[32] = {1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31, 0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30};
    unsigned short table_pattern[900];
    unsigned short table_pattern1[900];
    unsigned char P_index = 0;
    unsigned int length = 0;
    long long *new_data_ptr;
    unsigned short temp = 0;
    long long temp_buffer[1000];
    unsigned short line_num = 0;
    unsigned short temp_index = 0;
    unsigned int Nd = 0;
    unsigned int index,i,j;
    line_num = (D + 31) / 32;
    length = line_num * 32;
    Nd = length - D;//头部添加冗余数据
    //第一行有NULL，需要去除
    index = 0;
    memset(table_pattern,0xffff,Nd * 2);//头部添加冗余
    j = 0;
    for(i = Nd;i < length;i++)//按列取数
    {
      table_pattern[i] = j++;
    }
    for(i = 0;i < 32;i++)//按列取数
    {
      P_index = P_table[i];
      for(m = 0;m < line_num;m++)//行
      {
        temp = table_pattern[32 * m + P_index];
        if(temp != 0xffff)//去除NULL
          {
            table_pattern1[index++] = temp;
          }
      }
    }
    //循环移位
    for(index = 0;index < D;index++)
    {
        table_pattern[index] = table_pattern1[(index + cell_id) % D];
    }
    for(index = 0;index < D;index++)
    {
      temp_index = table_pattern[index];
      output_data_ptr1[2 * index] = input_data_ptr1[temp_index * 2];
      output_data_ptr1[index * 2 + 1] = input_data_ptr1[temp_index * 2 + 1];
      output_data_ptr2[2 * index] = input_data_ptr2[temp_index * 2];
      output_data_ptr2[index * 2 + 1] = input_data_ptr2[temp_index * 2 + 1];
    }
}


