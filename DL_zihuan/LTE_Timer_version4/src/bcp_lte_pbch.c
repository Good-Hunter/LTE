/***********************************************/
/*bcp_lte_pbch                                */
/*功能：完成BCP相关配置                        */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"


#if 0

/*配置速率匹配参数*/
static void prepare_lte_rmhdr_cfg 
(
    Bcp_RmHdr_LteCfg*           pLteRmHdrCfg, 
    unsigned int                lteChanType,
    BcpTest_LteCBParams*        pCodeBlkParams, 
    BcpTest_RateMatchParams*    pRmParams
)
{
    /* Setup the LTE RM header as per the inputs specified */        
    pLteRmHdrCfg->local_hdr_len             =   6;
    
    if (lteChanType == LTE_PDSCH || lteChanType == LTE_PUSCH_SIC)
        pLteRmHdrCfg->channel_type          =   0;        
    else
        pLteRmHdrCfg->channel_type          =   1;        

    if (lteChanType == LTE_PDSCH || lteChanType == LTE_PDCCH)
    {
        pLteRmHdrCfg->input_bit_format      =   0;        
        pLteRmHdrCfg->output_bit_format     =   0;
    }
    else
    {
        pLteRmHdrCfg->input_bit_format      =   1;        
        pLteRmHdrCfg->output_bit_format     =   1;
    }

    pLteRmHdrCfg->num_filler_bits_f         =   pCodeBlkParams->numFillerBits;        

    if (pCodeBlkParams->numCodeBksKm == 0)
    {
        pLteRmHdrCfg->num_code_blocks_c1    =   pCodeBlkParams->numCodeBksKp;        
        pLteRmHdrCfg->block_size_k1         =   pCodeBlkParams->codeBkSizeKp;        
        pLteRmHdrCfg->num_code_blocks_c2    =   0;        
        pLteRmHdrCfg->block_size_k2         =   0;        

        pLteRmHdrCfg->rv_start_column1      =   pRmParams->rvKp;        
        pLteRmHdrCfg->rv_start_column2      =   0;        
        pLteRmHdrCfg->param_ncb1_column     =   pRmParams->NcbKpCol;        
        pLteRmHdrCfg->param_ncb1_row        =   pRmParams->NcbKpRow;        
        pLteRmHdrCfg->param_ncb2_column     =   0;        
        pLteRmHdrCfg->param_ncb2_row        =   0;        
    }
    else
    {
        pLteRmHdrCfg->num_code_blocks_c1    =   pCodeBlkParams->numCodeBksKm;        
        pLteRmHdrCfg->block_size_k1         =   pCodeBlkParams->codeBkSizeKm;        
        pLteRmHdrCfg->num_code_blocks_c2    =   pCodeBlkParams->numCodeBksKp;        

        pLteRmHdrCfg->rv_start_column1      =   pRmParams->rvKm;        
        pLteRmHdrCfg->rv_start_column2      =   pRmParams->rvKp;        
        pLteRmHdrCfg->param_ncb1_column     =   pRmParams->NcbKmCol;        
        pLteRmHdrCfg->param_ncb1_row        =   pRmParams->NcbKmRow;        
        pLteRmHdrCfg->param_ncb2_column     =   pRmParams->NcbKpCol;        
        pLteRmHdrCfg->param_ncb2_row        =   pRmParams->NcbKpRow;        
        pLteRmHdrCfg->block_size_k2         =   pCodeBlkParams->codeBkSizeKp;        
    }
    pLteRmHdrCfg->num_code_blocks_ce1       =   (pCodeBlkParams->numCodeBks - pRmParams->Gamma);        
    pLteRmHdrCfg->block_size_e1             =   pRmParams->E0;        
    pLteRmHdrCfg->num_code_blocks_ce2       =   pRmParams->Gamma;        
    pLteRmHdrCfg->block_size_e2             =   pRmParams->E1;        

    return;        
}

