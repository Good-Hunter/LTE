/***********************************************/
/*bcp_func                                     */
/*π¶ƒ‹£∫bcp≈‰÷√∫Ø ˝                            */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"
/* QM Accumalator firmware include */
//#include "qmss_firmware.h"
#include "acc48_be_bin.h"

BCP_QmQueueTable BCP_QmTable[BCP_NUM_QUEUES] =
{
/*Idx        Que:     Thr:  Open:   Type:               DescNum: */
/*  0    */ {868,  0,   0,    BCP_QUETYPE_TXQ,    0                 }, //BCP Tx queue 0
/*  1    */ {896 , 0,   0,    BCP_QUETYPE_FDQ,    BCP_MAX_NUM_FD   }, //General purpose queue
/*  2    */ {897 , 0,   0,    BCP_QUETYPE_FDQ,    BCP_MAX_NUM_FD   }, //General purpose queue
/*  3    */ {898,  0,   0,    BCP_QUETYPE_OTHER,  0                 }, //General purpose queue
};

/* Notes:
1.	na:       CPPI LLD Handle for the flow (used internally)
2.	CDMA:     CDMA ID in TN, refer to CSL
3.	Flow:     flow ID (0 to N-1) for the specific CDMA
4.	RxQ:      Rx dest queue index
5.	FDQ0:     FDQ0 index from which to pop RX free descriptor
 */
BCP_CdmaChanTable BCP_cdmaChan[BCP_NUM_CDMA_CHANNELS] =
{
/* Purpose:            na: CDMA:                   Chan:     Pri:    Type: */
/* BCP  Tx Chan 0 */  {0,  Cppi_CpDma_BCP_CPDMA,    0,        0,      BCP_CDMACHTYPE_TX},
/* BCP  Rx Chan 0 */  {0,  Cppi_CpDma_BCP_CPDMA,    1,        0,      BCP_CDMACHTYPE_RX},
};


BCP_CdmaFlowTable BCP_cdmaFlow[BCP_NUM_CDMA_FLOWS] =
{
/* Name:          na: CDMA:                     Flow:             RxQ:        FDQ0 */
/* BCP FLOW 0 */{0, Cppi_CpDma_BCP_CPDMA,      FLOW0,         BCP_Rx_QUE,  BCP_Rx_FDQ  },
};

#pragma DATA_SECTION (rowTbl, ".testData");
uint32_t rowTbl [71] = {24,  48,  72,  96,  120,
                      144, 192, 216, 240, 288,
					  360, 384, 432, 480, 576,
					  600, 648, 720, 768, 864,
					  960, 1080,1152,1200,1296,
					  1440,1536,1728,1800,1920,
					  1944,2160,2304,2400,2592,
					  2880,3072,3240,3456,3600,
					  3840,3888,4320,4608,4800,
					  5184,5400,5760,5832,6144,
					  6480,6912,7200,7680,7776,
					  8640,9216,9600,10368,10800,
					  11520,11664,12288,12960,13824,
					  14400,15360,15552,17280,18432,
					  19200};



signed int enable_bcp (void)
{
#ifndef SIMULATOR_SUPPORT
    /* BCP power domain is turned OFF by default. It
     * needs to be turned on before doing any BCP device
     * register access.
     */
    /* Set BCP Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_BCP_FFTC_C_TCP3D_C);

    /* Enable the clocks too for BCP */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_BCP_FFTC_C_TCP3D_C, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_BCP_FFTC_C_TCP3D_C);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_BCP_FFTC_C_TCP3D_C));

    /* Return BCP PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_BCP_FFTC_C_TCP3D_C) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_BCP_FFTC_C_TCP3D_C) == PSC_MODSTATE_ENABLE))
    {
        /* BCP ON. Ready for use */            
        return 0;
    }
    else
    {
        /* BCP Power on failed. Return error */            
        return -1;            
    }
#else
    /* No power up needed on Sim */
    return 0;
#endif
}
#if 1
Qmss_Result BCP_initQm()
{
    Qmss_Result    qm_result = QMSS_SOK;
    Qmss_InitCfg   qm_init;

    memset(&qm_init, 0, sizeof(qm_init));
    qm_init.linkingRAM0Base = 0x00080000; //set to QMSS internal LRAM
    qm_init.linkingRAM0Size = QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE; //set to max (16K entries)
    qm_init.linkingRAM1Base = 0x00000000; //not used
    qm_init.maxDescNum      = QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE; //set to internal RAM size
    
    qm_init.pdspFirmware[0].pdspId     = Qmss_PdspId_PDSP1;
    qm_init.pdspFirmware[0].firmware = &acc48_be;
    qm_init.pdspFirmware[0].size     = sizeof (acc48_be);
    
    qm_result = Qmss_init(&qm_init, qmssGblCfgParams);
    if (qm_result != QMSS_SOK)
    {
      printf("Cannot initialize QMSS LLD");
    }
    return qm_result;
}
#endif
Cppi_Result BCP_initCppi(void)
{
    Cppi_Result cppi_result;
    cppi_result = Cppi_init(cppiGblCfgParams);
    if (cppi_result != CPPI_SOK)
    {
      printf("Cannot initialize CPPI LLD");
    }
    return cppi_result;
}

