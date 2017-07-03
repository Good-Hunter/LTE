/***********************************************/
/*fftc_func                                    */
/*功能：fftc配置函数                           */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"
#define SYMBOL_LEN (2048+144)
#define SYMBOL0_LEN (2048+160)
extern unsigned volatile short  slot_idx;
extern unsigned int fft_data1[10][2][16800];
extern unsigned int l2_global_address(unsigned int coreId, unsigned int addr);
signed int enable_fftc (void)
{
#ifndef SIMULATOR_SUPPORT
    /* FFTC power domain is turned OFF by default. It
     * needs to be turned on before doing any FFTC device
     * register access.
     */
    /* Set FFTC Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_FFTC_AB);

    /* Enable the clocks too for FFTC */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_FFTC_AB, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_FFTC_AB);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_FFTC_AB));

    /* Return FFTC PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_FFTC_AB) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_FFTC_AB) == PSC_MODSTATE_ENABLE))
    {
        /* FFTC ON. Ready for use */            
        return 0;
    }
    else
    {
        /* FFTC Power on failed. Return error */            
        return -1;            
    }
#else
    /* No power up needed on Sim */
    return 0;
#endif
}

unsigned int fftc_system_init(unsigned int fftc_inst)
{
    unsigned int setupVal, i;
    unsigned short qm_map[4];
    unsigned short idx;
    MNAV_HostPacketDescriptor_fftc *host_pkt;

    /*initilize CPPI/QMSS region and link ram*/
    memset((void *)(l2_global_address(glbCoreId, (unsigned int) gHostDescRegion_fftc)), 0, SIZE_FFTC_HOST_DESC * NUM_FFTC_HOST_DESC);
    setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1) << 16;
    set_memory_region(NUM_FFTC_MEM_REGION, l2_global_address(glbCoreId, (unsigned int) gHostDescRegion_fftc), LL2_HOST_DESC_LINK_ENTRY_SIZE, setupVal);
    set_link_ram(NUM_FFTC_LINK_RAM, QM_LRAM_REGION, 0x3fff); //internal link ram SRIO处设置
    
    /*Initialize FFTC PKTDMA channel*/
    if(fftc_inst == CSL_FFTC_A)
    {
        /* Program the logical queue managers for FFTC PKTDMA: */
        qm_map[0] = 0;
        qm_map[1] = 5000;
        qm_map[2] = 7000;
        qm_map[3] = 0xffff;         /* unused */
        config_pktdma_qm(FFTCA_PKTDMA_GBL_CFG_REGION, qm_map);
        enable_disable_loopback(FFTCA_PKTDMA_GBL_CFG_REGION, 0x00000000); // Disable CPPI Loopback
        /* Configure the Transmit Queue Threshold. */
        set_queue_threshold((unsigned short) NUM_FFTCA_QUE_TX_Q0, (unsigned int) 0x00000081); // hilo = hi, threshold = 1
        /* Open all CPPI Tx Channels. These will be used to send data to SRIO */
        for (i = 0; i < NUM_FFTC_TX_CHANNELS; i++) {
            /* Set Tx Channel priority equals to 2 */
            enable_tx_chan(FFTCA_PKTDMA_TX_CHAN_REGION, i, 0x80000000);
        }
        /* Open all CPPI Rx channels. These will be used by SRIO to stream data out. */
        for (i = 0; i < NUM_FFTC_RX_CHANNELS; i++) {
            enable_rx_chan(FFTCA_PKTDMA_RX_CHAN_REGION, i, 0x80000000);
        }
    }
    if(fftc_inst == CSL_FFTC_B)
    {
        /* Program the logical queue managers for FFTC PKTDMA: */
        qm_map[0] = 0;
        qm_map[1] = 5000;
        qm_map[2] = 7000;
        qm_map[3] = 0xffff;         /* unused */
        config_pktdma_qm(FFTCB_PKTDMA_GBL_CFG_REGION, qm_map);
        enable_disable_loopback(FFTCB_PKTDMA_GBL_CFG_REGION, 0x00000000); // Disable CPPI Loopback
        /* Configure the Transmit Queue Threshold. */
        set_queue_threshold((unsigned short) NUM_FFTCB_QUE_TX_Q0, (unsigned int) 0x00000081); // hilo = hi, threshold = 1
        /* Open all CPPI Tx Channels. These will be used to send data to SRIO */
        for (i = 0; i < NUM_FFTC_TX_CHANNELS; i++) {
            /* Set Tx Channel priority equals to 2 */
            enable_tx_chan(FFTCB_PKTDMA_TX_CHAN_REGION, i, 0x80000000);
        }
        /* Open all CPPI Rx channels. These will be used by SRIO to stream data out. */
        for (i = 0; i < NUM_FFTC_RX_CHANNELS; i++) {
            enable_rx_chan(FFTCB_PKTDMA_RX_CHAN_REGION, i, 0x80000000);
        }
    }
    
    /* Push Host Descriptors to Global Free Queue */
    for (idx = 0; idx < NUM_FFTC_HOST_DESC; idx++) {
        host_pkt = (MNAV_HostPacketDescriptor_fftc *) (l2_global_address(glbCoreId, (unsigned int) gHostDescRegion_fftc) + (idx * SIZE_FFTC_HOST_DESC));
        host_pkt->type_id = MNAV_DESC_TYPE_HOST;
        host_pkt->return_policy = 0; // Return the whole packet
        host_pkt->ret_push_policy = 0; // Return to tail of the Queue
        host_pkt->ps_reg_loc = 0; // PS in Descriptor
        host_pkt->epib = 0;     // Present EPIB
        push_queue(NUM_FFTC_QUE_GBL_FREE, 1, 0, (unsigned int) (host_pkt)); // Only write Reg D, so mode = 1, c_val = NULL 
    }
    //printf("Debug: Queue Manager and CPPI are initialized.\n");
    //printf("Debug: Host Region of FFTC is 0x%x\n", gHostDescRegion_fftc);

    return 0;
}

