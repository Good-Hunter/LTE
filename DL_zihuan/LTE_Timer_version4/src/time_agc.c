/***********************************************/
/*time_agc                                     */
/*功能：时域                                   */
/***********************************************/
#include <pl_comm.h>

#define IFFT_2048_SQRT  0x5A825A82  /*sqrt(2048)/32= 1.414 Q(16,2)*/
void time_agc_compensate(signed int* restrict e_time_data_in_ptr[NUM_MAX_ANTENNA],  //In & Out
                         signed int* restrict e_time_data_out_ptr[NUM_MAX_ANTENNA], 
                         signed int  recv_ant_num,       //Para
                         signed int num_antenna_port,
                         signed int length,
                         signed int start_symbol,
                         signed int end_symbol,
                         signed short* restrict agc_coef)
{
  signed int i, j;
  signed int  agc_coef_temp1;
  signed int  agc_coef_temp2;
  double temp1,temp2;
  signed int temp3,temp4;
  

    if((recv_ant_num == NUM_MAX_ANTENNA) && (num_antenna_port == NUM_ANTENNA_PORT_2))
    {
        for(i = 0; i < (end_symbol - start_symbol); i++)
        {
            //移位部分
            if((0 < agc_coef[i]) && (0 < agc_coef[6 + i]))   //agc_coef>0,左移
            {
                agc_coef_temp1 = agc_coef[i];
                agc_coef_temp2 = agc_coef[6 + i];
                for(j = 0; j < length; j++)
                {
                    temp1 = _mpy2(e_time_data_in_ptr[0][i * (length + 4) + j],IFFT_2048_SQRT);/*偏移agc因子*/
                    temp2 = _mpy2(e_time_data_in_ptr[1][i * (length + 4) + j],IFFT_2048_SQRT);
                    temp3 = _packh2(_hi(temp1),_lo(temp1));
                    temp4 = _packh2(_hi(temp2),_lo(temp2));
                    e_time_data_out_ptr[0][i * length + j] = _shl2(temp3, agc_coef_temp1);  //优化时可考虑用dshr2()
                    e_time_data_out_ptr[1][i * length + j] = _shl2(temp4, agc_coef_temp2);  //优化时可考虑用dshr2()
                }
              
            }
            else if((0 >= agc_coef[i]) && (0 < agc_coef[6 + i]))         //agc_coef<0,右移
            {
                agc_coef_temp1 = abs(agc_coef[i]);
                agc_coef_temp2 = agc_coef[6 + i];
                for(j = 0; j < length; j++)
                {
                    temp1 = _mpy2(e_time_data_in_ptr[0][i * (length + 4) + j],IFFT_2048_SQRT);/*偏移agc因子*/
                    temp2 = _mpy2(e_time_data_in_ptr[1][i * (length + 4) + j],IFFT_2048_SQRT);
                    temp3 = _packh2(_hi(temp1),_lo(temp1));
                    temp4 = _packh2(_hi(temp2),_lo(temp2));
                    e_time_data_out_ptr[0][i * length + j] = _shr2(temp3, agc_coef_temp1);  //优化时可考虑用dshr2()
                    e_time_data_out_ptr[1][i * length + j] = _shl2(temp4, agc_coef_temp2);  //优化时可考虑用dshr2()
                }
            }
            else if((0 >= agc_coef[i]) && (0 >= agc_coef[6 + i]))         //agc_coef<0,右移
            {
                agc_coef_temp1 = abs(agc_coef[i]);
                agc_coef_temp2 = abs(agc_coef[6 + i]);
                for(j = 0; j < length; j++)
                {
                    temp1 = _mpy2(e_time_data_in_ptr[0][i * (length + 4) + j],IFFT_2048_SQRT);/*偏移agc因子*/
                    temp2 = _mpy2(e_time_data_in_ptr[1][i * (length + 4) + j],IFFT_2048_SQRT);
                    temp3 = _packh2(_hi(temp1),_lo(temp1));
                    temp4 = _packh2(_hi(temp2),_lo(temp2));
                    e_time_data_out_ptr[0][i * length + j] = _shr2(temp3, agc_coef_temp1);  //优化时可考虑用dshr2()
                    e_time_data_out_ptr[1][i * length + j] = _shr2(temp4, agc_coef_temp2);  //优化时可考虑用dshr2()
                }
            }
            else         //agc_coef<0,右移
            {
                agc_coef_temp1 = agc_coef[i];
                agc_coef_temp2 = abs(agc_coef[6 + i]);
                for(j = 0; j < length; j++)
                {
                    temp1 = _mpy2(e_time_data_in_ptr[0][i * (length + 4) + j],IFFT_2048_SQRT);/*偏移agc因子*/
                    temp2 = _mpy2(e_time_data_in_ptr[1][i * (length + 4) + j],IFFT_2048_SQRT);
                    temp3 = _packh2(_hi(temp1),_lo(temp1));
                    temp4 = _packh2(_hi(temp2),_lo(temp2));
                    e_time_data_out_ptr[0][i * length + j] = _shl2(temp3, agc_coef_temp1);  //优化时可考虑用dshr2()
                    e_time_data_out_ptr[1][i * length + j] = _shr2(temp4, agc_coef_temp2);  //优化时可考虑用dshr2()
                }
            }
        }
        
    }
  
}
