/***********************************************/
/*layerdemapping                                 */
/*功能：完成解层映射功能                         */
/***********************************************/
#include "pl_comm.h"

void layerdemapping(
                  signed int *restrict modulation_data_0,/*码字0调制后的符号 Q(16,3)*/
                  signed int *restrict modulation_data_1,/*码字1调制后的符号 Q(16,3)*/
                  signed int *restrict layermapping_data,/*解预编码后的符号 Q(16,3)*/
                  signed int *restrict llr_data_0,/*码字0调制后的llr Q(16,1)*/
                  signed int *restrict llr_data_1,/*码字1调制后的llr Q(16,1)*/
                  signed int *restrict llr_data,/*解预编码后的llr Q(16,1)*/
                  unsigned int m_sybol,/*码字0调制后的符号个数*/  
                  unsigned int *m_laryer_symbol,/*层映射后的每层符号个数*/
                  unsigned int num_layer,/*层数*/
                  unsigned int num_codeword,/*码字个数*/
                  unsigned int diversity_mode/*下行传输模式*/
                  )
{
    signed int *restrict int_ptr1;
    signed int *restrict int_ptr2;
    signed int *restrict int_ptr3;
    signed int *restrict int_ptr4;
    signed short *restrict short_ptr1;
    signed short *restrict short_ptr2;
    signed short *restrict short_ptr3;
    signed short *restrict short_ptr4;
    signed int i;

    
    switch (num_layer)
    {
         case NUM_LAYER_1:
         {
             memcpy(modulation_data_0,layermapping_data,m_sybol * 4);/*单层只有一个码字，直接拷贝*/
             memcpy(llr_data_0,llr_data,m_sybol * 2);/*单层只有一个码字，直接拷贝*/
             break;
         }
         case NUM_LAYER_2:
         {
             /*空分复用/发射分集且码字个数为1*/
             if(((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_1 == num_codeword))
               || ((TRANSMIT_DIVERSITY == diversity_mode) && (NUM_CODEWORD_1 == num_codeword)))
             {
                 *m_laryer_symbol = m_sybol >> 1;
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 short_ptr1 = (signed short*)llr_data;
                 short_ptr2 = (signed short*)llr_data + (*m_laryer_symbol);
                 for(i = 0;i < (m_sybol >> 1);i++)
                 {
                     modulation_data_0[2 * i] = int_ptr1[i];
                     modulation_data_0[2 * i + 1] = int_ptr2[i];
                     llr_data_0[2 * i] = short_ptr1[i];
                     llr_data_0[2 * i + 1] = short_ptr2[i];
                 }
                 
             }
             /*空分复用且码字个数为2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol;/*两个码字符号个数一样*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 int_ptr3 = llr_data;
                 int_ptr4 = int_ptr3 + ((*m_laryer_symbol) >> 1);
                 #pragma MUST_ITERATE(, , 4);
                 for(i = 0;i < m_sybol;i++)
                 {
                	 modulation_data_0[i] = int_ptr1[i];
                	 modulation_data_1[i] = int_ptr2[i];
                 }
                 for(i = 0;i < (m_sybol >> 1);i++)
                 {
                	 llr_data_0[i] = int_ptr3[i];
                	 llr_data_1[i] = int_ptr4[i];
                 }


                 
             }
             break;
         }
         case NUM_LAYER_3:
         {
             /*空分复用且码字个数为2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol;/*码字1符号个数是码字0的2倍*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 int_ptr3 = layermapping_data + 2 * (*m_laryer_symbol);
                 short_ptr1 = (signed short*)llr_data;
                 short_ptr2 = (signed short*)llr_data + (*m_laryer_symbol);
                 short_ptr3 = (signed short*)llr_data + 2 * (*m_laryer_symbol);
                 for(i = 0;i < m_sybol;i++)
                 {
                     modulation_data_0[i] = int_ptr1[i];
                     modulation_data_1[2 * i] = int_ptr2[i];
                     modulation_data_1[2 * i + 1] = int_ptr3[i];
                     llr_data_0[i] = short_ptr1[i];
                     llr_data_1[2 * i] = short_ptr2[i];
                     llr_data_1[2 * i + 1] = short_ptr3[i];
                 }
                 
             }
             break;
         }
         case NUM_LAYER_4:
         {
             /*空分复用且码字个数为2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol >> 1;/*两个码字符号个数一样*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 int_ptr3 = layermapping_data + 2 * (*m_laryer_symbol);
                 int_ptr4 = layermapping_data + 3 * (*m_laryer_symbol);
                 short_ptr1 = (signed short*)llr_data;
                 short_ptr2 = (signed short*)llr_data + (*m_laryer_symbol);
                 short_ptr3 = (signed short*)llr_data + 2 * (*m_laryer_symbol);
                 short_ptr4 = (signed short*)llr_data + 3 * (*m_laryer_symbol);
                 for(i = 0;i < m_sybol;i++)
                 {
                     modulation_data_0[2 * i] = int_ptr1[i];
                     modulation_data_0[2 * i + 1] = int_ptr2[i];
                     modulation_data_1[2 * i] = int_ptr3[i];
                     modulation_data_1[2 * i + 1] = int_ptr4[i];
                     llr_data_0[2 * i] = short_ptr1[i];
                     llr_data_0[2 * i + 1] = short_ptr2[i];
                     llr_data_1[2 * i] = short_ptr3[i];
                     llr_data_1[2 * i + 1] = short_ptr4[i];

                 }
                 
             }
             /*发射分集且码字个数为1*/
             if((TRANSMIT_DIVERSITY == diversity_mode) && (NUM_CODEWORD_1 == num_codeword))
             {
                 *m_laryer_symbol = (m_sybol + 2) >> 2;/*防止m_sybol不是4的整数倍*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 int_ptr3 = layermapping_data + 2 * (*m_laryer_symbol);
                 int_ptr4 = layermapping_data + 3 * (*m_laryer_symbol);
                 short_ptr1 = (signed short*)llr_data;
                 short_ptr2 = (signed short*)llr_data + (*m_laryer_symbol);
                 short_ptr3 = (signed short*)llr_data + 2 * (*m_laryer_symbol);
                 short_ptr4 = (signed short*)llr_data + 3 * (*m_laryer_symbol);
                 for(i = 0;i < m_sybol;i++)
                 {
                     modulation_data_0[4 * i] = int_ptr1[i];
                     modulation_data_0[4 * i + 1] = int_ptr2[i];
                     modulation_data_0[4 * i + 2] = int_ptr3[i];
                     modulation_data_0[4 * i + 3] = int_ptr4[i];
                     llr_data_0[4 * i] = short_ptr1[i];
                     llr_data_0[4 * i + 1] = short_ptr2[i];
                     llr_data_0[4 * i + 2] = short_ptr3[i];
                     llr_data_0[4 * i + 3] = short_ptr4[i];
                 }
                 
             }
             break;
         }
         default:
             break;
             
    }           
}
