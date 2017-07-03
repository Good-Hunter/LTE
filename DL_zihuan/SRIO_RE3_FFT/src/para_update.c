/***********************************************/
/*para_update.c                                */
/*功能：小区和用户参数更新                     */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"
#include "PHY_PHYADP_interfaceV3.h"

unsigned volatile int ue_index=0;
unsigned volatile short  sfn;
unsigned volatile short  slot_idx;
unsigned volatile short  subframeN;

unsigned volatile int type3=0;
extern unsigned int memory_start;

extern volatile UEPHYADPtoPHYType1 mactophytype1[PARA_LEN];
extern volatile UEPHYADPtoPHYType3 mactophytype3;

extern unsigned int previous_RNTI;
extern unsigned int current_RNTI;
extern unsigned volatile int gxx_pbch_process_flag;

extern void get_flag();
extern void read_cell_para_config(FILE* fp,
        CELL_PARA* cell_para_ptr);
extern void read_user_para_config(FILE* fp,USER_PARA* user_para_ptr,CELL_PARA* cell_para_ptr);
#ifdef SINGLE_USER
void para_init(CONFIG_PARA* config_para_ptr)
{
    char i;
    vxx_config_para_init.num_code_word = NUM_CODEWORD_2;

    vxx_config_para_init.ueTransMode = SPATIAL_MULTIPLEXING;
    vxx_config_para_init.modulation = QAM64;
    vxx_config_para_init.rnti = 61;
    vxx_config_para_init.common_rnti = 1;
    vxx_config_para_init.num_PRB = 100;
    vxx_config_para_init.rvIdx = 0;
    vxx_config_para_init.tbSize = 75376;
    vxx_config_para_init.tbSize_subframe0 = 63776;//73712;
    vxx_config_para_init.tbSize_subframe1 = 55056;
    for(i = 0;i < 10;i++)
    {
      vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
    }
    vxx_config_para_init.phich_Durtion = 0;
    vxx_config_para_init.g_phich_Ng = 0;
    vxx_config_para_init.frame_config = 2;
    vxx_config_para_init.TM_mode = 3;
    vxx_config_para_init.n_group = 0;
    vxx_config_para_init.n_seq = 0;
}
#endif
#ifdef TWO_USER
void para_init(CONFIG_PARA* config_para_ptr)
{
    char i;
    vxx_config_para_init.num_code_word = NUM_CODEWORD_1;

    vxx_config_para_init.ueTransMode = TRANSMIT_DIVERSITY;
    vxx_config_para_init.modulation = QAM16;
    vxx_config_para_init.rnti = 62;
    vxx_config_para_init.common_rnti = 1;
    vxx_config_para_init.num_PRB = 48;
    vxx_config_para_init.rvIdx = 0;
    vxx_config_para_init.tbSize = 13536;
    vxx_config_para_init.tbSize_subframe0 = 13536;
    vxx_config_para_init.tbSize_subframe1 = 8504;
    for(i = 0;i < 10;i++)
    {
      vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
    }
    vxx_config_para_init.phich_Durtion = 0;
    vxx_config_para_init.g_phich_Ng = 0;
    vxx_config_para_init.frame_config = 2;
    vxx_config_para_init.TM_mode = 3;
    vxx_config_para_init.bitmap = 0x000fff00;//与num_PRB配合使用,表示起始prb是48
}
#endif
#ifdef TEN_USER
void para_init(CONFIG_PARA* config_para_ptr)
{
    char i;
    vxx_config_para_init.num_code_word = NUM_CODEWORD_1;
    vxx_config_para_init.rnti = 65;
    vxx_config_para_init.common_rnti = 1;


    for(i = 0;i < 10;i++)
    {
      vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
    }
    vxx_config_para_init.phich_Durtion = 0;
    vxx_config_para_init.g_phich_Ng = 0;
    vxx_config_para_init.frame_config = 2;
    vxx_config_para_init.TM_mode = 3;
    vxx_config_para_init.n_group = 1;
    vxx_config_para_init.n_seq = 1;
}
#endif
//五用户添加
#ifdef FIVE_USER
void para_init(CONFIG_PARA* config_para_ptr,CELL_PARA* cell_para_ptr)
{
	char i;
	CACHE_invL2((void *)&mactophytype1[ue_index], sizeof(mactophytype1[ue_index]), CACHE_WAIT);
	if(mactophytype1[ue_index].GHead.NewFlag==1)
	{
		vxx_config_para_init.rnti =mactophytype1[ue_index].DL_TYPE1_PDCCH_C.RNTI;
		vxx_config_para_init.common_rnti = 1;
		vxx_config_para_init.ueCategory = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.UeCategory+1;
		vxx_config_para_init.delay_mod = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.DelayMod;
		vxx_config_para_init.P_A = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.PA;

		mactophytype1[ue_index].GHead.NewFlag = 0;
		CACHE_wbL2((void *)&mactophytype1[ue_index].GHead.NewFlag, sizeof(mactophytype1[ue_index].GHead.NewFlag), CACHE_WAIT);

	}
#if 0
	else
	{
		vxx_config_para_init.rnti =61;
	    vxx_config_para_init.common_rnti = 1;
		vxx_config_para_init.ueCategory = 4;
		vxx_config_para_init.delay_mod = LARGE_DELAY_CDD;
		vxx_config_para_init.P_A = 4;
	}
#endif
	current_RNTI=vxx_config_para_init.rnti;

	CACHE_invL2((void *)&mactophytype3, sizeof(mactophytype3), CACHE_WAIT);
	if(mactophytype3.GHead.NewFlag==1)
	{
	    for(i = 0;i < 10;i++)
	    {
	    	vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];

	    }
		//vxx_config_para_init.subframe_direction[slot_idx>>1] = mactophytype3.DL_TYPE3_PDCCH_C.SFDirection;
		vxx_config_para_init.TM_mode = mactophytype3.DL_TYPE3_PDCCH_C.T_MODE;
		vxx_config_para_init.frame_config = 2;
		vxx_config_para_init.phich_Durtion = 0;
		vxx_config_para_init.g_phich_Ng = 0;
		vxx_config_para_init.n_group = 0;
		vxx_config_para_init.n_seq = 0;
		CACHE_invL2((void *)&mactophytype3.DL_TYPE3_PDCCH_C.MemStart, sizeof(mactophytype3.DL_TYPE3_PDCCH_C.MemStart), CACHE_WAIT);
		memory_start = mactophytype3.DL_TYPE3_PDCCH_C.MemStart;
		CACHE_wbL2((void *)&memory_start, sizeof(memory_start), CACHE_WAIT);

		get_flag();
		//gxx_pbch_process_flag=mactophytype3.DL_TYPE3_PDSCH_C.pbch_process_flag;
		//CACHE_wbL2((void*)&gxx_pbch_process_flag,sizeof(gxx_pbch_process_flag),CACHE_WAIT);

		mactophytype3.GHead.NewFlag = 0;
		CACHE_wbL2((void *)&mactophytype3.GHead.NewFlag, sizeof(mactophytype3.GHead.NewFlag), CACHE_WAIT);
		type3++;
	}
#if 0
	else
	{
		for(i = 0;i < 10;i++)
		{
		    vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
		}
		vxx_config_para_init.TM_mode = 3;
	    vxx_config_para_init.frame_config = 2;//该参数从何处获取？
	    vxx_config_para_init.phich_Durtion = 0;
		vxx_config_para_init.g_phich_Ng = 0;
		vxx_config_para_init.n_group = 0;
		vxx_config_para_init.n_seq = 0;


	}