void BCP_initBcp()
{
    Bcp_TmFlowEntry tmFlowCfg;
    if (Bcp_lldOpen (CSL_BCP, CSL_BCP_CONFIG_REGS, &glbBcpLldObj) != 0)
    {
        /* BCP LLD Instance open failed */            
        printf("BCP LLD Instance open failed");
    }
    
    pBcpLldObj = &glbBcpLldObj;
    Bcp_initDevice (pBcpLldObj);
   
    /* Setup TM flow entry configuration for BCP */      
    tmFlowCfg.pkt_type            =   0;      // No special pkt assignments
    tmFlowCfg.dsp_int_sel         =   0;      // signed interrupt core 0
#ifdef _BIG_ENDIAN 
	#ifdef SIMULATOR_SUPPORT
	tmFlowCfg.endian_in 		  =   Bcp_EndianFormat_128;
	tmFlowCfg.format_in 		  =   Bcp_DataFormat_NoChange;    
    tmFlowCfg.endian_out          =   Bcp_EndianFormat_128; 
    tmFlowCfg.format_out          =   Bcp_DataFormat_NoChange;
	#else
    tmFlowCfg.endian_in 		  =   Bcp_EndianFormat_32;
	tmFlowCfg.format_in 		  =   Bcp_DataFormat_NoChange;    
    tmFlowCfg.endian_out          =   Bcp_EndianFormat_32; 
    tmFlowCfg.format_out          =   Bcp_DataFormat_NoChange;
	#endif

#else
	tmFlowCfg.format_in 		  =   Bcp_DataFormat_NoChange;
	tmFlowCfg.endian_in           =   Bcp_EndianFormat_128;
    tmFlowCfg.endian_out          =   Bcp_EndianFormat_128; //same as endian_in
    tmFlowCfg.format_out          =   Bcp_DataFormat_NoChange; //same as format_in
#endif    
    tmFlowCfg.qfifo_out           =   1;      // Use QFIFO1
    tmFlowCfg.ps_flags            =   0;      // No PS for now
    Bcp_setTmFlowEntry (pBcpLldObj, FLOW0, &tmFlowCfg);
	
    /* It seems the dlg functionality is not simulated */
    Bcp_setModuleDlgControlReg(pBcpLldObj, Bcp_ModuleId_TM, Bcp_DlgCtrlMode_OneShot, 0x1ff);
}

void BCP_setupQm()
{
    uint16_t         idx, idx2;
    unsigned char          *baseAddr;
    unsigned int         descNum;
    unsigned char          dontCare;
    Qmss_QueueHnd  handle;
    Qmss_Result    qm_result;
    BCP_HostPacketDescriptor *desc;
    
    /* Call the LLD start function on all cores. */
    qm_result = Qmss_start();
    if (qm_result != QMSS_SOK)
    {  
        printf ("Cannot initialize QMSS LLD");
    }
    
    /* Setup all the queues in the data structures. */
    for (idx = 0; idx < BCP_NUM_QUEUES; idx ++)
    {
        {
            handle =
                Qmss_queueOpen(Qmss_QueueType_GENERAL_PURPOSE_QUEUE,
                         BCP_QmTable[idx].queue,
                        &dontCare);
    
            if ((handle < 0) || (handle != BCP_QmTable[idx].queue))
            {
                printf ("Cannot open QM queue");
                BCP_QmTable[idx].queue = 0xffffffff; //invalid
            }
            else
            {
                BCP_QmTable[idx].open = 1; //successful
            }
        }
    }
    
    baseAddr = (unsigned char *)&glbDescBuf[0][0];
    baseAddr = (unsigned char *)L2GLBMAP(baseAddr, glbCoreId);
    /* Setup memory region */
    {
        Qmss_Result    qm_result;
        Qmss_MemRegInfo tmpMemRegInfo;
        
        tmpMemRegInfo.descBase      = (unsigned int *)baseAddr;
        tmpMemRegInfo.descSize      = BCP_DESC_SIZE;
        tmpMemRegInfo.descNum       = BCP_MAX_NUM_FD*2;
        tmpMemRegInfo.memRegion     = Qmss_MemRegion_MEMORY_REGION2;
        tmpMemRegInfo.startIndex    = NUM_FFTC_HOST_DESC + LL2_HOST_DESC_LINK_ENTRY_SIZE;
        tmpMemRegInfo.manageDescFlag = Qmss_ManageDesc_UNMANAGED_DESCRIPTOR;
    
        qm_result = Qmss_insertMemoryRegion(&tmpMemRegInfo);
    
        if (qm_result < QMSS_SOK)
        {
            printf("Cannot insert QMSS Memory Region");
        }
    }
    
    /* Empty all the used queues */
    for (idx = 0; idx < BCP_NUM_QUEUES; idx ++)
    {
        if( BCP_QmTable[idx].type != BCP_QUETYPE_TXQ )
        {
            while( Qmss_getQueueEntryCount(BCP_QmTable[idx].queue) > 0 )
            {
                Qmss_queuePop(BCP_QmTable[idx].queue);
            }
        }
    }
    
    /* Initialize the FDQs with descriptors. */
    for (idx = 0; idx < BCP_NUM_QUEUES; idx ++)
    {
        if (BCP_QmTable[idx].type == BCP_QUETYPE_FDQ)
        {
            descNum  = BCP_QmTable[idx].descNum;
            for (idx2 = 0; idx2 < descNum; idx2 ++)
            {
                desc = (BCP_HostPacketDescriptor *)baseAddr;
                desc->type_id = BCP_DESC_TYPE_HOST;
                desc->return_policy = 1;
          
                Qmss_queuePushDesc(BCP_QmTable[idx].queue, (void*)baseAddr);
          
                baseAddr += BCP_DESC_SIZE;
            }
        }
    }
    return;
}

