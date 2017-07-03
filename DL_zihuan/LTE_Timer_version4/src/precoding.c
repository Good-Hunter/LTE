/***********************************************/
/*precoding                                    */
/*功能：完成预编码功能（支持2天线端口）        */
/***********************************************/
#include "pl_comm.h"

void precoding(
               signed int *restrict precoding_data[NUM_ANTENNA_PORT_4],/*预编码后的符号 Q(16,3)*/
               unsigned int *m_ap_symbol,/*预编码后的符号个数*/
               signed int *restrict layermapping_data0,/*层映射后的符号 Q(16,2)*/
               signed int *restrict layermapping_data1,/*层映射后的符号 Q(16,2)*/
               unsigned int m_laryer_symbol,/*层映射后的每层符号个数,是2的倍数吗？*/
               unsigned int num_layer,/*层数*/
               unsigned int num_antenna_port,/*天线端口个数*/
               unsigned int diversity_mode,/*下行传输模式*/
               unsigned int delay_mod /*延时模式*/
              )
{
    signed int *restrict int_ptr1;
    signed int *restrict int_ptr2;
    signed int *restrict int_ptr3;
    signed int *restrict int_ptr4;
    signed int i;
    long long temp1;
    long long temp2;
    double temp3;
    double temp4;
    double temp5;
    double temp6;

    
    switch (num_antenna_port)
    {
         case NUM_ANTENNA_PORT_1:
         {
             memcpy(precoding_data[0],layermapping_data0,m_laryer_symbol * num_layer * 4);/*单天线端口只有一层*/
             break;
         }
         case NUM_ANTENNA_PORT_2:
         {
             /*空分复用且大延时模式*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (LARGE_DELAY_CDD == delay_mod))
             {
                 /*此种情况codeindex = 0 W = 1/sqtr(2)[1,0;0,1]
                 i为偶数时，D(i) * U = 1/sqtr(2)[1,1;1,-1] 则W * D(i) * U = 1/2[1,1;1,-1]
                 i为奇数时，D(i) * U = 1/sqtr(2)[1,1;-1,1] 则W * D(i) * U = 1/2[1,1;-1,1]*/
                 if(NUM_LAYER_2 == num_layer)
                 {
                     *m_ap_symbol = m_laryer_symbol;
                     int_ptr1 = precoding_data[0];
                     int_ptr2 = precoding_data[1];
                     int_ptr3 = layermapping_data0;
                     int_ptr4 = layermapping_data1;
                     for(i = 0;i < (m_laryer_symbol >> 1);i++)
                     {
                         temp1 = _addsub2(_shr2(int_ptr3[2 * i],1),_shr2(int_ptr4[2 * i],1));/*先移位，防止溢出*/
                         temp2 = _addsub2(_shr2(int_ptr4[2 * i + 1],1),_shr2(int_ptr3[2 * i + 1],1));
                         int_ptr1[2 * i] = _hill(temp1);
                         int_ptr2[2 * i] = _loll(temp1);
                         int_ptr1[2 * i + 1] = _hill(temp2);
                         int_ptr2[2 * i + 1] = _loll(temp2);
                     }
                 }
             }
             /*发射分集*/
             if(TRANSMIT_DIVERSITY == diversity_mode)
             {
                 if(NUM_LAYER_2 == num_layer)
                 {
                     *m_ap_symbol = 2 * m_laryer_symbol;
                     int_ptr1 = precoding_data[0];
                     int_ptr2 = precoding_data[1];
                     int_ptr3 = layermapping_data0;
                     int_ptr4 = layermapping_data1;
                     for(i = 0;i < m_laryer_symbol;i++)
                     {
                         temp3 = _smpy2(INV_SQRT_2_Q15_PP,int_ptr3[i]);
                         temp4 = _smpy2(INV_SQRT_2_Q15_NP,int_ptr4[i]);
                         temp5 = _smpy2(INV_SQRT_2_Q15_PP,int_ptr4[i]);
                         temp6 = _smpy2(INV_SQRT_2_Q15_PN,int_ptr3[i]);
                         //2015.1.26修改，指针替代数组
                         /**int_ptr1++ = _packh2(_hi(temp3),_lo(temp3));
                         *int_ptr2++ = _packh2(_hi(temp4),_lo(temp4));
                         *int_ptr1++ = _packh2(_hi(temp5),_lo(temp5));
                         *int_ptr2++ = _packh2(_hi(temp6),_lo(temp6));*/
                         int_ptr1[2 * i] = _packh2(_hi(temp3),_lo(temp3));
                         int_ptr2[2 * i] = _packh2(_hi(temp4),_lo(temp4));
                         int_ptr1[2 * i + 1] = _packh2(_hi(temp5),_lo(temp5));
                         int_ptr2[2 * i + 1] = _packh2(_hi(temp6),_lo(temp6));
                     }
                 }
                 
             }
             break;
         }
         default:
             break;
             
    }           
}