#endif
}







    //vxx_config_para_init.num_code_word = NUM_CODEWORD_1;//去掉？在DCI中会更新
#if 0//type1参数更新，2015.3.25增加
    ue_index = ue_index%PARA_LEN;
    while(mactophytype1[ue_index].GHead.NewFlag==0)
    {
    	CACHE_invL2((void *)&mactophytype1[ue_index], sizeof(mactophytype1[ue_index]), CACHE_WAIT);
    }

	sfn = mactophytype1[ue_index].GHead.SFN;
	subframeN = mactophytype1[ue_index].GHead.SubframeN;
	slot_idx = subframeN*2;

    vxx_config_para_init.rnti =mactophytype1[ue_index].DL_TYPE1_PDCCH_C.RNTI;
    vxx_config_para_init.common_rnti = 1;

    vxx_config_para_init.ueCategory = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.UeCategory+1;
    vxx_config_para_init.delay_mod = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.DelayMod;
    vxx_config_para_init.P_A = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.PA;

    mactophytype1[ue_index].GHead.NewFlag = 0;
    CACHE_wbL2((void *)&mactophytype1[ue_index].GHead.NewFlag, sizeof(mactophytype1[ue_index].GHead.NewFlag), CACHE_WAIT);

    ue_index++;

    /*********Type3参数更新**********/
    while(mactophytype3.GHead.NewFlag==0)
    {
    	CACHE_invL2((void *)&mactophytype3, sizeof(mactophytype3), CACHE_WAIT);
    }

    vxx_config_para_init.subframe_direction[slot_idx>>1] = mactophytype3.DL_TYPE3_PDCCH_C.SFDirection;
    vxx_config_para_init.TM_mode = mactophytype3.DL_TYPE3_PDCCH_C.T_MODE;

    CACHE_invL2((void *)&mactophytype3.DL_TYPE3_PDCCH_C.MemStart, sizeof(mactophytype3.DL_TYPE3_PDCCH_C.MemStart), CACHE_WAIT);
    memory_start = mactophytype3.DL_TYPE3_PDCCH_C.MemStart;
    CACHE_wbL2((void *)&memory_start, sizeof(memory_start), CACHE_WAIT);

    mactophytype3.GHead.NewFlag = 0;
    CACHE_wbL2((void *)&mactophytype3.GHead.NewFlag, sizeof(mactophytype3.GHead.NewFlag), CACHE_WAIT);

#endif

#if 0
    /**********Type1************/
    vxx_config_para_init.rnti =61;
    vxx_config_para_init.common_rnti = 1;

    //2015.3.25添加
    vxx_config_para_init.ueCategory = 4;
    vxx_config_para_init.delay_mod = LARGE_DELAY_CDD;
    vxx_config_para_init.P_A = 4;

    /******Type3参数*******/
    for(i = 0;i < 10;i++)
    {
      vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
    }
    vxx_config_para_init.TM_mode = 3;
    vxx_config_para_init.frame_config = 2;//该参数从何处获取？
    vxx_config_para_init.frame_config = 2;//该参数从何处获取？
    vxx_config_para_init.phich_Durtion = 0;
    vxx_config_para_init.g_phich_Ng = 0;
    vxx_config_para_init.n_group = 0;
    vxx_config_para_init.n_seq = 0;
}
#endif
#endif
#if 0
void cell_para_update(CELL_PARA* cell_para_ptr,
                      unsigned char * restrict rsgen_output_buffer,
                      unsigned int * PSCH_data_buffer,
                      unsigned int * SSCH_data_subframe0_buffer,
                      unsigned int * SSCH_data_subframe5_buffer)
{
    unsigned char n_id_1;
    unsigned char n_id_2;
    FILE* pTestCfgFile;
    unsigned char i;
    

        /* 按照默认参数配置 */
        cell_para_ptr->cellID = 0;
        cell_para_ptr->N_cp = NORMAL_CP;
        cell_para_ptr->num_antenna_port = NUM_ANTENNA_PORT_2;
        cell_para_ptr->num_PRB_per_bw_config = 100;
        cell_para_ptr->CFI = 1;
        for(i = 0;i < 10;i++)
        {
          cell_para_ptr->subframe_direction[i] = vxx_config_para_init.subframe_direction[i];
        }
        cell_para_ptr->dwpts_symbol = 10;/*config 7*/
        cell_para_ptr->P_B = 1;


    if (cell_para_ptr->num_PRB_per_bw_config >= 64)
        cell_para_ptr->RBG_size = 4;/* as RBG = 4RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config >= 27)
        cell_para_ptr->RBG_size = 3; /* as RBG = 3RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config >= 11)
        cell_para_ptr->RBG_size = 2; /* as RBG = 2RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config <= 10)
        cell_para_ptr->RBG_size = 1; /* as RBG = 1RB*/
        
    /* set the start and end SC value occupied by PBCH & SCH on a port */
    cell_para_ptr->PBCHSyncSCStart = ((100*N_SC_PER_RB)/2) - 36;
    cell_para_ptr->PBCHSyncSCEnd = ((100*N_SC_PER_RB)/2) + 35;

    rs_sequences_gen(rsgen_output_buffer,/*最多17slot*3symbol*220char,存储的是实虚部共同索引*/
                     cell_para_ptr->cellID,
                     cell_para_ptr->N_cp
                     );
    n_id_1 = cell_para_ptr->cellID / 3;
    n_id_2 = cell_para_ptr->cellID % 3;
    PSCH_data_buffer = &psc_data[n_id_2 * TOTAL_PSC_SSC_SC_NUM];
    SSCH_data_subframe0_buffer = &ssc_subframe0_data[n_id_1 * TOTAL_PSC_SSC_SC_NUM];
    SSCH_data_subframe5_buffer = &ssc_subframe5_data[n_id_1 * TOTAL_PSC_SSC_SC_NUM];
}
#endif
void cell_para_update(CELL_PARA* cell_para_ptr)
{
    unsigned char i;
    

        /* 按照默认参数配置 */
        cell_para_ptr->cellID = 0;
        cell_para_ptr->N_cp = NORMAL_CP;
        cell_para_ptr->num_antenna_port = NUM_ANTENNA_PORT_2;
        cell_para_ptr->CFI = 1;
        cell_para_ptr->num_PRB_per_bw_config = N_DL_RB;
        switch(cell_para_ptr->num_PRB_per_bw_config)
        {
            case 100:
              cell_para_ptr->dl_Bandwidth = 5;
              break;
            case 75:
                cell_para_ptr->dl_Bandwidth = 4;
                break;
            case 50:
                cell_para_ptr->dl_Bandwidth = 3;
                break;
            case 25:
                cell_para_ptr->dl_Bandwidth = 2;
                break;
            case 15:
                cell_para_ptr->dl_Bandwidth = 1;
                break;
            case 6:
                cell_para_ptr->dl_Bandwidth = 0;
                break;
        }
        for(i = 0;i < 10;i++)
        {
          cell_para_ptr->subframe_direction[i] = vxx_config_para_init.subframe_direction[i];
        }
        cell_para_ptr->dwpts_symbol = 10;/*config 7*/
        cell_para_ptr->P_B = 1;

    if (cell_para_ptr->num_PRB_per_bw_config >= 64)
        cell_para_ptr->RBG_size = 4;/* as RBG = 4RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config >= 27)
        cell_para_ptr->RBG_size = 3; /* as RBG = 3RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config >= 11)
        cell_para_ptr->RBG_size = 2; /* as RBG = 2RB*/
    else if (cell_para_ptr->num_PRB_per_bw_config <= 10)
        cell_para_ptr->RBG_size = 1; /* as RBG = 1RB*/


    /* set the start and end SC value occupied by PBCH & SCH on a port */
    cell_para_ptr->PBCHSyncSCStart = ((cell_para_ptr->num_PRB_per_bw_config*N_SC_PER_RB)/2) - 36;
    cell_para_ptr->PBCHSyncSCEnd = ((cell_para_ptr->num_PRB_per_bw_config*N_SC_PER_RB)/2) + 35;
    cell_para_ptr->phich_Durtion = vxx_config_para_init.phich_Durtion;
    cell_para_ptr->g_phich_Ng = vxx_config_para_init.g_phich_Ng;
    cell_para_ptr->frame_config = vxx_config_para_init.frame_config;
    cell_para_ptr->TM_mode = vxx_config_para_init.TM_mode;
   // rs_sequences_gen(rsgen_output_buffer,/*最多17slot*3symbol*220char,存储的是实虚部共同索引*/
    //                 cell_para_ptr->cellID,
    //                 cell_para_ptr->N_cp
    //                 );*/


}

