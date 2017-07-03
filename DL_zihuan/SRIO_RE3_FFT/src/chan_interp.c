/*
 * chan_interp
 * 1，每次进行一个端口的频域信道估计插值（先频域+后时域）
 * 2，是只计算该用户所占的prb的crs，还是计算所有prb的？[与demapping一样的疑问]---现在是算全带宽的，可以改成按频带的，目前prb在内层
 * 3，时域上进行所有符号的插值---是可以考虑改为分段或者按符号进行的
 * 4，频域插值的叠加可能会溢出，不过有sadd2保护
 * 5，时域插值时coef_k_4有大于1的数，需要注意其处理；用Q(16,1)表示不一定合适呀？？？
  */
 
#include "pl_comm.h"

signed int   coef_k_6[6] = {0,          //0/6, Q(16,1), packed signed short
              0x15551555,         //1/6
              0x2aaa2aaa,     //2/6
              0x40004000,     //3/6
              0x55555555,     //4/6
              0x6aaa6aaa      //5/6
              };
signed int   coef_k_6_ext[6] = {
          0x40004000,    //1  Q(16,2), packed signed short
          0x4aaa4aaa,    //7/6
          0x55555555,    //4/3
          0x60006000,    //3
          0x6aaa6aaa,    //10/6
          0x75557555
              };
signed int   coef_k_4[7] = {0,          // 0无效，Q(16,2), packed signed short
              0x10001000,         //1/4
              0x20002000,     //2/4
              0x30003000,     //3/4
              0x40004000,     //4/4
              0x50005000,     //5/4
              0x60006000      //6/4
              };

signed int   coef_k_3[3] = {0,    // 0无效，Q(16,1), packed signed short
              0x2aaa2aaa,         //1/3
              0x55555555      //2/3
              };
signed int   coef_k_3_ext[3] = {// Q(16,3), packed signed short
              0x2aaa2aaa,    //4/3
              0x35553555,    //5/3 
              0x40004000,    //6/3   
              };
                                          
