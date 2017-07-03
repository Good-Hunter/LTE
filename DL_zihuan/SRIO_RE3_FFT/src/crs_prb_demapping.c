/*
 * prb_demapping
 * 1,目前仅适用于连续分配的prb
 * 2，每次分离一个端口的crs
 * 3，是只提取该用户所占的prb的crs，还是提取所有prb的？---目前是计算所有的，信道估计和信道估计插值同
 */

#include <pl_comm.h>

#if 0
void crs_prb_demapping(signed int* restrict e_freq_data_ptr,   //In, 频域数据首地址
               signed int  recv_ant_num,       //Para
               signed int  cell_id,        //Para,小区ID（0-503）
               signed int  port_no,        //Para,本次demapping的导频的端口号，目前支持0,1
               signed int* restrict e_freq_crs_ptr      //Out, 导频数据首地址
          )
{
  signed int* restrict freq_data_ptr;  //本次处理的符号的首地址
  signed int* restrict freq_crs_ptr;   //本次处理符号对应crs存放的首地址
  signed int  symb_no_crs;  //在导频符号中的符号序号
  signed int  i, prb_no,pos_k1, pos_k2,symb_no;
  unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

  //CRS prb_demapping
  //参数计算

  for(i = 0; i < recv_ant_num; i++)
  {
      for(symb_no_crs = 0; symb_no_crs < CRS_SYMB_NUM_NORMCP_1MS; symb_no_crs++)      //0-3
      {
          symb_no = crs_symb_no_table_2port[symb_no_crs];
          if(((0 == port_no) && (0 == symb_no_crs % (CRS_SYMB_NUM_NORMCP_1MS >> 1)))
          || ((1 == port_no) && (1 == symb_no_crs % (CRS_SYMB_NUM_NORMCP_1MS >> 1))))                 //port0
          {
              pos_k1 = PilotInRBK1[cell_id % 6][0];
              pos_k2 = PilotInRBK1[cell_id % 6][1];
          }
          else                     //port1
          {
              pos_k1 = PilotInRBK2[cell_id % 6][0];
              pos_k2 = PilotInRBK2[cell_id % 6][1];
          }
      //提取
      
        //freq_data_ptr = (signed int*)&e_freq_data_ptr[i*SUBCARR_NUM_20M*SYMB_NUM_NORMCP_1MS + symb_no*SUBCARR_NUM_20M + prb_start*SUBCARR_NUM_PRB];
        //第i根接收天线、第symb_no个符号的频域数据首地址
        freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + symb_no * N_SC_PER_RB * N_DL_RB];
        //第port_no个端口、第i根接收天线、第symb_no_crs个导频符号的导频存储区首地址
        freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                    + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                    + symb_no_crs * CRS_SUBCARR_NUM_20M];
      
        for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
        {
           freq_crs_ptr[prb_no*2 + 0] = freq_data_ptr[prb_no * N_SC_PER_RB + pos_k1];
           freq_crs_ptr[prb_no*2 + 1] = freq_data_ptr[prb_no * N_SC_PER_RB + pos_k2];
        }
      }
  }

  //others
}
#endif

void crs_prb_demapping(signed int* restrict e_freq_data_ptr,   //In, 频域数据首地址
               signed int  recv_ant_num,       //Para
               signed int  cell_id,        //Para,小区ID（0-503）
               signed int  port_no,        //Para,本次demapping的导频的端口号，目前支持0,1
               signed int* restrict e_freq_crs_ptr,      //Out, 导频数据首地址
               unsigned char offset ,//crs的起始位置
               unsigned char crs_length//crs的长度
          )
{
  signed int* restrict freq_data_ptr;  //本次处理的符号的首地址
  signed int* restrict freq_crs_ptr;   //本次处理符号对应crs存放的首地址
  signed int  symb_no_crs;  //在导频符号中的符号序号
  signed int  i, prb_no,pos_k1, pos_k2,symb_no;
  unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

  //CRS prb_demapping
  //参数计算

  for(i = 0; i < recv_ant_num; i++)
  {
      for(symb_no_crs = 0; symb_no_crs < crs_length; symb_no_crs++)      //0-3
      {
          symb_no = crs_symb_no_table_2port[symb_no_crs + offset];
          if(((0 == port_no) && (0 == (symb_no_crs + offset) % (CRS_SYMB_NUM_NORMCP_1MS >> 1)))
          || ((1 == port_no) && (1 == (symb_no_crs + offset) % (CRS_SYMB_NUM_NORMCP_1MS >> 1))))                 //port0
          {
              pos_k1 = PilotInRBK1[cell_id % 6][0];
              pos_k2 = PilotInRBK1[cell_id % 6][1];
          }
          else                     //port1
          {
              pos_k1 = PilotInRBK2[cell_id % 6][0];
              pos_k2 = PilotInRBK2[cell_id % 6][1];
          }
      //提取
      
        //freq_data_ptr = (signed int*)&e_freq_data_ptr[i*SUBCARR_NUM_20M*SYMB_NUM_NORMCP_1MS + symb_no*SUBCARR_NUM_20M + prb_start*SUBCARR_NUM_PRB];
        //第i根接收天线、第symb_no个符号的频域数据首地址
        freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + symb_no * N_SC_PER_RB * N_DL_RB];
        //第port_no个端口、第i根接收天线、第symb_no_crs个导频符号的导频存储区首地址
        freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                    + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                    + (symb_no_crs + offset) * CRS_SUBCARR_NUM_20M];
      
        for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
        {
           freq_crs_ptr[prb_no*2 + 0] = freq_data_ptr[prb_no * N_SC_PER_RB + pos_k1];
           freq_crs_ptr[prb_no*2 + 1] = freq_data_ptr[prb_no * N_SC_PER_RB + pos_k2];
        }
      }
  }

  //others
}