unsigned int user_para_update(USER_PARA* user_para_ptr,
                              unsigned char slot_idx,
                              CELL_PARA* cell_para_ptr,
                              DL_phich_para_struct *phich_para_ptr)
{

    
    /*12-4=8:导频所在prb中PDSCH可用子载波个数 rs_symbol:导频符号个数（除去CFI之后的）
    6：PSCH占用的PAB个数
    1:PSCH占用符号个数*/
    
#if 0    
    if((slot_idx == 2) || (slot_idx == 12))/*特殊子帧子帧存在PSCH*/
    {
        if(cell_para_ptr->dwpts_symbol < 11)
        {
            rs_symbol = 2;
        }
        else
        {
            rs_symbol = 3;
        }
        total_symbol = cell_para_ptr->dwpts_symbol;
        pbch_ssyc_symbol = 1;
        pbch_symbol7 = 0;
        user_para_ptr[0].tbSize = vxx_config_para_init.tbSize_subframe1;/*dwpts为10个符号*/
        /*PSCH借用PBCH的位置索引*/
        if(cell_para_ptr->PBCHSyncSCStart > vxx_config_para_init.num_PRB * N_SC_PER_RB - 1)
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               vxx_config_para_init.num_PRB * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB;
        }
        else if((cell_para_ptr->PBCHSyncSCStart <= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1) && (cell_para_ptr->PBCHSyncSCEnd >= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1))
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               47 * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB;
        }
        else
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               (vxx_config_para_init.num_PRB - TOTAL_PBCH_SC_NUM / N_SC_PER_RB) * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB - pbch_symbol7;
        }
        
        
        
    }
    else if(slot_idx == 10)/*SSCH存在slot11*/
    {
        rs_symbol = 3;
        total_symbol = 2 * N_SYM_PER_SLOT;
        pbch_ssyc_symbol = 1;
        pbch_symbol7 = 0;
        user_para_ptr[0].tbSize = vxx_config_para_init.tbSize;
        if(cell_para_ptr->PBCHSyncSCStart > vxx_config_para_init.num_PRB * N_SC_PER_RB - 1)
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               vxx_config_para_init.num_PRB * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB;
        }
        else if((cell_para_ptr->PBCHSyncSCStart <= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1) && (cell_para_ptr->PBCHSyncSCEnd >= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1))
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               47 * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB;
        }
        else
        {
               user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
               (vxx_config_para_init.num_PRB - TOTAL_PBCH_SC_NUM / N_SC_PER_RB) * N_SC_PER_RB * pbch_ssyc_symbol \
               +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB - pbch_symbol7;
        }
        
    }
    else if(slot_idx == 0)/*PBCH和SSCH存在slot0*/
    {
        rs_symbol = 3;
        total_symbol = 2 * N_SYM_PER_SLOT;
        pbch_ssyc_symbol = 4;
        pbch_symbol7 = 48;/*symbol7既有RS又有PBCH*/
        user_para_ptr[0].tbSize = vxx_config_para_init.tbSize_subframe0;
        if(cell_para_ptr->PBCHSyncSCStart > vxx_config_para_init.num_PRB * N_SC_PER_RB - 1)
        {
              user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
              vxx_config_para_init.num_PRB * N_SC_PER_RB * pbch_ssyc_symbol \
              +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB;
        }
        else if((cell_para_ptr->PBCHSyncSCStart <= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1) && (cell_para_ptr->PBCHSyncSCEnd >= vxx_config_para_init.num_PRB * N_SC_PER_RB - 1))
        {
              user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
              47 * N_SC_PER_RB * pbch_ssyc_symbol \
              +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB - 8 * (vxx_config_para_init.num_PRB - 47);
        }
        else
        {
              user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
              (vxx_config_para_init.num_PRB - TOTAL_PBCH_SC_NUM / N_SC_PER_RB) * N_SC_PER_RB * pbch_ssyc_symbol \
              +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB - pbch_symbol7;
        }
    }
    else
    {
        rs_symbol = 3;
        total_symbol = 2 * N_SYM_PER_SLOT;
        pbch_ssyc_symbol = 0;
        pbch_symbol7 = 0;
        user_para_ptr[0].tbSize = vxx_config_para_init.tbSize;
        user_para_ptr->m_sybol = vxx_config_para_init.num_PRB * (N_SC_PER_RB - 4) * rs_symbol + \
        (vxx_config_para_init.num_PRB - TOTAL_PBCH_SC_NUM / N_SC_PER_RB) * N_SC_PER_RB * pbch_ssyc_symbol \
        +(total_symbol - cell_para_ptr->CFI - pbch_ssyc_symbol - rs_symbol) * vxx_config_para_init.num_PRB * N_SC_PER_RB - pbch_symbol7;

    }

    user_para_ptr[0].num_code_word =  vxx_config_para_init.num_code_word;
    user_para_ptr[0].maxNumHarqProcess = 8;
    user_para_ptr[0].ueTransMode =  vxx_config_para_init.ueTransMode;
    user_para_ptr[0].Nir = 1827072/user_para_ptr[0].maxNumHarqProcess;/*UE等级4*/
    
    user_para_ptr[0].modulation =  vxx_config_para_init.modulation;
    user_para_ptr[0].numLayers = NUM_LAYER_2;

    user_para_ptr[0].num_PRB = 0;
#if 0
    for(i = 0;i < 32;i++)
    {
        user_para_ptr[0].num_PRB += ((user_para_ptr[0].bitmap >> i) & 1) * cell_para_ptr->RBG_size;
    }
#endif
    user_para_ptr[0].num_PRB = vxx_config_para_init.num_PRB;
    user_para_ptr[0].rvIdx = vxx_config_para_init.rvIdx;
    user_para_ptr[0].bitmap = vxx_config_para_init.bitmap;
    for(i = 0;i < 32;i++)
    {
        if(1 == user_para_ptr[0].bitmap >> (31 - i))
        {
            user_para_ptr[0].prb_start = i * cell_para_ptr->RBG_size;
        }
    }
    user_para_ptr[0].numBitsG = user_para_ptr[0].m_sybol * user_para_ptr[0].modulation;