signed int fftc_global_config(CSL_FftcRegs* fftc_cfgRegs)
{
    /* FFTC Global config resiter*/
    fftc_cfgRegs->CONFIG =  CSL_FMK (FFTC_CONFIG_Q3_FLOWID_OVERWRITE, 0x00) |
                                CSL_FMK (FFTC_CONFIG_Q2_FLOWID_OVERWRITE, 0x00) |
                                CSL_FMK (FFTC_CONFIG_Q1_FLOWID_OVERWRITE, 0x00) |
                                CSL_FMK (FFTC_CONFIG_Q0_FLOWID_OVERWRITE, 0x00) |        
                                CSL_FMK (FFTC_CONFIG_STARVATION_PERIOD, 0x00) |          
                                CSL_FMK (FFTC_CONFIG_QUEUE_3_PRIORITY, 0x00) |         
                                CSL_FMK (FFTC_CONFIG_QUEUE_2_PRIORITY, 0x00) |          
                                CSL_FMK (FFTC_CONFIG_QUEUE_1_PRIORITY, 0x00) |          
                                CSL_FMK (FFTC_CONFIG_QUEUE_0_PRIORITY, 0x00) |         
                                CSL_FMK (FFTC_CONFIG_FFT_DISABLE, 0x00);  
  return 0;
  
}

