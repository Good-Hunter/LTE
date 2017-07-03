/***********************************************/
/*bcp_lte_pdsch                                */
/*功能：完成BCP相关配置                        */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"
extern unsigned int error_pdschcrc;
extern unsigned int right_pdschcrc;

#pragma DATA_SECTION(gxx_bcp_rxdesc,".shareddata")
unsigned int gxx_bcp_rxdesc[30];

extern unsigned volatile short  slot_idx;
unsigned int countframe[10];
/*计算码块参数*/
static void compute_cbparams_ref (unsigned int tbSize, BcpTest_LteCBParams* pCodeBlkParams)
{
    unsigned int          numCodeBlocks;
    unsigned int          outputbitsNoFiller;
    unsigned int          firstSegmtSize;
    unsigned int          secondSegmtSize;
    unsigned int          firstSegCount;
    unsigned int          secondSegCount;
    unsigned int          numInputBits = tbSize + 24;  // tb size + 24 bits crc
    unsigned short        numFillerBits;
    signed int            idx;
    unsigned char         crcOrder = 24;
    unsigned char         delaK;
      
    /* Initialize the code block parameters. */
    memset (pCodeBlkParams, 0, sizeof (BcpTest_LteCBParams));
    
    /* calculate the number of code blocks to be generated from imput bit stream */
    if( numInputBits <= MAX_CODE_BLOCK_SIZE )
    {
        numCodeBlocks = 1;
        outputbitsNoFiller = numInputBits;
    }
    else
    { 
        numCodeBlocks = (unsigned int)ceil((double)numInputBits/( MAX_CODE_BLOCK_SIZE - crcOrder));
        outputbitsNoFiller = numInputBits + ( numCodeBlocks * crcOrder) ;
    }
    
    /* calculate number of bits in each block only when numCodeBlocks!=0 */
    if(numCodeBlocks != 0)
    {
    
        /* K+ = is minimum K in 3GPP TS 36.212 table 5.1.3-3 C * K >= B' */
        for(idx = 0; idx < MAX_BLOCK_INDEX; idx++)
        {
            firstSegmtSize = TurboInterTable[idx][0];
        
            if((numCodeBlocks * firstSegmtSize) >= outputbitsNoFiller)
            {
                firstSegCount = 1;
                break;
            }
        }
        pCodeBlkParams->f1Kp = TurboInterTable[idx][1];
        pCodeBlkParams->f2Kp = TurboInterTable[idx][2];
    
        if(numCodeBlocks == 1)
        {
            secondSegmtSize = 0;
            secondSegCount  = 0;
            /* no kminues */
            pCodeBlkParams->f1Km = 0;
            pCodeBlkParams->f2Km = 0;
        }
        else if(numCodeBlocks > 1)
        {
            for(idx = MAX_BLOCK_INDEX - 1; idx >= 0; idx--)
            {
                secondSegmtSize = TurboInterTable[idx][0];
                if(secondSegmtSize < firstSegmtSize )
                {
                    break;
                }
            }
            pCodeBlkParams->f1Km = TurboInterTable[idx][1];
            pCodeBlkParams->f2Km = TurboInterTable[idx][2];
            
            delaK = firstSegmtSize - secondSegmtSize;
            secondSegCount = ( numCodeBlocks * firstSegmtSize ) - outputbitsNoFiller;
            secondSegCount = (unsigned int)floor( secondSegCount / delaK );
            
            firstSegCount =  numCodeBlocks - secondSegCount;
        
        }
      
        numFillerBits = firstSegCount * firstSegmtSize + secondSegCount * secondSegmtSize - outputbitsNoFiller;
        
        /* Update the code block parameters with values calculated */ 
        pCodeBlkParams->numCodeBks          = numCodeBlocks;
        pCodeBlkParams->outputbitsNoFiller  =   outputbitsNoFiller;
        pCodeBlkParams->numCodeBksKp        =   firstSegCount;
        pCodeBlkParams->codeBkSizeKp        =   firstSegmtSize;
        pCodeBlkParams->numCodeBksKm        =   secondSegCount;
        pCodeBlkParams->codeBkSizeKm        =   secondSegmtSize;
        pCodeBlkParams->numFillerBits       =   numFillerBits;
    }

    return;
}