#endif


    user_para_ptr[0].rnti = vxx_config_para_init.rnti;
    user_para_ptr[0].ueCategory = vxx_config_para_init.ueCategory;
    user_para_ptr[0].delay_mod = vxx_config_para_init.delay_mod;
    user_para_ptr[0].P_A = vxx_config_para_init.P_A;
    user_para_ptr[0].codeWordIdx[0] = 0;
    user_para_ptr[0].codeWordIdx[1] = 1;
    user_para_ptr[0].tb_ptr[0] = (unsigned int)tb_buffer;
    //user_para_ptr[0].tb_ptr[1] = (unsigned int)&tb_buffer[(user_para_ptr[0].tbSize + 31)/32];
    user_para_ptr[0].tb_ptr[1] = (unsigned int)&tb_buffer[((MAX_TB_BLOCK_SIZE + 31)/32)];
    phich_para_ptr->n_group = vxx_config_para_init.n_group;
    phich_para_ptr->n_seq = vxx_config_para_init.n_seq;
    
    return 1;

}

void prb_demapping_update(
                         unsigned char  CFI,
                         signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
                         signed short* symbol_start,
                         signed int* subcarrier_demap_num,
                         signed int  prb_start,          //Para,***目前仅适用于连续分配的prb***
                         signed int  prb_num)
{

  signed int  symb_no;

  for(symb_no = CFI;symb_no < 2 * N_SYM_PER_SLOT;symb_no++)
  {
      if(subsfn == 0)    //子帧0(crs、pbch、sss)
      {
          if(0 == symb_no)    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if((7 == symb_no))     //跳过crs和pbch
          {
              if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  subcarrier_demap_num[symb_no] = (N_DL_RB/2 - 3 - prb_start) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - N_DL_RB/2 - 3) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (prb_start - 6) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
          }
          else if((8 == symb_no) || (9 == symb_no) || (10 == symb_no) || (13 == symb_no))     //跳过pbch或sss
          {
              
              if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  subcarrier_demap_num[symb_no] = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
            	  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      } 
      else if(subsfn == 5)    //子帧5(crs、sss)
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(13 == symb_no)     //跳过pbch或sss
          {
              if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  subcarrier_demap_num[symb_no] = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }    
      else if((subsfn == 1) || (subsfn == 6))     //子帧1、6(crs、pss)  
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(2 == symb_no)     //跳过pss
          {
              if((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1)<= N_DL_RB/2 - 3))  
              {
                  subcarrier_demap_num[symb_no] = (N_DL_RB/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > N_DL_RB/2 - 3) && ((prb_start + 1 + prb_num - 1) <= N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3)
              && ((prb_start + 1) > N_DL_RB/2 - 3) && ((prb_start + 1) <= N_DL_RB/2 + 3))  
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - N_DL_RB/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (N_DL_RB/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) > N_DL_RB/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  N_DL_RB/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }    
      else    //子帧3、4、8、9(crs)
      {
          if((0 == symb_no) || (7 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              
          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              //total_subcarrier_num = (prb_no - 1)*SUBCARR_NUM_PRB_DATA+freq_index-4;
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
  }
}
void prb_demapping_update_normalcp(
                         unsigned char  CFI,
                         signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
                         signed short* symbol_start,
                         signed int* subcarrier_demap_num,
                         signed int  prb_start,          //Para,***目前仅适用于连续分配的prb***
                         signed int  prb_num,
                         unsigned char cell_bw_prb)
{

  signed int  symb_no;

  for(symb_no = CFI;symb_no < 2 * N_SYM_PER_SLOT;symb_no++)
  {
      if(subsfn == 0)    //子帧0(crs、pbch、sss)
      {
          if(0 == symb_no)    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if((7 == symb_no))     //跳过crs和pbch
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (prb_start - 6) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
          }
          else if((8 == symb_no) || (9 == symb_no) || (10 == symb_no) || (13 == symb_no))     //跳过pbch或sss
          {

              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
            	  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else if(subsfn == 5)    //子帧5(crs、sss)
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(13 == symb_no)     //跳过pbch或sss
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;          //  12.23jx
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else if((subsfn == 1) || (subsfn == 6))     //子帧1、6(crs、pss)
      {
          if((0 == symb_no) || (7 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(2 == symb_no)     //跳过pss
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else    //子帧3、4、8、9(crs)
      {
          if((0 == symb_no) || (7 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if((4 == symb_no) || (11 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              //total_subcarrier_num = (prb_no - 1)*SUBCARR_NUM_PRB_DATA+freq_index-4;
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
  }
}
void prb_demapping_update_extendcp(
                         unsigned char  CFI,
                         signed int  subsfn,             //Para,本次处理的子帧号（可考虑与ns合并）
                         signed short* symbol_start,
                         signed int* subcarrier_demap_num,
                         signed int  prb_start,          //Para,***目前仅适用于连续分配的prb***
                         signed int  prb_num,
                         unsigned char cell_bw_prb)
{

  signed int  symb_no;
//扩展循环前缀CP 每个时隙的OFDM符号数变为6
  for(symb_no = CFI;symb_no < 2 * N_SYM_PER_SLOT;symb_no++)
  {
      if(subsfn == 0)    //子帧0(crs、pbch、sss)
      {
          if(0 == symb_no)    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if(3 == symb_no)   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
          }
          else if((6 == symb_no)||(9 == symb_no))     //跳过crs和pbch
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始//头尾都不在pbch
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))//头在pbch外，尾在pbch范围内
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))//头尾都在pbch内，可用为0
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))//尾在外，头在pbch内
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * SUBCARR_NUM_PRB_DATA;//可用调制符号从cell_bw_prb/2 - 3开始？为什么不是从prb_start开始？
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))//头尾都在pbch外
              {
                  subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
                  symbol_start[symb_no] = (prb_start - 6) * SUBCARR_NUM_PRB_DATA;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))//头尾范围横跨了pbch
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * SUBCARR_NUM_PRB_DATA;//少了pbch的6个RBG
                  symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              }
          }
          else if((7 == symb_no) || (8 == symb_no) || (11 == symb_no))     //跳过pbch或sss
          {

              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
            	  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else if(subsfn == 5)    //子帧5(crs、sss)
      {
          if((0 == symb_no) || (3 == symb_no)||(6 == symb_no) || (9 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if(11 == symb_no)     //跳过sss
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= N_DL_RB/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else if((subsfn == 1) || (subsfn == 6))     //子帧1、6(crs、pss)  特殊子帧
      {
          if((0 == symb_no) || (3 == symb_no)||(6 == symb_no) || (9 == symb_no))    //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if(2 == symb_no)     //跳过pss
          {
              if((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 - 3)  // prb_start必须从0开始
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1)<= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (cell_bw_prb/2 - 3 - prb_start) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) > cell_bw_prb/2 - 3) && ((prb_start + 1 + prb_num - 1) <= cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = 0;
                  symbol_start[symb_no] = 0;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3)
              && ((prb_start + 1) > cell_bw_prb/2 - 3) && ((prb_start + 1) <= cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = ((prb_start + prb_num) - cell_bw_prb/2 - 3) * N_SC_PER_RB;
                  symbol_start[symb_no] = (cell_bw_prb/2 - 3) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) > cell_bw_prb/2 + 3))
              {
                  subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
                  symbol_start[symb_no] = (prb_start - 6) * N_SC_PER_RB;
              }
              else if(((prb_start + 1 + prb_num - 1) >  cell_bw_prb/2 + 3) && ((prb_start + 1) <= cell_bw_prb/2 - 3))
              {
                  subcarrier_demap_num[symb_no] = (prb_num - 6) * N_SC_PER_RB;
                  symbol_start[symb_no] = prb_start * N_SC_PER_RB;
              }
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
      else    //子帧3、4、8、9(crs)
      {
          if((0 == symb_no) || (3 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;

          }
          else if((6 == symb_no) || (9 == symb_no))   //跳过crs
          {
              subcarrier_demap_num[symb_no] = prb_num * SUBCARR_NUM_PRB_DATA;
              symbol_start[symb_no] = prb_start * SUBCARR_NUM_PRB_DATA;
              //total_subcarrier_num = (prb_no - 1)*SUBCARR_NUM_PRB_DATA+freq_index-4;
          }
          else    //无其他信道/信号，连续拷贝
          {
              subcarrier_demap_num[symb_no] = prb_num * N_SC_PER_RB;
              symbol_start[symb_no] = prb_start * N_SC_PER_RB;
          }
      }
  }
}
void user_para_update_dci(USER_PARA* user_para_ptr,
                              unsigned char slot_idx,
                              CELL_PARA* cell_para_ptr,
                              union dci_data_format  *dci_data_ptr,
                              unsigned char dci_format)
{
    unsigned char i,j;

    
    
    
    switch(dci_format)
    {
        case Dci2:/*format 2*/         
        case Dci2A:/*format 2*/
        {
            
            user_para_ptr[0].tbSize[0] =  dci_data_ptr->dci_format_2.codeword0_tb_size;
            user_para_ptr[0].tbSize[1] =  dci_data_ptr->dci_format_2.codeword1_tb_size;
            user_para_ptr[0].num_code_word =  2;
            user_para_ptr[0].maxNumHarqProcess = 8;
            if(1 == user_para_ptr[0].num_code_word)
            {
                user_para_ptr[0].ueTransMode =  TRANSMIT_DIVERSITY;
            }
            else
            {
                user_para_ptr[0].ueTransMode =  SPATIAL_MULTIPLEXING;
            }
            user_para_ptr[0].Nir = 1827072/user_para_ptr[0].maxNumHarqProcess;/*UE等级4*/
            
            user_para_ptr[0].modulation[0] =  dci_data_ptr->dci_format_2.codeword0_qm;
            user_para_ptr[0].modulation[1] =  dci_data_ptr->dci_format_2.codeword1_qm;
            user_para_ptr[0].numLayers = dci_data_ptr->dci_format_2.layer_num;
            
            user_para_ptr[0].num_PRB = dci_data_ptr->dci_format_2.vrb_length;
            user_para_ptr[0].prb_start = dci_data_ptr->dci_format_2.vrb_start;
            user_para_ptr[0].rvIdx[0] = dci_data_ptr->dci_format_2.codeword0_rv;
            user_para_ptr[0].rvIdx[1] = dci_data_ptr->dci_format_2.codeword1_rv;
 

            break;
        }
        case Dci01A:/*format 0&1a*/
        {
            user_para_ptr[0].tbSize[0] =  dci_data_ptr->dci_format_1a.tb_size;
            user_para_ptr[0].num_code_word =  1;
            user_para_ptr[0].maxNumHarqProcess = 8;
            if(1 == user_para_ptr[0].num_code_word)
            {
                user_para_ptr[0].ueTransMode =  TRANSMIT_DIVERSITY;
            }
            else
            {
                user_para_ptr[0].ueTransMode =  SPATIAL_MULTIPLEXING;
            }
            user_para_ptr[0].Nir = 1827072/user_para_ptr[0].maxNumHarqProcess;/*UE等级4*/
            user_para_ptr[0].ndi = dci_data_ptr->dci_format_1a.ndi;
            user_para_ptr[0].modulation[0] =  dci_data_ptr->dci_format_1a.qm;
            user_para_ptr[0].numLayers = dci_data_ptr->dci_format_1a.layer_num;
            
            user_para_ptr[0].num_PRB = dci_data_ptr->dci_format_1a.vrb_length;
            user_para_ptr[0].prb_start = dci_data_ptr->dci_format_1a.vrb_start;
            user_para_ptr[0].rvIdx[0] = dci_data_ptr->dci_format_1a.rv;
            break;
        }
        case Dci1C:
        {
        	//ly 20150805添加1C，参照1a来写的更新
        	user_para_ptr[0].tbSize[0] = dci_data_ptr->dci_format_1c.tb_size;
            user_para_ptr[0].num_code_word =  1;
            user_para_ptr[0].maxNumHarqProcess = 8;
            if(1 == user_para_ptr[0].num_code_word)
            {
                user_para_ptr[0].ueTransMode =  TRANSMIT_DIVERSITY;
            }
            else
            {
                user_para_ptr[0].ueTransMode =  SPATIAL_MULTIPLEXING;
            }
            user_para_ptr[0].Nir = 1827072/user_para_ptr[0].maxNumHarqProcess;/*UE等级4*/
            user_para_ptr[0].modulation[0] =  dci_data_ptr->dci_format_1c.qm;
            user_para_ptr[0].numLayers = dci_data_ptr->dci_format_1c.layer_num;
            user_para_ptr[0].num_PRB = dci_data_ptr->dci_format_1c.vrb_length;
            user_para_ptr[0].prb_start = dci_data_ptr->dci_format_1c.vrb_start;
            user_para_ptr[0].rvIdx[0] = dci_data_ptr->dci_format_1c.rv;
            //ly 150810 添加ts0/1到disindex_PRB
            if(0 == slot_idx&1)//lycan  slot_idx已经变成子帧号了，需要通过数符号的方法来确定奇数还是偶数时隙
            {
                for(i=0;i<user_para_ptr[0].num_PRB;i++)
                {
                	user_para_ptr[0].disindex_PRB[i] = dci_data_ptr->dci_format_1c.ts0_prb_index[0];
                }
            }
            else
            {
                for(i=0;i<user_para_ptr[0].num_PRB;i++)
                {
                	user_para_ptr[0].disindex_PRB[i] = dci_data_ptr->dci_format_1c.ts1_prb_index[0];
                }
            }
        }
        default:
            break;
    }


}
void read_cell_para_config(FILE* fp,
                           CELL_PARA* cell_para_ptr)
{
    signed int         i,j;
    signed char            lineBuf[300];
    char*           ptrBuf;
  
    while (fgets (lineBuf, 300, fp) != NULL)
    {
        if ((ptrBuf = strstr(lineBuf, "="))!=NULL)
        {
            ptrBuf ++;
            if (strstr(lineBuf,"NcellID")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->cellID = i;
                 continue;
            }
    
            if (strstr(lineBuf,"Ncp")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->N_cp = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"NumAntennaPort")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->num_antenna_port = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"NDLRB")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->num_PRB_per_bw_config = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"CFI")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->CFI = i;
                 continue;
            }
            
            if (strstr(lineBuf,"UPDWONCONFIG")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 for(j = 0;j < 10;j++)
                 {
                     cell_para_ptr->subframe_direction[j] = up_down_config[i][j];
                 }

                 continue;
            }
            
            if (strstr(lineBuf,"SPECIALCONFIG")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->dwpts_symbol = special_config_normal[i][0];
                 continue;
            }
            
            if (strstr(lineBuf,"PB")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 cell_para_ptr->P_B = i;
                 continue;
            }
            
        }
    }

    return;
}
#if 0
void read_user_para_config(FILE* fp,USER_PARA* user_para_ptr,CELL_PARA* cell_para_ptr)
{
    signed int         i;
    signed char        lineBuf[300];
    char*       ptrBuf;
    
  
    while (fgets (lineBuf, 300, fp) != NULL)
    {
        if ((ptrBuf = strstr(lineBuf, "="))!=NULL)
        {
            ptrBuf ++;
            if (strstr(lineBuf,"Ncodeword")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].num_code_word = i;
                 continue;
            }
    
            if (strstr(lineBuf,"tbSize")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].tbSize = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"MaxNumHarqProcess")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].maxNumHarqProcess = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"ueTransMode")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].ueTransMode = i;
                 continue;
            
            }
            
            if (strstr(lineBuf,"Nir")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].Nir = i / user_para_ptr[0].maxNumHarqProcess;
                 continue;
            }
            
            if (strstr(lineBuf,"modulation")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 user_para_ptr[0].modulation = i;
                 continue;
            }
            
            if (strstr(lineBuf,"NUMLAYER")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].numLayers = i;
                 continue;
            }

            
            if (strstr(lineBuf,"bitmap")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].bitmap = i;
                 user_para_ptr[0].num_PRB = 0;
                 for(i = 0;i < 32;i++)
                 {
                   user_para_ptr[0].num_PRB += ((user_para_ptr[0].bitmap >> i) & 1) * cell_para_ptr->RBG_size;
                 }
                 continue;
            }
            
            if (strstr(lineBuf,"rnti")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].rnti = i;
                 continue;
            }
            
            if (strstr(lineBuf,"ueCategory")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].ueCategory = i;
                 continue;
            }
            
            if (strstr(lineBuf,"rvIdx")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].rvIdx = i;
                 continue;
            }
            
            if (strstr(lineBuf,"DelayMod")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].delay_mod = i;
                 continue;
            }
            
            if (strstr(lineBuf,"PA")!=NULL)
            {
                 sscanf(ptrBuf,"%d", &i);
                 
                 user_para_ptr[0].P_A = i;
                 continue;
            }

            
        }
    }

    return;
}
#endif