void chan_interp_freq(signed int* restrict h_freq_crs_ptr,        //In, 频域信道估计首地址（插值前），排列方式同e_freq_crs_ptr Q(16,1)
               signed int  recv_ant_num,          //Para
               signed int  prb_start,             //Para,***目前仅适用于连续分配的prb***
               signed int  prb_num,               //Para
               signed int  cell_id,         //Para,小区ID（0-503）
               signed int  port_no,         //Para,本次demapping的导频的端口号，目前支持0,1
               signed int* restrict h_freq_interp_ptr,      //Out, 频域信道估计首地址（插值后），排列方式同e_freq_data_ptr，kaV在最外层
               unsigned char offset, //crs的起始位置
               unsigned char crs_length//crs的长度
          )
{
    signed int  symb_no_crs;  //在导频符号中的符号序号
    signed int  symb_no;      //在符号中的符号序号
    signed int  i, k, prb_no, pos_k1, pos_k2;  
    signed int* restrict hh_freq_crs_ptr;
    signed int* restrict hh_freq_interp_ptr;
    
    signed int    pos_coef, mpy_temp;
    signed int    delta_h_1, delta_h_2, delta_h_3, h1, h2, h3, h0;
    unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

    //参数计算---导频频域位置
    for(i = 0; i < NUM_MAX_ANTENNA; i++)
    {
      //频域线性插值,4个导频符号，200->1200
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
          
          
          //第port_no个端口、第i根接收天线、第symb_no_crs列导频的频域信道估计首地址（插值前）
          //优化：可把部分位置计算挪至循环外
          hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                         + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                         + (symb_no_crs + offset)* CRS_SUBCARR_NUM_20M];
          //第port_no个端口、第i根接收天线、第symb_no个符号的频域信道估计首地址（插值后）
          hh_freq_interp_ptr = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                              + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                              + symb_no * N_SC_PER_RB * N_DL_RB];
                                                                      
          //第一个prb
          h1 = hh_freq_crs_ptr[0];
          h2 = hh_freq_crs_ptr[1];
          h3 = hh_freq_crs_ptr[2];
          delta_h_1 = _ssub2(h1, h2);
          delta_h_2 = _ssub2(h2, h1);
          delta_h_3 = _ssub2(h3, h2);
          for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1;  此处为了实现方便，对算法略做改动
          {
            pos_coef = coef_k_6[pos_k1 - k];    //[(k-pos_k1)/6]*(h2-h1)+h1 = [(pos_k1-k)/6]*(h1-h2)+h1
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_1), _smpyhl(pos_coef, delta_h_1));   //Q(16,1)*Q(16,1)<<1 -> Q(32,1) 取高16位 -> Q(16,1)
            hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1);       //Q(16,1) + Q(16,1)，有溢出的可能
          }
          for(k = pos_k1; k < pos_k2; k++)        //pos_k1 ~ pos_k2-1;  此处为了实现方便，对算法略做改动
          {
            pos_coef = coef_k_6[k - pos_k1];    //[(k-pos_k1)/6]*(h2-h1)+h1
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));
            hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1);
          }
          for(k = pos_k2; k < N_SC_PER_RB; k++)   //pos_k2 ~ 11
          {
            pos_coef = coef_k_6[k - pos_k2];    //[(k-pos_k2)/6]*(h3-h2)+h2
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_3), _smpyhl(pos_coef, delta_h_3));
            hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h2);
          }
          
          //第2-99个prb
          //20121230，优化
          for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1;
          {
              for(prb_no = 1; prb_no < (N_DL_RB - 1); prb_no++)
              {
                  h0 = hh_freq_crs_ptr[2*prb_no - 1];     //上个prb的第二个导频re
                  h1 = hh_freq_crs_ptr[2*prb_no];         //本prb的第一个导频re
                  //h2 = hh_freq_crs_ptr[2*prb_no + 1];     //本prb的第二个导频re
                  //h3 = hh_freq_crs_ptr[2*prb_no + 2];     //下个prb的第一个导频re
                  delta_h_1 = _ssub2(h1, h0);
                  //delta_h_2 = _ssub2(h2, h1);
                  //delta_h_3 = _ssub2(h3, h2);
                  pos_coef = coef_k_6[k + N_SC_PER_RB - pos_k2];    //[(k+12-pos_k1)/6]*(h1-h0)+h0
                  mpy_temp = _packh2(_smpyh(pos_coef, delta_h_1), _smpyhl(pos_coef, delta_h_1));
                  hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h0);
              }
          }
          for(k = pos_k1; k < pos_k2; k++)        //pos_k1 ~ pos_k2-1;
          {
              for(prb_no = 1; prb_no < (N_DL_RB - 1); prb_no++)
              {
                  //h0 = hh_freq_crs_ptr[2*prb_no - 1];     //上个prb的第二个导频re
                  h1 = hh_freq_crs_ptr[2*prb_no];         //本prb的第一个导频re
                  h2 = hh_freq_crs_ptr[2*prb_no + 1];     //本prb的第二个导频re
                  //h3 = hh_freq_crs_ptr[2*prb_no + 2];     //下个prb的第一个导频re
                  //delta_h_1 = _ssub2(h1, h0);
                  delta_h_2 = _ssub2(h2, h1);
                  //delta_h_3 = _ssub2(h3, h2);
                  pos_coef = coef_k_6[k - pos_k1];    //[(k-pos_k1)/6]*(h2-h1)+h1
                  mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));
                  hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h1);
              }
          }
          for(k = pos_k2; k < N_SC_PER_RB; k++)   //pos_k2 ~ 11
          {
              for(prb_no = 1; prb_no < (N_DL_RB - 1); prb_no++)
              {
                  //h0 = hh_freq_crs_ptr[2*prb_no - 1];     //上个prb的第二个导频re
                  //h1 = hh_freq_crs_ptr[2*prb_no];         //本prb的第一个导频re
                  h2 = hh_freq_crs_ptr[2*prb_no + 1];     //本prb的第二个导频re
                  h3 = hh_freq_crs_ptr[2*prb_no + 2];     //下个prb的第一个导频re
                  //delta_h_1 = _ssub2(h1, h0);
                  //delta_h_2 = _ssub2(h2, h1);
                  delta_h_3 = _ssub2(h3, h2);
                  pos_coef = coef_k_6[k - pos_k2];    //[(k-pos_k1)/6]*(h2-h1)+h1
                  mpy_temp = _packh2(_smpyh(pos_coef, delta_h_3), _smpyhl(pos_coef, delta_h_3));
                  hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h2);
              }
          }
          
          /*
          for(prb_no = 1; prb_no < (N_DL_RB - 1); prb_no++)
          {
              h0 = hh_freq_crs_ptr[2*prb_no - 1];     //上个prb的第二个导频re
              h1 = hh_freq_crs_ptr[2*prb_no];         //本prb的第一个导频re
              h2 = hh_freq_crs_ptr[2*prb_no + 1];     //本prb的第二个导频re
              h3 = hh_freq_crs_ptr[2*prb_no + 2];     //下个prb的第一个导频re
              delta_h_1 = _ssub2(h1, h0);
              delta_h_2 = _ssub2(h2, h1);
              delta_h_3 = _ssub2(h3, h2);
              for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1;
              {
                pos_coef = coef_k_6[k + N_SC_PER_RB - pos_k1];    //[(k+12-pos_k1)/6]*(h1-h0)+h0
                mpy_temp = _packh2(_smpyh(pos_coef, delta_h_1), _smpyhl(pos_coef, delta_h_1));
                hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h0);
              }
              for(k = pos_k1; k < pos_k2; k++)        //pos_k1 ~ pos_k2-1;
              {
                pos_coef = coef_k_6[k - pos_k1];    //[(k-pos_k1)/6]*(h2-h1)+h1
                mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));
                hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h1);
              }
              for(k = pos_k2; k < N_SC_PER_RB; k++)   //pos_k2 ~ 11
              {
                pos_coef = coef_k_6[k - pos_k2];    //[(k-pos_k2)/6]*(h3-h2)+h2
                mpy_temp = _packh2(_smpyh(pos_coef, delta_h_3), _smpyhl(pos_coef, delta_h_3));
                hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h2);
              }
          }*/
          
          //第100个prb
          h0 = hh_freq_crs_ptr[2 * prb_no - 1];
          h1 = hh_freq_crs_ptr[2 * prb_no];
          h2 = hh_freq_crs_ptr[2 * prb_no + 1];
          delta_h_1 = _ssub2(h1, h0);
          delta_h_2 = _ssub2(h2, h1);
          for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1; 
          {
            pos_coef = coef_k_6[k + N_SC_PER_RB - pos_k2];    //[(k+12-pos_k2)/6]*(h1-h0)+h0
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_1), _smpyhl(pos_coef, delta_h_1));
            hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h0);
          }
          for(k = pos_k1; k < pos_k1 + (N_SC_PER_RB >> 1); k++)   //pos_k1 ~ 11
          {
            pos_coef = coef_k_6[k - pos_k1];    //[(k-pos_k1)/6]*(h2-h1)+h1
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));
            hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h1);
          }
          for(k = pos_k1 + (N_SC_PER_RB >> 1); k < N_SC_PER_RB; k++)   //pos_k1 ~ 11
          {
            pos_coef = coef_k_6_ext[k - pos_k1 - (N_SC_PER_RB >> 1)];    //[(k-pos_k1)/6]*(h2-h1)+h1
            mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_2),1), _sshl(_smpyhl(pos_coef, delta_h_2),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) 取高16位 -> Q(16,1)
            hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h1);
          }
      }
    }
}