/*基本的FFT_IFFT功能，不支持去CP等*/
unsigned int fft_config_basic(unsigned int *dft_size_list,
              unsigned int dft_length,
              unsigned int *fftc_input_data_ptr,
              unsigned int dft_idft_flag,
              unsigned int *fftc_output_data_address,
              unsigned int fftc_inst,
              unsigned int input_data_size)
{
    unsigned int pkt_desc,i;
    unsigned int header_sz, pkt_sz;
    unsigned int *tx_buffer;

    unsigned int PSFiledLen,*PSField;
    unsigned int DFTSizeListLen;
    unsigned int DFTSizeList[26] = {0};
    unsigned int bLocalConfig, fftLocalCfg[5];
    signed int dft_idx;

    for(i = 0;i < 26;i++)
    {
       DFTSizeList[i] = 0;
    }

    /*Setup TX free queue*/
    if(Setup_Tx_FDQ() != 0)
    {
        printf ("[Core %d]: FFTC setup TX FDQ failed \n", glbCoreId);
        return 0;
    }
    /*Setup RX free queue*/
    if(Setup_Rx_FDQ(fftc_output_data_address) != 0)
    {
        printf ("[Core %d]: FFTC setup RX FDQ failed \n", glbCoreId);
        return 0;
    }

    /*CSL_FFTC_A:fftc_cfgRegs = (CSL_FftcRegs*)(0x021F0000)
     CSL_FFTC_B:fftc_cfgRegs = (CSL_FftcRegs*)(0x021F4000)*/
    /*FFTC Global config*/
    if(CSL_FFTC_A == fftc_inst)
    {
        if(fftc_global_config((CSL_FftcRegs*)0x021F0000) != 0)
        {
            printf ("[Core %d]: FFTC Global config failed \n", glbCoreId);
            return 0;
        }
    }
    else
    {
        if(fftc_global_config((CSL_FftcRegs*)0x021F4000) != 0)
        {
            printf ("[Core %d]: FFTC Global config failed \n", glbCoreId);
            return 0;
        }
    }



    /*Get a free descriptor from TX free queue*/
    pkt_desc = fftc_get_descriptor(NUM_FFTC_QUE_TX_FREE);
    if(pkt_desc == 0)
    {
        //printf ("[Core %d]: FFTC Get TX free descriptor failed \n", glbCoreId);
        return 0;
    }

    /* FFTC configure words*/
    tx_buffer = (unsigned int *)gTxBuffer[0];

    PSFiledLen    = 0;      //pass through field length
    PSField     = 0;      //pass through words

    if(dft_length == 1)
    {
      DFTSizeListLen  = 0;      //dft size list length

      pkt_sz = input_data_size;
      dft_idx = fftc_get_dft_idx(dft_size_list[0]);
    }
    else if(dft_length <= MAX_DFT_SIZE_LIST_LENGTH)
    {
      DFTSizeListLen  = dft_length;     //dft size list length

      dft_idx = 0x3f;
      pkt_sz = 0;
      pkt_sz = input_data_size;
      for(i=0;i<dft_length;i++)
      {
        DFTSizeList[i] = fftc_get_dft_idx(dft_size_list[i]);
      }
    }

    bLocalConfig  = 1;      //local configure register words
    fftLocalCfg[0]  = 0x00003fff; //DESTINATION_QUEUE_AND_SHIFTING_REGISTER
    fftLocalCfg[1]  = 0x22000000; //SCALING_REGISTER
    fftLocalCfg[2]  = 0;//0x000000a0; //CYCLIC_PREFIX_REGISTER
    fftLocalCfg[3]  = 0x00000000 |((unsigned int)dft_idx&0x0000003F)|((unsigned int)(dft_idft_flag << 6) & 0x00000040); //CONTROL_REGISTER
    fftLocalCfg[4]  = 0x00000000; //LTE_FREQUENCY_SHIFT_REGISTER



    /*Build FFTC pkt header*/
    header_sz = fftc_build_header(tx_buffer,
                  PSFiledLen, PSField,
                  DFTSizeListLen, DFTSizeList,
                  bLocalConfig, fftLocalCfg);

    /* Copy data to send buffer*/
    memcpy((unsigned int *)(tx_buffer+header_sz), fftc_input_data_ptr, pkt_sz*4);


    /* Send data to FFTC*/
    if(fftcInstNum == CSL_FFTC_A)
      fftc_send_pkt(pkt_desc, pkt_sz+header_sz, header_sz, tx_buffer, NUM_FFTCA_QUE_TX_Q0);
    if(fftcInstNum == CSL_FFTC_B)
      fftc_send_pkt(pkt_desc, pkt_sz+header_sz, header_sz, tx_buffer, NUM_FFTCB_QUE_TX_Q0);

    /* Receive data from FFTC*/
      fftc_recv_pkt(NUM_FFTC_QUE_RX_POLL,0,0);



      return 1;
}
/*支持去CP等*/
unsigned int fft_config(unsigned int *dft_size_list,
                        unsigned int dft_length,
                        unsigned int pkt_sz,
                        unsigned int *fftc_input_data_ptr,
                        unsigned int dft_idft_flag,
                        unsigned int *fftc_output_data_address,
                        unsigned int fftc_inst,
                        unsigned int cp_removal_len,
                        unsigned char start_symbol,
                        unsigned char end_symbol)
{
    unsigned int pkt_desc;
    unsigned int header_sz;
    unsigned int *tx_buffer;

    unsigned int PSFiledLen,*PSField;
    unsigned int DFTSizeListLen;
    unsigned int bLocalConfig, fftLocalCfg[5];
    signed int dft_idx,i,j;
    unsigned int DFTSizeList[26];

     for(i = 0;i < 26;i++)
     {
        DFTSizeList[i] = 0;
     }
     PSFiledLen    = 0;      //pass through field length
     PSField     = 0;      //pass through words
     
     if(dft_length == 1)
     {
       DFTSizeListLen  = 0;      //dft size list length
       dft_idx = fftc_get_dft_idx(dft_size_list[0]);
     }
     else if(dft_length <= MAX_DFT_SIZE_LIST_LENGTH)
     {
       DFTSizeListLen  = dft_length;     //dft size list length
     
       for(i = 0;i < dft_length;i++)
       {
            DFTSizeList[i] = fftc_get_dft_idx(dft_size_list[i]);
       }
       dft_idx = 0x3f;
     }
     
     bLocalConfig  = 1;      //local configure register words
     fftLocalCfg[0]  = 0x80003fff; //DESTINATION_QUEUE_AND_SHIFTING_REGISTER  Output circular shift enabled
     /*SCALING_REGISTER 改成静态agc,每一级右移1位，默认agc因子是3，因此需要补偿6-3=3位)
     fftLocalCfg[1]  = 0x02002554;*/
     /*改成动态agc因子*/
     fftLocalCfg[1]  = 0x22000000;
     fftLocalCfg[2]  = (0x00000000)|(((unsigned int)0 << 16 )&0x03FF0000);
     /*返回agc因子SUPPRESS_SIDE_INFO=0*/
     fftLocalCfg[3]  = 0x000 | ((unsigned int)dft_idx&0x0000003F)|((unsigned int)(dft_idft_flag << 6) & 0x00000040); //CONTROL_REGISTER
     fftLocalCfg[4]  = 0x00000000; //LTE_FREQUENCY_SHIFT_REGISTER

        
     for(j = 0;j < NUM_TX_DESC;j++)
     {
     
         /*Get a free descriptor from TX free queue*/
           pkt_desc = fftc_get_descriptor(NUM_FFTC_QUE_TX_FREE);
           if(pkt_desc == 0)
           {
               printf ("[Core %d]: FFTC Get TX free descriptor failed \n", glbCoreId);
               return 0;
           }
     
           /* FFTC configure words*/
           tx_buffer = (unsigned int *)gTxBuffer[j];
     
           /*Build FFTC pkt header*/
           header_sz = fftc_build_header(tx_buffer,
                         PSFiledLen, PSField,
                         DFTSizeListLen, DFTSizeList,
                         bLocalConfig, fftLocalCfg);
     
           /* Copy data to send buffer*/
           if(0 == (header_sz & 1))
           {
        	   if(start_symbol%7==0)
        	   {
				   fft_input_data_copy(
						  start_symbol,
						  end_symbol,
						  ( double *)(tx_buffer+header_sz),
						  ( double *)&fftc_input_data_ptr[j * SYMBOL0_LEN] /*每次配置一根天线*/,
						  j
							);
        	   }
        	   else
        	   {
				   fft_input_data_copy(
						  start_symbol,
						  end_symbol,
						  ( double *)(tx_buffer+header_sz),
						  ( double *)&fftc_input_data_ptr[j * SYMBOL_LEN] /*每次配置一根天线*/,
							j);
        	   }
           }
           else
           {
        	   if(start_symbol%7==0)
        	   {
				   fft_input_data_copy_int(
									start_symbol,
									end_symbol,
									(unsigned int *)(tx_buffer+header_sz),
									&fftc_input_data_ptr[j * SYMBOL0_LEN] /*每次配置一根天线*/,
									j
									);
        	   }
        	   else
        	   {
				   fft_input_data_copy_int(
									start_symbol,
									end_symbol,
									(unsigned int *)(tx_buffer+header_sz),
									&fftc_input_data_ptr[j * SYMBOL_LEN] /*每次配置一根天线*/,
									j
									);
        	   }
           }
           CACHE_wbL2(tx_buffer, (end_symbol - start_symbol) * FFT_SIZE * 4 + header_sz*4, CACHE_WAIT);

           if(slot_idx==16)
           {
         	  int pause = 0;
         	  pause = pause*2;
           }
     
     
           /* Send data to FFTC*/
           if(CSL_FFTC_A == fftc_inst)
           {
               fftc_send_pkt(pkt_desc, dft_length*pkt_sz+header_sz, header_sz, tx_buffer, NUM_FFTCA_QUE_TX_Q0);
           }
           else
           {
               fftc_send_pkt(pkt_desc, dft_length*pkt_sz+header_sz, header_sz, tx_buffer, NUM_FFTCB_QUE_TX_Q0);
           }
     }          
      
      /* Receive data from FFTC*/
      if(0 != start_symbol)
          fftc_recv_pkt(NUM_FFTC_QUE_RX_POLL,start_symbol,end_symbol);
      else
        fftc_recv_pkt_symbol0(NUM_FFTC_QUE_RX_POLL,start_symbol,end_symbol);

/*      if(fft_data[0][0]!=fft_data1[slot_idx>>1][0][0]||fft_data[1][0]!=fft_data1[slot_idx>>1][1][0])
      {
    	  int pause = 0;
    	  pause = pause*2;
      }
      */
      return 1;
}