void f_dl_database_updata(CELL_PARA* cell_para_ptr)
{
    unsigned char phich_Ng_table[4] = {4,12,24,48};//{1/6,1/2,1,2} * 24
    int temp_normal,temp_extend,i,n,mi,k;
    char symbol0_rs_table[2] = {0,3};
    char symbol0_reg_k_table[8];
    unsigned short symbol0_totall_reg_num,symbol0_pdcch_reg_num;
    unsigned  char temp_map[300];
    unsigned short temp_map1[300];
    unsigned char temp_map2[300];
    unsigned short temp_reg_index = 0;
    int k_line;
    unsigned  char subfn_index = 0;
    unsigned short n0,n1,n2,n_0,n_1,n_2,group_index;

    symbol0_totall_reg_num = cell_para_ptr->num_PRB_per_bw_config * 2;//符号1没有参考信号
    symbol0_rs_table[0] = (symbol0_rs_table[0] + (cell_para_ptr->cellID % 6)) % 6;
    symbol0_rs_table[1] = (symbol0_rs_table[1] + (cell_para_ptr->cellID % 6)) % 6;
    k = 0;
    n = 0;
    for(i = 0;i < 6;i++)
    {
      if(i == symbol0_rs_table[k])
      {
          k++;
      }
      else
      {
          symbol0_reg_k_table[n++] = i;

      }
    }
    for(i = 0;i < symbol0_totall_reg_num;i++)
    {
        g_dl_resource_mapping.symbol0_reg_k[i][0] = symbol0_reg_k_table[0] + i * 6;
        g_dl_resource_mapping.symbol0_reg_k[i][1] = symbol0_reg_k_table[1] + i * 6;
        g_dl_resource_mapping.symbol0_reg_k[i][2] = symbol0_reg_k_table[2] + i * 6;
        g_dl_resource_mapping.symbol0_reg_k[i][3] = symbol0_reg_k_table[3] + i * 6;
    }
    //第二个符号
    if(2 >= cell_para_ptr->num_antenna_port)
    {
        for(i = 0;i < 3 * cell_para_ptr->num_PRB_per_bw_config;i++)
        {
            g_dl_resource_mapping.symbol1_reg_k[i][0] = 0 + i * 4;
            g_dl_resource_mapping.symbol1_reg_k[i][1] = 1 + i * 4;
            g_dl_resource_mapping.symbol1_reg_k[i][2] = 2 + i * 4;
            g_dl_resource_mapping.symbol1_reg_k[i][3] = 3 + i * 4;
        }
    }
    else//同符号0一样
    {
        memcpy(&g_dl_resource_mapping.symbol1_reg_k[0][0],&g_dl_resource_mapping.symbol0_reg_k[0][0],2 * cell_para_ptr->num_PRB_per_bw_config * 4 * sizeof(int));
    }
    //第三个符号
    for(i = 0;i < 3 * cell_para_ptr->num_PRB_per_bw_config;i++)
    {
        g_dl_resource_mapping.symbol2_reg_k[i][0] = 0 + i * 4;
        g_dl_resource_mapping.symbol2_reg_k[i][1] = 1 + i * 4;
        g_dl_resource_mapping.symbol2_reg_k[i][2] = 2 + i * 4;
        g_dl_resource_mapping.symbol2_reg_k[i][3] = 3 + i * 4;
    }
    //计算pcfich位置
    k_line = cell_para_ptr->cellID % (2 * cell_para_ptr->num_PRB_per_bw_config);//直接转换为REG
    memset(temp_map,1,symbol0_totall_reg_num);
    for(i = 0;i < 4;i++)
    {
      temp_reg_index = k_line + ((i * cell_para_ptr->num_PRB_per_bw_config) >> 1);
      temp_map[temp_reg_index] = 0;
      g_dl_resource_mapping.pcfich_res[i * 4]     = g_dl_resource_mapping.symbol0_reg_k[temp_reg_index][0];
      g_dl_resource_mapping.pcfich_res[i * 4 + 1] = g_dl_resource_mapping.symbol0_reg_k[temp_reg_index][1];
      g_dl_resource_mapping.pcfich_res[i * 4 + 2] = g_dl_resource_mapping.symbol0_reg_k[temp_reg_index][2];
      g_dl_resource_mapping.pcfich_res[i * 4 + 3] = g_dl_resource_mapping.symbol0_reg_k[temp_reg_index][3];
    }
    n = 0;
    for(i = 0;i < symbol0_totall_reg_num;i++)
    {
      if(1 == temp_map[i])//此处空闲，可继续映射PHICH
      {
        temp_map1[n] = i;
        n++;
      }
    }
    //计算PHICH位置
    temp_normal = ((phich_Ng_table[cell_para_ptr->g_phich_Ng] * cell_para_ptr->num_PRB_per_bw_config >> 3) + 23) / 24;
    temp_extend = 2 * ((phich_Ng_table[cell_para_ptr->g_phich_Ng] * cell_para_ptr->num_PRB_per_bw_config >> 3) + 23) / 24;
    n2 = cell_para_ptr->num_PRB_per_bw_config * 3;//符号2没有参考信号
    if(1 == cell_para_ptr->num_antenna_port)
    {
     n1 = cell_para_ptr->num_PRB_per_bw_config * 3;//符号1没有参考信号
     n0 = (cell_para_ptr->num_PRB_per_bw_config * 2) - 4;//去掉参考信号以及4个PCFICH reg
    }
    else if(2 == cell_para_ptr->num_antenna_port)
    {
     n1 = cell_para_ptr->num_PRB_per_bw_config * 3;//符号1没有参考信号
     n0 = (cell_para_ptr->num_PRB_per_bw_config * 2) - 4;//去掉参考信号以及4个PCFICH reg
    }
    else if(4 == cell_para_ptr->num_antenna_port)
    {
        n1 = cell_para_ptr->num_PRB_per_bw_config * 2;//符号1有参考信号
        n0 = (cell_para_ptr->num_PRB_per_bw_config * 2) - 4;//去掉参考信号以及4个PCFICH reg
    }
    else
    {
     /*alarm*/
    }
    g_dl_resource_mapping.n0 = n0;
    g_dl_resource_mapping.n1 = n1;
    g_dl_resource_mapping.n2 = n2;//符号2没有参考信号
    //计算每个子帧的下行控制信道资源位置
    for(subfn_index = 0;subfn_index < MAX_SUBFN_NUM;subfn_index++)
    {
        //计算PCFICH信道的资源位置
        memcpy(temp_map2,temp_map,symbol0_totall_reg_num);
        mi = g_const_phich_mi[cell_para_ptr->frame_config][subfn_index];
        if (0 == cell_para_ptr->N_cp)
        {
          g_N_group_phich[subfn_index] = mi * temp_normal;
        }
        else
        {
          g_N_group_phich[subfn_index] = mi * temp_extend;
        }
        
      for(group_index = 0;group_index < g_N_group_phich[subfn_index];group_index++)
      {
        if(0 == cell_para_ptr->phich_Durtion)
        {

          n_0 = (cell_para_ptr->cellID * n0 / n0 + group_index) % n0;
          n_1 = (cell_para_ptr->cellID * n0 / n0 + n0 / 3 + group_index) % n0;
          n_2 = (cell_para_ptr->cellID * n0 / n0 + 2 * n0 / 3 + group_index) % n0;
          g_dl_resource_mapping.phich_reg[subfn_index][group_index][0] = temp_map1[n_0];
          g_dl_resource_mapping.phich_reg[subfn_index][group_index][1] = temp_map1[n_1];
          g_dl_resource_mapping.phich_reg[subfn_index][group_index][2] = temp_map1[n_2];
          temp_map2[temp_map1[n_0]] = 0;
          temp_map2[temp_map1[n_1]] = 0;
          temp_map2[temp_map1[n_2]] = 0;
        }
        else//extended phich duration
        {
                //后续
        }
      }

      symbol0_pdcch_reg_num = n0 - g_N_group_phich[subfn_index] * 3;
      g_dl_resource_mapping.symbol_pdcch_reg_num[subfn_index][0] = symbol0_pdcch_reg_num;
      g_dl_resource_mapping.symbol_pdcch_reg_num[subfn_index][1] = n1;
      g_dl_resource_mapping.symbol_pdcch_reg_num[subfn_index][2] = n2;
      memcpy(&g_dl_resource_mapping.reg_unused_flag[subfn_index][0],temp_map2,symbol0_totall_reg_num);
      
      

    }


}

