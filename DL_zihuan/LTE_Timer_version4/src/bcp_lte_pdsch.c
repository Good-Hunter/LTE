/***********************************************/
/*bcp_lte_pdsch                                */
/*功能：完成BCP相关配置                        */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"

/*计算码块参数*/
static void compute_cbparams_ref (unsigned int tbSize, BcpTest_LteCBParams* pCodeBlkParams,unsigned int lteChanType)
{
    unsigned int          numCodeBlocks;
    unsigned int          outputbitsNoFiller;
    unsigned int          firstSegmtSize;
    unsigned int          secondSegmtSize;
    unsigned int          firstSegCount;
    unsigned int          secondSegCount;
    unsigned int          numInputBits ;  
    unsigned short        numFillerBits;
    signed int            idx;
    unsigned char         crcOrder;
    unsigned char         delaK;
    
    if((LTE_PBCH == lteChanType) || (LTE_PDCCH == lteChanType)) 
    {    numInputBits = tbSize + 16;  // tb size + 24 bits crc
        crcOrder = 16;
    }
    else 
    {    numInputBits = tbSize + 24;  // tb size + 24 bits crc
        crcOrder = 24;
    }
        
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
/*计算CRC参数*/
void prepare_crchdr_cfg 
(
    Bcp_CrcHdrCfg*      pCrcHdrCfg, 
    Bcp_RadioStd        radioStd,
    unsigned int        tbSize,
    unsigned int        numFillerBits,
    unsigned int        method2Id,
    unsigned int        dtxFormat,
    signed char         scrambFlag,
    unsigned int        crcSize, 
    unsigned int        numTb, 
    unsigned int        dataFormatIn, 
    unsigned int        numTrCh, 
    unsigned int*       trChLen
)
{
    memset (pCrcHdrCfg, 0, sizeof (Bcp_CrcHdrCfg));

    /* Setup the CRC header configuration as per the inputs passed. */
    if (scrambFlag == 1)
        pCrcHdrCfg->num_scramble_sys= 0x1000000;
    else
        pCrcHdrCfg->num_scramble_sys= 0;

    pCrcHdrCfg->filler_bits = numFillerBits;

    if ((radioStd == Bcp_RadioStd_LTE) || (radioStd == Bcp_RadioStd_WIMAX_802_16E))
        pCrcHdrCfg->bit_order       =   1;  //LTE bit order
    else
        pCrcHdrCfg->bit_order       =   0;  //WCDMA bit order

    pCrcHdrCfg->dtx_format          =   dtxFormat;
    pCrcHdrCfg->method2_id          =   method2Id;

    /* Initialize header length to 2 words. we'll increment it as 
     * and how we add more config.*/
    pCrcHdrCfg->local_hdr_len       =   2; 
        
    if (dataFormatIn == 2)//transport channel concatenation  
    {
        if (numTrCh >= 1)
        {
                pCrcHdrCfg->va_blk_len  =   trChLen[0]; 
                pCrcHdrCfg->va_blks     =   1;
                pCrcHdrCfg->va_crc          =   Bcp_CrcFormat_Crc0;
            
                pCrcHdrCfg->local_hdr_len ++; 
        }
        
        if (numTrCh >= 2)
        {
                pCrcHdrCfg->vb_blk_len  =   trChLen[1]; 
                pCrcHdrCfg->vb_blks     =   1;
                pCrcHdrCfg->vb_crc          =   Bcp_CrcFormat_Crc0;
            
                pCrcHdrCfg->local_hdr_len ++; 
        }
        
        if (numTrCh >= 3)
        {
                pCrcHdrCfg->vc_blk_len  =   trChLen[2]; 
                pCrcHdrCfg->vc_blks     =   1;
                pCrcHdrCfg->vc_crc          =   Bcp_CrcFormat_Crc0;
        
                pCrcHdrCfg->local_hdr_len ++; 
        }
        
        if (numTrCh >= 4)
        {
                pCrcHdrCfg->d1_blk_len  =   trChLen[3];                
                pCrcHdrCfg->d1_blks     =   1;                
                pCrcHdrCfg->d1_crc          =   Bcp_CrcFormat_Crc0;                
        
                pCrcHdrCfg->local_hdr_len ++; 
        }
        
        if (numTrCh >= 5)
        {
                pCrcHdrCfg->d2_blk_len  =   trChLen[4];                
                pCrcHdrCfg->d2_blks     =   1;                
                pCrcHdrCfg->d2_crc          =   Bcp_CrcFormat_Crc0;                
            
                pCrcHdrCfg->local_hdr_len ++; 
        }
        
        if (numTrCh >= 6)
        {
                pCrcHdrCfg->dc_blk_len  =   trChLen[5];                
                pCrcHdrCfg->dc_blks     =   1;                
                pCrcHdrCfg->dc_crc          =   Bcp_CrcFormat_Crc0;                
        
                pCrcHdrCfg->local_hdr_len ++; 
        }
    }
    else//CRC
    {
        pCrcHdrCfg->va_blk_len      =   tbSize;
    
        if (radioStd == Bcp_RadioStd_WCDMA_R99)
            pCrcHdrCfg->va_blks     =   numTb;
        else
            pCrcHdrCfg->va_blks     =   1;
    
        if (radioStd == Bcp_RadioStd_LTE)
            pCrcHdrCfg->va_crc      =   Bcp_CrcFormat_Crc24a;   
        else if (radioStd == Bcp_RadioStd_WIMAX_802_16E)
            pCrcHdrCfg->va_crc      =   Bcp_CrcFormat_Crc16;   
        else if (radioStd == Bcp_RadioStd_WCDMA_R99)
        {
            if (crcSize == 0)
                pCrcHdrCfg->va_crc  =   Bcp_CrcFormat_Crc0;   
            else if (crcSize == 8)
                pCrcHdrCfg->va_crc  =   Bcp_CrcFormat_Crc8;   
            else if (crcSize == 12)
                pCrcHdrCfg->va_crc  =   Bcp_CrcFormat_Crc12;   
            else if (crcSize == 16)
                pCrcHdrCfg->va_crc  =   Bcp_CrcFormat_Crc16;   
            else if (crcSize == 24)
                pCrcHdrCfg->va_crc  =   Bcp_CrcFormat_Crc24b;   
            else
            {
                printf ("Wrong CRC size \n");                    
                return;                    
            }
        }
        else
            pCrcHdrCfg->va_crc      =   Bcp_CrcFormat_Crc16;   
        pCrcHdrCfg->local_hdr_len ++; 
    }
            
    return;        
}


/*计算速率匹配参数*/
void compute_rmGamma_ref
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

/*编码参数计算*/
void prepare_lte_enchdr_cfg
(
    Bcp_EncHdrCfg*              pEncHdrCfg, 
    Bcp_RadioStd                radioStd, 
    unsigned int                      lteChanType, 
    BcpTest_LteCBParams*        pCodeBlkParams
)
{
    unsigned int                      i;

    /* Setup the Encoder header as per inputs passed. */        
    pEncHdrCfg->local_hdr_len       =   3;

    if (lteChanType == LTE_PDSCH || lteChanType == LTE_PUSCH_SIC_HARD)
    {
        pEncHdrCfg->turbo_conv_sel  =   1;

    }
    else
    {
        pEncHdrCfg->turbo_conv_sel  =   0;
    }

    if ((lteChanType == LTE_PDSCH) || (lteChanType == LTE_PBCH) || (lteChanType == LTE_PDCCH))
    {
        if (pCodeBlkParams->numCodeBks == 1)
            pEncHdrCfg->scr_crc_en  =   0;  //Dont add CRC
        else
            pEncHdrCfg->scr_crc_en  =   1;  //Add CRC per FEC block
    }
    else
    {
        pEncHdrCfg->scr_crc_en      =   2;  //SCR, CRC16
    }

    pEncHdrCfg->code_rate_flag      =   0;

    if (pCodeBlkParams->numCodeBksKm == 0)
    {
        pEncHdrCfg->blockCfg[0].block_size      =   pCodeBlkParams->codeBkSizeKp;
        pEncHdrCfg->blockCfg[0].num_code_blks   =   pCodeBlkParams->numCodeBksKp;

        pEncHdrCfg->blockCfg[1].block_size      =   0;
        pEncHdrCfg->blockCfg[1].num_code_blks   =   0;

        pEncHdrCfg->blockCfg[0].intvpar0        =   pCodeBlkParams->f1Kp;
        pEncHdrCfg->blockCfg[0].intvpar1        =   pCodeBlkParams->f2Kp;

        pEncHdrCfg->blockCfg[1].intvpar0        =   0;
        pEncHdrCfg->blockCfg[1].intvpar1        =   0;
    }
    else
    {
        pEncHdrCfg->blockCfg[0].block_size      =   pCodeBlkParams->codeBkSizeKm;
        pEncHdrCfg->blockCfg[0].num_code_blks   =   pCodeBlkParams->numCodeBksKm;

        pEncHdrCfg->blockCfg[1].block_size      =   pCodeBlkParams->codeBkSizeKp;
        pEncHdrCfg->blockCfg[1].num_code_blks   =   pCodeBlkParams->numCodeBksKp;

        pEncHdrCfg->blockCfg[0].intvpar0        =   pCodeBlkParams->f1Km;
        pEncHdrCfg->blockCfg[0].intvpar1        =   pCodeBlkParams->f2Km;

        pEncHdrCfg->blockCfg[1].intvpar0        =   pCodeBlkParams->f1Kp;
        pEncHdrCfg->blockCfg[1].intvpar1        =   pCodeBlkParams->f2Kp;
    }
    pEncHdrCfg->blockCfg[2].block_size          =   0;
    pEncHdrCfg->blockCfg[2].num_code_blks       =   0;

    pEncHdrCfg->blockCfg[2].intvpar0            =   0;
    pEncHdrCfg->blockCfg[2].intvpar1            =   0;

    for (i = 0; i < 3; i ++)
    {
        pEncHdrCfg->blockCfg[i].intvpar2        =   0;
        pEncHdrCfg->blockCfg[i].intvpar3        =   0;
    }

    return;        
}

/*配置速率匹配参数*/
void prepare_lte_rmhdr_cfg 
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
void prepare_modhdr_cfg 
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


void prepare_tmhdr_cfg (Bcp_TmHdrCfg* pTmHdrCfg)
{
    pTmHdrCfg->ps_data_size     =   0;
    pTmHdrCfg->info_data_size   =   0;

    return;
}


static signed int add_bcp_config_data(unsigned char*  pDataBuffer,
                                   USER_PARA *user_para_ptr,
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
    unsigned char               codeword_to_layer_mapping;

    /* Get the code block params */
    compute_cbparams_ref (user_para_ptr->tbSize[numTestPkts], &codeBlkParams,LTE_PDSCH);

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
    prepare_crchdr_cfg (&crcHdrCfg, 
                        radioStd, 
                        user_para_ptr->tbSize[numTestPkts],
                        codeBlkParams.numFillerBits, 
                        0, 0, 0, 0, 0, 0, 0, NULL);
    if (Bcp_addCRCHeader (&crcHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add CRC Header to packet \n");
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen = 0;

    /* Header 3: Encoder Header */
    prepare_lte_enchdr_cfg (&encHdrCfg, radioStd, lteChanType, &codeBlkParams);
    if (Bcp_addEncoderHeader (&encHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add Encoder Header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen      = 0;
#if 1
    /* Header 4: Rate matching header */
    if(0 == numTestPkts)/*传输块0*/
        codeword_to_layer_mapping = codeword_to_layer_mapping_0[user_para_ptr->num_code_word - 1][user_para_ptr->numLayers - 1];
    else
    	codeword_to_layer_mapping = codeword_to_layer_mapping_1[user_para_ptr->num_code_word - 1][user_para_ptr->numLayers - 1];
    compute_rmGamma_ref( user_para_ptr->numBitsG[numTestPkts],
                         user_para_ptr->modulation[numTestPkts],
                         codeword_to_layer_mapping,
                         &codeBlkParams,
                         user_para_ptr->rvIdx,
                         0, //ratio is 0
                         0,
                         user_para_ptr->Nir,
                         lteChanType,
                         &rmParams);

    prepare_lte_rmhdr_cfg(&lteRmHdrCfg, lteChanType, &codeBlkParams, &rmParams);

    if (Bcp_addLte_RMHeader(&lteRmHdrCfg, pDataBuffer, &tmpLen) < 0)
    {          
        printf ("Failed to add lte rate modulation header to packet \n");            
        return -1;
    }
    pDataBuffer   +=  tmpLen;
    dataBufferLen +=  tmpLen;
    tmpLen = 0;

    /* Header 5: Modulation header */
    cInit = user_para_ptr->rnti * (1 << 14) + \
    user_para_ptr->codeWordIdx[numTestPkts] * (1 << 13) + ns/2  * (1 << 9) + cell_para_ptr->cellID;
    prepare_modhdr_cfg( &modHdrCfg, 
                        radioStd, 
                        0, // number of subcarrier, not used,
                        user_para_ptr->modulation[numTestPkts],
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
    CACHE_invL2((void *)user_para_ptr->tb_ptr[numTestPkts],((user_para_ptr->tbSize[numTestPkts] + 31)>>5)*4,CACHE_NOWAIT);
    memcpy(pDataBuffer,(const void*)user_para_ptr->tb_ptr[numTestPkts],((user_para_ptr->tbSize[numTestPkts] + 31)>>5)*4);
    dataBufferLen += ((user_para_ptr->tbSize[numTestPkts] + 31)>>5)*4;/*byte为单位*/
    /* Successfully read the test configuration */        
    return dataBufferLen;
}

/*计算和配置bcp相关参数*/
unsigned int bcp_lte_pdsch(USER_PARA *user_para_ptr,
                           CELL_PARA *cell_para_ptr,
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
       
    unsigned char* pDataBuffer;
    unsigned char* pRxDataBuffer;
    unsigned int tx_num_packet = user_para_ptr->num_code_word;

    for (numTestPkts = 0; numTestPkts < tx_num_packet; numTestPkts ++)
    {
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
        dataBufferLenUsed = add_bcp_config_data(pDataBuffer,
                                                user_para_ptr,
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

    }

    /* Wait on data to be received from BCP and validate it. Poll on Rx queue for results. */
    while (Qmss_getQueueEntryCount (BCP_Rx_QUE) < tx_num_packet);


    while (Qmss_getQueueEntryCount (BCP_Rx_QUE) == tx_num_packet)
    {
        if(tx_num_packet==0)
        	break;                      //2015.8.7JX加
        for (numTestPkts = 0; numTestPkts < tx_num_packet; numTestPkts ++)
        {
            /* Data could arrive scattered across multiple linked descriptors.
             * Collect data from all linked descriptors and validate it.
             */
            rxDataTotalLen = 0;
            
            pRxDesc = (BCP_HostPacketDescriptor*) Qmss_queuePop (BCP_Rx_QUE);
            
            descType = Cppi_getDescType ((Cppi_Desc*)pRxDesc);
            
            /* Get Data buffer containing the output and its length */
            Cppi_getData (descType, (Cppi_Desc*)pRxDesc, &pRxDataBuffer, &rxDataBufferLen);
            user_para_ptr->modulation_data_ptr[numTestPkts] = (unsigned int)pRxDataBuffer;
            
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
               
    }
    
    return(test_ok_flag);
}