void BCP_setupCppi()
{
    int idx;
    /* Open CDMA */
    {
        Cppi_CpDmaInitCfg cfg;
       
        memset ( &cfg, 0, sizeof (Cppi_CpDmaInitCfg));
        
        cfg.dmaNum = (Cppi_CpDma) Cppi_CpDma_BCP_CPDMA;
        cfg.qm0BaseAddress = 0x34020000; // BCP_QM_BASEADDRREG_0;
        cfg.qm1BaseAddress = 0x34030000; //BCP_QM_BASEADDRREG_1;
        cfg.qm2BaseAddress = 0; //BCP_QM_BASEADDRREG_2;
        cfg.qm3BaseAddress = 0; //BCP_QM_BASEADDRREG_3;
        
        glbBCPCdmaHdl = Cppi_open(&cfg);
        
        if (glbBCPCdmaHdl == NULL)
        {
              printf ("Cannot open CDMA LLD");
        }
        else
        {
            /* Disable loopback */ 
            Cppi_setCpdmaLoopback  ( glbBCPCdmaHdl, 0 );  
        }   
    }
    
    /* Open Tx/Rx channels */
    for( idx = 0; idx < BCP_NUM_CDMA_CHANNELS; idx++ )
    {
        uint16_t cdma;
        unsigned char  dontcare;
        Cppi_RxChInitCfg rxcfg;
        Cppi_TxChInitCfg txcfg;
    
        cdma = BCP_cdmaChan[idx].cdma;
        
        if (BCP_cdmaChan[idx].chan_type == BCP_CDMACHTYPE_TX) 
        {
            /* Open the Tx channel. */
            txcfg.channelNum = BCP_cdmaChan[idx].channel;
            txcfg.txEnable   = Cppi_ChState_CHANNEL_ENABLE;
            txcfg.priority   = BCP_cdmaChan[idx].tx_pri;
            txcfg.filterEPIB = 0; //don't pass EPIB
            txcfg.filterPS   = 0; //pass PS data
            txcfg.aifMonoMode= 0;
            
            BCP_cdmaChan[idx].handle =
              Cppi_txChannelOpen(glbBCPCdmaHdl, &txcfg, &dontcare);
            
            if (BCP_cdmaChan[idx].handle == NULL)
            {
                printf ("Cannot open CDMA %d Tx Channel %d", cdma, idx);
            }
        }
        else
        {
            /* Open the Rx channel. */
            rxcfg.channelNum = BCP_cdmaChan[idx].channel;
            rxcfg.rxEnable   = Cppi_ChState_CHANNEL_ENABLE;
            
            BCP_cdmaChan[idx].handle =
              Cppi_rxChannelOpen(glbBCPCdmaHdl, &rxcfg, &dontcare);
            
            if (BCP_cdmaChan[idx].handle == NULL)
            {
                printf ("Cannot open CDMA %d Rx Channel %d", cdma, idx);
            }
        }
    }//End of Open Tx/Rx channels 
    
    /* Open flow */
    for( idx = 0; idx < BCP_NUM_CDMA_FLOWS; idx++ )
    {
        uint16_t cdma;
        uint16_t qn;
        unsigned char  dontcare;
        BCP_CdmaFlowTable *flo_tbl;
        Cppi_RxFlowCfg flow;
        
        memset(&flow, 0, sizeof(flow));
        
        flo_tbl = &BCP_cdmaFlow[idx];
        cdma    = flo_tbl->cdma;
        
        // Fill in common parameters (unused fields are left zero)
        flow.flowIdNum = flo_tbl->flow_id;
        qn = flo_tbl->dest_q;
        flow.rx_dest_qmgr = (qn & 0x3000) >> 12;
        flow.rx_dest_qnum = (qn & 0x0fff);
        flow.rx_ps_location = 0; //PS data located in descriptor (FFTC=64 byte desc)
        flow.rx_desc_type = BCP_DESC_TYPE_HOST;
        flow.rx_sop_offset = 0;  
        flow.rx_psinfo_present = 1; //pass PS data
        flow.rx_einfo_present = 0;  //drop EPIB data
        
        qn = flo_tbl->fdq_0;
        flow.rx_fdq0_sz0_qmgr =(qn & 0x3000) >> 12;
        flow.rx_fdq0_sz0_qnum =(qn & 0x0fff);
        
        flo_tbl->handle =
          Cppi_configureRxFlow(glbBCPCdmaHdl, &flow, &dontcare);
        
        if (flo_tbl->handle == NULL)
        {
            printf ("Cannot open CDMA %d Rx Flow %d", cdma, idx);
        }
    }//End of open flow
}