unsigned short f_dl_pdcch_mapping_k(unsigned short *symbol0_reg_k_ptr,
                        unsigned short *symbol1_reg_k_ptr,
                        unsigned short *symbol2_reg_k_ptr,
                        char pdcch_symbol_num,
                        char subfn_index,
                        char dl_port_num,
                        unsigned char dlrb_num,
                        unsigned short * pdcch_data_table,
                        unsigned char *reg_unused_flag_ptr
                        )
{
  unsigned char symbol_index = 0;
  unsigned short n = 0;
  unsigned short symbol0_k = 0;
  unsigned short symbol1_k = 0;
  unsigned short symbol2_k = 0;
  unsigned short out_put_index = 0;
  unsigned char *mapping_table_ptr;
  unsigned char reg_num_per_rb = 0;
  unsigned short N_reg = 0;
  if(3 == pdcch_symbol_num)
  {
    if(4 == dl_port_num)
    {
      mapping_table_ptr = &g_const_pdcch_mapping_table[0][0];
      reg_num_per_rb = 7;//3个符号加起来每个RB可以有7个REG
      N_reg = dlrb_num * 2 + dlrb_num * 2 + dlrb_num * 3;
    }
    else
    {
      mapping_table_ptr = &g_const_pdcch_mapping_table[1][0];
      reg_num_per_rb = 8;//3个符号加起来每个RB可以有8个REG
      N_reg = dlrb_num * 2 + dlrb_num * 3 + dlrb_num * 3;
    }
  }
  else if(2 == pdcch_symbol_num)
  {
    if(4 == dl_port_num)
    {
      mapping_table_ptr = &g_const_pdcch_mapping_table[2][0];
      reg_num_per_rb = 4;//2个符号加起来每个RB可以有4个REG
      N_reg = dlrb_num * 2 + dlrb_num * 2;
    }
    else
    {
      mapping_table_ptr = &g_const_pdcch_mapping_table[3][0];
      reg_num_per_rb = 5;//2个符号加起来每个RB可以有5个REG
      N_reg = dlrb_num * 2 + dlrb_num * 3;
    }
  }
  else
  {
    mapping_table_ptr = &g_const_pdcch_mapping_table[4][0];
    reg_num_per_rb = 2;//1个符号每个RB可以有2个REG
    N_reg = dlrb_num * 2;
  }

  for(n = 0;n < N_reg;n++)//给REG按编号计算其应该的时频位置
  {
    symbol_index = mapping_table_ptr[n % reg_num_per_rb];
    if(0 == symbol_index)
    {
      if(1 == reg_unused_flag_ptr[symbol0_k])
      {
        pdcch_data_table[out_put_index * 4] = symbol0_reg_k_ptr[symbol0_k * 4+ 0];
        pdcch_data_table[out_put_index * 4 + 1] = symbol0_reg_k_ptr[symbol0_k * 4+ 1];
        pdcch_data_table[out_put_index * 4 + 2] = symbol0_reg_k_ptr[symbol0_k * 4+ 2];
        pdcch_data_table[out_put_index * 4 + 3] = symbol0_reg_k_ptr[symbol0_k * 4+ 3];
        out_put_index++;
      }
      symbol0_k++;
    }
    else if(1 == symbol_index)
    {
      pdcch_data_table[out_put_index * 4] = symbol1_reg_k_ptr[symbol1_k * 4 + 0] + dlrb_num * 12;
      pdcch_data_table[out_put_index * 4 + 1] = symbol1_reg_k_ptr[symbol1_k * 4 + 1] + dlrb_num * 12;
      pdcch_data_table[out_put_index * 4 + 2] = symbol1_reg_k_ptr[symbol1_k * 4 + 2] + dlrb_num * 12;
      pdcch_data_table[out_put_index * 4 + 3] = symbol1_reg_k_ptr[symbol1_k * 4 + 3] + dlrb_num * 12;
      out_put_index++;
      symbol1_k++;
    }
    else if(2 == symbol_index)
    {
      pdcch_data_table[out_put_index * 4] = symbol2_reg_k_ptr[symbol2_k * 4 + 0] + dlrb_num * 12 * 2;
      pdcch_data_table[out_put_index * 4 + 1] = symbol2_reg_k_ptr[symbol2_k * 4 + 1] + dlrb_num * 12 * 2;
      pdcch_data_table[out_put_index * 4 + 2] = symbol2_reg_k_ptr[symbol2_k * 4 + 2] + dlrb_num * 12 * 2;
      pdcch_data_table[out_put_index * 4 + 3] = symbol2_reg_k_ptr[symbol2_k * 4 + 3] + dlrb_num * 12 * 2;
      out_put_index++;
      symbol2_k++;
    }
    else
    {
      //alarm!
    }
  }
  return out_put_index;
}