/*计算调制参数 */
static void prepare_modhdr_cfg 
(
    Bcp_ModHdrCfg*        pModHdrCfg,
    Bcp_RadioStd          radioStd,
    unsigned int          Rprime, 
    unsigned char         mod,   
    unsigned char         operationMode, 
    unsigned short        rms,
    unsigned char         numOFDMsymPerSubfrm,
    unsigned short        numCqiSubcs,
    unsigned short        numRiSubcs,
    unsigned short        numAckSubcs,
    unsigned int          cinit,
    unsigned char         qFmt
)
{
  unsigned int   Rprime1;
  unsigned char  prbList[45]={1, 2, 3, 4, 5, 6, 8, 9, 10,12,15,16, 18,20,24,25, 27,30,32,36, 40,45,48,50, 54,60,64,72, 75,80,
                              81,90,96,100,108,120,128,144,150,160,162,180,192,200,216};
        
    /* Prepare modulation header as per inputs provided */        
    pModHdrCfg->local_hdr_len   =   4;
    if (operationMode == 3)
        pModHdrCfg->sh_mod_sel      =   Bcp_ModulationMode_Soft;        
    else
        pModHdrCfg->sh_mod_sel      =   (Bcp_ModulationMode)operationMode;        
    pModHdrCfg->jack_bit        =   0;        
    if (mod == 1)
        pModHdrCfg->mod_type_sel=   Bcp_ModulationType_BPSK;
    else
        pModHdrCfg->mod_type_sel=   (Bcp_ModulationType)(mod/2);

    if (operationMode == 2 || operationMode == 3) // soft modulator
    {
      pModHdrCfg->uva_val     = rms;          
    }
    else
    {
        if (operationMode == 0) // uncompressed mode hard modulator
        {
            if (mod == 1 || mod == 2) // BPSK
                pModHdrCfg->uva_val =   (rms * 23170 + 0x4000) >> 15;        
            else if (mod == 4) //16 QAM
                pModHdrCfg->uva_val =   (rms * 10362 + 0x4000) >> 15;        
            else if (mod == 6) //64 QAM 
                pModHdrCfg->uva_val =   (rms * 5056 + 0x4000) >> 15;        
            else //256 QAM
                pModHdrCfg->uva_val =   (rms * 2513 + 0x4000) >> 15;
        }
        else //compressed mode hard modulator
        {
            pModHdrCfg->uva_val =       0;
        }
    }

    if (radioStd == Bcp_RadioStd_LTE)
        pModHdrCfg->scr_en      =   1;
    else
        pModHdrCfg->scr_en      =   0;        

    if (operationMode == 2 || operationMode == 3)
    {
        pModHdrCfg->rmux_ln     =   Rprime;        

        if (operationMode == 2) // single layer
            Rprime1 =   12 * prbList[Rprime];                
        else // 2 layers
            Rprime1 =   12 * prbList[Rprime] * 2;                

        pModHdrCfg->b_table_index=  gind_row_index(Rprime1 * mod);        
        pModHdrCfg->cmux_ln     =   (Bcp_CmuxLength) (numOFDMsymPerSubfrm-9);        
        pModHdrCfg->q_format    =   (Bcp_QFormat) qFmt;        
        pModHdrCfg->cqi_ln      =   numCqiSubcs;        
        pModHdrCfg->ri_ln       =   numRiSubcs;        
        pModHdrCfg->ack_ln      =   numAckSubcs;        
    }
    else
    {
        pModHdrCfg->rmux_ln     =   0;        
        pModHdrCfg->b_table_index=  0;        
        pModHdrCfg->cmux_ln     =   (Bcp_CmuxLength) 0;        
        pModHdrCfg->q_format    =   (Bcp_QFormat) 0;        
        pModHdrCfg->cqi_ln      =   0;        
        pModHdrCfg->ri_ln       =   0;        
        pModHdrCfg->ack_ln      =   0;        
    }
    
    pModHdrCfg->cinit_p2        =   cinit;        
    if (operationMode == 2)
        pModHdrCfg->split_mode_en   =   0;        
    else
        pModHdrCfg->split_mode_en   =   1;        

    return;
}
#endif

