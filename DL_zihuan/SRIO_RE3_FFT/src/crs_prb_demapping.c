/*
 * prb_demapping
 * 1,Ŀǰ�����������������prb
 * 2��ÿ�η���һ���˿ڵ�crs
 * 3����ֻ��ȡ���û���ռ��prb��crs��������ȡ����prb�ģ�---Ŀǰ�Ǽ������еģ��ŵ����ƺ��ŵ����Ʋ�ֵͬ
 */

#include <pl_comm.h>

#if 0
void crs_prb_demapping(signed int* restrict e_freq_data_ptr,   //In, Ƶ�������׵�ַ
               signed int  recv_ant_num,       //Para
               signed int  cell_id,        //Para,С��ID��0-503��
               signed int  port_no,        //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
               signed int* restrict e_freq_crs_ptr      //Out, ��Ƶ�����׵�ַ
          )
{
  signed int* restrict freq_data_ptr;  //���δ���ķ��ŵ��׵�ַ
  signed int* restrict freq_crs_ptr;   //���δ�����Ŷ�Ӧcrs��ŵ��׵�ַ
  signed int  symb_no_crs;  //�ڵ�Ƶ�����еķ������
  signed int  i, prb_no,pos_k1, pos_k2,symb_no;
  unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

  //CRS prb_demapping
  //��������

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
      //��ȡ
      
        //freq_data_ptr = (signed int*)&e_freq_data_ptr[i*SUBCARR_NUM_20M*SYMB_NUM_NORMCP_1MS + symb_no*SUBCARR_NUM_20M + prb_start*SUBCARR_NUM_PRB];
        //��i���������ߡ���symb_no�����ŵ�Ƶ�������׵�ַ
        freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + symb_no * N_SC_PER_RB * N_DL_RB];
        //��port_no���˿ڡ���i���������ߡ���symb_no_crs����Ƶ���ŵĵ�Ƶ�洢���׵�ַ
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

void crs_prb_demapping(signed int* restrict e_freq_data_ptr,   //In, Ƶ�������׵�ַ
               signed int  recv_ant_num,       //Para
               signed int  cell_id,        //Para,С��ID��0-503��
               signed int  port_no,        //Para,����demapping�ĵ�Ƶ�Ķ˿ںţ�Ŀǰ֧��0,1
               signed int* restrict e_freq_crs_ptr,      //Out, ��Ƶ�����׵�ַ
               unsigned char offset ,//crs����ʼλ��
               unsigned char crs_length//crs�ĳ���
          )
{
  signed int* restrict freq_data_ptr;  //���δ���ķ��ŵ��׵�ַ
  signed int* restrict freq_crs_ptr;   //���δ�����Ŷ�Ӧcrs��ŵ��׵�ַ
  signed int  symb_no_crs;  //�ڵ�Ƶ�����еķ������
  signed int  i, prb_no,pos_k1, pos_k2,symb_no;
  unsigned char crs_symb_no_table_2port[4] = {0,4,7,11};

  //CRS prb_demapping
  //��������

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
      //��ȡ
      
        //freq_data_ptr = (signed int*)&e_freq_data_ptr[i*SUBCARR_NUM_20M*SYMB_NUM_NORMCP_1MS + symb_no*SUBCARR_NUM_20M + prb_start*SUBCARR_NUM_PRB];
        //��i���������ߡ���symb_no�����ŵ�Ƶ�������׵�ַ
        freq_data_ptr = (signed int*)&e_freq_data_ptr[i * N_SC_PER_RB * N_DL_RB * 2 * N_SYM_PER_SLOT + symb_no * N_SC_PER_RB * N_DL_RB];
        //��port_no���˿ڡ���i���������ߡ���symb_no_crs����Ƶ���ŵĵ�Ƶ�洢���׵�ַ
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
