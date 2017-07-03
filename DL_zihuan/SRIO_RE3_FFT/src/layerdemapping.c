/***********************************************/
/*layerdemapping                                 */
/*���ܣ���ɽ��ӳ�书��                         */
/***********************************************/
#include "pl_comm.h"

void layerdemapping(
                  signed int *restrict modulation_data_0,/*����0���ƺ�ķ��� Q(16,3)*/
                  signed int *restrict modulation_data_1,/*����1���ƺ�ķ��� Q(16,3)*/
                  signed int *restrict layermapping_data,/*��Ԥ�����ķ��� Q(16,3)*/
                  signed int *restrict llr_data_0,/*����0���ƺ��llr Q(16,1)*/
                  signed int *restrict llr_data_1,/*����1���ƺ��llr Q(16,1)*/
                  signed int *restrict llr_data,/*��Ԥ������llr Q(16,1)*/
                  unsigned int m_sybol,/*����0���ƺ�ķ��Ÿ���*/  
                  unsigned int *m_laryer_symbol,/*��ӳ����ÿ����Ÿ���*/
                  unsigned int num_layer,/*����*/
                  unsigned int num_codeword,/*���ָ���*/
                  unsigned int diversity_mode/*���д���ģʽ*/
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
             memcpy(modulation_data_0,layermapping_data,m_sybol * 4);/*����ֻ��һ�����֣�ֱ�ӿ���*/
             memcpy(llr_data_0,llr_data,m_sybol * 2);/*����ֻ��һ�����֣�ֱ�ӿ���*/
             break;
         }
         case NUM_LAYER_2:
         {
             /*�շָ���/����ּ������ָ���Ϊ1*/
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
             /*�շָ��������ָ���Ϊ2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol;/*�������ַ��Ÿ���һ��*/
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
             /*�շָ��������ָ���Ϊ2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol;/*����1���Ÿ���������0��2��*/
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
             /*�շָ��������ָ���Ϊ2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol >> 1;/*�������ַ��Ÿ���һ��*/
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
             /*����ּ������ָ���Ϊ1*/
             if((TRANSMIT_DIVERSITY == diversity_mode) && (NUM_CODEWORD_1 == num_codeword))
             {
                 *m_laryer_symbol = (m_sybol + 2) >> 2;/*��ֹm_sybol����4��������*/
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
