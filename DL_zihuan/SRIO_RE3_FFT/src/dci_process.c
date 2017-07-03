/***********************************************/
/*fcc_dci_data_distill.c                       */
/*功能：PDCCH解码主进程                     */
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
    if(mapping_type == CXX_LOCALIZED_MAPPING_TYPE)//集中式资源分配
    {
        for(i=0;i<vrb_num;i++)
        {
            ts0_prb_index[i] = vrb_index[i];
            ts1_prb_index[i] = vrb_index[i];
        }
    }
    else//分布式资源分配参考协议36.213-7.1.6.3//此部分可用
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
            temp_nvrb = vrb_index[i]%nvrb;//n_VRB mod N_~_DL_VRB 代表n_~_VRB
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
//按位段解析DCI内容
void fcc_dci_data_distill(unsigned int *data_in,
		unsigned int *data_out,
		unsigned char dci_format,
		unsigned char trans_mode,
		unsigned char subfn_index,
		CELL_PARA *cell_para_ptr,//只取出了带宽，还取出了CP模式，其他没改
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
    /*根据不同格式提取信息*/
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
                      dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,19);/*n_rb_ul 等于100时，13bit*/
                    }
                    else
                    {
                      dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                      dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,20);/*n_rb_ul 等于100时，12bit*/
                    }
                    mod_tbs_index = _extu(data_in[0],15,27);/*5bit*/
                    dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],20,28);/*4bit*/
                    dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],24,31);/*1bit*/
                    dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],25,30);/*2bit*/
                    dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],27,30);/*2bit*/
                    dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],29,30);/*2bit*/
                    /*qm和tb_size*/
                    dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    /*layer数目*/
                    dci_data_ptr->dci_format_1a.layer_num =  2;
                    /*计算vru的开始位置和长度*/
                    riv = dci_data_ptr->dci_format_1a.res_block;
                    rb_start = riv%CXX_PRB_MAX_NUM;
                    l_crbs = riv/CXX_PRB_MAX_NUM;
                    if(50 >= l_crbs)/*开始位置小于50*/
                    {
                        dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                        dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                    }
                    else/*开始位置大于50*/
                    {
                        dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                        dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                    }
                    vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*对异常配置进行保护*/
                    for(i=0;i<vrb_num;i++)
                    {
                        vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                    }
                    /*2015.1.12修改 特殊子帧采用不同的处理方式*/
                    if(subfn_index ==1||subfn_index==6)
                    	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                    else
                    	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                    /*vrb到prb的映射*/
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
                    if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*跳频时*/
                    {
                        dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,30);/*2bit*/
                        dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],4,21);/*n_rb_ul 等于100时，11bit*/
                    }
                    else/*非跳频*/
                    {
                       dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,19);/*n_rb_ul 等于100时，13bit*/
                    }
                    dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],15,27);/*5bit*/
                    dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],20,31);/*1bit*/
                    dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],21,30);/*2bit*/
                    dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],23,29);/*3bit*/
                    dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],26,30);/*2bit*/
                    dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],28,31);/*1bit*/
                    /*计算vrb的开始位置和长度*/
                    riv = dci_data_ptr->dci_format_0.res_block;
                    rb_start = riv%CXX_NRB_UL_NUM;
                    l_crbs = riv/CXX_NRB_UL_NUM;
                    if(50 >= rb_start)/*开始位置小于50*/
                    {
                        dci_data_ptr->dci_format_0.vrb_start = rb_start;
                        dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                    }
                    else/*开始位置大于50*/
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
                    resblock_temp = _bitr(data_in[0]);/*翻转bitmap*///_bitr?
                    dci_data_ptr->dci_format_1.res_block = _extu(resblock_temp,6,10);/*n_rb_dl 等于100时，22bit*/
                }
                else/*type0*/
                {
                   resblock_temp = _bitr(data_in[0]);//_bitr?
                   dci_data_ptr->dci_format_1.res_block = _extu(resblock_temp,6,7);/*n_rb_dl 等于100时，25bit*/
                }
                mod_tbs_index = _extu(data_in[0],26,27);/*5bit*/
                dci_data_ptr->dci_format_1.harq_pro_num = _extu(data_in[0],31,28)^(data_in[1]>>29);/*4bit*/
                dci_data_ptr->dci_format_1.ndi = _extu(data_in[1],3,31);/*1bit*/
                dci_data_ptr->dci_format_1.rv = _extu(data_in[1],4,30);/*2bit*/
                dci_data_ptr->dci_format_1.tpc_pucch = _extu(data_in[1],6,30);/*2bit*/
                dci_data_ptr->dci_format_1.down_assign_index = _extu(data_in[1],8,30);/*2bit*/
                 /*qm和tb_size*/
                dci_data_ptr->dci_format_1.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                /*layer数目*/
                dci_data_ptr->dci_format_1.layer_num =  1;
                /*计算vrb的开始位置和长度*/
                riv = dci_data_ptr->dci_format_1.res_block;
                l_crbs = 0;
                rb_start = 0;
                rb_start_flag = 0;
                riv_index = 0;
                if( 1 == dci_data_ptr->dci_format_1.res_alloc_type )/*修改type1解析为850*/
                {
                    if( 0 == dci_data_ptr->dci_format_1.shift_res_span)/* 不需要进行shift */
                    {
                        for(i=0;i<22;i++)
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                    else/* 需要进行shift */
                    {
                        if(0 == dci_data_ptr->dci_format_1.res_blocks_subset)
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                        if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                if(vrp_num>100)/*如果出现参数异常，CC把vrb的长度设定为0*/
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
                    /*layer数目*/
                    dci_data_ptr->dci_format_1c.layer_num =  1;
                    /*qm和tb_size*/
                    dci_data_ptr->dci_format_1c.qm = 2;

                    dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*需要修改*/



                    /*计算vrb的开始位置和长度*/
                    riv = dci_data_ptr->dci_format_1c.res_block;
                    rb_start = riv%CXX_PRB_MAX_NUM;
                    rb_start = rb_start<<2;
                    l_crbs = riv/CXX_PRB_MAX_NUM;
                    l_crbs = l_crbs<<2;
                    if(50 >= l_crbs)/*开始位置小于50*/
                    {
                        dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                        dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                    }
                    else/*开始位置大于50*/
                    {
                        dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                        dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                    }
                    vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*对异常配置进行保护*/
                    for(i=0;i<vrb_num;i++)
                    {
                        vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                    }

                    /*vrb到prb的映射*/
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
                    dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,6,10);/*n_rb_dl 等于100时，22bit*/
                }
                else/*type0*/
                {
                   resblock_temp = _bitr(data_in[0]);
                   dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,6,7);/*n_rb_dl 等于100时，25bit*/
                }
                dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],26,30);/*2bit*/
                dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],28,30);/*2bit*/
                dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],30,28)^(data_in[1]>>30);/*4bit*/
                dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[1],2,31);/*1bit*/
                mod_tbs_index = _extu(data_in[1],3,27);/*5bit*/
                dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[1],3,27);/*5bit*/
                dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[1],8,31);/*1bit*/
                dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[1],9,30);/*2bit*/
                //codeword_enable判断
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
                //codeword_enable判断
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
                        dci_data_ptr->dci_format_2.layer_num = 1;//单码字，传输分集
                    }
                    else
                    {
                        dci_data_ptr->dci_format_2.layer_num = 2;//两码字，空分
                    }
                }
                else//tm4
                {
                	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],19,29);/*3bit*/
                	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//单码字
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
                	else//两码字
                	{
                		dci_data_ptr->dci_format_2.layer_num = 2;
                	}
                }
                /*计算vru的开始位置和长度*/
                riv = dci_data_ptr->dci_format_2.res_block;
                l_crbs = 0;
                rb_start = 0;
                rb_start_flag = 0;
                riv_index = 0;
                if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*修改type1解析为850*/
                {
                    if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* 不需要进行shift */
                    {
                        for(i=0;i<22;i++)
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                    else/* 需要进行shift */
                    {
                        if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//(99/4)%4 = 0所以只有两种情况，没有小于的
                        {
                            for(i=0;i<22;i++)
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                {
                                    rb_start = 16 * ((6+i)>>2) + ((i+6) & 3);//此时shift固定为6，这里是直接计算的，省略了中间的选择计算过程
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
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                        if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                if(vrp_num>100)/*如果出现参数异常，CC把vrb的长度设定为0*/
                {
                    rb_start = 0;
                    l_crbs = 1;
                }
                dci_data_ptr->dci_format_2.vrb_start = rb_start;
                dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                 /*第1个codeword 的qm和tb_size*/
                dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                /*第2个codeword 的qm和tb_size*/
                dci_data_ptr->dci_format_2.codeword1_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 1];
                tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index1*3 + 2];
                dci_data_ptr->dci_format_2.codeword1_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                
                break;
            }
        case Dci3:/*format 3*/
            {
                dci_data_ptr->dci_format_3.tpc_user = data_in[0]>>4;/*28bit,2bit表示一个用户*/
                break;
            }
        case Dci3A:/*format 3a*/
            {
                dci_data_ptr->dci_format_3a.tpc_user = data_in[0]>>4;/*28bit，1bit表示一个用户*/
                break;
            }
         default:
            break;
    }
    }
    //20150814 lycan 10M bandwidth
    if(50 == cell_bw_prb )
    {

        /*根据不同格式提取信息*/
        switch(dci_format)
        {
            case Dci01A:/*format 0&1a*/
                {
                  if(1 == data_in[0]>>31)//1a
                    {
                	    //ly 还不用改，提取与原来一样
                        dci_data_ptr->dci_format_1a.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_1a.dis_tran_flag = _extu(data_in[0],1,31);/*1bit*/
                        //ly 已经改了
                        if( CXX_LOCALIZED_MAPPING_TYPE == dci_data_ptr->dci_format_1a.dis_tran_flag)/*local*/
                        {
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,21);/*n_rb_dl 等于50时，11bit*/
                        }
                        else
                        {
                          dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,22);/*n_rb_dl 等于50时，10bit*/
                        }
                        // ly已经改了
                        mod_tbs_index = _extu(data_in[0],13,27);/*5bit*/
                        dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],18,28);/*4bit*/
                        dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],22,31);/*1bit*/
                        dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],23,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],25,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],27,30);/*2bit*/

                        /*qm和tb_size*/
                        dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                        tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                        /*layer数目*/
                        dci_data_ptr->dci_format_1a.layer_num =  2;
                        /*计算vru的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_1a.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;//ly CXX_PRB_MAX_NUM宏要改
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        if(50 >= l_crbs)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                        }
                        else/*开始位置大于50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*对异常配置进行保护*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                        }
                        /*2015.1.12修改 特殊子帧采用不同的处理方式*/
                        if(subfn_index ==1||subfn_index==6)
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                        else
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                        /*vrb到prb的映射*/
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
                        if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*跳频时*/
                        {
                            dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,30);/*2bit*/
                            dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],4,23);/*n_rb_dl 等于50时，9bit*/
                        }
                        else/*非跳频*/
                        {
                           dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,21);/*n_rb_dl 等于50时，11bit*/
                        }
                        dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],13,27);/*5bit*/
                        dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],18,31);/*1bit*/
                        dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],19,30);/*2bit*/
                        dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],21,29);/*3bit*/
                        dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],24,30);/*2bit*///(只出现在上下行配置为0的操作中，目前为2应该无此字段)
                        //lycan 少了下行分配序号（DAI）2比特(只出现在上下行配置为1 - 6 的操作中，目前为2，应该有此字段)
                        dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],26,31);/*1bit*/
                        /*计算vrb的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_0.res_block;
                        rb_start = riv%CXX_NRB_UL_NUM;
                        l_crbs = riv/CXX_NRB_UL_NUM;
                        if(50 >= rb_start)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                        }
                        else/*开始位置大于50*/
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
                        /*layer数目*/
                        dci_data_ptr->dci_format_1c.layer_num =  1;
                        /*qm和tb_size*/
                        dci_data_ptr->dci_format_1c.qm = 2;

                        dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*需要加表lycan 20150825*/



                        /*计算vrb的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_1c.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;
                        rb_start = rb_start<<2;//4re
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        l_crbs = l_crbs<<2;
                        if(50 >= l_crbs)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                        }
                        else/*开始位置大于50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*对异常配置进行保护*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                        }

                        /*vrb到prb的映射*/
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
                        resblock_temp = _bitr(data_in[0]);//为什么要反转一下？
                        dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,14,18);/*n_rb_dl 等于50时，14bit，注意是反转后的移位，取中间14比特*/
                    }
                    else/*type0*/
                    {
                       resblock_temp = _bitr(data_in[0]);
                       dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,14,15);/*n_rb_dl 等于50时，17bit注意是反转后的移位，取中间14比特*/
                    }
                    dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],18,30);/*2bit*/
                    dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],20,30);/*2bit*///只适用于TDD上下行配置为1-6的情况，在FDD中不存在
                    dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],22,28);/*4bit*/
                    dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[0],26,31);/*1bit*/
                    mod_tbs_index = _extu(data_in[0],27,27);/*5bit*/
                    dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[0],27,27);/*5bit*/
                    dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[1],0,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[1],1,30);/*2bit*/
                    //codeword_enable判断
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
                    //codeword_enable判断
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
                            dci_data_ptr->dci_format_2.layer_num = 1;//单码字，传输分集
                        }
                        else
                        {
                            dci_data_ptr->dci_format_2.layer_num = 2;//两码字，空分
                        }
                    }
                    else//tm4
                    {
                    	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],11,29);/*3bit*/
                    	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//单码字
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
                    	else//两码字
                    	{
                    		dci_data_ptr->dci_format_2.layer_num = 2;
                    	}
                    }
                    /*计算vru的开始位置和长度*/
                    riv = dci_data_ptr->dci_format_2.res_block;
                    l_crbs = 0;
                    rb_start = 0;
                    rb_start_flag = 0;
                    riv_index = 0;
                    if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*修改type1解析为850*/
                    {
                        if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* 不需要进行shift */
                        {
                            for(i=0;i<14;i++)//10M 14
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                        else/* 需要进行shift */
                        {
                            if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//p < (((N_DL_RB - 1)/P)%P)
                            {
                                for(i=0;i<14;i++)//10M 14
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                    {
                                        rb_start = 9 * ((4+i)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+4) % 3);//算n_rbgsubset_prb(p)，此时shift固定为4，这里是直接计算的，省略了中间的选择计算过程
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
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                    {
                                        rb_start = 9 * ((i + 3)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+3) % 3);//算n_rbgsubset_prb(p)，此时shift固定为3
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
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                    {
                                        rb_start = 9 * ((i + 1)/3) + 3 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 3);//算n_rbgsubset_prb(p)，此时shift固定为1
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
                        for(i=0;i<25;i++)//25为20M的情况，10M时为17
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                            {
                                rb_start = i*3;// P = 4,现在为3，要改
                                rb_start_flag = 1;// 用来区分第一个分配的RBG,计算RB起始编号
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs)] = 3*i;// P = 4,现在为3，要改
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs + 1)] = 3*i + 1;
                                dci_data_ptr->dci_format_2.ts0_prb_index[(3*l_crbs + 2)] = 3*i + 2;

                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs)] = 3*i;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs + 1)] = 3*i + 1;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(3*l_crbs + 2)] = 3*i + 2;

                                l_crbs = l_crbs + 1;
                            }
                        }
                        l_crbs = (l_crbs*3);// P = 4,现在为3，要改
                    }
                    vrp_num = rb_start + l_crbs;
                    if(vrp_num>100)/*如果出现参数异常，CC把vrb的长度设定为0*/
                    {
                        rb_start = 0;
                        l_crbs = 1;
                    }
                    dci_data_ptr->dci_format_2.vrb_start = rb_start;
                    dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                     /*第1个codeword 的qm和tb_size*/
                    dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                    /*第2个codeword 的qm和tb_size*/
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
        /*根据不同格式提取信息*/
        switch(dci_format)
        {
            case Dci01A:/*format 0&1a*/
                {
                  if(1 == data_in[0]>>31)//1a
                    {
                	    //ly 还不用改，提取与原来一样
                        dci_data_ptr->dci_format_1a.format_01a_flag = data_in[0]>>31;/*1bit*/
                        dci_data_ptr->dci_format_1a.dis_tran_flag = _extu(data_in[0],1,31);/*1bit*/
                        //ly 已经改了
                        if( CXX_LOCALIZED_MAPPING_TYPE == dci_data_ptr->dci_format_1a.dis_tran_flag)/*local*/
                        {
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],2,23);/*n_rb_dl 等于25时，9bit*/
                        }
                        else
                        {
                          dci_data_ptr->dci_format_1a.n_gap = _extu(data_in[0],2,31);/*1bit*/
                          dci_data_ptr->dci_format_1a.res_block = _extu(data_in[0],3,24);/*n_rb_dl 等于25时，8bit*/
                        }
                        // ly 0827 已经改了
                        mod_tbs_index = _extu(data_in[0],11,27);/*5bit*/
                        dci_data_ptr->dci_format_1a.harq_pro_num = _extu(data_in[0],16,28);/*4bit*/
                        dci_data_ptr->dci_format_1a.ndi = _extu(data_in[0],20,31);/*1bit*/
                        dci_data_ptr->dci_format_1a.rv = _extu(data_in[0],21,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.tpc_pucch = _extu(data_in[0],23,30);/*2bit*/
                        dci_data_ptr->dci_format_1a.down_assign_index = _extu(data_in[0],25,30);/*2bit*/

                        /*qm和tb_size*/
                        dci_data_ptr->dci_format_1a.qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                        tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                        /*layer数目*/
                        dci_data_ptr->dci_format_1a.layer_num =  2;
                        /*计算vru的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_1a.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;//ly CXX_PRB_MAX_NUM宏要改
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        if(50 >= l_crbs)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (l_crbs + 1);
                        }
                        else/*开始位置大于50*/
                        {
                            dci_data_ptr->dci_format_1a.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1a.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1a.vrb_length; /*对异常配置进行保护*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1a.vrb_start + i;
                        }
                        /*2015.1.12修改 特殊子帧采用不同的处理方式*/
                        if(subfn_index ==1||subfn_index==6)
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num*3/4-1];
                        else
                        	dci_data_ptr->dci_format_1a.tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][vrb_num-1];
                        /*vrb到prb的映射*/
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
                        if( 1 == dci_data_ptr->dci_format_0.hopping_flag )/*跳频时*/
                        {
                            dci_data_ptr->dci_format_0.n_ulhop = _extu(data_in[0],2,31);/*1bit*///lycan 0827
                            dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],3,25);/*n_rb_dl 等于25时，8bit*/
                        }
                        else/*非跳频*/
                        {
                           dci_data_ptr->dci_format_0.res_block = _extu(data_in[0],2,23);/*n_rb_dl 等于25时，9bit*/
                        }
                        dci_data_ptr->dci_format_0.mod_cod_rv = _extu(data_in[0],11,27);/*5bit*/
                        dci_data_ptr->dci_format_0.ndi = _extu(data_in[0],16,31);/*1bit*/
                        dci_data_ptr->dci_format_0.tpc_pusch = _extu(data_in[0],17,30);/*2bit*/
                        dci_data_ptr->dci_format_0.cyclic_shift_dmrs = _extu(data_in[0],19,29);/*3bit*/
                        dci_data_ptr->dci_format_0.ul_index = _extu(data_in[0],22,30);/*2bit*///(只出现在上下行配置为0的操作中，目前为2应该无此字段)
                        //lycan 少了下行分配序号（DAI）2比特(只出现在上下行配置为1 - 6 的操作中，目前为2，应该有此字段)
                        dci_data_ptr->dci_format_0.cqi = _extu(data_in[0],24,31);/*1bit*/
                        /*计算vrb的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_0.res_block;
                        rb_start = riv%CXX_NRB_UL_NUM;
                        l_crbs = riv/CXX_NRB_UL_NUM;
                        if(50 >= rb_start)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_0.vrb_start = rb_start;
                            dci_data_ptr->dci_format_0.vrb_length = (l_crbs + 1)%51;
                        }
                        else/*开始位置大于50*/
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
                        /*layer数目*/
                        dci_data_ptr->dci_format_1c.layer_num =  1;
                        /*qm和tb_size*/
                        dci_data_ptr->dci_format_1c.qm = 2;

                        dci_data_ptr->dci_format_1c.tb_size = VCC_BLOCK_SIZE_TABLE_DCI1C[tb_index];/*需要加表lycan 20150825*/



                        /*计算vrb的开始位置和长度*/
                        riv = dci_data_ptr->dci_format_1c.res_block;
                        rb_start = riv%CXX_PRB_MAX_NUM;
                        rb_start = rb_start<<2;//4re
                        l_crbs = riv/CXX_PRB_MAX_NUM;
                        l_crbs = l_crbs<<2;
                        if(50 >= l_crbs)/*开始位置小于50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (l_crbs + 1);
                        }
                        else/*开始位置大于50*/
                        {
                            dci_data_ptr->dci_format_1c.vrb_start = CXX_PRB_MAX_NUM - 1 - rb_start;
                            dci_data_ptr->dci_format_1c.vrb_length = (CXX_PRB_MAX_NUM + 1 - l_crbs);
                        }
                        vrb_num = dci_data_ptr->dci_format_1c.vrb_length; /*对异常配置进行保护*/
                        for(i=0;i<vrb_num;i++)
                        {
                            vrb_index[i] = dci_data_ptr->dci_format_1c.vrb_start + i;
                        }

                        /*vrb到prb的映射*/
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
                        dci_data_ptr->dci_format_2.res_blocks_subset = _extu(data_in[0],1,31);/*n_rb_dl 25时 1bit*/
                        dci_data_ptr->dci_format_2.shift_res_span = _extu(data_in[0],2,31);/*1bit*/
                        resblock_temp = _bitr(data_in[0]);//为什么要反转一下？
                        dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,18,21);/*n_rb_dl 等于25时，11bit，注意是反转后的移位，取中间11比特*/
                    }
                    else/*type0*/
                    {
                       resblock_temp = _bitr(data_in[0]);
                       dci_data_ptr->dci_format_2.res_block = _extu(resblock_temp,18,19);/*n_rb_dl 等于25时，13bit注意是反转后的移位，取中间13比特*/
                    }
                    dci_data_ptr->dci_format_2.tpc_pucch = _extu(data_in[0],14,30);/*2bit*/
                    dci_data_ptr->dci_format_2.down_assign_index = _extu(data_in[0],16,30);/*2bit*///只适用于TDD上下行配置为1-6的情况，在FDD中不存在
                    dci_data_ptr->dci_format_2.harq_pro_num = _extu(data_in[0],18,28);/*4bit*/
                    dci_data_ptr->dci_format_2.codeword_swap_flag = _extu(data_in[0],22,31);/*1bit*/
                    mod_tbs_index = _extu(data_in[0],23,27);/*5bit*/
                    dci_data_ptr->dci_format_2.mod_tbs_index = _extu(data_in[0],23,27);/*5bit*/
                    dci_data_ptr->dci_format_2.codeword0_ndi = _extu(data_in[0],28,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword0_rv = _extu(data_in[0],29,30);/*2bit*/
                    //codeword_enable判断
                    if((0 == dci_data_ptr->dci_format_2.mod_tbs_index) && (1 == dci_data_ptr->dci_format_2.codeword0_rv))
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 0;
                    }
                    else
                    {
                    	dci_data_ptr->dci_format_2.codeword0_enable_flag = 1;
                    }
                    mod_tbs_index1 = (_extu(data_in[0],31,27))^(data_in[1]>>28);/*5bit*///lycan 20150827需要拼凑1bit加4bit
                    dci_data_ptr->dci_format_2.codeword1_ndi = _extu(data_in[1],4,31);/*1bit*/
                    dci_data_ptr->dci_format_2.codeword1_rv = _extu(data_in[1],5,30);/*2bit*/
                    //codeword_enable判断
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
                            dci_data_ptr->dci_format_2.layer_num = 1;//单码字，传输分集
                        }
                        else
                        {
                            dci_data_ptr->dci_format_2.layer_num = 2;//两码字，空分
                        }
                    }
                    else//tm4
                    {
                    	dci_data_ptr->dci_format_2.precoding_index = _extu(data_in[1],7,29);/*3bit*/
                    	if((0 == dci_data_ptr->dci_format_2.codeword0_enable_flag) || (0 == dci_data_ptr->dci_format_2.codeword1_enable_flag))//单码字
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
                    	else//两码字
                    	{
                    		dci_data_ptr->dci_format_2.layer_num = 2;
                    	}
                    }
                    /*计算vru的开始位置和长度*/
                    riv = dci_data_ptr->dci_format_2.res_block;
                    l_crbs = 0;
                    rb_start = 0;
                    rb_start_flag = 0;
                    riv_index = 0;
                    //lycan 20150827 afternoon 17:00///////////////////////////////////////////
                    if( 1 == dci_data_ptr->dci_format_2.res_alloc_type )/*修改type1解析为850*/
                    {
                        if( 0 == dci_data_ptr->dci_format_2.shift_res_span)/* 不需要进行shift */
                        {
                            for(i=0;i<11;i++)//5M 11
                            {
                                riv_index = (riv>>i)&0x1;
                                if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
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
                        else/* 需要进行shift */
                        {
                            if(0 == dci_data_ptr->dci_format_2.res_blocks_subset)//p == (((N_DL_RB - 1)/P)%P)=0
                            {
                                for(i=0;i<11;i++)//5M 11
                                {
                                    riv_index = (riv>>i)&0x1;
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                    {
                                        rb_start = 4 * ((2+i)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+2) % 2);//算n_rbgsubset_prb(p)，此时shift固定为2，这里是直接计算的，省略了中间的选择计算过程
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
                                    if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                                    {
                                        rb_start = 4 * ((i + 1)/2) + 2 * (dci_data_ptr->dci_format_2.res_blocks_subset) +((i+1) % 2);//算n_rbgsubset_prb(p)，此时shift固定为1
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
                        for(i=0;i<13;i++)//25为20M的情况，10M时为17,5M时为13
                        {
                            riv_index = (riv>>i)&0x1;
                            if((1 == riv_index)&&(0 == rb_start_flag))/*vrb开始的位置*/
                            {
                                rb_start = i*2;// P = 4,现在为2，要改，5M时P=2
                                rb_start_flag = 1;// 用来区分第一个分配的RBG,计算RB起始编号
                            }
                            if(1 == riv_index)
                            {
                                dci_data_ptr->dci_format_2.ts0_prb_index[(2*l_crbs)] = 2*i;// P = 4,现在为2，要改
                                dci_data_ptr->dci_format_2.ts0_prb_index[(2*l_crbs + 1)] = 2*i + 1;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(2*l_crbs)] = 2*i;
                                dci_data_ptr->dci_format_2.ts1_prb_index[(2*l_crbs + 1)] = 2*i + 1;
                                l_crbs = l_crbs + 1;
                            }
                        }
                        l_crbs = (l_crbs*2);// P = 4,现在为2，要改
                    }
                    vrp_num = rb_start + l_crbs;
                    if(vrp_num>100)/*如果出现参数异常，CC把vrb的长度设定为0*/
                    {
                        rb_start = 0;
                        l_crbs = 1;
                    }
                    dci_data_ptr->dci_format_2.vrb_start = rb_start;
                    dci_data_ptr->dci_format_2.vrb_length = l_crbs;
                     /*第1个codeword 的qm和tb_size*/
                    dci_data_ptr->dci_format_2.codeword0_qm = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 1];
                    tb_index = VCC_MOD_TBS_INDEX_TABLE[mod_tbs_index*3 + 2];
                    dci_data_ptr->dci_format_2.codeword0_tb_size = VCC_BLOCK_SIZE_TABLE[tb_index][l_crbs-1];
                    /*第2个codeword 的qm和tb_size*/
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
                             subfn_index,             //Para,本次处理的子帧号（可考虑与ns合并）
                             user_para_ptr[0].symbol_start,
                             user_para_ptr[0].subcarrier_demap_num,
                             user_para_ptr[0].prb_start,          //Para,***目前仅适用于连续分配的prb***
                             user_para_ptr[0].num_PRB,
                             cell_bw_prb);
    }
    else if(EXTENDED_CP == cell_para_ptr->N_cp)
    {
        prb_demapping_update_extendcp(
                             vxx_cell_para.CFI,
                             subfn_index,             //Para,本次处理的子帧号（可考虑与ns合并）
                             user_para_ptr[0].symbol_start,
                             user_para_ptr[0].subcarrier_demap_num,
                             user_para_ptr[0].prb_start,          //Para,***目前仅适用于连续分配的prb***
                             user_para_ptr[0].num_PRB,
                             cell_bw_prb);
    }
    else
    {
    	printf("/n CP modle select error in dci_process! /n");
    }

    return;
}
