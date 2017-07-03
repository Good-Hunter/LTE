/*
 * prb_demapping
 * 1,Ŀǰ�����������������prb
 * 2��ÿ�η���һ���˿ڵ�crs
 * 3����ֻ��ȡ���û���ռ��prb��crs��������ȡ����prb�ģ�---Ŀǰ�Ǽ������еģ��ŵ����ƺ��ŵ����Ʋ�ֵͬ
 */

#include <system_init.h>

#define SUBCARR_NUM_PRB_DATA 8


//һ�δ������˿ڵ�crs��Ϊ�˷���pdsch_demapping
//һ�δ���һ�����ţ�Ϊ�˷���ʱ���Ų�
//һ�δ�������������
void prb_demapping_pdsch(signed int* restrict e_freq_data_ptr,   //In, Ƶ�������׵�ַ
                         signed int  recv_ant_num,       //Para
                         signed int  cell_id,        //Para,С��ID��0-503��
                         //signed int  port_no,        //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
                         signed int  symb_no,        //Para,����demapping�ķ��ţ��������Խ�����ƥ��ʱ��������ڼ������ſ�ʼ����...��
                         signed int  subsfn,             //Para,���δ������֡�ţ��ɿ�����ns�ϲ���
                         signed int  ns,           //Para,���δ����ʱ϶�ţ�port2/3ʱ��Ҫ��
                         signed int* restrict e_freq_data_demap_ptr_0,    //In/Out, demapping��Ľ�������0��Ƶ�������׵�ַ��ÿ�δ��뱾���Ÿô�ŵ��׵�ַ�������¸����Ÿô�ŵ��׵�ַ��
                         signed int* restrict e_freq_data_demap_ptr_1    //In/Out, demapping��Ľ�������0��Ƶ�������׵�ַ��ÿ�δ��뱾���Ÿô�ŵ��׵�ַ�������¸����Ÿô�ŵ��׵�ַ��
                         )
{
  signed int  v = 0, v_shift = 0, v_sum_0 = 0, v_sum_1 = 0, thresh_0 = 0, thresh_1 = 0;
  signed int* restrict freq_data_ptr;  //���δ���ķ��ŵ��׵�ַ
  signed int* restrict freq_crs_ptr_0;   //���δ�����Ŷ�Ӧcrs��ŵ��׵�ַ(�˿�0)
  signed int* restrict freq_crs_ptr_1;   //���δ�����Ŷ�Ӧcrs��ŵ��׵�ַ(�˿�1)
  signed int  symb_no_crs;  //�ڵ�Ƶ�����еķ������
  signed int  i, prb_no, k1, k2;
  signed int  port_no = 0;    //�˿ںţ�Ŀǰ֧��0,1
  signed int  freq_index = 0;
  signed int* restrict e_freq_data_demap_ptr;

  //CRS prb_demapping
  //��������
  v_shift = cell_id % 6;
  if((0 == symb_no) || (7 == symb_no))  //l=0
  {
    v = 0;
    if(0 == symb_no)
    {
      symb_no_crs = 0;
    }
    else
    {
      symb_no_crs = 2;
    }
  }
  else if((4 == symb_no) || (11 == symb_no))  //l!=0
  {
    v = 3;
    if(4 == symb_no)
    {
      symb_no_crs = 1;
    }
    else if(11 == symb_no)
    {
      symb_no_crs = 3;
    }
  }
  //if(0 == port_no)                //port0
  {
    v_sum_0 = (v + v_shift)%6;          //�˴���Ҫģ6������
    }
    //else                            //port1
    {
        v_sum_1 = (v + v_shift + 3)%6;
    }
    
  
    //PDSCH prb_demapping
    for(i = 0; i < recv_ant_num; i++)
    {
      //��i���������ߡ���symb_no�����ŵ�Ƶ�������׵�ַ
      freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_DL_RB * N_SC_PER_RB * N_SYM_PER_SLOT * 2 + symb_no * N_DL_RB * N_SC_PER_RB];
      if (i == 0)
      {
          e_freq_data_demap_ptr = e_freq_data_demap_ptr_0;    //��������0
      }
      else
      {
          e_freq_data_demap_ptr = e_freq_data_demap_ptr_1;    //��������1
      }
      if(subsfn == 0)    //��֡0(crs��pbch��sss)
      {
          if(0 == symb_no)    //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }

              //20121230���Ż�
              for(freq_index = 0; freq_index < thresh_0; freq_index++)
              {
                  for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
              {
                  for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
              {
                  for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
              {
                  for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
              {
                  for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                  {
                       e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
                  
                  
              /*for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              //}*/
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;

              }
              /*for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              //}*/
              
                  //20121230���Ż�
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                      {
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                      }
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                      {
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                      }
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                      {
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                      }
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                      {
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                      }
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
                      {
                           e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                      }
                  }
                  //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
                  //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if((7 == symb_no))     //����crs��pbch
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }
              //for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
                          e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
                          e_freq_data_demap_ptr[(prb_no-6)*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
                          e_freq_data_demap_ptr[(prb_no-6)*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
                          e_freq_data_demap_ptr[(prb_no-6)*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
                          e_freq_data_demap_ptr[(prb_no-6)*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
                          e_freq_data_demap_ptr[(prb_no-6)*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start�����0��ʼ
              {
                  total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  total_subcarrier_num = (N_DL_RB/2 - 3 - prb_start) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = ((prb_start + prb_num) - N_DL_RB/2 - 3) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (prb_start - 6) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  total_subcarrier_num = (prb_num - 6) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }*/
          }
          else if((8 == symb_no) || (9 == symb_no) || (10 == symb_no) || (13 == symb_no))     //����pbch��sss
          {
              for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[(prb_no-6)*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start�����0��ʼ
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  total_subcarrier_num = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  total_subcarrier_num = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }*/
          }
          else    //�������ŵ�/�źţ���������
          {
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * N_SC_PER_RB;
              //symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      } 
      else if(subsfn == 5)    //��֡5(crs��sss)
      {
          if((0 == symb_no) || (7 == symb_no))    //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }

              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;

              }
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(13 == symb_no)     //����pbch��sss
          {
              for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[(prb_no-6)*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start�����0��ʼ
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  total_subcarrier_num = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  total_subcarrier_num = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }*/
          }
          else    //�������ŵ�/�źţ���������
          {
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * N_SC_PER_RB;
              //symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }    
      else if((subsfn == 1) || (subsfn == 6))     //��֡1��6(crs��pss)  
      {
          if((0 == symb_no) || (7 == symb_no))    //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;

              }
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(2 == symb_no)     //����pss
          {
              for(prb_no = 0; prb_no < (N_DL_RB/2-3); prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              for(prb_no = (N_DL_RB/2+3); prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[(prb_no-6)*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start�����0��ʼ
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  total_subcarrier_num = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  total_subcarrier_num = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  total_subcarrier_num = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  total_subcarrier_num = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }*/
          }
          else    //�������ŵ�/�źţ���������
          {
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * N_SC_PER_RB;
              //symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }    
      else    //��֡3��4��8��9(crs)
      {
          if((0 == symb_no) || (7 == symb_no))   //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //����crs
          {
              if(v_shift < 3)  //l=0��v_shift<3ʱ��v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0��v_shift>=3ʱ��v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_0;
                    thresh_1 = v_sum_1;

              }
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < thresh_0; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+1; freq_index < thresh_1; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-1] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+1; freq_index < thresh_0+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-2] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_0+N_SC_PER_RB/2+1; freq_index < thresh_1+N_SC_PER_RB/2; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-3] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
                  for(freq_index = thresh_1+N_SC_PER_RB/2+1; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*SUBCARR_NUM_PRB_DATA+freq_index-4] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * SUBCARR_NUM_PRB_DATA;
              //symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else    //�������ŵ�/�źţ���������
          {
              for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
              {
                  for(freq_index = 0; freq_index < N_SC_PER_RB; freq_index++)
                  {
                      e_freq_data_demap_ptr[prb_no*N_SC_PER_RB+freq_index] = freq_data_ptr[prb_no*N_SC_PER_RB+freq_index];
                  }
              }
              //total_subcarrier_num = prb_num * N_SC_PER_RB;
              //symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      

  }
  //return total_subcarrier_num;

}
void prb_demapping_pbch(signed int* restrict e_freq_data_ptr_0,//in
                        signed int* restrict e_freq_data_ptr_1,//in
                        signed int* restrict e_freq_pbch_data_ptr_0,//out
                        signed int* restrict e_freq_pbch_data_ptr_1,//out
                        unsigned char* restrict PBCHCarrierIdx
                       )
{
    int k,i;
    
    for(k = 0; k < 2;k++)//����7��8
    {

         #pragma MUST_ITERATE(, , 4);
         for(i = 0;i < TOTAL_PBCH_SC_NUM;i = i + 6)
         {
             //����0
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[0] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[1] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[2] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[3] + i);
             //����1
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[0] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[1] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[2] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[3] + i); 
            
             
         }
         e_freq_data_ptr_0 = e_freq_data_ptr_0 + N_DL_RB * N_SC_PER_RB;
         e_freq_data_ptr_1 = e_freq_data_ptr_1 + N_DL_RB * N_SC_PER_RB;
         
    }
    for(k = 0; k < 2;k++)//����9��10
    {

         #pragma MUST_ITERATE(, , 8);
         for(i = 0;i < TOTAL_PBCH_SC_NUM;i++)
         {
             *(e_freq_pbch_data_ptr_0++) = e_freq_data_ptr_0[i];
             *(e_freq_pbch_data_ptr_1++) = e_freq_data_ptr_1[i];

         }
         e_freq_data_ptr_0 = e_freq_data_ptr_0 + N_DL_RB * N_SC_PER_RB;
         e_freq_data_ptr_1 = e_freq_data_ptr_1 + N_DL_RB * N_SC_PER_RB;
    }
}
//����������˳��ȡ��Ƶ������
void f_pcfich_freq_data_recover(int* restrict input_data_ptr,
                            int* restrict out_put_ptr,
                            DL_resource_mapping_struct *dl_resource_mapping_ptr,
                            CELL_PARA *cell_para_ptr
                            )
{
  int temp_k = 0;
  int index = 0;
  int ante_num;
  char total_pcfich_res = 4*4;
  ante_num = cell_para_ptr->num_antenna_port;
  if(2 == ante_num)
  {
    for(index = 0;index < total_pcfich_res;index++)
    {
      temp_k = dl_resource_mapping_ptr->pcfich_res[index];
      out_put_ptr[index] = input_data_ptr[temp_k];
      out_put_ptr[4 * 4 + index] = input_data_ptr[2 * N_SYM_PER_SLOT * N_DL_RB * N_SC_PER_RB + temp_k];
    }
  }
  else if (1 == ante_num)
  {
    for(index = 0;index < total_pcfich_res;index++)
    {
      temp_k = dl_resource_mapping_ptr->pcfich_res[index];
      out_put_ptr[index] = input_data_ptr[temp_k];
    }
  }
  else
  {
    //ALARM!
  }
}

//����������˳��ȡ��Ƶ������
void f_phich_freq_data_recover(int* restrict input_data_ptr,
                            int* restrict out_put_ptr,
                            DL_resource_mapping_struct *dl_resource_mapping_ptr,
                            CELL_PARA *cell_para_ptr,
                            unsigned char slot_idx
                            )
{
  int temp_k = 0;
  int index = 0;
  int ante_num;
  ante_num = cell_para_ptr->num_antenna_port;
  if(2 == ante_num)
  {
    for(index = 0;index < 12 * g_N_group_phich[slot_idx>>1];index++)
    {
      temp_k = dl_resource_mapping_ptr->phich_mapping[slot_idx>>1][index];
      out_put_ptr[index] = input_data_ptr[temp_k];
      out_put_ptr[12 * g_N_group_phich[slot_idx>>1] + index] = input_data_ptr[2 * N_SYM_PER_SLOT * N_DL_RB * N_SC_PER_RB + temp_k];
    }
  }
  else if (1 == ante_num)
  {
    for(index = 0;index < 12 * g_N_group_phich[slot_idx>>1];index++)
    {
      temp_k = dl_resource_mapping_ptr->phich_mapping[slot_idx>>1][index];
      out_put_ptr[index] = input_data_ptr[temp_k];
    }
  }
  else
  {
    //ALARM!
  }
}
//����������˳��ȡ��Ƶ������
void f_pdcch_freq_data_recover(int* restrict input_data_ptr,
                               int* restrict out_put_ptr,
                               unsigned short* restrict pdcch_data_table_ptr,
                               unsigned short length,
                               char ante_num
                               )
{
  unsigned short temp_k = 0;
  int index = 0;
  if(2 == ante_num)
  {
    for(index = 0;index < length;index++)
    {
      temp_k = pdcch_data_table_ptr[index];
      out_put_ptr[index] = input_data_ptr[temp_k];
      out_put_ptr[length + index] = input_data_ptr[2 * N_SYM_PER_SLOT * N_DL_RB * N_SC_PER_RB + temp_k];
    }
  }
  else if (1 == ante_num)
  {
    for(index = 0;index < length;index++)
    {
      temp_k = pdcch_data_table_ptr[index];
      out_put_ptr[index] = input_data_ptr[temp_k];
    }
  }
  else
  {
    //ALARM!
  }
}