int Setup_Rx_FDQ ()
{
    unsigned char                       i;        
    MNAV_HostPacketDescriptor_fftc   *host_pkt;
    unsigned int            tmp;
    unsigned int            setupVal;

    /* Attach some free descriptors to the Rx free queue we just opened. */
    for (i = 0; i < NUM_RX_DESC; i++)
    {

     /* Get a free descriptor from the global free queue we setup 
    * during initialization.
    */
    tmp = pop_queue(NUM_FFTC_QUE_GBL_FREE);
     
     /* The descriptor address returned from the hardware has the 
    * descriptor size appended to the address in the last 4 bits.
    *
    * To get the true descriptor size, always mask off the last 
    * 4 bits of the address.
    */
    //printf("Debug: RX descriptor address is 0x%8x\n", tmp);
    tmp &= 0xFFFFFFF0;  // Set DESC_SIZE field to zero

     /* Setup the Completion queue:
    *
    * Setup the return policy for this desc to return to the free q we just
    * setup instead of the global free queue.
    */
    host_pkt = (MNAV_HostPacketDescriptor_fftc *)tmp;
    
    host_pkt->pkt_return_qmgr = 0;
    host_pkt->pkt_return_qnum = NUM_FFTC_QUE_RX_FREE; 

    host_pkt->buffer_ptr    = l2_global_address(glbCoreId, (unsigned int)&gRxBuffer[i][0]);

    host_pkt->buffer_len    = CPPI_FFTC_BUF_LEN;
    host_pkt->orig_buff0_ptr  = host_pkt->buffer_ptr;
    host_pkt->orig_buff0_len  = host_pkt->buffer_len;
    host_pkt->packet_length   = host_pkt->buffer_len;
    
    setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1);
    tmp   |= setupVal;        
    
     /* Push descriptor to Rx free queue */
    push_queue(NUM_FFTC_QUE_RX_FREE, 1, 0, tmp);
    }       
    if (i != NUM_RX_DESC)
    {
        printf ("Error allocating Rx free descriptors \n");
        return -1;
    }

    /* Setup a Rx Flow.
     *
     * A Rx flow encapsulates all relevant data properties that CPDMA would
     * have to know in order to succefully receive data.
     */
  /*config for FFTCA*/
  config_rx_flow(FFTCA_PKTDMA_RX_FLOW_REGION, 
           NUM_FFTC_RX_FLOW_ID, 
           0x60000000+NUM_FFTC_QUE_RX_POLL, /* Value of PKTDMA_REG_RX_FLOW_CFG_A */
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_B */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_C */ 
           0x02E302E3,  /* Value of PKTDMA_REG_RX_FLOW_CFG_D */ 
           0x02E302E3,  /* Value of PKTDMA_REG_RX_FLOW_CFG_E */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_F */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_G */ 
           0x00000000); /* Value of PKTDMA_REG_RX_FLOW_CFG_H */
  /*config for FFTCB*/
  config_rx_flow(FFTCB_PKTDMA_RX_FLOW_REGION, 
           NUM_FFTC_RX_FLOW_ID, 
           0x60000000+NUM_FFTC_QUE_RX_POLL, /* Value of PKTDMA_REG_RX_FLOW_CFG_A */
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_B */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_C */ 
           0x02E302E3,  /* Value of PKTDMA_REG_RX_FLOW_CFG_D */ 
           0x02E302E3,  /* Value of PKTDMA_REG_RX_FLOW_CFG_E */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_F */ 
           0x00000000,  /* Value of PKTDMA_REG_RX_FLOW_CFG_G */ 
           0x00000000); /* Value of PKTDMA_REG_RX_FLOW_CFG_H */
    return 0;
  
}