/*计算速率匹配参数*/
static void compute_rmGamma_ref
(
    unsigned int                numChannelBits,
    unsigned char               mod, 
    unsigned char               numLayers,
    BcpTest_LteCBParams*        pCodeBlkParams, 
    unsigned char               rvIndex,
    float                       ratioKm, 
    float                       ratioKp,
    unsigned int                Nir,  
    unsigned char               testFlag, 
    BcpTest_RateMatchParams*    pRmParams
)
{
    unsigned short Gprime;
    unsigned int Gamma, E0, E1;   
    unsigned int G; 
    unsigned char modulationType , blkMap, numCodeBks;
    unsigned short numRows;
    unsigned int vBufferSize;
    
    G = numChannelBits;
    modulationType = mod;
    blkMap = numLayers;
    numCodeBks = pCodeBlkParams->numCodeBks;
    
                 
    Gprime = G / (blkMap * modulationType);
    
    Gamma = Gprime % numCodeBks;        
    
    E0 = blkMap *modulationType * (Gprime /numCodeBks);
    if (Gprime % numCodeBks==0)
        E1 = 0;
    else
        E1 = blkMap *modulationType * (Gprime /numCodeBks+1);
         
    pRmParams->Gamma   = Gamma;
    pRmParams->E0      = E0;
    pRmParams->E1      = E1;
    
    if (testFlag==LTE_PDSCH) //PDSCH
    {
        if(pRmParams->E0 > 64000) //satuarate to maximum 
            pRmParams->E0 = 64000/modulationType * modulationType;
        if (pRmParams->E1 > 64000)
            pRmParams->E1 = 64000/modulationType * modulationType;;
    
    }
    
    if (pCodeBlkParams->numCodeBksKm)
    {
        numRows = ((pCodeBlkParams->codeBkSizeKm + 4) + 31)/32;
        vBufferSize = numRows * 32 * 3;
    
        if(testFlag==LTE_PDSCH) //PDSCH
        {
            if(ratioKm != 0)
                pRmParams->NcbKm = (unsigned int) (ratioKm * numRows * 32.f);
            else //Nir is the input
            {
                if (Nir/numCodeBks < vBufferSize)
                    pRmParams->NcbKm =Nir/numCodeBks;
                else
                    pRmParams->NcbKm = vBufferSize;
            }
        }
        else //PUSCH_SIC
            pRmParams->NcbKm = vBufferSize;
    
        if(pRmParams->NcbKm == numRows * 32)
            pRmParams->NcbKm ++; //not in the end of systematic bits
    
        pRmParams->rvKmCol = (unsigned int) (2 * ceil(pRmParams->NcbKm/(8.f * numRows)) * rvIndex + 2);
        pRmParams->NcbKmCol = ( pRmParams->NcbKm - numRows * 32 - 1)/( 2 * numRows);
        pRmParams->NcbKmRow = ( pRmParams->NcbKm - numRows * 32 - 1)% (2 * numRows);
        
        if (pRmParams->rvKmCol > 31)
        {
          pRmParams->rvKm = (pRmParams->rvKmCol - 32)/ 2 + 32;
        
        }
        else
          pRmParams->rvKm = pRmParams->rvKmCol;
    
    }
    else
    {
        pRmParams->NcbKm = 0;
        pRmParams->rvKm = 0;
        pRmParams->rvKmCol = 0;
        pRmParams->NcbKmCol = 0;
        pRmParams->NcbKmRow = 0;
    }
    
    if(pCodeBlkParams->numCodeBksKp)
    {
        numRows = ((pCodeBlkParams->codeBkSizeKp + 4) + 31)/32;
        vBufferSize = numRows * 32 * 3;
        
        if(testFlag==LTE_PDSCH) //PDSCH
        {
            if(ratioKp != 0)
              pRmParams->NcbKp = (unsigned int) (ratioKp * numRows * 32.f);
            else
            {
              if (Nir/numCodeBks < vBufferSize)
                pRmParams->NcbKp =Nir/numCodeBks;
              else
                pRmParams->NcbKp = vBufferSize;
            }
        }
        else
        {
            pRmParams->NcbKp = vBufferSize;
        }
        
        if(pRmParams->NcbKp == numRows * 32)
        {
            pRmParams->NcbKp++;
        }
        
        pRmParams->rvKpCol = (unsigned int) (2 * ceil(pRmParams->NcbKp/(8.f * numRows)) * rvIndex + 2);
        pRmParams->NcbKpCol = ( pRmParams->NcbKp - numRows * 32 - 1)/( 2 * numRows);
        pRmParams->NcbKpRow = ( pRmParams->NcbKp - numRows * 32 - 1)% (2 * numRows);
        
        if(pRmParams->rvKpCol > 31)
        {
            pRmParams->rvKp = (pRmParams->rvKpCol - 32)/ 2 + 32;
        
        }
        else
        {
            pRmParams->rvKp = pRmParams->rvKpCol;
        }
    }
    else
    {
        pRmParams->NcbKp = 0;
        pRmParams->rvKp = 0;
        pRmParams->rvKpCol = 0;
        pRmParams->NcbKpCol = 0;
        pRmParams->NcbKpRow = 0;
    }
    
    return;
}



