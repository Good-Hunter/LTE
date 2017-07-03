/***********************************************/
/*layermapping                                 */
/*���ܣ���ɲ�ӳ�书��                         */
/***********************************************/
#include "pl_comm.h"

void layermapping(
                  signed int *restrict modulation_data_0,/*����0���ƺ�ķ��� Q(16,3)*/
                  signed int *restrict modulation_data_1,/*����1���ƺ�ķ��� Q(16,3)*/
                  signed int *restrict layermapping_data,/*��ӳ���ķ��� Q(16,3)*/
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
    signed int i;

    
    switch (num_layer)
    {
         case NUM_LAYER_1:
         {
             memcpy(layermapping_data,modulation_data_0,m_sybol * 4);/*����ֻ��һ�����֣�ֱ�ӿ���*/
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
                 for(i = 0;i < (m_sybol >> 1);i++)
                 {
                	 //2015.1.26ָ���������
                	 int_ptr1[i] = modulation_data_0[2 * i];
                     int_ptr2[i] = modulation_data_0[2 * i + 1];
                	 /*int_ptr1++ = *modulation_data_0++;
                	 *int_ptr2++ = *modulation_data_0++;*/
                 }
                 
             }
             /*�շָ��������ָ���Ϊ2*/
             if((SPATIAL_MULTIPLEXING == diversity_mode) && (NUM_CODEWORD_2 == num_codeword))
             {
                 *m_laryer_symbol = m_sybol;/*�������ַ��Ÿ���һ��*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 memcpy(int_ptr1,modulation_data_0,m_sybol * 4);/*�ֱ𿽱�*/
                 memcpy(int_ptr2,modulation_data_1,m_sybol * 4);/*�ֱ𿽱�*/
                 
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
                 for(i = 0;i < m_sybol;i++)
                 {
                     int_ptr1[i] = modulation_data_0[i];
                     int_ptr2[i] = modulation_data_1[2 * i];
                     int_ptr3[i] = modulation_data_1[2 * i + 1];
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
                 for(i = 0;i < m_sybol;i++)
                 {
                     int_ptr1[i] = modulation_data_0[2 * i];
                     int_ptr2[i] = modulation_data_0[2 * i + 1];
                     int_ptr3[i] = modulation_data_1[2 * i];
                     int_ptr4[i] = modulation_data_1[2 * i + 1];
                 }
                 
             }
             /*����ּ������ָ���Ϊ2*/
             if((TRANSMIT_DIVERSITY == diversity_mode) && (NUM_CODEWORD_1 == num_codeword))
             {
                 *m_laryer_symbol = (m_sybol + 2) >> 2;/*��ֹm_sybol����4��������*/
                 int_ptr1 = layermapping_data;
                 int_ptr2 = layermapping_data + (*m_laryer_symbol);
                 int_ptr3 = layermapping_data + 2 * (*m_laryer_symbol);
                 int_ptr4 = layermapping_data + 3 * (*m_laryer_symbol);
                 for(i = 0;i < m_sybol;i++)
                 {
                     int_ptr1[i] = modulation_data_0[4 * i];
                     int_ptr2[i] = modulation_data_0[4 * i + 1];
                     int_ptr3[i] = modulation_data_0[4 * i + 2];
                     int_ptr4[i] = modulation_data_0[4 * i + 3];
                 }
                 
             }
             break;
         }
         default:
             break;
             
    }           
}
