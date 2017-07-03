/***********************************************/
/*prbmapping                                   */
/*���ܣ����������Դӳ��                       */
/***********************************************/
#include "system_init.h"

signed int pdsch_psc_ssc_pbch_prbmapping( 
                                         USER_PARA *user_para_ptr,
                                         CELL_PARA *cell_para_ptr,
                                         unsigned char slot_idx,/*����Ϊż��0,2,4~18*/
                                         unsigned char OFDMA_idx_l,/*����Ϊ0~13*/
                                         signed int *  input_buffer[NUM_ANTENNA_PORT_2],
                                         signed int *  output_data[NUM_ANTENNA_PORT_2],
                                         signed short *rho_a_b,
                                         signed int * restrict PBCH_data_buffer[NUM_ANTENNA_PORT_2],
                                         signed int * restrict PSCH_data_buffer,
                                         signed int * restrict SSCH_data_buffer,
                                         unsigned char user_index
                                       )
{

    signed int *  output_temp;
    signed int * restrict input_data;
    signed int * restrict PBCH_data_buffer_temp;
    signed int * restrict PSCH_data_buffer_temp;
    signed int * restrict SSCH_data_buffer_temp;
    unsigned char *PBCHCarrierIdx;
    unsigned int input_data_used = 0;
    signed int rho_a;
  
    unsigned char *PDSCHCarrierIdx;
    unsigned short subc_per_freq_dim;
    unsigned short subc_start;
    unsigned short subc_end;
    unsigned short i;
    unsigned char bitmap_length;
    unsigned char length;
    unsigned char num_PRB_org;
    unsigned char num_RBG;
    unsigned char rem_RBG;
    unsigned char j;
    unsigned char PBCHSyncPresentFlag;
    unsigned char PBCHPresentFlag;
    unsigned char SSyncPresentFlag;
    unsigned char PSyncPresentFlag;
    double temp1;
    double temp2;
    double temp3;
    double temp4;
    double temp5;
    
    unsigned int bitmap = user_para_ptr->bitmap;
    unsigned char num_PRB = user_para_ptr->num_PRB;
    unsigned short ncell_id = cell_para_ptr->cellID;
    unsigned short PBCHSyncSCStart = cell_para_ptr->PBCHSyncSCStart;
    unsigned short PBCHSyncSCEnd = cell_para_ptr->PBCHSyncSCEnd;
    unsigned char RBG_size = cell_para_ptr->RBG_size;
    unsigned char num_PRB_per_bw_config = cell_para_ptr->num_PRB_per_bw_config;
    unsigned char num_antenna_port = cell_para_ptr->num_antenna_port;

    
    /***************PDSCH��ӳ��map���� **************************/
    /* PDSCH�ڵ����߶˿��µ�5�����ź͵�12�����ŵ�ÿ��PRB��ӳ��map */
    if((OFDMA_idx_l == (N_SYM_PER_SLOT - 3) || OFDMA_idx_l == (2 * N_SYM_PER_SLOT - 3)) && (num_antenna_port == NUM_ANTENNA_PORT_1))
    {
        PDSCHCarrierIdx = (unsigned char*)(PDSCH_InRB_SinglePort_4Symb[(ncell_id % 6)]);    
    }
    /* PDSCH�ڵ����߶˿��µ�1�����ź͵�8�����ŵ�ÿ��PRB��ӳ��map */
    else if(num_antenna_port == NUM_ANTENNA_PORT_1)
    {
        PDSCHCarrierIdx = (unsigned char*)(PDSCH_InRB_SinglePort_0Symb[(ncell_id % 6)]);
    }
    /* PDSCH��2/4���߶˿���ÿ�����ŵ�ÿ��PRB��ӳ��map */
    else
    {
        PDSCHCarrierIdx = (unsigned char*)(PDSCH_InRB_MultiplePort[(ncell_id % 3)]);
    }

    /***************PBCH��SCH��ӳ��map���� *********************/
    PBCHPresentFlag = 0;
    SSyncPresentFlag = 0;
    PSyncPresentFlag = 0;
    PBCHSyncPresentFlag = 0;
    /* PBCH������ÿ������֡��slot1��SSCH������֡0��ʱ϶1*/
    if(slot_idx == 0)
    {
        PBCHPresentFlag = PBCHPresent[OFDMA_idx_l];
        SSyncPresentFlag = SSyncPresent[OFDMA_idx_l];
        
    }
    /* SSCH������֡0��ʱ϶11 */
    else if(slot_idx == 10)
    {
        SSyncPresentFlag = SSyncPresent[OFDMA_idx_l];

    }
    /* PSCH����DWPTS����֡6�ĵ�3������ */
    else if((slot_idx == 2) || (slot_idx == 12))
    {
        PSyncPresentFlag = PSyncPresent[OFDMA_idx_l];
    }
    PBCHSyncPresentFlag = PBCHPresentFlag | SSyncPresentFlag | PSyncPresentFlag;

    /**************************PDSCH��dataӳ��*********************************/

    for (j = 0; j < num_antenna_port; j++)
    {
        
      num_PRB_org = user_para_ptr->num_PRB;
      rem_RBG = num_PRB_org % RBG_size;/*����һ��RBG��prb����*/
      bitmap_length = (num_PRB_per_bw_config + RBG_size - 1) / RBG_size;/*�Ӹߵ���ÿ��bit����һ��RBG�Ƿ�ռ��*/
      num_RBG = num_PRB_org / RBG_size;
      num_PRB = RBG_size;
      input_data = input_buffer[j];
        input_data_used = 0;
            
        for (length = 0; length < bitmap_length; length++)
        {
            if (((bitmap >> (31 - length)) & 0x00000001) == 1)/*bitΪ1������RBG��ռ�ã�RBG�ǴӸ�λ����λ*/
            {
               subc_per_freq_dim = (unsigned short)(N_SC_PER_RB * num_PRB);
               subc_start = (unsigned short)(N_SC_PER_RB * length * num_PRB);
               subc_end = (unsigned short)(subc_start + subc_per_freq_dim);

               /* ���һ��RBG����rem_RBG���������� */
               if ((num_RBG == 0) && (rem_RBG !=0))
               {
                   subc_per_freq_dim = (unsigned short)(N_SC_PER_RB * rem_RBG);
                   subc_end = (unsigned short)(subc_start + subc_per_freq_dim);
               }
        
               /* �������PBCH / SCH����Ҫ�������ǵ����� */
               if (PBCHSyncPresentFlag == 1)
               {
                    if ((subc_start >= PBCHSyncSCStart) && (subc_end <= PBCHSyncSCEnd))
                        subc_end = subc_start;
                    else if ((subc_start < PBCHSyncSCStart) && (subc_end > PBCHSyncSCStart))
                        subc_end = PBCHSyncSCStart;
                    else if ((subc_start < PBCHSyncSCEnd) && (subc_end > PBCHSyncSCEnd))
                        subc_start = PBCHSyncSCEnd+1;
               }
               output_temp = output_data[j] + ((num_PRB_per_bw_config) * (N_SC_PER_RB)* (OFDMA_idx_l)) + subc_start;

               if (subc_end != subc_start )
               {
                   /* �˷��Ų����ڵ�Ƶ */
                   if (SymbolWithPilot[num_antenna_port - 1][OFDMA_idx_l] == 0)
                   {
                       rho_a = (rho_a_b[0] << 16)|rho_a_b[0];
                       for (i = 0; i< (subc_end - subc_start)>>1;i++)
                       { 
                           temp1 = _smpy2(input_data[input_data_used],rho_a);
                           temp2 = _smpy2(input_data[input_data_used + 1],rho_a);
                           output_temp[2 * i] = _packh2(_hi(temp1),_lo(temp1));
                           output_temp[2 * i + 1] = _packh2(_hi(temp2),_lo(temp2));
                           input_data_used += 2;
                       }   
                   }
                   /* ���ڵ�Ƶ�ķ��� */
                   else
                   {
                       rho_a = (rho_a_b[1] << 16)|rho_a_b[1];
                       if(num_antenna_port == NUM_ANTENNA_PORT_1)
                       {
                           for (i = 0; i < (subc_end - subc_start)/6 ;i++)
                           {
                               temp1 = _smpy2(input_data[input_data_used],rho_a);
                               temp2 = _smpy2(input_data[input_data_used + 1],rho_a);
                               temp3 = _smpy2(input_data[input_data_used + 2],rho_a);
                               temp4 = _smpy2(input_data[input_data_used + 3],rho_a);
                               temp5 = _smpy2(input_data[input_data_used + 4],rho_a);
                               output_temp[6 * i + PDSCHCarrierIdx[0]] = _packh2(_hi(temp1),_lo(temp1));
                               output_temp[6 * i + PDSCHCarrierIdx[1]] = _packh2(_hi(temp2),_lo(temp2));
                               output_temp[6 * i + PDSCHCarrierIdx[2]] = _packh2(_hi(temp3),_lo(temp3));
                               output_temp[6 * i + PDSCHCarrierIdx[3]] = _packh2(_hi(temp4),_lo(temp4));
                               output_temp[6 * i + PDSCHCarrierIdx[4]] = _packh2(_hi(temp5),_lo(temp5));
                               input_data_used += 5;
                           }
                       }
                       else
                       {
                           for (i = 0; i< (subc_end - subc_start)/6 ;i++)
                           {
                               temp1 = _smpy2(input_data[input_data_used],rho_a);
                               temp2 = _smpy2(input_data[input_data_used + 1],rho_a);
                               temp3 = _smpy2(input_data[input_data_used + 2],rho_a);
                               temp4 = _smpy2(input_data[input_data_used + 3],rho_a);
                               output_temp[6 * i + PDSCHCarrierIdx[0]] = _packh2(_hi(temp1),_lo(temp1));
                               output_temp[6 * i + PDSCHCarrierIdx[1]] = _packh2(_hi(temp2),_lo(temp2));
                               output_temp[6 * i + PDSCHCarrierIdx[2]] = _packh2(_hi(temp3),_lo(temp3));
                               output_temp[6 * i + PDSCHCarrierIdx[3]] = _packh2(_hi(temp4),_lo(temp4));
   
                               input_data_used += 4;
                           }
                       }
                   } /* end of num_pilots not equal to 0 */
               }
               num_RBG--;
            }/* end of loop for bitmap = 1*/
        } /*end of bitmap length*/    

     if(0 == user_index)
     {
        if (PBCHSyncPresentFlag == 1)
        {
            output_temp = output_data[j] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l)) + PBCHSyncSCStart;
            SSCH_data_buffer_temp = SSCH_data_buffer;
            PSCH_data_buffer_temp = PSCH_data_buffer;
            PBCH_data_buffer_temp = PBCH_data_buffer[j];
            
            if (SSyncPresentFlag == 1 && j==0)
            {
                for (i=0; i < TOTAL_PSC_SSC_SC_NUM ; i++)
                  *(output_temp++) = *(SSCH_data_buffer_temp++);
            }
            if (PSyncPresentFlag == 1 && j==0)
            {
                for (i=0; i < TOTAL_PSC_SSC_SC_NUM ; i++)
                  *(output_temp++) = *(PSCH_data_buffer_temp++);
            }
            if (PBCHPresentFlag == 1)
            {
                /*ӳ���ǰ������߶˿�0~3�ϵĵ�Ƶ������ӳ��*/
                PBCHCarrierIdx = (unsigned char*)(PBCH_index[(ncell_id % 3)]);

                if ((OFDMA_idx_l == 7 ) || (OFDMA_idx_l == 8 ))
                {
                    for (i = 0; i< TOTAL_PBCH_SC_NUM ;i = i + 6)
                    {
                        *(output_temp + PBCHCarrierIdx[0]) = 
                                                 *(PBCH_data_buffer_temp++);
                        *(output_temp + PBCHCarrierIdx[1]) = 
                                                 *(PBCH_data_buffer_temp++);
                        *(output_temp + PBCHCarrierIdx[2]) = 
                                                 *(PBCH_data_buffer_temp++);
                        *(output_temp + PBCHCarrierIdx[3]) = 
                                                 *(PBCH_data_buffer_temp++);
                                             
                        output_temp += 6;     
                                             
                    }
                        
                }
                else if ((OFDMA_idx_l == 9 ) || (OFDMA_idx_l == 10 ))
                {
                    for (i = 0; i < TOTAL_PBCH_SC_NUM; i++)
                     *(output_temp++) = *(PBCH_data_buffer_temp++);  
                }
                
            }
        }
     }
        
    }/* end of antenna loop*/

    return (input_data_used);
}