void chan_interp_time(
		       signed int total_num,
		       signed int  port_no,         //Para,本次demapping的导频的端口号，目前支持0,1
               signed int  symb_start,
               signed int  symb_end,
               signed int* restrict h_freq_interp_ptr      //Out, 频域信道估计首地址（插值后），排列方式同e_freq_data_ptr，kaV在最外层
               )
{
    signed int  symb_no;      //在符号中的符号序号
    signed int  i, k;  
    signed int* restrict hh_freq_interp_ptr;
    signed int* restrict hh_freq_interp_ptr_0;
    signed int* restrict hh_freq_interp_ptr_1;
    signed int* restrict hh_freq_interp_ptr_2;
    signed int* restrict hh_freq_interp_ptr_3;
    
    signed int    pos_coef, mpy_temp;
    signed int    delta_h_1, delta_h_2, delta_h_3, h1, h2, h3, h0;
    unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

    //参数计算---导频频域位置
    for(i = 0; i < NUM_MAX_ANTENNA; i++)
    {      
      //时域线性插值
      //第port_no个端口、第i根接收天线、第0个符号（第1列导频）的频域信道估计首地址（插值后）                                                    
      hh_freq_interp_ptr_0 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 0 * N_SC_PER_RB * N_DL_RB];
      //第port_no个端口、第i根接收天线、第4个符号（第2列导频）的频域信道估计首地址（插值后）                                                    
      hh_freq_interp_ptr_1 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 4 * N_SC_PER_RB * N_DL_RB];
      //第port_no个端口、第i根接收天线、第7个符号（第3列导频）的频域信道估计首地址（插值后）                                                     
      hh_freq_interp_ptr_2 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 7 * N_SC_PER_RB * N_DL_RB];
      //第port_no个端口、第i根接收天线、第11个符号（第4列导频）的频域信道估计首地址（插值后） S子帧不一定有第4列导频                                                    
      hh_freq_interp_ptr_3 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 11 * N_SC_PER_RB * N_DL_RB];
      for(symb_no = symb_start; symb_no < symb_end; symb_no++)      //0-13
      {
          //第port_no个端口、第i根接收天线、第symb_no个符号的频域信道估计首地址（插值后）
          hh_freq_interp_ptr = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                            + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                            + symb_no * N_SC_PER_RB * N_DL_RB];
                                                                                                                                                                                                                     
          if((symb_no > 0) && (symb_no < 4))  //1,2,3
          {
            pos_coef = coef_k_4[symb_no - crs_symb_no_table_2port[0]];   //[(k-pos_l0)/(pos_l1-pos_l0)]*(h1-h0)+h0
            for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
            {
                h0 = hh_freq_interp_ptr_0[k];           //第一列导频   
                h1 = hh_freq_interp_ptr_1[k];           //第二列导频
                delta_h_1 = _ssub2(h1, h0);                 
            

                mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_1),1), _sshl(_smpyhl(pos_coef, delta_h_1),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) 取高16位 -> Q(16,1)                
                hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h0);  //Q(16,3)
            }
          }
          if((symb_no > 4) && (symb_no < 7))  //5,6 
          {
            pos_coef = coef_k_3[symb_no - crs_symb_no_table_2port[1]];   //[(k-pos_l1)/(pos_l2-pos_l1)]*(h2-h1)+h1
            for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
            {
                h1 = hh_freq_interp_ptr_1[k];           //第二列导频   
                h2 = hh_freq_interp_ptr_2[k];           //第三列导频
                delta_h_2 = _ssub2(h2, h1);                 
            

                mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));//Q(16,1)*Q(16,1)<<1 -> Q(32,1) 取高16位 -> Q(16,1)
                hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1); //Q(16,3)
            }
          }
          if((symb_no > 7) && (symb_no < 14))  //8,9,10,12,13
          {
            if(11 < total_num)
            {
                pos_coef = coef_k_4[symb_no - crs_symb_no_table_2port[2]];   //[(k-pos_l2)/(pos_l3-pos_l2)]*(h3-h2)+h2
                for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
                {
                    h2 = hh_freq_interp_ptr_2[k];           //第三列导频   
                    h3 = hh_freq_interp_ptr_3[k];           //第四列导频
                    delta_h_3 = _ssub2(h3, h2);                 
                
                
                    mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_3),1), _sshl(_smpyhl(pos_coef, delta_h_3),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) 取高16位 -> Q(16,1)
                    hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h2);  //Q(16,3)
                }
            }
            else
            {
                pos_coef = coef_k_3_ext[(symb_no - crs_symb_no_table_2port[1]) - crs_symb_no_table_2port[1]];   //[(k-pos_l1)/(pos_l2-pos_l1)]*(h2-h1)+h1-> Q(16,3)
                for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
                {
                    h1 = hh_freq_interp_ptr_1[k];           //第二列导频   
                    h2 = hh_freq_interp_ptr_2[k];           //第三列导频
                    delta_h_2 = _ssub2(h2, h1);                 
                
                
                    mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_2),2), _sshl(_smpyhl(pos_coef, delta_h_2),2));//Q(16,3)*Q(16,1)<<3 -> Q(32,1) 取高16位 -> Q(16,1)
                    hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1); //Q(16,3)
                }
            }
          }
      }
    }

}