int Setup_Tx_FDQ (void)
{
  unsigned char       i;        
  unsigned int    tmp;
  unsigned int    setupVal;
  MNAV_HostPacketDescriptor_fftc   *host_pkt;

    /* Attach some free descriptors to the Tx completion queue we just opened. */
    for (i = 0; i < NUM_TX_DESC; i++)
    {
      /* Get a free descriptor from the global free queue we setup 
         * during initialization.
         */
    tmp = pop_queue(NUM_FFTC_QUE_GBL_FREE);
      //printf("Debug: TX descriptor address is 0x%8x\n", tmp);
      /* The descriptor address returned from the hardware has the 
         * descriptor size appended to the address in the last 4 bits.
         *
         * To get the true descriptor size, always mask off the last 
         * 4 bits of the address.
         */
        tmp &= 0xFFFFFFF0;  // Set DESC_SIZE field to zero

      /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the free q
         */
    host_pkt = (MNAV_HostPacketDescriptor_fftc *)tmp;
    host_pkt->pkt_return_qmgr = 0;
    host_pkt->pkt_return_qnum = NUM_FFTC_QUE_TX_FREE; 
    /* Our host descriptors will be 80 bytes and it is dividable by 16, the multiplier is 5, then minus 1 equals 4, 
    * so the value of Descriptor Size is 00000004. */
    setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1);
    tmp   |= setupVal;  // Set DESC_SIZE to 4 for 80 byte host descriptors    

    /* Push descriptor to Tx free queue */
    push_queue(NUM_FFTC_QUE_TX_FREE, 1, 0, tmp);
    }
    if (i != NUM_TX_DESC)
    {
        printf ("Error allocating Tx free descriptors \n");            
        return -1;
    }
    /* All done with Tx configuration. Return success. */
    return 0;
}