/*配置解速率匹配参数*/
static void prepare_lte_rdhdr_cfg 
(
    Bcp_RdHdr_LteCfg*             pRdHdrCfg,
    BcpTest_LteCBParams*          pCodeBlkParams,
    BcpTest_RateMatchParams*      pRmParams,
    unsigned char                       numCqiPayload,
    unsigned short                      numCqiOutBits,
    unsigned char                       uciFlag,
    unsigned char                       harqFlag,
    unsigned char                       cqiPassThroughFlag
)
{
    /* Setup the LTE Rate dematching header as per inputs */
    memset (pRdHdrCfg, 0, sizeof (Bcp_RdHdr_LteCfg));        
    pRdHdrCfg->local_hdr_len                =   7;        

    if (!pCodeBlkParams->numCodeBksKm)
    {
        pRdHdrCfg->num_code_blocks_c1       =   pCodeBlkParams->numCodeBksKp;        
        pRdHdrCfg->block_size_k1            =   pCodeBlkParams->codeBkSizeKp;        
        pRdHdrCfg->num_code_blocks_c2       =   0;        
        pRdHdrCfg->block_size_k2            =   0;        
    }
    else
    {
        pRdHdrCfg->num_code_blocks_c1       =   pCodeBlkParams->numCodeBksKm;        
        pRdHdrCfg->block_size_k1            =   pCodeBlkParams->codeBkSizeKm;        
        pRdHdrCfg->num_code_blocks_c2       =   pCodeBlkParams->numCodeBksKp;        
        pRdHdrCfg->block_size_k2            =   pCodeBlkParams->codeBkSizeKp;        
    }
    pRdHdrCfg->num_code_blocks_ce1          =   pCodeBlkParams->numCodeBks - pRmParams->Gamma;
    pRdHdrCfg->block_size_e1                =   pRmParams->E0; 
    pRdHdrCfg->num_code_blocks_ce2          =   pRmParams->Gamma;
    pRdHdrCfg->block_size_e2                =   pRmParams->E1;

    if (uciFlag)
    {
        if (cqiPassThroughFlag)
            pRdHdrCfg->block_size_outq_cqi  =   numCqiOutBits;              
        else
            pRdHdrCfg->block_size_outq_cqi  =   numCqiPayload;              
        pRdHdrCfg->block_size_inq_cqi       =   numCqiOutBits;              
        pRdHdrCfg->cqi_pass_through         =   cqiPassThroughFlag;
    }
    else
    {
        pRdHdrCfg->block_size_outq_cqi      =   0;              
        pRdHdrCfg->block_size_inq_cqi       =   0;              
        pRdHdrCfg->cqi_pass_through         =   1;
    }
    pRdHdrCfg->rv_start_column              =   pRmParams->rvKp; 
    pRdHdrCfg->num_filler_bits_f            =   pCodeBlkParams->numFillerBits;
  if (harqFlag)
        pRdHdrCfg->enable_harq_input        =   1;
  else
        pRdHdrCfg->enable_harq_input        =   0;

    pRdHdrCfg->enable_harq_output           =   1;
    pRdHdrCfg->harq_input_address           =   0x90000000;
    pRdHdrCfg->harq_output_address          =   0x91000000;
    pRdHdrCfg->init_cb_flowId               =   0;
    pRdHdrCfg->flowId_hi                    =   0;
    pRdHdrCfg->flowId_cqi_offset            =   0;
    pRdHdrCfg->tcp3d_scale_factor           =   16;
    pRdHdrCfg->tcp3d_dyn_range              =   0;

    return;
} 