void BCP_setupDesc()
{
    BCP_HostPacketDescriptor * pkt;
    signed int idx, qn;
    
    /* Setup TX FDQ descriptors */
    for( idx = 0; idx < BCP_MAX_NUM_FD; idx++ )
    {
        pkt = (BCP_HostPacketDescriptor *)Qmss_queuePop(BCP_Tx_FDQ);
        memset(pkt, 0, sizeof(BCP_HostPacketDescriptor));
        
        /* Configed at initial time */      
        pkt->type_id        = BCP_DESC_TYPE_HOST;
        pkt->ps_reg_loc     = 0;
        pkt->ps_flags       = 0;
        pkt->buffer_len     = sizeof(unsigned int)*INBUF_SIZE;
        pkt->buffer_ptr     = L2GLBMAP((unsigned int)srcBuf[idx],glbCoreId);
        pkt->psv_word_count = 0;
        qn = BCP_Tx_FDQ;
        pkt->pkt_return_qmgr    = qn >> 12;
        pkt->pkt_return_qnum    = qn & 0x0fff;
        pkt->next_desc_ptr      = NULL;
        pkt->orig_buff0_len     = sizeof(unsigned int)*INBUF_SIZE;
        pkt->orig_buff0_ptr     = L2GLBMAP((unsigned int)srcBuf[idx],glbCoreId);
        pkt->src_tag_lo         = FLOW0;
        pkt->packet_length  = sizeof(unsigned int)*INBUF_SIZE; 
        Qmss_queuePushDesc(BCP_Tx_FDQ, (void*)pkt);
    }
    
    /* Setup RX FDQ descriptors */
    for( idx = 0; idx < BCP_MAX_NUM_FD; idx++ )
    {
        pkt = (BCP_HostPacketDescriptor *)Qmss_queuePop(BCP_Rx_FDQ);
        memset(pkt, 0, sizeof(BCP_HostPacketDescriptor));
        
        /* Configed at initial time */      
        pkt->type_id        = BCP_DESC_TYPE_HOST;
        pkt->ps_reg_loc     = 0;
        pkt->ps_flags       = 0;
        pkt->buffer_len     = sizeof(unsigned int)*OUTBUF_SIZE;
        pkt->buffer_ptr     = L2GLBMAP((unsigned int)dstBuf[idx],glbCoreId);
        pkt->psv_word_count = 0;
        qn = BCP_Rx_FDQ;
        pkt->pkt_return_qmgr    = qn >> 12;
        pkt->pkt_return_qnum    = qn & 0x0fff;
        pkt->next_desc_ptr      = NULL;
        pkt->orig_buff0_len     = sizeof(unsigned int)*OUTBUF_SIZE;
        pkt->orig_buff0_ptr     = (unsigned int)L2GLBMAP((unsigned int)dstBuf[idx],glbCoreId);
        Qmss_queuePushDesc(BCP_Rx_FDQ, (void*)pkt);
    }
}

int8_t gind_row_index (uint32_t input)
{
	int8_t ii;

	for (ii = 0; ii < 71; ii++)
	{
		if (rowTbl[ii] == input)
			return ii;
	}

    return 0;
}
