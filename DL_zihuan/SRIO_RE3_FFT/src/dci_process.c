/***********************************************/
/*fcc_dci_data_distill.c                       */
/*���ܣ�PDCCH����������                     */
/***********************************************/
#include <system_init.h>


void fcc_dl_sch_vrb_prb_mapping(unsigned char vrb_num,
		unsigned char ngap_type,
		unsigned char mapping_type,
		unsigned char *vrb_index,
		unsigned char *prb_index_out[],
		unsigned char cell_bw_prb)
{
    unsigned int i;
    unsigned int ngap;
    unsigned int nvrb;
    unsigned int nrow;
    unsigned int nnull;
    unsigned int temp_nvrb;
    unsigned int nprb1pie;
    unsigned int nprb2pie;
    unsigned char *ts0_prb_index;
    unsigned char *ts1_prb_index;

    ts0_prb_index = prb_index_out[0];
    ts1_prb_index = prb_index_out[1];
    if(mapping_type == CXX_LOCALIZED_MAPPING_TYPE)//����ʽ��Դ����
    {
        for(i=0;i<vrb_num;i++)
        {
            ts0_prb_index[i] = vrb_index[i];
            ts1_prb_index[i] = vrb_index[i];
        }
    }
    else//�ֲ�ʽ��Դ����ο�Э��36.213-7.1.6.3//�˲��ֿ���
    {
        if(ngap_type == CCC_NGAP1_VALUE)
        {
            nvrb = 96;//N_~_DL_VRB
            ngap = 48;//N_gap = N_gap1
        }
        else
        {
            nvrb = 32;//N_~_DL_VRB
            ngap = 16;//N_gap = N_gap2
        }
        if(100 == cell_bw_prb)
            nrow = ((nvrb + 15) >> 4) << 2;  //lycan 15/08/04  211-6.2.3.2 nrow=cell(nvrb/(4*P)),where P=4
        else if(50 == cell_bw_prb)
        	nrow = ((nvrb/12)+1) *3;//nrow=cell(nvrb/(4*P))
        nnull = 4 * nrow - nvrb;
        for(i=0;i<vrb_num;i++)
        {
            temp_nvrb = vrb_index[i]%nvrb;//n_VRB mod N_~_DL_VRB ����n_~_VRB
            nprb1pie = 2 * nrow * (temp_nvrb & 1) + (temp_nvrb >> 1) + nvrb * (vrb_index[i]/nvrb);
            nprb2pie = nrow * (temp_nvrb & 3) + (temp_nvrb >> 2) + nvrb * (vrb_index[i]/nvrb);
            if((nnull != 0) && (temp_nvrb >= (nvrb - nnull)) && ((temp_nvrb & 1) == 1))
            {
                ts0_prb_index[i] = nprb1pie - nrow;
            }
            else if((nnull != 0) && (temp_nvrb >= (nvrb - nnull)) && ((temp_nvrb & 1) == 0))
            {
                ts0_prb_index[i] = nprb1pie - nrow + (nnull >> 1);
            }
            else if((nnull != 0) && (temp_nvrb < (nvrb - nnull)) && ((temp_nvrb & 3) >= 2))
            {
                ts0_prb_index[i] = nprb2pie - (nnull >> 1);
            }
            else
            {
                ts0_prb_index[i] = nprb2pie;
            }
            ts1_prb_index[i] = ((ts0_prb_index[i] + (nvrb >> 1)) % nvrb) + nvrb * (vrb_index[i] / nvrb);
            if(ts0_prb_index[i] < (nvrb >> 1))
            {
                ts0_prb_index[i] = ts0_prb_index[i];
            }
            else
            {
                ts0_prb_index[i] = ts0_prb_index[i] + ngap - (nvrb >> 1);
            }
            if(ts1_prb_index[i] < (nvrb >> 1))
            {
                ts1_prb_index[i] = ts1_prb_index[i];
            }
            else
            {
                ts1_prb_index[i] = ts1_prb_index[i] + ngap - (nvrb >> 1);
            }
        }

    }
}
//��λ�ν���DCI����
void fcc_dci_data_distill(unsigned int *data_in,
		unsigned int *data_out,
		unsigned char dci_format,
		unsigned char trans_mode,
		unsigned char subfn_index,
		CELL_PARA *cell_para_ptr,//ֻȡ���˴�����ȡ����CPģʽ������û��
		USER_PARA *user_para_ptr)
{
    unsigned char  l_crbs,rb_start;
    unsigned char  i;
    unsigned char  vrp_num = 0;
    unsigned char  rb_start_flag,riv_index;
    unsigned char  tb_index,mod_tbs_index,mod_tbs_index1;
    unsigned char  vrb_index[100];
    unsigned char  vrb_num, ngap_type,mapping_type;
    unsigned char  *prb_index_ptr[2];
    unsigned int   riv=0;
    unsigned int   resblock_temp = 0;
    union dci_data_format *dci_data_ptr;
    char cell_bw_prb = 0;//20150814 ly change bw

    cell_bw_prb = cell_para_ptr->num_PRB_per_bw_config;
    dci_data_ptr = (union dci_data_format *)data_out;
    if( 100 == cell_bw_prb )
    {
    /*���ݲ�ͬ��ʽ��ȡ��Ϣ*/
    switch(dci_format)
    {
        case Dci01A:/*format 0&1a*/
            {
              if(1 == data_in[0]>>31)//1a
                {
                    dci_data_ptr->dci_format_1a.format_01a_flag = data_in[0]>>31;/*1bit*/
                    dci_data_ptr->dci_format_1a.dis_tran_flag = _extu(data_in[0],1,31);/*1bit*/
                    if( CXX_LOCALIZED_MAPPING_TYPE == dci_data_ptr->dci_format_1a.dis_tran_flag)/*local*/
                    {
                      dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,19);/*n_rb_ul ����100ʱ��13bit*/
                    }
                    else
                    {
                      dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                      dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,20);/*n_rb_ul ����100ʱ��12bit*/
                    }
                    mod_tbs_index = _extu(data_in[0],15,27);/*5bit*/
                    dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],20,28);/*4bit*/
                    dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],24,31);/*1bit*/
                    dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],25,30);/*2bit*/
                    dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],27,30);/*2bit*/
                    dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],29,30);/*2bit*/
                    /*qm��tb_size*/
                    dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    /*layer��Ŀ*/
                    dci_data_ptr->dci_format_1a.layer_num =  2;
                    /*����vru�Ŀ�ʼλ�úͳ���*/
                    riv = dci_data_ptr->dci_format_1a.res_block;
                    rb_start = riv%CXX_PRB_MAX_NUM;
                    l_crbs = riv/CXX_PRB_MAX_NUM;
                    if(50 >= l_crbs)/*��ʼλ��С��50*/
                    {
                        dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                        dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                    }
                    else/*��ʼλ�ô���50*/
                    {
                        dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                        dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                    }
                    vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*���쳣���ý��б���*/
                    for(i=0;i<vrb_num;i++)
                    {
                        vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                    }
                    /*2015.1.12�޸� ������֡���ò�ͬ�Ĵ���ʽ*/
                    if(subfn_index ==1||subfn_index==6)
                    	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                    else
                    	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                    /*vrb��prb��ӳ��*/
                    ngap_type = dci_data_ptr->dci_format_1a.n_gap;
                    mapping_type = dci_data_ptr->dci_format_1a.dis_tran_flag;
                    prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts0_prb_index[0]);
                    prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts1_prb_index[0]);
                    fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                    break;
                }
                else//DCI0
                {
                  dci_data_ptr->dci_format_0.format_01a_flag = data_in[0]>>31;/*1bit*/
                    dci_data_ptr->dci_format_0.hopping_flag = _extu(data_in[0],1,31);/*1bit*/
                    if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*��Ƶʱ*/
                    {
                        dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,30);/*2bit*/
                        dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],4,21);/*n_rb_ul ����100ʱ��11bit*/
                    }
                    else/*����Ƶ*/
                    {
                       dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,19);/*n_rb_ul ����100ʱ��13bit*/
                    }
                    dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],15,27);/*5bit*/
                    dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],20,31);/*1bit*/
                    dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],21,30);/*2bit*/
                    dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],23,29);/*3bit*/
                    dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],26,30);/*2bit*/
                    dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],28,31);/*1bit*/
                    /*����vrb�Ŀ�ʼλ�úͳ���*/
                    riv = dci_data_ptr->dci_format_0.res_block;
                    rb_start = riv%CXX_NRB_UL_NUM;
                    l_crbs = riv/CXX_NRB_UL_NUM;
                    if(50 >= rb_start)/*��ʼλ��С��50*/
                    {
                        dci_data_ptr->dci_format_0.vrb_start = rb_start;
                        dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                    }
                    else/*��ʼλ�ô���50*/
                    {
                        dci_data_ptr->dci_format_0.vrb_start = CXX_NRB_UL_NUM - 1 - rb_start;
                        dci_data_ptr->dci_format_0.vrb_length = (CXX_NRB_UL_NUM + 1 - l_crbs)%51;
                    }
                    break;
                    }
            }
        case Dci1:/*format 1*/
            {
                dci_data_ptr->dci_format_1.res_alloc_type = data_in[0]>>31;/*1bit*/
                if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*type1*/
                {
                    dci_data_ptr->dci_format_1.res_blocks_subset = _extu(data_in[0],1,30);/*n_rb_dl 100 2bit*/
                    dci_data_ptr->dci_format_1.shift_res_span = _extu(data_in[0],3,31) ;/*1bit*/
                    resblock_temp = _bitr(data_in[0]);/*��תbitmap*///_bitr?
                    dci_data_ptr->dci_format_1.res_block = _extu(resblock_temp,6,10);/*n_rb_dl ����100ʱ��22bit*/
                }
                else/*type0*/
                {
                   resblock_temp = _bitr(data_in[0]);//_bitr?
                   dci_data_ptr->dci_format_1.res_block = _extu(resblock_temp,6,7);/*n_rb_dl ����100ʱ��25bit*/
                }
                mod_tbs_index = _extu(data_in[0],26,27);/*5bit*/
                dci_data_ptr->dci_format_1.harq_pro_num = _extu(data_in[0],31,28)^(data_in[1]>>29);/*4bit*/
                dci_data_ptr->dci_format_1.ndi = _extu(data_in[1],3,31);/*1bit*/
                dci_data_ptr->dci_format_1.rv = _extu(data_in[1],4,30);/*2bit*/
                dci_data_ptr->dci_format_1.tpc_pucch = _extu(data_in[1],6,30);/*2bit*/
                dci_data_ptr->dci_format_1.down_assign_index = _extu(data_in[1],8,30);/*2bit*/
                 /*qm��tb_size*/
                dci_data_ptr->dci_format_1.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                /*layer��Ŀ*/
                dci_data_ptr->dci_format_1.layer_num =  1;
                /*����vrb�Ŀ�ʼλ�úͳ���*/
                riv = dci_data_ptr->dci_format_1.res_block;
                l_crbs = 0;
                rb_start = 0;
                rb_start_flag = 0;
                riv_index = 0;
                if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*�޸�type1����Ϊ850*/
                {
                    if( 0 == dci_data_ptr->dci_format_1.shift_res_span)/* ����Ҫ����shift */
                    {
                        for(i=0;i<22;i++)
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                            {
                                rb_start = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + (i & 3);
                                rb_start_flag = 1;
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_1.ts0_prb_index[l_crbs] = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + (i & 3);
                                dci_data_ptr->dci_format_1.ts1_prb_index[l_crbs] = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + (i & 3);
                                l_crbs = l_crbs + 1;
                            }
                        }
                    }
                    else/* ��Ҫ����shift */
                    {
                        if(0 == dci_data_ptr->dci_format_1.res_blocks_subset)
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 16 * ((6+i)>>2) + ((i+6) & 3);
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_1.ts0_prb_index[l_crbs] = 16 * ((6+i)>>2) + ((i+6) & 3);
                                    dci_data_ptr->dci_format_1.ts1_prb_index[l_crbs] = 16 * ((6+i)>>2) + ((i+6) & 3);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                        else
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + ((i+2) & 3);
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_1.ts0_prb_index[l_crbs] = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + ((i+2) & 3);
                                    dci_data_ptr->dci_format_1.ts1_prb_index[l_crbs] = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_1.res_blocks_subset) + ((i+2) & 3);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                    }
                }
                else/*type0*/
                {
                    for(i=0;i<25;i++)
                    {
                        riv_index = (riv>>i)&0x1;
                        if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                        {
                            rb_start = i*4;
                            rb_start_flag = 1;
                        }
                        if(1 == riv_index)
                        {
                            dci_data_ptr->dci_format_1.ts0_prb_index[(4*l_crbs)] = 4*i;
                            dci_data_ptr->dci_format_1.ts0_prb_index[(4*l_crbs + 1)] = 4*i + 1;
                            dci_data_ptr->dci_format_1.ts0_prb_index[(4*l_crbs + 2)] = 4*i + 2;
                            dci_data_ptr->dci_format_1.ts0_prb_index[(4*l_crbs + 3)] = 4*i + 3;
                            dci_data_ptr->dci_format_1.ts1_prb_index[(4*l_crbs)] = 4*i;
                            dci_data_ptr->dci_format_1.ts1_prb_index[(4*l_crbs + 1)] = 4*i + 1;
                            dci_data_ptr->dci_format_1.ts1_prb_index[(4*l_crbs + 2)] = 4*i + 2;
                            dci_data_ptr->dci_format_1.ts1_prb_index[(4*l_crbs + 3)] = 4*i + 3;
                            l_crbs = l_crbs + 1;
                        }
                    }
                    l_crbs = (l_crbs*4);
                }
                vrp_num = rb_start + l_crbs;
                if(vrp_num>100)/*������ֲ����쳣��CC��vrb�ĳ����趨Ϊ0*/
                {
                    rb_start = 0;
                    l_crbs = 1;
                }
                dci_data_ptr->dci_format_1.vrb_start = rb_start;
                dci_data_ptr->dci_format_1.vrb_length = l_crbs;
                /*tb_size*/
                dci_data_ptr->dci_format_1.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                break;
            }
        case Dci1C:/*format 1c*/
            {
              dci_data_ptr->dci_format_1c.gap_type = data_in[0]>>31;/*1bit*/  //cs 15/08/04 indicate the type of gap, Ngap= Ngap,1 or Ngap,2
              if(dci_data_ptr->dci_format_1c.gap_type==0)
              {
                    dci_data_ptr->dci_format_1c.res_block =_extu(data_in[0],1,23);/*9bit*/ //cs 15/08/04  resorce block allocation
                    //l_crbs = dci_data_ptr->dci_format_1c.res_block;
                    tb_index = _extu(data_in[0],10,27);/*5bit*/   //cs 15/08/04  TBS INDEX 213-7.1.7.2.3
                    dci_data_ptr->dci_format_1c.rv = 0;/*2bit*/
                    /*layer��Ŀ*/
                    dci_data_ptr->dci_format_1c.layer_num =  1;
                    /*qm��tb_size*/
                    dci_data_ptr->dci_format_1c.qm = 2;

                    dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*��Ҫ�޸�*/



                    /*����vrb�Ŀ�ʼλ�úͳ���*/
                    riv = dci_data_ptr->dci_format_1c.res_block;
                    rb_start = riv%CXX_PRB_MAX_NUM;
                    rb_start = rb_start<<2;
                    l_crbs = riv/CXX_PRB_MAX_NUM;
                    l_crbs = l_crbs<<2;
                    if(50 >= l_crbs)/*��ʼλ��С��50*/
                    {
                        dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                        dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                    }
                    else/*��ʼλ�ô���50*/
                    {
                        dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                        dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                    }
                    vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*���쳣���ý��б���*/
                    for(i=0;i<vrb_num;i++)
                    {
                        vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                    }

                    /*vrb��prb��ӳ��*/
                    ngap_type = 0; //cs 15/08/04 0 means
                    mapping_type = 1;  //cs 15/08/04 0 means localized and 1 means distributed
                    prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts0_prb_index[0]);
                    prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts1_prb_index[0]);
                    fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                }

                break;
            }
        case Dci2:/*format 2*/
        case Dci2A:/*format 2A*/
            {
                dci_data_ptr->dci_format_2.res_alloc_type = data_in[0]>>31;/*1bit*/
                if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*type1*/
                {
                    dci_data_ptr->dci_format_2.res_blocks_subset = _extu(data_in[0],1,30);/*n_rb_dl 100 2bit*/
                    dci_data_ptr->dci_format_2.shift_res_span = _extu(data_in[0],3,31) ;/*1bit*/
                    resblock_temp = _bitr(data_in[0]);
                    dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,6,10);/*n_rb_dl ����100ʱ��22bit*/
                }
                else/*type0*/
                {
                   resblock_temp = _bitr(data_in[0]);
                   dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,6,7);/*n_rb_dl ����100ʱ��25bit*/
                }
                dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],26,30);/*2bit*/
                dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],28,30);/*2bit*/
                dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],30,28)^(data_in[1]>>30);/*4bit*/
                dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[1],2,31);/*1bit*/
                mod_tbs_index = _extu(data_in[1],3,27);/*5bit*/
                dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[1],3,27);/*5bit*/
                dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[1],8,31);/*1bit*/
                dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[1],9,30);/*2bit*/
                //codeword_enable�ж�
                if((0 == dci_data_ptr->dci_format_2.mod_tbs_index) && (1 == dci_data_ptr->dci_format_2.codeword0_rv))
                {
                	dci_data_ptr->dci_format_2.codeword0_enable_flag = 0;
                }
                else
                {
                	dci_data_ptr->dci_format_2.codeword0_enable_flag = 1;
                }
                mod_tbs_index1 = _extu(data_in[1],11,27);/*5bit*/
                dci_data_ptr->dci_format_2.codeword1_ndi = _extu(data_in[1],16,31);/*1bit*/
                dci_data_ptr->dci_format_2.codeword1_rv = _extu(data_in[1],17,30);/*2bit*/
                //codeword_enable�ж�
                if((0 == mod_tbs_index1) && (1 == dci_data_ptr->dci_format_2.codeword1_rv))
                {
                	dci_data_ptr->dci_format_2.codeword1_enable_flag = 0;
                }
                else
                {
                	dci_data_ptr->dci_format_2.codeword1_enable_flag = 1;
                }
                if(dci_format == Dci2A)//tm3
                {
                    if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))
                    {
                        dci_data_ptr->dci_format_2.layer_num = 1;//�����֣�����ּ�
                    }
                    else
                    {
                        dci_data_ptr->dci_format_2.layer_num = 2;//�����֣��շ�
                    }
                }
                else//tm4
                {
                	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],19,29);/*3bit*/
                	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//������
                	{
                		if(0 == dci_data_ptr->dci_format_2.precoding_index)
                		{
                			dci_data_ptr->dci_format_2.layer_num = 2;
                		}
                		else
                		{
                			dci_data_ptr->dci_format_2.layer_num = 1;
                		}
                	}
                	else//������
                	{
                		dci_data_ptr->dci_format_2.layer_num = 2;
                	}
                }
                /*����vru�Ŀ�ʼλ�úͳ���*/
                riv = dci_data_ptr->dci_format_2.res_block;
                l_crbs = 0;
                rb_start = 0;
                rb_start_flag = 0;
                riv_index = 0;
                if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*�޸�type1����Ϊ850*/
                {
                    if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* ����Ҫ����shift */
                    {
                        for(i=0;i<22;i++)
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                            {
                                rb_start = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i & 3);
                                rb_start_flag = 1;
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i & 3);
                                dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 16 * (i>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i & 3);
                                l_crbs = l_crbs + 1;
                            }
                        }
                    }
                    else/* ��Ҫ����shift */
                    {
                        if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//(99/4)%4 = 0����ֻ�����������û��С�ڵ�
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 16 * ((6+i)>>2) + ((i+6) & 3);//��ʱshift�̶�Ϊ6��������ֱ�Ӽ���ģ�ʡ�����м��ѡ��������
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 16 * ((6+i)>>2) + ((i+6) & 3);
                                    dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 16 * ((6+i)>>2) + ((i+6) & 3);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                        else
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + ((i+2) & 3);
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + ((i+2) & 3);
                                    dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 16 * ((2+i)>>2) + 4 * (dci_data_ptr->dci_format_2.res_blocks_subset) + ((i+2) & 3);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                    }
                }
                else/*type0*/
                {
                    for(i=0;i<25;i++)
                    {
                        riv_index = (riv>>i)&0x1;
                        if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                        {
                            rb_start = i*4;
                            rb_start_flag = 1;
                        }
                        if(1 == riv_index)
                        {
                            dci_data_ptr->dci_format_2.ts0_prb_index[(4*l_crbs)] = 4*i;
                            dci_data_ptr->dci_format_2.ts0_prb_index[(4*l_crbs + 1)] = 4*i + 1;
                            dci_data_ptr->dci_format_2.ts0_prb_index[(4*l_crbs + 2)] = 4*i + 2;
                            dci_data_ptr->dci_format_2.ts0_prb_index[(4*l_crbs + 3)] = 4*i + 3;
                            dci_data_ptr->dci_format_2.ts1_prb_index[(4*l_crbs)] = 4*i;
                            dci_data_ptr->dci_format_2.ts1_prb_index[(4*l_crbs + 1)] = 4*i + 1;
                            dci_data_ptr->dci_format_2.ts1_prb_index[(4*l_crbs + 2)] = 4*i + 2;
                            dci_data_ptr->dci_format_2.ts1_prb_index[(4*l_crbs + 3)] = 4*i + 3;
                            l_crbs = l_crbs + 1;
                        }
                    }
                    l_crbs = (l_crbs*4);
                }
                vrp_num = rb_start + l_crbs;
                if(vrp_num>100)/*������ֲ����쳣��CC��vrb�ĳ����趨Ϊ0*/
                {
                    rb_start = 0;
                    l_crbs = 1;
                }
                dci_data_ptr->dci_format_2.vrb_start = rb_start;
                dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                 /*��1��codeword ��qm��tb_size*/
                dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                /*��2��codeword ��qm��tb_size*/
                dci_data_ptr->dci_format_2.codeword1_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 2];
                dci_data_ptr->dci_format_2.codeword1_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                
                break;
            }
        case Dci3:/*format 3*/
            {
                dci_data_ptr->dci_format_3.tpc_user = data_in[0]>>4;/*28bit,2bit��ʾһ���û�*/
                break;
            }
        case Dci3A:/*format 3a*/
            {
                dci_data_ptr->dci_format_3a.tpc_user = data_in[0]>>4;/*28bit��1bit��ʾһ���û�*/
                break;
            }
         default:
            break;
    }
    }
    //20150814 lycan 10M bandwidth
    if(50 == cell_bw_prb )
    {

        /*���ݲ�ͬ��ʽ��ȡ��Ϣ*/
        switch(dci_format)
        {
            case Dci01A:/*format 0&1a*/
                {
                  if(1 == data_in[0]>>31)//1a
                    {
                	    //ly �����øģ���ȡ��ԭ��һ��
                        dci_data_ptr->dci_format_1a.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_1a.dis_tran_flag = _extu(data_in[0],1,31);/*1bit*/
                        //ly �Ѿ�����
                        if( CXX_LOCALIZED_MAPPING_TYPE == dci_data_ptr->dci_format_1a.dis_tran_flag)/*local*/
                        {
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,21);/*n_rb_dl ����50ʱ��11bit*/
                        }
                        else
                        {
                          dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,22);/*n_rb_dl ����50ʱ��10bit*/
                        }
                        // ly�Ѿ�����
                        mod_tbs_index = _extu(data_in[0],13,27);/*5bit*/
                        dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],18,28);/*4bit*/
                        dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],22,31);/*1bit*/
                        dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],23,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],25,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],27,30);/*2bit*/

                        /*qm��tb_size*/
                        dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                        tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                        /*layer��Ŀ*/
                        dci_data_ptr->dci_format_1a.layer_num =  2;
                        /*����vru�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_1a.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;//ly CXX_PRB_MAX_NUM��Ҫ��
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        if(50 >= l_crbs)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*���쳣���ý��б���*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                        }
                        /*2015.1.12�޸� ������֡���ò�ͬ�Ĵ���ʽ*/
                        if(subfn_index ==1||subfn_index==6)
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                        else
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                        /*vrb��prb��ӳ��*/
                        ngap_type = dci_data_ptr->dci_format_1a.n_gap;
                        mapping_type = dci_data_ptr->dci_format_1a.dis_tran_flag;
                        prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts0_prb_index[0]);
                        prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts1_prb_index[0]);
                        fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                        break;
                    }
                    else//DCI0
                    {
                      dci_data_ptr->dci_format_0.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_0.hopping_flag = _extu(data_in[0],1,31);/*1bit*/
                        if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*��Ƶʱ*/
                        {
                            dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,30);/*2bit*/
                            dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],4,23);/*n_rb_dl ����50ʱ��9bit*/
                        }
                        else/*����Ƶ*/
                        {
                           dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,21);/*n_rb_dl ����50ʱ��11bit*/
                        }
                        dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],13,27);/*5bit*/
                        dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],18,31);/*1bit*/
                        dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],19,30);/*2bit*/
                        dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],21,29);/*3bit*/
                        dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],24,30);/*2bit*///(ֻ����������������Ϊ0�Ĳ����У�ĿǰΪ2Ӧ���޴��ֶ�)
                        //lycan �������з�����ţ�DAI��2����(ֻ����������������Ϊ1 - 6 �Ĳ����У�ĿǰΪ2��Ӧ���д��ֶ�)
                        dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],26,31);/*1bit*/
                        /*����vrb�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_0.res_block;
                        rb_start = riv%CXX_NRB_UL_NUM;
                        l_crbs = riv/CXX_NRB_UL_NUM;
                        if(50 >= rb_start)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = CXX_NRB_UL_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (CXX_NRB_UL_NUM + 1 - l_crbs)%51;
                        }
                        break;
                        }
                }

            case Dci1C:/*format 1c*/
                {
                  dci_data_ptr->dci_format_1c.gap_type = data_in[0]>>31;/*1bit*/  //lycan 20150825 indicate the type of gap, Ngap= Ngap,1 or Ngap,2
                  if(dci_data_ptr->dci_format_1c.gap_type==0)
                  {
                        dci_data_ptr->dci_format_1c.res_block =_extu(data_in[0],1,25);/*7bit*/ //lycan 20150825  resorce block allocation
                        //l_crbs = dci_data_ptr->dci_format_1c.res_block;
                        tb_index = _extu(data_in[0],8,27);/*5bit*/   //lycan 20150825 TBS INDEX 213-7.1.7.2.3
                        dci_data_ptr->dci_format_1c.rv = 0;/*2bit*/
                        /*layer��Ŀ*/
                        dci_data_ptr->dci_format_1c.layer_num =  1;
                        /*qm��tb_size*/
                        dci_data_ptr->dci_format_1c.qm = 2;

                        dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*��Ҫ�ӱ�lycan 20150825*/



                        /*����vrb�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_1c.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;
                        rb_start = rb_start<<2;//4re
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        l_crbs = l_crbs<<2;
                        if(50 >= l_crbs)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*���쳣���ý��б���*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                        }

                        /*vrb��prb��ӳ��*/
                        ngap_type = 0; //cs 15/08/04 0 means ngap_type0
                        mapping_type = 1;  //cs 15/08/04 0 means localized and 1 means distributed
                        prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts0_prb_index[0]);
                        prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts1_prb_index[0]);
                        fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                    }

                    break;
                }
            case Dci2:/*format 2*/
            case Dci2A:/*format 2A*/
                {
                    dci_data_ptr->dci_format_2.res_alloc_type = data_in[0]>>31;/*1bit*/
                    if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*type1*/
                    {
                        dci_data_ptr->dci_format_2.res_blocks_subset = _extu(data_in[0],1,30);/*n_rb_dl 50 2bit*/
                        dci_data_ptr->dci_format_2.shift_res_span = _extu(data_in[0],3,31) ;/*1bit*/
                        resblock_temp = _bitr(data_in[0]);//ΪʲôҪ��תһ�£�
                        dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,14,18);/*n_rb_dl ����50ʱ��14bit��ע���Ƿ�ת�����λ��ȡ�м�14����*/
                    }
                    else/*type0*/
                    {
                       resblock_temp = _bitr(data_in[0]);
                       dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,14,15);/*n_rb_dl ����50ʱ��17bitע���Ƿ�ת�����λ��ȡ�м�14����*/
                    }
                    dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],18,30);/*2bit*/
                    dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],20,30);/*2bit*///ֻ������TDD����������Ϊ1-6���������FDD�в�����
                    dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],22,28);/*4bit*/
                    dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[0],26,31);/*1bit*/
                    mod_tbs_index = _extu(data_in[0],27,27);/*5bit*/
                    dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[0],27,27);/*5bit*/
                    dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[1],0,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[1],1,30);/*2bit*/
                    //codeword_enable�ж�
                    if((0 == dci_data_ptr->dci_format_2.mod_tbs_index) && (1 == dci_data_ptr->dci_format_2.codeword0_rv))
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 0;
                    }
                    else
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 1;
                    }
                    mod_tbs_index1 = _extu(data_in[1],3,27);/*5bit*/
                    dci_data_ptr->dci_format_2.codeword1_ndi = _extu(data_in[1],8,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword1_rv = _extu(data_in[1],9,30);/*2bit*/
                    //codeword_enable�ж�
                    if((0 == mod_tbs_index1) && (1 == dci_data_ptr->dci_format_2.codeword1_rv))
                    {
                    	dci_data_ptr->dci_format_2.codeword1_enable_flag = 0;
                    }
                    else
                    {
                    	dci_data_ptr->dci_format_2.codeword1_enable_flag = 1;
                    }
                    if(dci_format == Dci2A)//tm3
                    {
                        if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))
                        {
                            dci_data_ptr->dci_format_2.layer_num = 1;//�����֣�����ּ�
                        }
                        else
                        {
                            dci_data_ptr->dci_format_2.layer_num = 2;//�����֣��շ�
                        }
                    }
                    else//tm4
                    {
                    	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],11,29);/*3bit*/
                    	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//������
                    	{
                    		if(0 == dci_data_ptr->dci_format_2.precoding_index)
                    		{
                    			dci_data_ptr->dci_format_2.layer_num = 2;
                    		}
                    		else
                    		{
                    			dci_data_ptr->dci_format_2.layer_num = 1;
                    		}
                    	}
                    	else//������
                    	{
                    		dci_data_ptr->dci_format_2.layer_num = 2;
                    	}
                    }
                    /*����vru�Ŀ�ʼλ�úͳ���*/
                    riv = dci_data_ptr->dci_format_2.res_block;
                    l_crbs = 0;
                    rb_start = 0;
                    rb_start_flag = 0;
                    riv_index = 0;
                    if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*�޸�type1����Ϊ850*/
                    {
                        if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* ����Ҫ����shift */
                        {
                            for(i=0;i<14;i++)//10M 14
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 9 * (i/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 3);
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 9 * (i/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 3);
                                    dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 9 * (i/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 3);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                        else/* ��Ҫ����shift */
                        {
                            if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//p < (((N_DL_RB - 1)/P)%P)
                            {
                                for(i=0;i<14;i++)//10M 14
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                    {
                                        rb_start = 9 * ((4+i)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+4) % 3);//��n_rbgsubset_prb(p)����ʱshift�̶�Ϊ4��������ֱ�Ӽ���ģ�ʡ�����м��ѡ��������
                                        rb_start_flag = 1;
                                    }
                                    if(1 == riv_index)
                                    {
                                        dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 9 * ((4+i)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+4) % 3);
                                        dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 9 * ((4+i)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+4) % 3);
                                        l_crbs = l_crbs + 1;
                                    }
                                }
                            }
                            if(1 == dci_data_ptr->dci_format_2.res_blocks_subset)//p == (((N_DL_RB - 1)/P)%P)
                            {
                                for(i=0;i<14;i++)//10M 14
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                    {
                                        rb_start = 9 * ((i + 3)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+3) % 3);//��n_rbgsubset_prb(p)����ʱshift�̶�Ϊ3
                                        rb_start_flag = 1;
                                    }
                                    if(1 == riv_index)
                                    {
                                        dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 9 * ((i + 3)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+3) % 3);
                                        dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 9 * ((i + 3)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+3) % 3);
                                        l_crbs = l_crbs + 1;
                                    }
                                }
                            }
                            if(2 == dci_data_ptr->dci_format_2.res_blocks_subset)//p > (((N_DL_RB - 1)/P)%P)
                            {
                                for(i=0;i<14;i++)//10M 14
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                    {
                                        rb_start = 9 * ((i + 1)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 3);//��n_rbgsubset_prb(p)����ʱshift�̶�Ϊ1
                                        rb_start_flag = 1;
                                    }
                                    if(1 == riv_index)
                                    {
                                        dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 9 * ((i + 1)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 3);
                                        dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 9 * ((i + 1)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 3);
                                        l_crbs = l_crbs + 1;
                                    }
                                }
                            }
                        }
                    }
                    else/*type0*/
                    {
                        for(i=0;i<25;i++)//25Ϊ20M�������10MʱΪ17
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                            {
                                rb_start = i*3;// P = 4,����Ϊ3��Ҫ��
                                rb_start_flag = 1;// �������ֵ�һ�������RBG,����RB��ʼ���
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs)] = 3*i;// P = 4,����Ϊ3��Ҫ��
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs + 1)] = 3*i + 1;
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs + 2)] = 3*i + 2;

                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs)] = 3*i;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs + 1)] = 3*i + 1;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs + 2)] = 3*i + 2;

                                l_crbs = l_crbs + 1;
                            }
                        }
                        l_crbs = (l_crbs*3);// P = 4,����Ϊ3��Ҫ��
                    }
                    vrp_num = rb_start + l_crbs;
                    if(vrp_num>100)/*������ֲ����쳣��CC��vrb�ĳ����趨Ϊ0*/
                    {
                        rb_start = 0;
                        l_crbs = 1;
                    }
                    dci_data_ptr->dci_format_2.vrb_start = rb_start;
                    dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                     /*��1��codeword ��qm��tb_size*/
                    dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                    /*��2��codeword ��qm��tb_size*/
                    dci_data_ptr->dci_format_2.codeword1_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 2];
                    dci_data_ptr->dci_format_2.codeword1_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];

                    break;
                }

             default:
                break;
        }

    }
    //lycan 20150827 5m
    if(25 == cell_bw_prb )
    {
        /*���ݲ�ͬ��ʽ��ȡ��Ϣ*/
        switch(dci_format)
        {
            case Dci01A:/*format 0&1a*/
                {
                  if(1 == data_in[0]>>31)//1a
                    {
                	    //ly �����øģ���ȡ��ԭ��һ��
                        dci_data_ptr->dci_format_1a.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_1a.dis_tran_flag = _extu(data_in[0],1,31);/*1bit*/
                        //ly �Ѿ�����
                        if( CXX_LOCALIZED_MAPPING_TYPE == dci_data_ptr->dci_format_1a.dis_tran_flag)/*local*/
                        {
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,23);/*n_rb_dl ����25ʱ��9bit*/
                        }
                        else
                        {
                          dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,24);/*n_rb_dl ����25ʱ��8bit*/
                        }
                        // ly 0827 �Ѿ�����
                        mod_tbs_index = _extu(data_in[0],11,27);/*5bit*/
                        dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],16,28);/*4bit*/
                        dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],20,31);/*1bit*/
                        dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],21,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],23,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],25,30);/*2bit*/

                        /*qm��tb_size*/
                        dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                        tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                        /*layer��Ŀ*/
                        dci_data_ptr->dci_format_1a.layer_num =  2;
                        /*����vru�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_1a.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;//ly CXX_PRB_MAX_NUM��Ҫ��
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        if(50 >= l_crbs)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*���쳣���ý��б���*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                        }
                        /*2015.1.12�޸� ������֡���ò�ͬ�Ĵ���ʽ*/
                        if(subfn_index ==1||subfn_index==6)
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                        else
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                        /*vrb��prb��ӳ��*/
                        ngap_type = dci_data_ptr->dci_format_1a.n_gap;
                        mapping_type = dci_data_ptr->dci_format_1a.dis_tran_flag;
                        prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts0_prb_index[0]);
                        prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1a.ts1_prb_index[0]);
                        fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                        break;
                    }
                    else//DCI0
                    {
                      dci_data_ptr->dci_format_0.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_0.hopping_flag = _extu(data_in[0],1,31);/*1bit*/
                        if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*��Ƶʱ*/
                        {
                            dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,31);/*1bit*///lycan 0827
                            dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],3,25);/*n_rb_dl ����25ʱ��8bit*/
                        }
                        else/*����Ƶ*/
                        {
                           dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,23);/*n_rb_dl ����25ʱ��9bit*/
                        }
                        dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],11,27);/*5bit*/
                        dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],16,31);/*1bit*/
                        dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],17,30);/*2bit*/
                        dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],19,29);/*3bit*/
                        dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],22,30);/*2bit*///(ֻ����������������Ϊ0�Ĳ����У�ĿǰΪ2Ӧ���޴��ֶ�)
                        //lycan �������з�����ţ�DAI��2����(ֻ����������������Ϊ1 - 6 �Ĳ����У�ĿǰΪ2��Ӧ���д��ֶ�)
                        dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],24,31);/*1bit*/
                        /*����vrb�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_0.res_block;
                        rb_start = riv%CXX_NRB_UL_NUM;
                        l_crbs = riv/CXX_NRB_UL_NUM;
                        if(50 >= rb_start)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = CXX_NRB_UL_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (CXX_NRB_UL_NUM + 1 - l_crbs)%51;
                        }
                        break;
                        }
                }

            case Dci1C:/*format 1c*///5m is same to 10m lycan 20150827
                {
                  dci_data_ptr->dci_format_1c.gap_type = data_in[0]>>31;/*1bit*/  //lycan 20150827 indicate the type of gap, Ngap= Ngap,1 or Ngap,2
                  if(dci_data_ptr->dci_format_1c.gap_type==0)
                  {
                        dci_data_ptr->dci_format_1c.res_block =_extu(data_in[0],1,25);/*7bit*/ //lycan 20150825  resorce block allocation
                        //l_crbs = dci_data_ptr->dci_format_1c.res_block;
                        tb_index = _extu(data_in[0],8,27);/*5bit*/   //lycan 20150825 TBS INDEX 213-7.1.7.2.3
                        dci_data_ptr->dci_format_1c.rv = 0;/*2bit*/
                        /*layer��Ŀ*/
                        dci_data_ptr->dci_format_1c.layer_num =  1;
                        /*qm��tb_size*/
                        dci_data_ptr->dci_format_1c.qm = 2;

                        dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*��Ҫ�ӱ�lycan 20150825*/



                        /*����vrb�Ŀ�ʼλ�úͳ���*/
                        riv = dci_data_ptr->dci_format_1c.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;
                        rb_start = rb_start<<2;//4re
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        l_crbs = l_crbs<<2;
                        if(50 >= l_crbs)/*��ʼλ��С��50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                        }
                        else/*��ʼλ�ô���50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*���쳣���ý��б���*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                        }

                        /*vrb��prb��ӳ��*/
                        ngap_type = 0; //cs 15/08/04 0 means ngap_type0
                        mapping_type = 1;  //cs 15/08/04 0 means localized and 1 means distributed
                        prb_index_ptr[0] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts0_prb_index[0]);
                        prb_index_ptr[1] = (unsigned char *)&(dci_data_ptr->dci_format_1c.ts1_prb_index[0]);
                        fcc_dl_sch_vrb_prb_mapping(vrb_num,ngap_type,mapping_type, &vrb_index[0], prb_index_ptr,cell_bw_prb);
                    }

                    break;
                }
            case Dci2:/*format 2*/
            case Dci2A:/*format 2A*/
                {
                    dci_data_ptr->dci_format_2.res_alloc_type = data_in[0]>>31;/*1bit*/
                    if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*type1*/
                    {
                        dci_data_ptr->dci_format_2.res_blocks_subset = _extu(data_in[0],1,31);/*n_rb_dl 25ʱ 1bit*/
                        dci_data_ptr->dci_format_2.shift_res_span = _extu(data_in[0],2,31);/*1bit*/
                        resblock_temp = _bitr(data_in[0]);//ΪʲôҪ��תһ�£�
                        dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,18,21);/*n_rb_dl ����25ʱ��11bit��ע���Ƿ�ת�����λ��ȡ�м�11����*/
                    }
                    else/*type0*/
                    {
                       resblock_temp = _bitr(data_in[0]);
                       dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,18,19);/*n_rb_dl ����25ʱ��13bitע���Ƿ�ת�����λ��ȡ�м�13����*/
                    }
                    dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],14,30);/*2bit*/
                    dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],16,30);/*2bit*///ֻ������TDD����������Ϊ1-6���������FDD�в�����
                    dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],18,28);/*4bit*/
                    dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[0],22,31);/*1bit*/
                    mod_tbs_index = _extu(data_in[0],23,27);/*5bit*/
                    dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[0],23,27);/*5bit*/
                    dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[0],28,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[0],29,30);/*2bit*/
                    //codeword_enable�ж�
                    if((0 == dci_data_ptr->dci_format_2.mod_tbs_index) && (1 == dci_data_ptr->dci_format_2.codeword0_rv))
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 0;
                    }
                    else
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 1;
                    }
                    mod_tbs_index1 = (_extu(data_in[0],31,27))^(data_in[1]>>28);/*5bit*///lycan 20150827��Ҫƴ��1bit��4bit
                    dci_data_ptr->dci_format_2.codeword1_ndi = _extu(data_in[1],4,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword1_rv = _extu(data_in[1],5,30);/*2bit*/
                    //codeword_enable�ж�
                    if((0 == mod_tbs_index1) && (1 == dci_data_ptr->dci_format_2.codeword1_rv))
                    {
                    	dci_data_ptr->dci_format_2.codeword1_enable_flag = 0;
                    }
                    else
                    {
                    	dci_data_ptr->dci_format_2.codeword1_enable_flag = 1;
                    }
                    if(dci_format == Dci2A)//tm3
                    {
                        if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))
                        {
                            dci_data_ptr->dci_format_2.layer_num = 1;//�����֣�����ּ�
                        }
                        else
                        {
                            dci_data_ptr->dci_format_2.layer_num = 2;//�����֣��շ�
                        }
                    }
                    else//tm4
                    {
                    	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],7,29);/*3bit*/
                    	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//������
                    	{
                    		if(0 == dci_data_ptr->dci_format_2.precoding_index)
                    		{
                    			dci_data_ptr->dci_format_2.layer_num = 2;
                    		}
                    		else
                    		{
                    			dci_data_ptr->dci_format_2.layer_num = 1;
                    		}
                    	}
                    	else//������
                    	{
                    		dci_data_ptr->dci_format_2.layer_num = 2;
                    	}
                    }
                    /*����vru�Ŀ�ʼλ�úͳ���*/
                    riv = dci_data_ptr->dci_format_2.res_block;
                    l_crbs = 0;
                    rb_start = 0;
                    rb_start_flag = 0;
                    riv_index = 0;
                    //lycan 20150827 afternoon 17:00///////////////////////////////////////////
                    if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*�޸�type1����Ϊ850*/
                    {
                        if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* ����Ҫ����shift */
                        {
                            for(i=0;i<11;i++)//5M 11
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                {
                                    rb_start = 4 * (i/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 2);
                                    rb_start_flag = 1;
                                }
                                if(1 == riv_index)
                                {
                                    dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 4 * (i/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 2);
                                    dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 4 * (i/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) + (i % 2);
                                    l_crbs = l_crbs + 1;
                                }
                            }
                        }
                        else/* ��Ҫ����shift */
                        {
                            if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//p == (((N_DL_RB - 1)/P)%P)=0
                            {
                                for(i=0;i<11;i++)//5M 11
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                    {
                                        rb_start = 4 * ((2+i)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+2) % 2);//��n_rbgsubset_prb(p)����ʱshift�̶�Ϊ2��������ֱ�Ӽ���ģ�ʡ�����м��ѡ��������
                                        rb_start_flag = 1;
                                    }
                                    if(1 == riv_index)
                                    {
                                        dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 4 * ((2+i)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+2) % 2);
                                        dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 4 * ((2+i)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+2) % 2);
                                        l_crbs = l_crbs + 1;
                                    }
                                }
                            }
                            if(1 == dci_data_ptr->dci_format_2.res_blocks_subset)//p > (((N_DL_RB - 1)/P)%P)
                            {
                                for(i=0;i<11;i++)//5M 11
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                                    {
                                        rb_start = 4 * ((i + 1)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 2);//��n_rbgsubset_prb(p)����ʱshift�̶�Ϊ1
                                        rb_start_flag = 1;
                                    }
                                    if(1 == riv_index)
                                    {
                                        dci_data_ptr->dci_format_2.ts0_prb_index[l_crbs] = 4 * ((i + 1)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 2);
                                        dci_data_ptr->dci_format_2.ts1_prb_index[l_crbs] = 4 * ((i + 1)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 2);
                                        l_crbs = l_crbs + 1;
                                    }
                                }
                            }
                        }
                    }
                    else/*type0*/
                    {
                        for(i=0;i<13;i++)//25Ϊ20M�������10MʱΪ17,5MʱΪ13
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb��ʼ��λ��*/
                            {
                                rb_start = i*2;// P = 4,����Ϊ2��Ҫ�ģ�5MʱP=2
                                rb_start_flag = 1;// �������ֵ�һ�������RBG,����RB��ʼ���
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_2.ts0_prb_index[(2*l_crbs)] = 2*i;// P = 4,����Ϊ2��Ҫ��
                                dci_data_ptr->dci_format_2.ts0_prb_index[(2*l_crbs + 1)] = 2*i + 1;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(2*l_crbs)] = 2*i;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(2*l_crbs + 1)] = 2*i + 1;
                                l_crbs = l_crbs + 1;
                            }
                        }
                        l_crbs = (l_crbs*2);// P = 4,����Ϊ2��Ҫ��
                    }
                    vrp_num = rb_start + l_crbs;
                    if(vrp_num>100)/*������ֲ����쳣��CC��vrb�ĳ����趨Ϊ0*/
                    {
                        rb_start = 0;
                        l_crbs = 1;
                    }
                    dci_data_ptr->dci_format_2.vrb_start = rb_start;
                    dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                     /*��1��codeword ��qm��tb_size*/
                    dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                    /*��2��codeword ��qm��tb_size*/
                    dci_data_ptr->dci_format_2.codeword1_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 2];
                    dci_data_ptr->dci_format_2.codeword1_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];

                    break;
                }

             default:
                break;
        }


    }
    user_para_update_dci(user_para_ptr,
                         subfn_index ,
                         &vxx_cell_para,
                         dci_data_ptr,
                         dci_format
                         );
    if(NORMAL_CP == cell_para_ptr->N_cp)
    {
        prb_demapping_update_normalcp(
                             vxx_cell_para.CFI,
                             subfn_index,             //Para,���δ������֡�ţ��ɿ�����ns�ϲ���
                             user_para_ptr[0].symbol_start,
                             user_para_ptr[0].subcarrier_demap_num,
                             user_para_ptr[0].prb_start,          //Para,***Ŀǰ�����������������prb***
                             user_para_ptr[0].num_PRB,
                             cell_bw_prb);
    }
    else if(EXTENDED_CP == cell_para_ptr->N_cp)
    {
        prb_demapping_update_extendcp(
                             vxx_cell_para.CFI,
                             subfn_index,             //Para,���δ������֡�ţ��ɿ�����ns�ϲ���
                             user_para_ptr[0].symbol_start,
                             user_para_ptr[0].subcarrier_demap_num,
                             user_para_ptr[0].prb_start,          //Para,***Ŀǰ�����������������prb***
                             user_para_ptr[0].num_PRB,
                             cell_bw_prb);
    }
    else
    {
    	printf("/n CP modle select error in dci_process! /n");
    }

    return;
}