static void prepare_tmhdr_cfg (Bcp_TmHdrCfg* pTmHdrCfg)
{
    pTmHdrCfg->ps_data_size     =   0;
    pTmHdrCfg->info_data_size   =   0;

    return;
}


static signed int add_bcp_config_data(unsigned char*  pDataBuffer,
                                   USER_PARA *user_para_ptr,
                                   CELL_PARA *cell_para_ptr,
                                   unsigned int numTestPkts)
{
    BcpTest_LteCBParams     pcodeBlkParams;
    BcpTest_RateMatchParams pRmParams;
    Bcp_RadioStd            radioStd;
    Bcp_GlobalHdrCfg        bcpGlblHdrCfg;
    Bcp_TmHdrCfg            tmHdrCfg;
    Bcp_RdHdr_LteCfg        rdHdrCfg;
    unsigned char*          pDataBuffer_ptr;
    unsigned int          dataBufferLen;
    unsigned int          tmpLen;
    unsigned int          lteChanType;
    unsigned int          headerLen;
    unsigned char         codeword_to_layer_mapping;
  

    /* Get the code block params */
    compute_cbparams_ref (user_para_ptr->tbSize[numTestPkts], &pcodeBlkParams);
    user_para_ptr->num_of_cb[numTestPkts] = pcodeBlkParams.numCodeBks;


    /* To begin with, lets test LTE PUSCH channel */
    radioStd    =   Bcp_RadioStd_LTE; 
    lteChanType =   LTE_PUSCH;/*借用PUSCH的配置来解PDSCH*/
    
    /* Initialize our data buffer length running counter */
    dataBufferLen   =   0;
    tmpLen = 0;
    if(0 == numTestPkts)/*传输块0*/
        codeword_to_layer_mapping = codeword_to_layer_mapping_0[user_para_ptr->num_code_word - 1][user_para_ptr->numLayers - 1];
    else
        codeword_to_layer_mapping = codeword_to_layer_mapping_1[user_para_ptr->num_code_word - 1][user_para_ptr->numLayers - 1];
    compute_rmGamma_ref( user_para_ptr->numBitsG[numTestPkts],
                         user_para_ptr->modulation[numTestPkts],
                         codeword_to_layer_mapping,
                         &pcodeBlkParams,
                         user_para_ptr->rvIdx[numTestPkts],
                         0, //ratio is 0
                         0,
                         user_para_ptr->Nir,
                         LTE_PDSCH,
                         &pRmParams);


    /* Start adding BCP Packet headers based on the test configuration we read. */
    pDataBuffer_ptr = pDataBuffer;

    pDataBuffer   +=  8;/*Global Header len*/
    dataBufferLen +=  8;/*Global Header len*/
    tmpLen = 0;

    /* Header 2: Rate de-matching header */
    //prepare_lte_rdhdr_cfg (&rdHdrCfg, pCodeBlkParams, pRmParams, numCqiPayload, numCqiOutBits, uciFlag, harqFlag, cqiPassThroughFlag);
    prepare_lte_rdhdr_cfg (&rdHdrCfg, &pcodeBlkParams, &pRmParams, 0, 0, 0, 0, 1);
    if (Bcp_addLTE_RDHeader (&rdHdrCfg, pDataBuffer, &tmpLen) < 0)
    {
        printf ("Failed to add lte rate modulation header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen      = 0;

    /* Header 3: Traffic Manager header */
    prepare_tmhdr_cfg (&tmHdrCfg);
    if (Bcp_addTMHeader (&tmHdrCfg, pDataBuffer, &tmpLen) < 0)
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

    /* No Info words/PS to be added for now */

    /* Add padding to align data on 128 bits */    
    pDataBuffer   +=  0;
    dataBufferLen +=  0;
    headerLen = dataBufferLen;

    /* Finally add the data to the packet in the same order as the headers */

    dataBufferLen += user_para_ptr->numBitsG[numTestPkts];/*byte为单位*/
    //EndianConverter_8b((uint32_t*)pDataBuffer,user_para_ptr->numBitsG/4);/*必须按照小端输入*/
    /* Successfully read the test configuration */        
    return dataBufferLen;

}

unsigned char* pRxDataBuffer;
unsigned int gxx_error = 0;
unsigned int gxx_bcp_count = 0;
unsigned int rxDataTotalLen = 0;
/*计算和配置bcp相关参数*/

unsigned int bcp_lte_pdsch_receive_part1(USER_PARA *user_para_ptr,
                                         CELL_PARA *cell_para_ptr,
                                         unsigned int slot_idx,
                                         unsigned int * core_start_numBitsG,
                                         unsigned int* core_m_symbol)
{
    BCP_HostPacketDescriptor* pCppiDesc;
    BCP_HostPacketDescriptor* pRxDesc;

    //unsigned int dataBufferLen;
    unsigned int dataBufferLenUsed;
    //unsigned int rxDataBufferLen;
    //unsigned int rxDataTotalLen = 0;
    unsigned int test_ok_flag = 1;
    unsigned int numTestPkts;

    unsigned char* pDataBuffer;
    unsigned int Ok_flag=0;

    unsigned int tx_num_packet = user_para_ptr->num_code_word;
    signed char numtestcbs;
    int cycle,cycle1,i,k;
    unsigned int cycle_start,cycle_end,pkt_cnt, pkt_cnt_temp,cycle_start1,cycle_end1;

    for (numTestPkts = 0; numTestPkts < tx_num_packet; numTestPkts ++)
    {

    	for (i = 0; i < BCP_MAX_NUM_FD; i ++)
        {
            /* Build and Send a packet with LTE DL parameters for BCP Processing */
              if ((pCppiDesc = (BCP_HostPacketDescriptor*) Qmss_queuePop (BCP_Tx_FDQ)) == NULL)
              {
                  printf ("Error poping bcp tx packet \n");
                  test_ok_flag = 0;
                  return(test_ok_flag);
              }
              else if(pCppiDesc->buffer_ptr == (unsigned int)srcBuf[numTestPkts])
              {
                   break;
              }
              else
              {
                   Qmss_queuePushDesc (BCP_Tx_FDQ, pCppiDesc);
        
              }
        }

        pCppiDesc->src_tag_lo = FLOW0;
        pDataBuffer = (unsigned char*)pCppiDesc->buffer_ptr;
        //dataBufferLen = pCppiDesc->buffer_len;
        
        //memset (pDataBuffer, 0, dataBufferLen);

        /* Read test configuration */
        dataBufferLenUsed = add_bcp_config_data(pDataBuffer,
                                                user_para_ptr,
                                                cell_para_ptr,
                                                numTestPkts);
        if(0 >= dataBufferLenUsed)
        {
            printf ("Error populating bcp packet \n");
            test_ok_flag = 0;
            return(test_ok_flag);
        }

        pCppiDesc->packet_length = dataBufferLenUsed;
        CACHE_wbL2(pDataBuffer, dataBufferLenUsed, CACHE_WAIT);
    
        Qmss_queuePushDescSize(BCP_Tx_QUE, (void*)pCppiDesc, BCP_DESC_SIZE);
        cycle_start1 = TSCL;


        	if(1 == numTestPkts)
            {
                /*第一个码字的解速率匹配结果已经准备好，可以利用等待时间处理第一个码字的译码,因为最多2流故简单处理*/
            	rxDataTotalLen = 0;
            	bcp_lte_pdsch_receive_part2(user_para_ptr,
                                            cell_para_ptr,
                                            user_para_ptr->num_of_cb[0],
                                            0,
                                            numTestPkts - 1);
            }

        cycle_end1 = TSCL;
        cycle1 = cycle_end1 - cycle_start1;
        cycle_start = TSCL;
        /* Wait on data to be received from BCP and validate it. Poll on Rx queue for results. */
        while (Qmss_getQueueEntryCount (BCP_Rx_QUE) < user_para_ptr->num_of_cb[numTestPkts]);//user_para_ptr->num_of_cb);
        cycle_end = TSCL;
        cycle = cycle_end - cycle_start;

        pkt_cnt = Qmss_getQueueEntryCount (BCP_Rx_QUE);

        for (numtestcbs = 0; numtestcbs < pkt_cnt; numtestcbs ++)
        {
              /* Data could arrive scattered across multiple linked descriptors.
              * Collect data from all linked descriptors and validate it.
              */
              gxx_bcp_count++;
              pRxDesc = (BCP_HostPacketDescriptor*) Qmss_queuePop (BCP_Rx_QUE);
              if(0 == numTestPkts)
                  gxx_bcp_rxdesc[numtestcbs] = (unsigned int)pRxDesc;//保存下来给core2使用
              else
                  gxx_bcp_rxdesc[user_para_ptr->num_of_cb[numTestPkts - 1] + numtestcbs] = (unsigned int)pRxDesc;//保存下来给core2使用
              
              
              Qmss_queuePushDescSize(BCP_Rx_FDQ, (void*)pRxDesc, BCP_DESC_SIZE);

        
        }


    }
    //CACHE_wbL2(gxx_bcp_rxdesc,sizeof(gxx_bcp_rxdesc),CACHE_NOWAIT);
    if(1 < tx_num_packet)
    {
    	Ok_flag=bcp_lte_pdsch_receive_part2(user_para_ptr,
                                cell_para_ptr,
                                user_para_ptr->num_of_cb[0] + user_para_ptr->num_of_cb[1],
                                user_para_ptr->num_of_cb[0],
                                numTestPkts - 1);
    	return(Ok_flag);
    }
    else
    {
    	rxDataTotalLen = 0;
    	Ok_flag=bcp_lte_pdsch_receive_part2(user_para_ptr,
    	                            cell_para_ptr,
    	                            user_para_ptr->num_of_cb[0],
    	                            0,
    	                            numTestPkts - 1);
    	return(Ok_flag);                                                       //12.18jx加
    }
}

/*计算和配置bcp相关参数*/
extern unsigned int cycle5;
unsigned int  cycle_bcp1,cycle_bcp2,cycle_bcp3,cycle_bcp4;
unsigned char gxx_tcp3d_llr_index = 0;

unsigned int bcp_lte_pdsch_receive_part2(USER_PARA *user_para_ptr,
                                         CELL_PARA *cell_para_ptr,
                                         signed short total_cbs,
                                         signed char numtestcbs_offset,
                                         unsigned int numTestPkts)
{


    Cppi_DescType descType;


    unsigned int rxDataBufferLen;

    unsigned int test_ok_flag = 1;//1代表正常译码，0代表有错误结果
    //unsigned int numTestPkts;
    unsigned char tcp3d_flag = 1;


    signed short* pRxDataBuffer_short_ptr;
    //unsigned int tx_num_packet = user_para_ptr->num_code_word;
    signed char numtestcbs,i;
    unsigned int used_cb_len;
    signed short cb_len,cb_index,cb_count;

    unsigned char  crcOrder = 24;

    unsigned int  numInputBits;  // tb size + 24 bits crc

    //int decode_flag = 1;//1代表正常译码，0代表有错误结果

        //CACHE_invL2(gxx_bcp_rxdesc,sizeof(gxx_bcp_rxdesc),CACHE_NOWAIT);
        /* calculate the number of code blocks to be generated from imput bit stream */
        /*total_cbs = 0;
        for (numtestcbs = 0; numtestcbs < tx_num_packet; numtestcbs ++)
        {
            numInputBits = user_para_ptr->tbSize[numtestcbs] + 24;
            if( numInputBits <= MAX_CODE_BLOCK_SIZE )
            {
                user_para_ptr->num_of_cb = 1;
            }
            else
            {
                user_para_ptr->num_of_cb = (unsigned int)ceil((double)numInputBits/( MAX_CODE_BLOCK_SIZE - crcOrder));
            }
            total_cbs = total_cbs + user_para_ptr->num_of_cb;
        }*/

    
            cb_count = 0;
            //numTestPkts = 0;
            for (numtestcbs = numtestcbs_offset; numtestcbs < total_cbs; numtestcbs ++)
            {
                  /* Data could arrive scattered across multiple linked descriptors.
                  * Collect data from all linked descriptors and validate it.
                  */
                  cycle_bcp1 = TSCL;
              
                  descType = Cppi_getDescType ((Cppi_Desc*)gxx_bcp_rxdesc[numtestcbs]);
                  cycle_bcp2 = TSCL;
                  /* Get Data buffer containing the output and its length */
                  Cppi_getData (descType, (Cppi_Desc*)gxx_bcp_rxdesc[numtestcbs], &pRxDataBuffer, &rxDataBufferLen);
                  cycle_bcp3 = TSCL;


                  cycle_bcp4 = TSCL;
                  pRxDataBuffer_short_ptr = (signed short *)pRxDataBuffer;
                  CACHE_invL2((void*)pRxDataBuffer,128,CACHE_WAIT);
#ifndef SIMULATOR
                  cb_len = pRxDataBuffer_short_ptr[0];
                  CACHE_invL2((void*)pRxDataBuffer,16+6144*3+12,CACHE_WAIT);
                  cb_index = pRxDataBuffer_short_ptr[7];//BCP输出结果是大端模式
#else       
                  cb_len = pRxDataBuffer_short_ptr[7];
                  cb_index = pRxDataBuffer_short_ptr[0];
#endif      
            
                  user_para_ptr->cb_len[cb_index] = cb_len;
                  //memcpy((void *)(user_para_ptr->rmdematch_data_ptr[numTestPkts] + rxDataTotalLen),(const void*)(pRxDataBuffer + 16),rxDataBufferLen - 32);
                  if((cb_index == 0) && (0 == numTestPkts))//仅在第一个码流第一个cb块置0，后面连续拷贝到tbbuffer中
                  {
                    used_cb_len = 0;
                    //numTestPkts = 1;
                  }
                  else if((cb_index == 0) && (0 != numTestPkts))
                  {
                    //因为有可能最后一包多于实际的tbs，需要按照真实情况拷贝译码结果
                    used_cb_len = (user_para_ptr->tbSize[numTestPkts] - rxDataTotalLen * 8) >> 3;
                  }
                  else
                  {
                    used_cb_len = (user_para_ptr->cb_len[cb_index - 1] - 24) >> 3;
                  }
            
                  cycle5 = TSCL;
                  test_ok_flag = tcp3d_lte_pdsch(&vxx_user_para[0],
                  			  (signed char*)(pRxDataBuffer + 16),
                              (unsigned char*)tb_buffer,
                              user_para_ptr->cb_len[cb_index - 1],
                              cb_len,
                              gxx_tcp3d_llr_index,
                              tcp3d_flag,
                              rxDataTotalLen,
                              total_cbs - numtestcbs_offset);



                  //rxDataTotalLen += (rxDataBufferLen - 32);/*不需要 Packet Header for CB and 最后尾巴上的beta*/
                  gxx_tcp3d_llr_index += 1;
                  tcp3d_flag = 0;

                  if(test_ok_flag==0)//译码出错
                  {
                	  break;
                  }
              
                  //pRxDesc = (BCP_HostPacketDescriptor*)Cppi_getNextBD (descType, (Cppi_Desc*)pRxDesc);
                  /* Check if there are any descriptors linked to this Rx desc */
            
                  //printf ("[Pkt %d Cb_index %d]: Cb Len: %d \n", numTestPkts, cb_index,rxDataBufferLen - 32);
                  //cb_count++;
                  rxDataTotalLen += used_cb_len;
            
            }

    if(test_ok_flag == 0)//译码出错
        return 0;

    //说明是最后一包需要拷贝的数据
    tcp3d_flag = 2;
    test_ok_flag = tcp3d_lte_pdsch(&vxx_user_para[0],
                    (signed char*)(pRxDataBuffer + 16),
                    (unsigned char*)&tb_buffer,
                     user_para_ptr->cb_len[cb_index],
                     cb_len,
                     gxx_tcp3d_llr_index,
                     tcp3d_flag,
                     rxDataTotalLen,
                     total_cbs - numtestcbs_offset);
    if(test_ok_flag == 0)//译码出错
    {
    	//countframe[slot_idx>>1]++;
    	error_pdschcrc++;
        return 0;
    }
    else
    {
    	right_pdschcrc++;
    	return(1);
    }

}