unsigned int fftc_build_header(unsigned int *tx_buffer,
            unsigned int PSFiledLen,    unsigned int *PSField, 
            unsigned int DFTSizeListLen,  unsigned int *DFTSizeList, 
            unsigned int bLocalConfig,  unsigned int *fftLocalCfg)
{
    unsigned int              *pData; 
    int           i;
    unsigned int            fftc_header_length;
    unsigned int            bPSPassThruPresent,bDFTSizeListPresent;
    unsigned int            DFTSizeListLen_M = 0;
    
    /*Initialize*/
    pData = tx_buffer;
    fftc_header_length = 0;
  
    /*Setup the FFT control header configuration.*/
    if(PSFiledLen != 0)
      bPSPassThruPresent    =   1;
    else
      bPSPassThruPresent    =   0;

    if(DFTSizeList != 0)
      bDFTSizeListPresent   =   1;
    else
      bDFTSizeListPresent   =   0;
  
    DFTSizeListLen_M = DFTSizeListLen/5+1;
    /* Compile the FFTC Control Header */
    *pData++ = (((PSFiledLen << 0x00000018) & (0x07000000)) |          /* populate the Protocol Specific Field length */
                ((DFTSizeListLen_M << 0x00000010) & (0x001F0000)) |       /* populate the DFT size list length */
                ((bPSPassThruPresent << 0x00000002) & (0x00000004)) |   /* populate the debug halt enable bit */
                ((bDFTSizeListPresent << 0x00000001) & (0x00000002)) |  /* populate the DFT list sizes present bit */
                ((bLocalConfig << 0x00000000) & (0x00000001)));  /* populate the local config present bit */  
  fftc_header_length += 1;
  
  /*Create Local Configuration*/
  for(i=0; i<5; i++)
  {
    *pData++ = fftLocalCfg[i];
  }
  if(bLocalConfig)
    fftc_header_length += 5;
  
  /*Create DFT size List*/
  for(i=0; i<DFTSizeListLen/5+1; i++)
  {
    *pData++ = (((DFTSizeList[i * 5 + 4] << 0x00000018) & (0x3F000000)) |          /* populate the Protocol Specific Field length */
                  ((DFTSizeList[i * 5 + 3] << 0x00000012) & (0x00FC0000)) |       /* populate the DFT size list length */
                  ((DFTSizeList[i * 5 + 2] << 0x0000000C) & (0x0003F000)) |   /* populate the debug halt enable bit */
                  ((DFTSizeList[i * 5 + 1] << 0x00000006) & (0x00000FC0)) |  /* populate the DFT list sizes present bit */
                  ((DFTSizeList[i * 5 + 0] << 0x00000000) & (0x0000003F)));  /* populate the local config present bit */
  }
  if(bDFTSizeListPresent)
    fftc_header_length += (DFTSizeListLen/5+1);
    
  /*Create PS words*/
  for(i=0; i<PSFiledLen; i++)
  {
    *pData++ = PSField[i];
  }
  if(bPSPassThruPresent)
    fftc_header_length += PSFiledLen;
      
    return fftc_header_length; 
}

unsigned int fftc_get_descriptor(unsigned int queue_num)
{
  unsigned int tmp;
  if ((tmp = pop_queue(queue_num)) == NULL)
    {
        //printf ("No Tx free descriptor. Cant run send/rcv test \n");
        return 0;
    }
    return tmp;
}

int fftc_get_dft_idx(unsigned int dft_sz)
{
  int i;
  for(i=0; i<50; i++)
  {
    if(dft_sz == DFT_sz_list[i])
    {
      break;
    }
  }
  if(i>=50)
    return -1;
  else
    return i;
    
}

