/*
 * prb_demapping
 * 1,目前仅适用于连续分配的prb
 * 2，每次分离一个端口的crs
 * 3，是只提取该用户所占的prb的crs，还是提取所有prb的？---目前是计算所有的，信道估计和信道估计插值同
 */

#include <system_init.h>

#define SUBCARR_NUM_PRB_DATA 8


//一次处理多个端口的crs，为了方便pdsch_demapping
//一次处理一个符号，为了方便时序排布
//一次处理多根接收天线
void prb_demapping_pdsch(signed int* restrict e_freq_data_ptr,   //In, 频域数据首地址
                         signed int  recv_ant_num,       //Para
                         signed int  cell_id,        //Para,小区ID（0-503）
                         //signed int  port_no,        //Para,本次demapping的导频的端口号，目前支持0,1
                         signed int  symb_no,        //Para,本次demapping的符号，这样可以较灵活的匹配时序调整（第几个符号开始处理...）
                         signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
                         signed int  ns,           //Para,本次处理的时隙号（port2/3时需要）
                         signed int* restrict e_freq_data_demap_ptr_0,    //In/Out, demapping后的接收天线0的频域数据首地址（每次传入本符号该存放的首地址，传出下个符号该存放的首地址）
                         signed int* restrict e_freq_data_demap_ptr_1    //In/Out, demapping后的接收天线0的频域数据首地址（每次传入本符号该存放的首地址，传出下个符号该存放的首地址）
                         )
{
  signed int  v = 0, v_shift = 0, v_sum_0 = 0, v_sum_1 = 0, thresh_0 = 0, thresh_1 = 0;
  signed int* restrict freq_data_ptr;  //本次处理的符号的首地址
  signed int* restrict freq_crs_ptr_0;   //本次处理符号对应crs存放的首地址(端口0)
  signed int* restrict freq_crs_ptr_1;   //本次处理符号对应crs存放的首地址(端口1)
  signed int  symb_no_crs;  //在导频符号中的符号序号
  signed int  i, prb_no, k1, k2;
  signed int  port_no = 0;    //端口号，目前支持0,1
  signed int  freq_index = 0;
  signed int* restrict e_freq_data_demap_ptr;

  //CRS prb_demapping
  //参数计算
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
    v_sum_0 = (v + v_shift)%6;          //此处需要模6？？？
    }
    //else                            //port1
    {
        v_sum_1 = (v + v_shift + 3)%6;
    }
    
  
    //PDSCH prb_demapping
    for(i = 0; i < recv_ant_num; i++)
    {
      //第i根接收天线、第symb_no个符号的频域数据首地址
      freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_DL_RB * N_SC_PER_RB * N_SYM_PER_SLOT * 2 + symb_no * N_DL_RB * N_SC_PER_RB];
      if (i == 0)
      {
          e_freq_data_demap_ptr = e_freq_data_demap_ptr_0;    //接收天线0
      }
      else
      {
          e_freq_data_demap_ptr = e_freq_data_demap_ptr_1;    //接收天线1
      }
      if(subsfn == 0)    //子帧0(crs、pbch、sss)
      {
          if(0 == symb_no)    //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
              { 
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;

              }

              //20121230，优化
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
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
              
                  //20121230，优化
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
          else if((7 == symb_no))     //跳过crs和pbch
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
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
          else if((8 == symb_no) || (9 == symb_no) || (10 == symb_no) || (13 == symb_no))     //跳过pbch或sss
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
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
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
          else    //无其他信道/信号，连续拷贝
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
      else if(subsfn == 5)    //子帧5(crs、sss)
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else if(13 == symb_no)     //跳过pbch或sss
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
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
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
          else    //无其他信道/信号，连续拷贝
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
      else if((subsfn == 1) || (subsfn == 6))     //子帧1、6(crs、pss)  
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else if(2 == symb_no)     //跳过pss
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
              /*if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
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
          else    //无其他信道/信号，连续拷贝
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
      else    //子帧3、4、8、9(crs)
      {
          if((0 == symb_no) || (7 == symb_no))   //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_0;
                  thresh_1 = v_sum_1;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              if(v_shift < 3)  //l=0且v_shift<3时，v_sum_0<v_sum_1
              {
                  thresh_0 = v_sum_1;
                  thresh_1 = v_sum_0;
              }
              else    //l=0且v_shift>=3时，v_sum_0>v_sum_1
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
          else    //无其他信道/信号，连续拷贝
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
    
    for(k = 0; k < 2;k++)//符号7和8
    {

         #pragma MUST_ITERATE(, , 4);
         for(i = 0;i < TOTAL_PBCH_SC_NUM;i = i + 6)
         {
             //天线0
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[0] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[1] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[2] + i);
             *(e_freq_pbch_data_ptr_0++) = *(e_freq_data_ptr_0 + PBCHCarrierIdx[3] + i);
             //天线1
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[0] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[1] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[2] + i); 
             *(e_freq_pbch_data_ptr_1++) = *(e_freq_data_ptr_1 + PBCHCarrierIdx[3] + i); 
            
             
         }
         e_freq_data_ptr_0 = e_freq_data_ptr_0 + N_DL_RB * N_SC_PER_RB;
         e_freq_data_ptr_1 = e_freq_data_ptr_1 + N_DL_RB * N_SC_PER_RB;
         
    }
    for(k = 0; k < 2;k++)//符号9和10
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
//按照索引表顺序取出频域数据
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

//按照索引表顺序取出频域数据
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
//按照索引表顺序取出频域数据
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