void f_gen_interleave_pattern(unsigned short *interleave_pattern_out_ptr,
                              unsigned short *buffer_ptr,
                              unsigned int D,
                              int cell_id,
                              unsigned short * pdcch_data_table_ptr,
                              unsigned short *reg_k_ptr)
{
  unsigned char m = 0;
  unsigned char P_table[32] = {1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31, 0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30};
  unsigned short *table_pattern;
  unsigned short *table_pattern1;
  unsigned char P_index = 0;
  unsigned int length = 0;
  unsigned short temp = 0;
  unsigned short line_num = 0;
  unsigned short temp_index = 0;
  unsigned int Nd = 0;
  unsigned int index,i,j;
  table_pattern = buffer_ptr;
  table_pattern1 = &buffer_ptr[900];
  line_num = (D + 31) / 32;
  length = line_num * 32;
  Nd = length - D;//头部添加冗余数据
  //第一行有NULL，需要去除
  index = 0;
  memset(table_pattern,0xffff,Nd * 2);//头部添加冗余
  j = 0;
  for(i = Nd;i < length;i++)//按列取数
  {
    table_pattern[i] = j++;
  }
  for(i = 0;i < 32;i++)//按列取数
  {
    P_index = P_table[i];
    for(m = 0;m < line_num;m++)//行
    {
      temp = table_pattern[32 * m + P_index];
      if(temp != 0xffff)//去除NULL
        {
          table_pattern1[index++] = temp;
        }
    }
  }
  //循环移位
  for(index = 0;index < D;index++)
  {
     interleave_pattern_out_ptr[index] = table_pattern1[(index + cell_id) % D];
     temp_index = table_pattern1[(index + cell_id) % D];
     //生成cce顺序图样
     pdcch_data_table_ptr[temp_index * 4] = reg_k_ptr[index * 4];
     pdcch_data_table_ptr[temp_index * 4 + 1] = reg_k_ptr[index * 4 + 1];
     pdcch_data_table_ptr[temp_index * 4 + 2] = reg_k_ptr[index * 4 + 2];
     pdcch_data_table_ptr[temp_index * 4 + 3] = reg_k_ptr[index * 4 + 3];
  }
}
//计算REG同K的对应关系
void f_dl_phich_mapping(char subfn_index,
                      char N_group,
                      unsigned short *phich_mapping_ptr)
{
    unsigned char symbol_index = 0;
    unsigned short n = 0,i = 0;
    unsigned short reg_index = 0;
    unsigned short symbol0_k = 0;
    unsigned short symbol1_k = 0;
    unsigned short symbol2_k = 0;
    unsigned short out_put_index = 0;
    unsigned char *mapping_table_ptr;
    unsigned char reg_num_per_rb = 0;
    unsigned short N_reg = 0;
    unsigned short N_cce = 0;
    unsigned short *mapping_temp_ptr;
    int reg_index0,reg_index1,reg_index2,reg_index3;

    mapping_temp_ptr = phich_mapping_ptr;
    for(n = 0;n < N_group;n++)
    {
        for(i = 0;i < 3;i++)
        {
          reg_index = g_dl_resource_mapping.phich_reg[subfn_index][n][i];
          mapping_temp_ptr[i * 4 + 0] = g_dl_resource_mapping.symbol0_reg_k[reg_index][0];
          mapping_temp_ptr[i * 4 + 1] = g_dl_resource_mapping.symbol0_reg_k[reg_index][1];
          mapping_temp_ptr[i * 4 + 2] = g_dl_resource_mapping.symbol0_reg_k[reg_index][2];
          mapping_temp_ptr[i * 4 + 3] = g_dl_resource_mapping.symbol0_reg_k[reg_index][3];
        }
        mapping_temp_ptr+=12;
    }
}