unsigned int fftc_recv_pkt(unsigned int queue_num,unsigned char OFDMA_start,unsigned char OFDMA_end)
{
  unsigned int           *pDataBuffer;
  unsigned int *restrict fftc_input_data_ptr1;
  int                       i,j,index,offset;
  unsigned int            setupVal;
  unsigned int            pkt_cnt = 0, tmp;
  unsigned int            cycle_start, cycle_end,cycle;
  
  MNAV_HostPacketDescriptor_fftc   *host_pkt; 
  //CACHE_invL2(gRxBuffer[0], CPPI_FFTC_BUF_LEN*4, CACHE_WAIT);
  //CACHE_invL2(gRxBuffer[1], CPPI_FFTC_BUF_LEN*4, CACHE_WAIT);
  cycle_start = TSCL;
for(i = 0; i < NUM_TX_DESC;i++)
{

  while(!(pkt_cnt = get_descriptor_count(queue_num)))
  {
    asm(" nop 5");
  }

  for(j=0; j<pkt_cnt; j++)
  {
      tmp = pop_queue(queue_num);
      tmp &= 0xFFFFFFF0;
      host_pkt = (MNAV_HostPacketDescriptor_fftc *)tmp;

      host_pkt->pkt_return_qmgr = 0;
      host_pkt->pkt_return_qnum = NUM_FFTC_QUE_RX_FREE;

      pDataBuffer = (unsigned int *)(host_pkt->buffer_ptr);
      //printf("\n\nDebug: FFTC output Data length is:\n");
      //printf(" FFTC output Data length is %d,\n", host_pkt->packet_length/4);

      setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1);
      tmp   |= setupVal;
      CACHE_invL2((void *)host_pkt->buffer_ptr, host_pkt->packet_length, CACHE_WAIT);
	  CSL_XMC_invalidatePrefetchBuffer();
	  		    _mfence();
	  		    _mfence();
      /* Push descriptor to Rx free queue */
      push_queue(NUM_FFTC_QUE_RX_FREE, 1, 0, tmp);
      offset = host_pkt->packet_length / 4 / (OFDMA_end - OFDMA_start);
      for(index = OFDMA_start;index < OFDMA_end;index++)
      {

    	  vxx_shift_factor[i + j][index] = (signed short)(*((unsigned int *)pDataBuffer + (index - OFDMA_start) * offset + 3));
          vxx_shift_factor[i + j][index] = vxx_shift_factor[i + j][index] - vxx_max_shift_factor;
          /*424 = 1024 - 600，FFT结果是从低向高的顺序，DC carrier在601位置*/
          fftc_input_data_ptr1 = (unsigned int *)(pDataBuffer + (index - OFDMA_start) * offset + 424 + 4);
          //fftc_input_data_ptr2 = (unsigned int *)fftc_output_data_address[j] + FFT_SIZE * N_SYM_PER_SLOT + 424;
          fft_data_copy(i + j,//表示的是天线，因为fft的配置包就是按照天线配置的
    		        index,/*开始的符号*/
    		        index + 1,/*截止的符号（此符号不处理）*/
                    fftc_input_data_ptr1,
                    vxx_shift_factor[i + j][index]
                    );
      }
  }
  if(pkt_cnt == NUM_TX_DESC)//说明已经收到所有结果，不用继续等待,跳出for循环
  break;
  
}
cycle_end = TSCL;
cycle = cycle_end - cycle_start;
  return 0;
}

unsigned int fftc_recv_pkt_symbol0(unsigned int queue_num,unsigned char OFDMA_start,unsigned char OFDMA_end)
{
  unsigned int                      pDataBuffer[NUM_TX_DESC];
  int                       i,j;
  unsigned int            setupVal;
  unsigned int            tmp;
  unsigned int *          fftc_input_data_ptr1;
  int length[2];

  MNAV_HostPacketDescriptor_fftc   *host_pkt;

  while(NUM_TX_DESC != get_descriptor_count(queue_num))
  {
    asm(" nop 5");
  }

  for(j=0; j<NUM_TX_DESC; j++)
  {
      tmp = pop_queue(queue_num);
      tmp &= 0xFFFFFFF0;
      host_pkt = (MNAV_HostPacketDescriptor_fftc *)tmp;

      host_pkt->pkt_return_qmgr = 0;
      host_pkt->pkt_return_qnum = NUM_FFTC_QUE_RX_FREE;

      pDataBuffer[j] = host_pkt->buffer_ptr;
      //printf("\n\nDebug: FFTC output Data length is:\n");
      //printf("%d,\n", host_pkt->packet_length/4);

      setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1);
      tmp   |= setupVal;

      CACHE_invL2((void *)host_pkt->buffer_ptr, host_pkt->packet_length, CACHE_WAIT);

      length[j] = host_pkt->packet_length;
      /* Push descriptor to Rx free queue */
      push_queue(NUM_FFTC_QUE_RX_FREE, 1, 0, tmp);

  }

  CSL_XMC_invalidatePrefetchBuffer();
  		    _mfence();
  		    _mfence();
