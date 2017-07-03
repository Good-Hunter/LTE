/*
 * chan_est
 * 1，每次计算一个端口的频域信道估计
 * 2，是只计算该用户所占的prb的crs，还是计算所有prb的？[与demapping一样的疑问]
 * 3，信道估计的频域、时域插值在专门的函数中进行
  */

#include "pl_comm.h"
#if 0 
void chan_est(signed int* restrict e_freq_crs_ptr,   //In, 导频接收数据首地址 Q(16,1)
            unsigned char* restrict d_freq_crs_ptr,   //In, 本地导频序列首地址，排列方式同e_freq_crs_ptr；为了方便实现e/d，此处直接存d的共轭
            signed int  recv_ant_num,         //Para
            signed int  prb_start,            //Para,***目前仅适用于连续分配的prb***
            signed int  prb_num,              //Para
            signed int  port_no,          //Para,本次demapping的导频的端口号，目前支持0,1
            signed int  amp_ratio,      //Para,由导频功率/数据功率折算出的二者幅度比；为了方便实现e/ratio，此处直接存数据/导频幅度比；以复数形式存储，高16bit有效（实部），低16bit为0 Q(16,3)
            signed int* restrict h_freq_crs_ptr        //Out, 频域信道估计首地址（插值前），排列方式同e_freq_crs_ptrQ(16,1)
       )
{
  signed int* restrict ee_freq_crs_ptr;
  unsigned char* restrict dd_freq_crs_ptr;
  signed int* restrict hh_freq_crs_ptr;
  signed int  i = 0, symb_no_crs, prb_no, d_temp1, d_amp_temp1, e_temp1, h_temp1;
  signed int  d_temp2, d_amp_temp2, e_temp2, h_temp2;

  for(i = 0; i < recv_ant_num; i++)
  {
    for(symb_no_crs = 0; symb_no_crs < CRS_SYMB_NUM_NORMCP_1MS; symb_no_crs++)
    {
      //第port_no个端口、第i根接收天线的导频接收数据首地址
      ee_freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + symb_no_crs * CRS_SUBCARR_NUM_20M];

      //第port_no个端口、第i根接收天线的本地导频序列首地址
      dd_freq_crs_ptr = (unsigned char*)&d_freq_crs_ptr[(symb_no_crs / (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 3 * 220
                                                    + (symb_no_crs % (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 2 * 220 + N_MAX_DL_RB - N_DL_RB];/*导频是按照0,1,4存储*/
      //第port_no个端口、第i根接收天线的频域信道估计首地址（插值前）
      hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + symb_no_crs * CRS_SUBCARR_NUM_20M];

      //信道估计计算：h = e*(d*)*amp_ratio
      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
      {
        //导频re1
        d_temp1 = rs_lut_conj[dd_freq_crs_ptr[prb_no * 2]];
        d_amp_temp1 = _cmpyr1(d_temp1, amp_ratio);    //(d*)*amp_ratio；可以直接用乘法，用short存储amp_ratio Q(16,1)*Q(16,3)=Q(16,3)
        e_temp1 = ee_freq_crs_ptr[prb_no * 2];
        h_temp1 = _cmpyr1(d_amp_temp1, e_temp1);     //e*d_amp Q(16,1)*Q(16,3)=Q(16,3)
        hh_freq_crs_ptr[prb_no * 2] = _shl2(h_temp1,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no * 2] = h_temp1<<2;//Q(16,3)<<2=Q(16,1)
        //导频re2
        d_temp2 = rs_lut_conj[dd_freq_crs_ptr[prb_no*2 + 1]];
        d_amp_temp2 = _cmpyr1(d_temp2, amp_ratio);      //(d*)*amp_ratio
        e_temp2 = ee_freq_crs_ptr[prb_no*2 + 1];
        h_temp2 = _cmpyr1(d_amp_temp2, e_temp2);     //e*d_amp
        hh_freq_crs_ptr[prb_no*2 + 1] = _shl2(h_temp2,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no*2 + 1] = h_temp2<<2;//Q(16,3)<<2=Q(16,1)
      }
    }
  }
}
#endif


void chan_est(signed int* restrict e_freq_crs_ptr,   //In, 导频接收数据首地址 Q(16,1)
            unsigned char* restrict d_freq_crs_ptr,   //In, 本地导频序列首地址，排列方式同e_freq_crs_ptr；为了方便实现e/d，此处直接存d的共轭
            signed int  recv_ant_num,         //Para
            signed int  prb_start,            //Para,***目前仅适用于连续分配的prb***
            signed int  prb_num,              //Para
            signed int  port_no,          //Para,本次demapping的导频的端口号，目前支持0,1
            signed int  amp_ratio,      //Para,由导频功率/数据功率折算出的二者幅度比；为了方便实现e/ratio，此处直接存数据/导频幅度比；以复数形式存储，高16bit有效（实部），低16bit为0 Q(16,3)
            signed int* restrict h_freq_crs_ptr,        //Out, 频域信道估计首地址（插值前），排列方式同e_freq_crs_ptrQ(16,1)
            unsigned char offset, //crs的起始位置
            unsigned char crs_length//crs的长度
       )
{
  signed int* restrict ee_freq_crs_ptr;
  unsigned char* restrict dd_freq_crs_ptr;
  signed int* restrict hh_freq_crs_ptr;
  signed int  i = 0, symb_no_crs, prb_no, d_temp1, d_amp_temp1, e_temp1, h_temp1;
  signed int  d_temp2, d_amp_temp2, e_temp2, h_temp2;
  

  for(i = 0; i < recv_ant_num; i++)
  {
    for(symb_no_crs = 0; symb_no_crs < crs_length; symb_no_crs++)
    {
      //第port_no个端口、第i根接收天线的导频接收数据首地址
      ee_freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + (symb_no_crs + offset) * CRS_SUBCARR_NUM_20M];

      //第port_no个端口、第i根接收天线的本地导频序列首地址
      dd_freq_crs_ptr = (unsigned char*)&d_freq_crs_ptr[((symb_no_crs + offset) / (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 3 * 220
                                                    + ((symb_no_crs + offset) % (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 2 * 220 + N_MAX_DL_RB - N_DL_RB];/*导频是按照0,1,4存储*/
      //第port_no个端口、第i根接收天线的频域信道估计首地址（插值前）
      hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + (symb_no_crs + offset) * CRS_SUBCARR_NUM_20M];

      //信道估计计算：h = e*(d*)*amp_ratio
      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
      {
        //导频re1
        d_temp1 = rs_lut_conj[dd_freq_crs_ptr[prb_no * 2]];
        d_amp_temp1 = _cmpyr1(d_temp1, amp_ratio);    //(d*)*amp_ratio；可以直接用乘法，用short存储amp_ratio Q(16,1)*Q(16,3)=Q(16,3)
        e_temp1 = ee_freq_crs_ptr[prb_no * 2];
        h_temp1 = _cmpyr1(d_amp_temp1, e_temp1);     //e*d_amp Q(16,1)*Q(16,3)=Q(16,3)
        hh_freq_crs_ptr[prb_no * 2] = _shl2(h_temp1,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no * 2] = h_temp1<<2;//Q(16,3)<<2=Q(16,1)
        //导频re2
        d_temp2 = rs_lut_conj[dd_freq_crs_ptr[prb_no*2 + 1]];
        d_amp_temp2 = _cmpyr1(d_temp2, amp_ratio);      //(d*)*amp_ratio
        e_temp2 = ee_freq_crs_ptr[prb_no*2 + 1];
        h_temp2 = _cmpyr1(d_amp_temp2, e_temp2);     //e*d_amp
        hh_freq_crs_ptr[prb_no*2 + 1] = _shl2(h_temp2,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no*2 + 1] = h_temp2<<2;//Q(16,3)<<2=Q(16,1)
      }
    }
  }
}
