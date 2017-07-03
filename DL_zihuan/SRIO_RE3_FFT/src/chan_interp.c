/*
 * chan_interp
 * 1��ÿ�ν���һ���˿ڵ�Ƶ���ŵ����Ʋ�ֵ����Ƶ��+��ʱ��
 * 2����ֻ������û���ռ��prb��crs�����Ǽ�������prb�ģ�[��demappingһ��������]---��������ȫ����ģ����Ըĳɰ�Ƶ���ģ�Ŀǰprb���ڲ�
 * 3��ʱ���Ͻ������з��ŵĲ�ֵ---�ǿ��Կ��Ǹ�Ϊ�ֶλ��߰����Ž��е�
 * 4��Ƶ���ֵ�ĵ��ӿ��ܻ������������sadd2����
 * 5��ʱ���ֵʱcoef_k_4�д���1��������Ҫע���䴦����Q(16,1)��ʾ��һ������ѽ������
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
signed int   coef_k_4[7] = {0,          // 0��Ч��Q(16,2), packed signed short
              0x10001000,         //1/4
              0x20002000,     //2/4
              0x30003000,     //3/4
              0x40004000,     //4/4
              0x50005000,     //5/4
              0x60006000      //6/4
              };

signed int   coef_k_3[3] = {0,    // 0��Ч��Q(16,1), packed signed short
              0x2aaa2aaa,         //1/3
              0x55555555      //2/3
              };
signed int   coef_k_3_ext[3] = {// Q(16,3), packed signed short
              0x2aaa2aaa,    //4/3
              0x35553555,    //5/3 
              0x40004000,    //6/3   
              };
                                          
void chan_interp_freq(signed int* restrict h_freq_crs_ptr,        //In, Ƶ���ŵ������׵�ַ����ֵǰ�������з�ʽͬe_freq_crs_ptr Q(16,1)
               signed int  recv_ant_num,          //Para
               signed int  prb_start,             //Para,***Ŀǰ�����������������prb***
               signed int  prb_num,               //Para
               signed int  cell_id,         //Para,С��ID��0-503��
               signed int  port_no,         //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
               signed int* restrict h_freq_interp_ptr,      //Out, Ƶ���ŵ������׵�ַ����ֵ�󣩣����з�ʽͬe_freq_data_ptr��kaV�������
               unsigned char offset, //crs����ʼλ��
               unsigned char crs_length//crs�ĳ���
          )
{
    signed int  symb_no_crs;  //�ڵ�Ƶ�����еķ������
    signed int  symb_no;      //�ڷ����еķ������
    signed int  i, k, prb_no, pos_k1, pos_k2;  
    signed int* restrict hh_freq_crs_ptr;
    signed int* restrict hh_freq_interp_ptr;
    
    signed int    pos_coef, mpy_temp;
    signed int    delta_h_1, delta_h_2, delta_h_3, h1, h2, h3, h0;
    unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

    //��������---��ƵƵ��λ��
    for(i = 0; i < NUM_MAX_ANTENNA; i++)
    {
      //Ƶ�����Բ�ֵ,4����Ƶ���ţ�200->1200
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
          
          
          //��port_no���˿ڡ���i���������ߡ���symb_no_crs�е�Ƶ��Ƶ���ŵ������׵�ַ����ֵǰ��
          //�Ż����ɰѲ���λ�ü���Ų��ѭ����
          hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                         + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                         + (symb_no_crs + offset)* CRS_SUBCARR_NUM_20M];
          //��port_no���˿ڡ���i���������ߡ���symb_no�����ŵ�Ƶ���ŵ������׵�ַ����ֵ��
          hh_freq_interp_ptr = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                              + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                              + symb_no * N_SC_PER_RB * N_DL_RB];
                                                                      
          //��һ��prb
          h1 = hh_freq_crs_ptr[0];
          h2 = hh_freq_crs_ptr[1];
          h3 = hh_freq_crs_ptr[2];
          delta_h_1 = _ssub2(h1, h2);
          delta_h_2 = _ssub2(h2, h1);
          delta_h_3 = _ssub2(h3, h2);
          for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1;  �˴�Ϊ��ʵ�ַ��㣬���㷨�����Ķ�
          {
            pos_coef = coef_k_6[pos_k1 - k];    //[(k-pos_k1)/6]*(h2-h1)+h1 = [(pos_k1-k)/6]*(h1-h2)+h1
            mpy_temp = _packh2(_smpyh(pos_coef, delta_h_1), _smpyhl(pos_coef, delta_h_1));   //Q(16,1)*Q(16,1)<<1 -> Q(32,1) ȡ��16λ -> Q(16,1)
            hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1);       //Q(16,1) + Q(16,1)��������Ŀ���
          }
          for(k = pos_k1; k < pos_k2; k++)        //pos_k1 ~ pos_k2-1;  �˴�Ϊ��ʵ�ַ��㣬���㷨�����Ķ�
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
          
          //��2-99��prb
          //20121230���Ż�
          for(k = 0; k < pos_k1; k++)           //0 ~ pos_k1-1;
          {
              for(prb_no = 1; prb_no < (N_DL_RB - 1); prb_no++)
              {
                  h0 = hh_freq_crs_ptr[2*prb_no - 1];     //�ϸ�prb�ĵڶ�����Ƶre
                  h1 = hh_freq_crs_ptr[2*prb_no];         //��prb�ĵ�һ����Ƶre
                  //h2 = hh_freq_crs_ptr[2*prb_no + 1];     //��prb�ĵڶ�����Ƶre
                  //h3 = hh_freq_crs_ptr[2*prb_no + 2];     //�¸�prb�ĵ�һ����Ƶre
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
                  //h0 = hh_freq_crs_ptr[2*prb_no - 1];     //�ϸ�prb�ĵڶ�����Ƶre
                  h1 = hh_freq_crs_ptr[2*prb_no];         //��prb�ĵ�һ����Ƶre
                  h2 = hh_freq_crs_ptr[2*prb_no + 1];     //��prb�ĵڶ�����Ƶre
                  //h3 = hh_freq_crs_ptr[2*prb_no + 2];     //�¸�prb�ĵ�һ����Ƶre
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
                  //h0 = hh_freq_crs_ptr[2*prb_no - 1];     //�ϸ�prb�ĵڶ�����Ƶre
                  //h1 = hh_freq_crs_ptr[2*prb_no];         //��prb�ĵ�һ����Ƶre
                  h2 = hh_freq_crs_ptr[2*prb_no + 1];     //��prb�ĵڶ�����Ƶre
                  h3 = hh_freq_crs_ptr[2*prb_no + 2];     //�¸�prb�ĵ�һ����Ƶre
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
              h0 = hh_freq_crs_ptr[2*prb_no - 1];     //�ϸ�prb�ĵڶ�����Ƶre
              h1 = hh_freq_crs_ptr[2*prb_no];         //��prb�ĵ�һ����Ƶre
              h2 = hh_freq_crs_ptr[2*prb_no + 1];     //��prb�ĵڶ�����Ƶre
              h3 = hh_freq_crs_ptr[2*prb_no + 2];     //�¸�prb�ĵ�һ����Ƶre
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
          
          //��100��prb
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
            mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_2),1), _sshl(_smpyhl(pos_coef, delta_h_2),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) ȡ��16λ -> Q(16,1)
            hh_freq_interp_ptr[prb_no * N_SC_PER_RB + k] = _sadd2(mpy_temp, h1);
          }
      }
    }
}


void chan_interp_time(
		       signed int total_num,
		       signed int  port_no,         //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
               signed int  symb_start,
               signed int  symb_end,
               signed int* restrict h_freq_interp_ptr      //Out, Ƶ���ŵ������׵�ַ����ֵ�󣩣����з�ʽͬe_freq_data_ptr��kaV�������
               )
{
    signed int  symb_no;      //�ڷ����еķ������
    signed int  i, k;  
    signed int* restrict hh_freq_interp_ptr;
    signed int* restrict hh_freq_interp_ptr_0;
    signed int* restrict hh_freq_interp_ptr_1;
    signed int* restrict hh_freq_interp_ptr_2;
    signed int* restrict hh_freq_interp_ptr_3;
    
    signed int    pos_coef, mpy_temp;
    signed int    delta_h_1, delta_h_2, delta_h_3, h1, h2, h3, h0;
    unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

    //��������---��ƵƵ��λ��
    for(i = 0; i < NUM_MAX_ANTENNA; i++)
    {      
      //ʱ�����Բ�ֵ
      //��port_no���˿ڡ���i���������ߡ���0�����ţ���1�е�Ƶ����Ƶ���ŵ������׵�ַ����ֵ��                                                    
      hh_freq_interp_ptr_0 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 0 * N_SC_PER_RB * N_DL_RB];
      //��port_no���˿ڡ���i���������ߡ���4�����ţ���2�е�Ƶ����Ƶ���ŵ������׵�ַ����ֵ��                                                    
      hh_freq_interp_ptr_1 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 4 * N_SC_PER_RB * N_DL_RB];
      //��port_no���˿ڡ���i���������ߡ���7�����ţ���3�е�Ƶ����Ƶ���ŵ������׵�ַ����ֵ��                                                     
      hh_freq_interp_ptr_2 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 7 * N_SC_PER_RB * N_DL_RB];
      //��port_no���˿ڡ���i���������ߡ���11�����ţ���4�е�Ƶ����Ƶ���ŵ������׵�ַ����ֵ�� S��֡��һ���е�4�е�Ƶ                                                    
      hh_freq_interp_ptr_3 = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                          + 11 * N_SC_PER_RB * N_DL_RB];
      for(symb_no = symb_start; symb_no < symb_end; symb_no++)      //0-13
      {
          //��port_no���˿ڡ���i���������ߡ���symb_no�����ŵ�Ƶ���ŵ������׵�ַ����ֵ��
          hh_freq_interp_ptr = (signed int*)&h_freq_interp_ptr[port_no * NUM_MAX_ANTENNA * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                            + i * 2 * N_SYM_PER_SLOT * N_SC_PER_RB * N_DL_RB
                                                            + symb_no * N_SC_PER_RB * N_DL_RB];
                                                                                                                                                                                                                     
          if((symb_no > 0) && (symb_no < 4))  //1,2,3
          {
            pos_coef = coef_k_4[symb_no - crs_symb_no_table_2port[0]];   //[(k-pos_l0)/(pos_l1-pos_l0)]*(h1-h0)+h0
            for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
            {
                h0 = hh_freq_interp_ptr_0[k];           //��һ�е�Ƶ   
                h1 = hh_freq_interp_ptr_1[k];           //�ڶ��е�Ƶ
                delta_h_1 = _ssub2(h1, h0);                 
            

                mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_1),1), _sshl(_smpyhl(pos_coef, delta_h_1),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) ȡ��16λ -> Q(16,1)                
                hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h0);  //Q(16,3)
            }
          }
          if((symb_no > 4) && (symb_no < 7))  //5,6 
          {
            pos_coef = coef_k_3[symb_no - crs_symb_no_table_2port[1]];   //[(k-pos_l1)/(pos_l2-pos_l1)]*(h2-h1)+h1
            for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
            {
                h1 = hh_freq_interp_ptr_1[k];           //�ڶ��е�Ƶ   
                h2 = hh_freq_interp_ptr_2[k];           //�����е�Ƶ
                delta_h_2 = _ssub2(h2, h1);                 
            

                mpy_temp = _packh2(_smpyh(pos_coef, delta_h_2), _smpyhl(pos_coef, delta_h_2));//Q(16,1)*Q(16,1)<<1 -> Q(32,1) ȡ��16λ -> Q(16,1)
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
                    h2 = hh_freq_interp_ptr_2[k];           //�����е�Ƶ   
                    h3 = hh_freq_interp_ptr_3[k];           //�����е�Ƶ
                    delta_h_3 = _ssub2(h3, h2);                 
                
                
                    mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_3),1), _sshl(_smpyhl(pos_coef, delta_h_3),1));//Q(16,2)*Q(16,1)<<2 -> Q(32,1) ȡ��16λ -> Q(16,1)
                    hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h2);  //Q(16,3)
                }
            }
            else
            {
                pos_coef = coef_k_3_ext[(symb_no - crs_symb_no_table_2port[1]) - crs_symb_no_table_2port[1]];   //[(k-pos_l1)/(pos_l2-pos_l1)]*(h2-h1)+h1-> Q(16,3)
                for(k = 0; k < N_SC_PER_RB * N_DL_RB; k++)    //0-1199
                {
                    h1 = hh_freq_interp_ptr_1[k];           //�ڶ��е�Ƶ   
                    h2 = hh_freq_interp_ptr_2[k];           //�����е�Ƶ
                    delta_h_2 = _ssub2(h2, h1);                 
                
                
                    mpy_temp = _packh2(_sshl(_smpyh(pos_coef, delta_h_2),2), _sshl(_smpyhl(pos_coef, delta_h_2),2));//Q(16,3)*Q(16,1)<<3 -> Q(32,1) ȡ��16λ -> Q(16,1)
                    hh_freq_interp_ptr[k] = _sadd2(mpy_temp, h1); //Q(16,3)
                }
            }
          }
      }
    }

}