void para_init_f(CONFIG_PARA* config_para_ptr,CELL_PARA* cell_para_ptr)
{
	char i;
	CACHE_invL2((void *)&mactophytype1[ue_index], sizeof(mactophytype1[ue_index]), CACHE_WAIT);
	if(mactophytype1[ue_index].GHead.NewFlag==1)
	{
		vxx_config_para_init.rnti =mactophytype1[ue_index].DL_TYPE1_PDCCH_C.RNTI;
		vxx_config_para_init.common_rnti = 1;
		vxx_config_para_init.ueCategory = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.UeCategory+1;
		vxx_config_para_init.delay_mod = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.DelayMod;
		vxx_config_para_init.P_A = mactophytype1[ue_index].DL_TYPE1_PDSCH_C.PA;

		mactophytype1[ue_index].GHead.NewFlag = 0;
		CACHE_wbL2((void *)&mactophytype1[ue_index].GHead.NewFlag, sizeof(mactophytype1[ue_index].GHead.NewFlag), CACHE_WAIT);

	}
 else
	{
		vxx_config_para_init.rnti =63;
	    vxx_config_para_init.common_rnti = 1;
		vxx_config_para_init.ueCategory = 4;
		vxx_config_para_init.delay_mod = LARGE_DELAY_CDD;
		vxx_config_para_init.P_A = 4;
	}

	current_RNTI=vxx_config_para_init.rnti;

	CACHE_invL2((void *)&mactophytype3, sizeof(mactophytype3), CACHE_WAIT);
	if(mactophytype3.GHead.NewFlag==1)
	{
	    for(i = 0;i < 10;i++)
	    {
	    	vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];

	    }
		//vxx_config_para_init.subframe_direction[slot_idx>>1] = mactophytype3.DL_TYPE3_PDCCH_C.SFDirection;
		vxx_config_para_init.TM_mode = mactophytype3.DL_TYPE3_PDCCH_C.T_MODE;
		vxx_config_para_init.frame_config = 2;
		vxx_config_para_init.phich_Durtion = 0;
		vxx_config_para_init.g_phich_Ng = 0;
		vxx_config_para_init.n_group = 0;
		vxx_config_para_init.n_seq = 0;
		CACHE_invL2((void *)&mactophytype3.DL_TYPE3_PDCCH_C.MemStart, sizeof(mactophytype3.DL_TYPE3_PDCCH_C.MemStart), CACHE_WAIT);
		memory_start = mactophytype3.DL_TYPE3_PDCCH_C.MemStart;
		CACHE_wbL2((void *)&memory_start, sizeof(memory_start), CACHE_WAIT);

		get_flag();
		//gxx_pbch_process_flag=mactophytype3.DL_TYPE3_PDSCH_C.pbch_process_flag;
		//CACHE_wbL2((void*)&gxx_pbch_process_flag,sizeof(gxx_pbch_process_flag),CACHE_WAIT);

		mactophytype3.GHead.NewFlag = 0;
		CACHE_wbL2((void *)&mactophytype3.GHead.NewFlag, sizeof(mactophytype3.GHead.NewFlag), CACHE_WAIT);
		type3++;
	}

	else
	{
		for(i = 0;i < 10;i++)
		{
		    vxx_config_para_init.subframe_direction[i] = up_down_config[2][i];
		}
		vxx_config_para_init.TM_mode = 3;
	    vxx_config_para_init.frame_config = 2;//该参数从何处获取？
	    vxx_config_para_init.phich_Durtion = 0;
		vxx_config_para_init.g_phich_Ng = 0;
		vxx_config_para_init.n_group = 0;
		vxx_config_para_init.n_seq = 0;


	}

}