void prbmapping( unsigned char slot_idx,/*һ������֡�е�ʱ϶����������Ϊż��0,2,4~18*/
                 USER_PARA *user_para_ptr,
                 CELL_PARA *cell_para_ptr,
                 unsigned char * restrict rsgen_output_buffer[3],/*3symbol*220char,�洢����ʵ�鲿��ͬ����*/
                 signed int * restrict SCH_data_buffer[NUM_ANTENNA_PORT_2],/*SCH_data_buffer[ant_port][���ƺ�����з���]*/
                 signed int * restrict output_data[NUM_ANTENNA_PORT_2],/*һ����֡output_data[ant_port][14*100*12]*/
                 signed int * restrict PBCH_data_buffer[NUM_ANTENNA_PORT_2],
                 signed int * restrict PSCH_data_buffer,
                 signed int * restrict SSCH_data_buffer,
                 unsigned char total_symbol_num,
                 unsigned char user_index
                )
{
    signed int * restrict output_temp;
    signed int * restrict output_temp1;
    signed short rho_a_b[2];
    unsigned int input_data_used = 0;
    unsigned char power_offset;
    
    unsigned short len;
    unsigned char OFDMA_idx_l;
    unsigned char m_dash;
    unsigned char t;
    unsigned char four_ant_transmit_div_flag = 0;
    unsigned short Ncell_id = cell_para_ptr->cellID;
    unsigned char CFI = cell_para_ptr->CFI;
    unsigned char num_PRB_per_bw_config = cell_para_ptr->num_PRB_per_bw_config;
    unsigned char num_antenna_port = cell_para_ptr->num_antenna_port;
    unsigned char diversity_mode = user_para_ptr->ueTransMode;
    unsigned char P_A = user_para_ptr->P_A;
    unsigned char P_B = cell_para_ptr->P_B;
    unsigned char slot_index;
    unsigned volatile short *reg_table_ptr;
 
    
    /*PA/PB=1*/    
    if (((num_antenna_port == NUM_ANTENNA_PORT_2) || (num_antenna_port == NUM_ANTENNA_PORT_4)) && (P_B == 1))
    {
        rho_a_b[0] = 8192; /* 1 Q(16,3) */
        rho_a_b[1] = 8192; /* 1 Q(16,3) */
    }
    /*PA/PB=1*/
    else if ((num_antenna_port == NUM_ANTENNA_PORT_1) && (P_B == 0))
    {
        rho_a_b[0] = 8192; /* 1 Q(16,3) */
        rho_a_b[1] = 8192; /* 1 Q(16,3) */
    }
    else
    {
        if ((num_antenna_port == NUM_ANTENNA_PORT_4) && (diversity_mode == TRANSMIT_DIVERSITY))
        {
            four_ant_transmit_div_flag = 1;
            
        }
        power_offset = 0;       
        /*PA/PB~=1ʱ��Ҫ����P_B��Ӧ�ı��� */        
        rho_a_b[0] = rho_a_lut[power_offset][four_ant_transmit_div_flag][P_A];
        rho_a_b[1] = (rho_a_b[0] * ratio_b_lut[num_antenna_port - 1][P_B]) >> 15;/*Q(16,3)*/
        rho_a_b[0] = rho_a_b[0] >> 1; /*Q(16,3)*/
    }

    
    /* CRS prbmapping */
    m_dash = (unsigned char)(N_MAX_DL_RB - num_PRB_per_bw_config);
    
    if(0 == user_index)//��һ���û�Ҫ����Ƶӳ�䣬�����û��Ͳ���Ҫ��
    {
        for (OFDMA_idx_l = 0; OFDMA_idx_l < total_symbol_num; OFDMA_idx_l++)
        {
            /* ���˿ں����˿ڵķ���0*/
            slot_index = OFDMA_idx_l / 7;
            if ((OFDMA_idx_l % N_SYM_PER_SLOT) == 0)
            {
                t = m_dash;
                output_temp = output_data[0] + (num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l);
                /* mapping on port 0 */
                for (len = 0; len < num_PRB_per_bw_config; len++)
                {
                    *(output_temp + PilotInRBK1[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3][t++]];
                    *(output_temp + PilotInRBK1[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3][t++]];
                    output_temp += 12;
                }
                /* mapping on port 1 */
                if (num_antenna_port > NUM_ANTENNA_PORT_1)
                {
                    output_temp = output_data[1] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                    t = m_dash;
                    for (len = 0; len < num_PRB_per_bw_config; len++)
                    {
                        *(output_temp + PilotInRBK2[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3][t++]];
                        *(output_temp + PilotInRBK2[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3][t++]];
                        output_temp += 12;
                    }
                }
                
            }
            /* ���˿ں����˿ڵķ���4*/
            else if ((OFDMA_idx_l % N_SYM_PER_SLOT) == (N_SYM_PER_SLOT - 3))
            {
                t = m_dash;
                /* mapping on port 0 */
                output_temp = output_data[0] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                for (len = 0; len < num_PRB_per_bw_config; len++)
                {
                    *(output_temp + PilotInRBK2[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 2][t++]];
                    *(output_temp + PilotInRBK2[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 2][t++]];
                    output_temp += 12;
                }
                /* mapping on port 1 */
                if (num_antenna_port > NUM_ANTENNA_PORT_1)
                {
                    output_temp = output_data[1] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                    t = m_dash;
                    for (len = 0; len < num_PRB_per_bw_config; len++)
                    {
                        *(output_temp + PilotInRBK1[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 2][t++]];
                        *(output_temp + PilotInRBK1[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 2][t++]];
                        output_temp += 12;
                    }
                }
            }
            /* �Ķ˿�*/
            else if (num_antenna_port == NUM_ANTENNA_PORT_4)
            {
                /* ����1*/
                if ((OFDMA_idx_l % (total_symbol_num)) == 1)
                {
                    t = m_dash;
                    /*mapping on port 3 and port 4*/
                    output_temp = output_data[2] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                    output_temp1 = output_data[3] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                    for (len = 0; len < num_PRB_per_bw_config; len++)
                    {
                        *(output_temp + PilotInRBK1[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 1][t]];
                        *(output_temp1+ PilotInRBK2[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 1][t++]];
                        *(output_temp + PilotInRBK1[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 1][t]];
                        *(output_temp1+ PilotInRBK2[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[slot_index * 3 + 1][t++]];
                        output_temp +=12;
                        output_temp1 +=12;
                    }
                }
                /* ����8*/
                else if ((OFDMA_idx_l % (total_symbol_num)) == 8)
                {
                    
                    t = m_dash;
                    
                    /*mapping on port 3 and port 4*/
                    output_temp = output_data[2] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                    output_temp1 = output_data[3] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
            
                    for (len = 0; len < num_PRB_per_bw_config; len++)
                    {
                        *(output_temp + PilotInRBK2[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[1][t]];
                        *(output_temp1 + PilotInRBK1[Ncell_id % 6][0]) = rs_lut[rsgen_output_buffer[1][t++]];
                        *(output_temp + PilotInRBK2[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[1][t]];
                        *(output_temp1 + PilotInRBK1[Ncell_id % 6][1]) = rs_lut[rsgen_output_buffer[1][t++]];
                        output_temp +=12;
                        output_temp1 +=12;
                    }
                }
            }
        
        
            if(OFDMA_idx_l >= CFI)
            {
                /*���ƺ�ķ����ǽ�������*/
                if (num_antenna_port == NUM_ANTENNA_PORT_4)
                {
                    SCH_data_buffer[0] += input_data_used;
                    SCH_data_buffer[1] += input_data_used;
                    SCH_data_buffer[2] += input_data_used;
                    SCH_data_buffer[3] += input_data_used;
                }
                else if (num_antenna_port == NUM_ANTENNA_PORT_2)
                {
                    SCH_data_buffer[0] += input_data_used;
                    SCH_data_buffer[1] += input_data_used;
                }
                else if (num_antenna_port == NUM_ANTENNA_PORT_1)
                {
                    SCH_data_buffer[0] += input_data_used;
                }
                input_data_used = pdsch_psc_ssc_pbch_prbmapping(user_para_ptr,
                                                                cell_para_ptr,
                                                                slot_idx,
                                                                OFDMA_idx_l,
                                                                (signed int **)SCH_data_buffer,
                                                                (signed int **)output_data,
                                                                &rho_a_b[0],
                                                                PBCH_data_buffer,
                                                                PSCH_data_buffer,
                                                                SSCH_data_buffer,
                                                                user_index
                                                                );
           
            
                /* give offset to PBCH input buffer for next symbol */
                if ((OFDMA_idx_l == 7 ) || (OFDMA_idx_l == 8 ))
                {
                    if (num_antenna_port == NUM_ANTENNA_PORT_2)
                    {
                        PBCH_data_buffer[0] += N_PBCH_SYM_7_8;
                        PBCH_data_buffer[1] += N_PBCH_SYM_7_8;
                    }
                    else if (num_antenna_port == NUM_ANTENNA_PORT_1)
                    {
                        PBCH_data_buffer[0] += N_PBCH_SYM_7_8;
                    }
           
                }
                else if (OFDMA_idx_l == 9 )
                {
                    if (num_antenna_port == NUM_ANTENNA_PORT_2)
                    {
                        PBCH_data_buffer[0] += N_PBCH_DATA_SC;
                        PBCH_data_buffer[1] += N_PBCH_DATA_SC;
                    }
                    else if (num_antenna_port == NUM_ANTENNA_PORT_1)
                    {
                        PBCH_data_buffer[0] += N_PBCH_DATA_SC;
                    }                  
                }
            }
            else
            {
                if(0 == OFDMA_idx_l)
                {
                    //PCFICH
                    output_temp = output_data[0] + (num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l);
                    for (len = 0; len < 16; len++)
                    {
                        output_temp[g_dl_resource_mapping.pcfich_res[len]] = g_pcfich_data_after_precoding[0][len];
                    }
                    /* mapping on port 1 */
                    if (num_antenna_port > NUM_ANTENNA_PORT_1)
                    {
                        output_temp = output_data[1] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                        //PCFICH
                        for (len = 0; len < 16; len++)
                        {
                            output_temp[g_dl_resource_mapping.pcfich_res[len]] = g_pcfich_data_after_precoding[1][len];
                        }
                    }
                    //PHICH
                    output_temp = output_data[0] + (num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l);
                    reg_table_ptr = &g_phich_data.phich_mapping[slot_idx >> 1][0];
                    for (len = 0; len < 12 * g_N_group_phich[slot_idx>>1]; len++)
                    {
                        output_temp[reg_table_ptr[len]] = g_phich_data_after_precoding[0][len];
                    }
                    /* mapping on port 1 */
                    if (num_antenna_port > NUM_ANTENNA_PORT_1)
                    {
                        output_temp = output_data[1] + ((num_PRB_per_bw_config) * (N_SC_PER_RB) * (OFDMA_idx_l));
                        //PCFICH
                        for (len = 0; len < 12 * g_N_group_phich[slot_idx>>1]; len++)
                        {
                            output_temp[reg_table_ptr[len]] = g_phich_data_after_precoding[1][len];
                        }
                    }
                }
                
            }
        }
        reg_table_ptr = &g_dl_resource_mapping.pdcch_mapping_carrier[slot_idx >> 1][0];
        if (num_antenna_port == NUM_ANTENNA_PORT_2)
        {
            output_temp = output_data[0];
            output_temp1 = output_data[1];
            for (len = 0; len < cell_para_ptr->reg_unused_num[slot_idx >> 1]; len++)
            {
                output_temp[reg_table_ptr[4 * len + 0]] = g_pdcch_data_after_interleaver[0][4 * len + 0];
                output_temp[reg_table_ptr[4 * len + 1]] = g_pdcch_data_after_interleaver[0][4 * len + 1];
                output_temp[reg_table_ptr[4 * len + 2]] = g_pdcch_data_after_interleaver[0][4 * len + 2];
                output_temp[reg_table_ptr[4 * len + 3]] = g_pdcch_data_after_interleaver[0][4 * len + 3];
                output_temp1[reg_table_ptr[4 * len + 0]] = g_pdcch_data_after_interleaver[1][4 * len + 0];
                output_temp1[reg_table_ptr[4 * len + 1]] = g_pdcch_data_after_interleaver[1][4 * len + 1];
                output_temp1[reg_table_ptr[4 * len + 2]] = g_pdcch_data_after_interleaver[1][4 * len + 2];
                output_temp1[reg_table_ptr[4 * len + 3]] = g_pdcch_data_after_interleaver[1][4 * len + 3];
            }
        }
    }
    else
    { 
        for (OFDMA_idx_l = CFI; OFDMA_idx_l < total_symbol_num; OFDMA_idx_l++)
        {        
             /*���ƺ�ķ����ǽ�������*/
             if (num_antenna_port == NUM_ANTENNA_PORT_4)
             {
                 SCH_data_buffer[0] += input_data_used;
                 SCH_data_buffer[1] += input_data_used;
                 SCH_data_buffer[2] += input_data_used;
                 SCH_data_buffer[3] += input_data_used;
             }
             else if (num_antenna_port == NUM_ANTENNA_PORT_2)
             {
                 SCH_data_buffer[0] += input_data_used;
                 SCH_data_buffer[1] += input_data_used;
             }
             else if (num_antenna_port == NUM_ANTENNA_PORT_1)
             {
                 SCH_data_buffer[0] += input_data_used;
             }
             input_data_used = pdsch_psc_ssc_pbch_prbmapping(user_para_ptr,
                                                             cell_para_ptr,
                                                             slot_idx,
                                                             OFDMA_idx_l,
                                                             (signed int **)SCH_data_buffer,
                                                             (signed int **)output_data,
                                                             &rho_a_b[0],
                                                             PBCH_data_buffer,
                                                             PSCH_data_buffer,
                                                             SSCH_data_buffer,
                                                             user_index
                                                             );


        }
    }

}