/*agc因子，负数表明要右移，正数为左移*/

  //CACHE_invL2((void *)pDataBuffer[0], length[0], CACHE_WAIT);
  //CACHE_invL2((void *)pDataBuffer[1], length[1], CACHE_WAIT);
 
  vxx_shift_factor[0][OFDMA_start] = (signed short)(*((unsigned int *)pDataBuffer[0] + 3));
  vxx_shift_factor[1][OFDMA_start] = (signed short)(*((unsigned int *)pDataBuffer[1] + 3));
  /*取2根天线中最大的，所有agc因子向其拉齐*/
  vxx_max_shift_factor = vxx_shift_factor[0][OFDMA_start];
  if(vxx_shift_factor[0][OFDMA_start] < vxx_shift_factor[1][OFDMA_start])
  {
      vxx_max_shift_factor = vxx_shift_factor[1][OFDMA_start];
  }
  vxx_max_shift_factor = vxx_max_shift_factor + 1;/*fftc是按照2^14的目标来移位，建议挪到2^13为目标，故需要增大1位scale因子*/
  /*获得移位scale因子*/
  vxx_shift_factor[0][OFDMA_start] = vxx_shift_factor[0][OFDMA_start] - vxx_max_shift_factor; 
  vxx_shift_factor[1][OFDMA_start] = vxx_shift_factor[1][OFDMA_start] - vxx_max_shift_factor; 
  
  for(j=0; j<NUM_TX_DESC; j++)
  {
      fftc_input_data_ptr1 = (unsigned int *)&gRxBuffer[j][424 + 4];

      //CACHE_invL2((void *)pDataBuffer[j], length[j], CACHE_WAIT);

      fft_data_copy(j,//表示的是天线，因为fft的配置包就是按照天线配置的
    		        OFDMA_start,/*开始的符号*/
    		        OFDMA_end,/*截止的符号（此符号不处理）*/
                    fftc_input_data_ptr1,
                    vxx_shift_factor[j][OFDMA_start]
                    );



  }

  return 0;
}

unsigned int fftc_send_pkt(unsigned int pkt_desc, unsigned int buffer_sz, unsigned int header_sz, unsigned int *tx_buffer, unsigned int queue_num)
{
    MNAV_HostPacketDescriptor_fftc   *host_pkt;
   
  host_pkt = (MNAV_HostPacketDescriptor_fftc *)(pkt_desc & 0xFFFFFFF0);

  host_pkt->buffer_ptr    = l2_global_address(glbCoreId, (unsigned int)tx_buffer);
  host_pkt->buffer_len    = buffer_sz*4;
  host_pkt->packet_length   = (buffer_sz - header_sz)*4;
  
  host_pkt->psv_word_count  = header_sz;
  host_pkt->ps_flags      = 1;
  host_pkt->ps_reg_loc    = 1;
  
  host_pkt->orig_buff0_len  = 0;
  host_pkt->orig_buff0_ptr  = NULL;
  
  /* Our host descriptors will be 80 bytes and it is dividable by 16, the multiplier is 5, then minus 1 equals 4, 
   * so the value of Descriptor Size is 00000004. */ 
  pkt_desc |= (SIZE_FFTC_HOST_DESC / 16 - 1);
  
  /* Push descriptor to Tx free queue */
  push_queue(queue_num, 2, (unsigned int)buffer_sz, pkt_desc);
  
  return 0;
}


unsigned int l2_global_address(unsigned int coreId, unsigned int addr)
{
  if ((addr >= 0x00800000) && (addr < 0x00F08000))
  {
    return (addr + (0x10000000 + (coreId * 0x01000000)));
  }

  return (addr);
}

void fftc_system_deinit(void)
{
    push_queue(NUM_FFTC_QUE_GBL_FREE, 1, 0, (unsigned int)0);
    push_queue(NUM_FFTCA_QUE_TX_Q0, 1, 0, 0);
    push_queue(NUM_FFTCB_QUE_TX_Q0, 1, 0, 0);
    push_queue(NUM_FFTC_QUE_RX_FREE, 1, 0, 0);
    push_queue(NUM_FFTC_QUE_TX_FREE, 1, 0, 0);
    push_queue(NUM_FFTC_QUE_RX_ACC, 1, 0, 0);
    push_queue(NUM_FFTC_QUE_RX_POLL, 1, 0, 0);
  
}