static signed int add_bcp_config_data_pbch(unsigned char*  pDataBuffer,
                                   CELL_PARA *cell_para_ptr,
                                   unsigned int numTestPkts,
                                   unsigned char ns)
{
    unsigned int                dataBufferLen, tmpLen;
    BcpTest_LteCBParams         codeBlkParams;
    unsigned int                lteChanType, cInit;
    Bcp_RadioStd                radioStd;
    Bcp_GlobalHdrCfg            bcpGlblHdrCfg;
    Bcp_CrcHdrCfg               crcHdrCfg;
    Bcp_EncHdrCfg               encHdrCfg;
    BcpTest_RateMatchParams     rmParams;
    Bcp_RmHdr_LteCfg            lteRmHdrCfg;
    Bcp_ModHdrCfg               modHdrCfg;
    Bcp_TmHdrCfg                tmHdrCfg;
    unsigned char*              pDataBuffer_ptr;
    unsigned char               codeword_to_layer_mapping,R_subblock_TC;
    unsigned int                method2_id;

    /* Get the code block params */
    memset(&codeBlkParams,0,sizeof(codeBlkParams));
    R_subblock_TC = (PBCH_TBSIZE + 16 + 31) / 32;//32为矩阵的列数
    codeBlkParams.numFillerBits = 0;
    codeBlkParams.numCodeBks = 1;
    codeBlkParams.codeBkSizeKp = PBCH_TBSIZE + 16;
    codeBlkParams.numCodeBksKp = 1;
    codeBlkParams.outputbitsNoFiller = PBCH_TBSIZE + 16;
    
    
    /* To begin with, lets test LTE PSDCH channel */
    radioStd    =   Bcp_RadioStd_LTE; 
    lteChanType =   LTE_PDSCH;
    
    /* Start adding BCP Packet headers based on the test configuration we read. */

    /* Initialize our data buffer length running counter */
    dataBufferLen   =   0;
    tmpLen = 0;

    pDataBuffer_ptr = pDataBuffer;

    pDataBuffer   +=  8;/*Global Header len*/
    dataBufferLen +=  8;/*Global Header len*/
    tmpLen = 0;

    /* Header 2: CRC Header */
    //PBCH CRC 掩码
    if(NUM_ANTENNA_PORT_1 == cell_para_ptr->num_antenna_port)
    {
        method2_id = 0;//<0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0>
    }
    else if(NUM_ANTENNA_PORT_2 == cell_para_ptr->num_antenna_port)
    {
        method2_id = 0xffff0000;//<1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1>
    }
    else
    {
        method2_id = 0x55550000;//<0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1>
    }
    
    prepare_crchdr_cfg (&crcHdrCfg, 
                        Bcp_RadioStd_WIMAX_802_16E, //借用此配置为PBCH配置CRC16
                        PBCH_TBSIZE, 
                        codeBlkParams.numFillerBits, 
                        method2_id, 0, 0, 0, 0, 0, 0, NULL);
    if (Bcp_addCRCHeader (&crcHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add CRC Header to packet \n");
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen = 0;

    /* Header 3: Encoder Header */
    prepare_lte_enchdr_cfg (&encHdrCfg, radioStd, LTE_PBCH, &codeBlkParams);
    if (Bcp_addEncoderHeader (&encHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add Encoder Header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen      = 0;

    /* Header 4: Rate matching header */
    //对于PBCH端口数就是层数
    codeword_to_layer_mapping = codeword_to_layer_mapping_0[0][cell_para_ptr->num_antenna_port - 1];
    
    memset(&rmParams,0,sizeof(rmParams));
    rmParams.NcbKp = R_subblock_TC * 32 * 3;//缓冲区
    rmParams.E0 = PBCH_BIT_LEN;
    prepare_lte_rmhdr_cfg(&lteRmHdrCfg, LTE_PDCCH, &codeBlkParams, &rmParams);

    if (Bcp_addLte_RMHeader(&lteRmHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add lte rate modulation header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen = 0;
#if 1
    /* Header 5: Modulation header */
    cInit = cell_para_ptr->cellID;
    prepare_modhdr_cfg( &modHdrCfg, 
                        radioStd, 
                        0, // number of subcarrier, not used,
                        QPSK,
                        0, //uncompressed mode
                        16384,//rms Q(16,2)
                        0, //numOFDMsymPerSubfrm, not used
                        0, //numCqiSubcs
                        0, //numRiSubcs
                        0, //numAckSubcs
                        cInit,
                        0); //qformat
    if(Bcp_addModulatorHeader(&modHdrCfg, pDataBuffer, &tmpLen) < 0)
    {           
        printf ("Failed to add Modulation header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen      = 0;
#endif
    /* Header 6: Traffic Manager header */
    prepare_tmhdr_cfg(&tmHdrCfg);
    if (Bcp_addTMHeader(&tmHdrCfg, pDataBuffer, &tmpLen) < 0)
    {           
        printf ("Failed to add Traffic Manager header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen      = 0;

    /* Header 1: Global Header */
    bcpGlblHdrCfg.pkt_type          =   Bcp_PacketType_Normal;
    bcpGlblHdrCfg.flush             =   0;
    bcpGlblHdrCfg.drop              =   0;
    bcpGlblHdrCfg.halt              =   0;
    bcpGlblHdrCfg.radio_standard    =   radioStd;
    bcpGlblHdrCfg.hdr_end_ptr       =   dataBufferLen >> 2;
    bcpGlblHdrCfg.flow_id           =   FLOW0;
    bcpGlblHdrCfg.destn_tag         =   0xDEAD;
    if (Bcp_addGlobalHeader (&bcpGlblHdrCfg, pDataBuffer_ptr, &tmpLen) < 0)
    {
        printf ("Failed to add Global Header to packet \n");
        return -1;
    }

    memcpy(pDataBuffer,(const void*)&vxx_pbch_data,((PBCH_TBSIZE + 31)>>5)*4);//必须以128bit单位输入，从低bit向高bit输入
    dataBufferLen += ((PBCH_TBSIZE + 31)>>5)*4;/*byte为单位*/
    /* Successfully read the test configuration */        
    return dataBufferLen;
}

/*计算和配置bcp相关参数*/
unsigned int bcp_lte_pbch(CELL_PARA *cell_para_ptr,
                           unsigned char slot_idx
                           )
{
    BCP_HostPacketDescriptor* pCppiDesc;
    BCP_HostPacketDescriptor* pRxDesc;
    Cppi_DescType descType;
    unsigned int dataBufferLen;
    unsigned int dataBufferLenUsed;
    unsigned int rxDataBufferLen;
    unsigned int rxDataTotalLen;
    unsigned int test_ok_flag = 1;
    unsigned int numTestPkts;
    unsigned int tx_num_packet = 1;
       
    unsigned char* pDataBuffer;
    unsigned char* pRxDataBuffer;
    unsigned int i;
    unsigned int *restrict input_ptr1;
    unsigned int *restrict input_ptr2;


        /* Build and Send a packet with LTE DL parameters for BCP Processing */
        if ((pCppiDesc = (BCP_HostPacketDescriptor*) Qmss_queuePop (BCP_Tx_FDQ)) == NULL)
        {
            printf ("Error poping bcp tx packet \n");
            test_ok_flag = 0;
            return(test_ok_flag);
        }
        pCppiDesc->src_tag_lo = FLOW0;
        pDataBuffer = (unsigned char*)pCppiDesc->buffer_ptr;
        dataBufferLen = pCppiDesc->buffer_len;
        
        //memset (pDataBuffer, 0, dataBufferLen);

        /* Read test configuration */
        dataBufferLenUsed = add_bcp_config_data_pbch(pDataBuffer,
                                                cell_para_ptr,
                                                numTestPkts,
                                                slot_idx);
        if(0 >= dataBufferLenUsed)
        {
            printf ("Error populating bcp packet \n");
            test_ok_flag = 0;
            return(test_ok_flag);
        }

        pCppiDesc->packet_length = dataBufferLenUsed;
        CACHE_wbL2(pDataBuffer, dataBufferLenUsed, CACHE_WAIT);
    
        Qmss_queuePushDescSize(BCP_Tx_QUE, (void*)pCppiDesc, BCP_DESC_SIZE);



    /* Wait on data to be received from BCP and validate it. Poll on Rx queue for results. */
    while (Qmss_getQueueEntryCount (BCP_Rx_QUE) < tx_num_packet);


    while (Qmss_getQueueEntryCount (BCP_Rx_QUE) == tx_num_packet)
    {
        

            /* Data could arrive scattered across multiple linked descriptors.
             * Collect data from all linked descriptors and validate it.
             */
            rxDataTotalLen = 0;
            
            pRxDesc = (BCP_HostPacketDescriptor*) Qmss_queuePop (BCP_Rx_QUE);
            
            descType = Cppi_getDescType ((Cppi_Desc*)pRxDesc);
            
            /* Get Data buffer containing the output and its length */
            Cppi_getData (descType, (Cppi_Desc*)pRxDesc, &pRxDataBuffer, &rxDataBufferLen);
            input_ptr1 = (unsigned int*)cell_para_ptr->pbch_modulation_data_ptr;
            input_ptr2 = (unsigned int*)pRxDataBuffer;
            memcpy(input_ptr1,input_ptr2,rxDataBufferLen);
            
            rxDataTotalLen += rxDataBufferLen;
            
            Qmss_queuePushDescSize(BCP_Rx_FDQ, (void*)pRxDesc, BCP_DESC_SIZE);
            
            pRxDesc = (BCP_HostPacketDescriptor*)Cppi_getNextBD (descType, (Cppi_Desc*)pRxDesc);
            /* Check if there are any descriptors linked to this Rx desc */
            while (pRxDesc)
            {
                /* Get Data buffer containing the output and its length */
                pRxDesc = (BCP_HostPacketDescriptor*)Cppi_getNextBD (descType, (Cppi_Desc*)pRxDesc);
            
                /* Get Data buffer containing the output and its length */
                Cppi_getData (descType, (Cppi_Desc*)pRxDesc, &pRxDataBuffer, &rxDataBufferLen);                                 
            
                rxDataTotalLen  +=  rxDataBufferLen;
            
                Qmss_queuePushDescSize(BCP_Rx_FDQ, (void*)pRxDesc, BCP_DESC_SIZE);
            }
            //printf ("[Pkt %d]: Total Len: %d \n", numTestPkts, rxDataTotalLen);

               
    }
    
    return(test_ok_flag);
}

