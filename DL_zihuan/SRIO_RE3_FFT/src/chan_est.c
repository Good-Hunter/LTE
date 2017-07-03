/*
 * chan_est
 * 1��ÿ�μ���һ���˿ڵ�Ƶ���ŵ�����
 * 2����ֻ������û���ռ��prb��crs�����Ǽ�������prb�ģ�[��demappingһ��������]
 * 3���ŵ����Ƶ�Ƶ��ʱ���ֵ��ר�ŵĺ����н���
  */

#include "pl_comm.h"
#if 0 
void chan_est(signed int* restrict e_freq_crs_ptr,   //In, ��Ƶ���������׵�ַ Q(16,1)
            unsigned char* restrict d_freq_crs_ptr,   //In, ���ص�Ƶ�����׵�ַ�����з�ʽͬe_freq_crs_ptr��Ϊ�˷���ʵ��e/d���˴�ֱ�Ӵ�d�Ĺ���
            signed int  recv_ant_num,         //Para
            signed int  prb_start,            //Para,***Ŀǰ�����������������prb***
            signed int  prb_num,              //Para
            signed int  port_no,          //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
            signed int  amp_ratio,      //Para,�ɵ�Ƶ����/���ݹ���������Ķ��߷��ȱȣ�Ϊ�˷���ʵ��e/ratio���˴�ֱ�Ӵ�����/��Ƶ���ȱȣ��Ը�����ʽ�洢����16bit��Ч��ʵ��������16bitΪ0 Q(16,3)
            signed int* restrict h_freq_crs_ptr        //Out, Ƶ���ŵ������׵�ַ����ֵǰ�������з�ʽͬe_freq_crs_ptrQ(16,1)
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
      //��port_no���˿ڡ���i���������ߵĵ�Ƶ���������׵�ַ
      ee_freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + symb_no_crs * CRS_SUBCARR_NUM_20M];

      //��port_no���˿ڡ���i���������ߵı��ص�Ƶ�����׵�ַ
      dd_freq_crs_ptr = (unsigned char*)&d_freq_crs_ptr[(symb_no_crs / (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 3 * 220
                                                    + (symb_no_crs % (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 2 * 220 + N_MAX_DL_RB - N_DL_RB];/*��Ƶ�ǰ���0,1,4�洢*/
      //��port_no���˿ڡ���i���������ߵ�Ƶ���ŵ������׵�ַ����ֵǰ��
      hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + symb_no_crs * CRS_SUBCARR_NUM_20M];

      //�ŵ����Ƽ��㣺h = e*(d*)*amp_ratio
      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
      {
        //��Ƶre1
        d_temp1 = rs_lut_conj[dd_freq_crs_ptr[prb_no * 2]];
        d_amp_temp1 = _cmpyr1(d_temp1, amp_ratio);    //(d*)*amp_ratio������ֱ���ó˷�����short�洢amp_ratio Q(16,1)*Q(16,3)=Q(16,3)
        e_temp1 = ee_freq_crs_ptr[prb_no * 2];
        h_temp1 = _cmpyr1(d_amp_temp1, e_temp1);     //e*d_amp Q(16,1)*Q(16,3)=Q(16,3)
        hh_freq_crs_ptr[prb_no * 2] = _shl2(h_temp1,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no * 2] = h_temp1<<2;//Q(16,3)<<2=Q(16,1)
        //��Ƶre2
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


void chan_est(signed int* restrict e_freq_crs_ptr,   //In, ��Ƶ���������׵�ַ Q(16,1)
            unsigned char* restrict d_freq_crs_ptr,   //In, ���ص�Ƶ�����׵�ַ�����з�ʽͬe_freq_crs_ptr��Ϊ�˷���ʵ��e/d���˴�ֱ�Ӵ�d�Ĺ���
            signed int  recv_ant_num,         //Para
            signed int  prb_start,            //Para,***Ŀǰ�����������������prb***
            signed int  prb_num,              //Para
            signed int  port_no,          //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
            signed int  amp_ratio,      //Para,�ɵ�Ƶ����/���ݹ���������Ķ��߷��ȱȣ�Ϊ�˷���ʵ��e/ratio���˴�ֱ�Ӵ�����/��Ƶ���ȱȣ��Ը�����ʽ�洢����16bit��Ч��ʵ��������16bitΪ0 Q(16,3)
            signed int* restrict h_freq_crs_ptr,        //Out, Ƶ���ŵ������׵�ַ����ֵǰ�������з�ʽͬe_freq_crs_ptrQ(16,1)
            unsigned char offset, //crs����ʼλ��
            unsigned char crs_length//crs�ĳ���
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
      //��port_no���˿ڡ���i���������ߵĵ�Ƶ���������׵�ַ
      ee_freq_crs_ptr = (signed int*)&e_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + (symb_no_crs + offset) * CRS_SUBCARR_NUM_20M];

      //��port_no���˿ڡ���i���������ߵı��ص�Ƶ�����׵�ַ
      dd_freq_crs_ptr = (unsigned char*)&d_freq_crs_ptr[((symb_no_crs + offset) / (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 3 * 220
                                                    + ((symb_no_crs + offset) % (CRS_SYMB_NUM_NORMCP_1MS >> 1)) * 2 * 220 + N_MAX_DL_RB - N_DL_RB];/*��Ƶ�ǰ���0,1,4�洢*/
      //��port_no���˿ڡ���i���������ߵ�Ƶ���ŵ������׵�ַ����ֵǰ��
      hh_freq_crs_ptr = (signed int*)&h_freq_crs_ptr[port_no * NUM_MAX_ANTENNA * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + i * CRS_SYMB_NUM_NORMCP_1MS * CRS_SUBCARR_NUM_20M
                                                     + (symb_no_crs + offset) * CRS_SUBCARR_NUM_20M];

      //�ŵ����Ƽ��㣺h = e*(d*)*amp_ratio
      for(prb_no = 0; prb_no < N_DL_RB; prb_no++)
      {
        //��Ƶre1
        d_temp1 = rs_lut_conj[dd_freq_crs_ptr[prb_no * 2]];
        d_amp_temp1 = _cmpyr1(d_temp1, amp_ratio);    //(d*)*amp_ratio������ֱ���ó˷�����short�洢amp_ratio Q(16,1)*Q(16,3)=Q(16,3)
        e_temp1 = ee_freq_crs_ptr[prb_no * 2];
        h_temp1 = _cmpyr1(d_amp_temp1, e_temp1);     //e*d_amp Q(16,1)*Q(16,3)=Q(16,3)
        hh_freq_crs_ptr[prb_no * 2] = _shl2(h_temp1,2);//Q(16,3)<<2=Q(16,1)
        //hh_freq_crs_ptr[prb_no * 2] = h_temp1<<2;//Q(16,3)<<2=Q(16,1)
        //��Ƶre2
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
