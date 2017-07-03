/***********************************************/
/*fftc_func                                    */
/*功能：fftc配置函数                           */
/***********************************************/
#include "system_init.h"
#include "pl_comm.h"
#include "TSC.h"
int fftc_instart0,fftc_instart1,fftc_incycle0,fftc_incycle1;

#define IFFT_2048_SQRT  0x5A820000  /*sqrt(2048)/32= 1.414 Q(16,2)*/
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
    set_link_ram(NUM_FFTC_LINK_RAM, QM_LRAM_REGION, 0x3fff); //internal link ram
    
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

unsigned int fft_config(unsigned int *dft_size_list,
                        unsigned int dft_length,
                        unsigned int pkt_sz,
                        unsigned int dft_idft_flag,
                        unsigned int *restrict fftc_output_data_address,
                        unsigned int fftc_inst,
                        unsigned int zero_pad_value,
                        unsigned int variable_shift_input,
                        unsigned int cp_len,
                        unsigned char start_symbol,
                        unsigned char end_symbol)
{
#if 1
    unsigned int pkt_desc;
    unsigned int header_sz;
    unsigned int *tx_buffer;
    unsigned int *restrict tx_buffer_ptr;
    unsigned int *restrict fftc_input_data_ptr1;
    unsigned int *restrict fftc_input_data_ptr2;

    unsigned int PSFiledLen,*PSField;
    unsigned int DFTSizeListLen;
    unsigned int bLocalConfig, fftLocalCfg[5];
    signed int dft_idx,i,j;
    unsigned int DFTSizeList[26];

    for(i = 0;i < 26;i++)
    {
       DFTSizeList[i] = 0;
    }


        

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
      tx_buffer = (unsigned int *)gRxBuffer[j];

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
      fftLocalCfg[0]  = ((unsigned int)variable_shift_input&0x0FFF0000)|0x00003fff; //DESTINATION_QUEUE_AND_SHIFTING_REGISTER

      fftLocalCfg[1]  = 0x22000000;
      fftLocalCfg[2]  = cp_len; //CYCLIC_PREFIX_REGISTER
      /*不返回agc因子SUPPRESS_SIDE_INFO=0*/
      fftLocalCfg[3]  = ((unsigned int)zero_pad_value&0x1FFF0000)|((unsigned int)dft_idx&0x0000003F)|((unsigned int)(dft_idft_flag << 6) & 0x00000040); //CONTROL_REGISTER
      fftLocalCfg[4]  = 0x00000000; //LTE_FREQUENCY_SHIFT_REGISTER



      /*Build FFTC pkt header*/
      header_sz = fftc_build_header(tx_buffer,
                    PSFiledLen, PSField,
                    DFTSizeListLen, DFTSizeList,
                    bLocalConfig, fftLocalCfg);

      /* Copy data to send buffer*/
      //memcpy((unsigned int *)(tx_buffer+header_sz), fftc_input_data_ptr, dft_length*pkt_sz*4);
      tx_buffer_ptr = tx_buffer+header_sz;
      int copy_cycles,copy_start = TSCL;
      fft_data_copy(vxx_cell_para.num_antenna_port,
                    start_symbol,/*开始的符号*/
                    end_symbol,/*截止的符号（此符号不处理）*/
                    tx_buffer_ptr,
                    j);
      copy_cycles = TSC_delay_cycle(copy_start);
      //printf("copy_cycles:%d\n",copy_cycles);
      CACHE_wbL2(tx_buffer, dft_length*pkt_sz*4 + header_sz, CACHE_WAIT);


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
      fftc_recv_pkt(NUM_FFTC_QUE_RX_POLL,start_symbol,end_symbol);
      return 1;
#endif
}

int Setup_Rx_FDQ (unsigned int* pDataBuffer)
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
    host_pkt->buffer_ptr    = l2_global_address(glbCoreId, (unsigned int)(pDataBuffer[i]));
    host_pkt->buffer_len    = CPPI_FFTC_BUF_LEN_TX;
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
        //printf ("Error allocating Rx free descriptors \n");
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
        //printf ("Error allocating Tx free descriptors \n");
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
  unsigned int pDataBuffer[NUM_TX_DESC];
  int i,j;
  unsigned int setupVal;
  unsigned int pkt_cnt, tmp,temp1,temp2;
  int factor0,factor1,length;
  long long factor2,factor3;
  
  MNAV_HostPacketDescriptor_fftc   *host_pkt; 
  
  while(NUM_TX_DESC != get_descriptor_count(queue_num))
  {
    asm(" nop 5");
  }
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

      pDataBuffer[j] = host_pkt->buffer_ptr;
      //printf("\n\nDebug: FFTC output Data length is:\n");
      //printf("%d,\n", host_pkt->packet_length/4);

      setupVal = (SIZE_FFTC_HOST_DESC / 16 - 1);
      tmp   |= setupVal;

		CSL_XMC_invalidatePrefetchBuffer();
	  		    _mfence();
	  		    _mfence();

      CACHE_invL2((void *)host_pkt->buffer_ptr, host_pkt->packet_length, CACHE_WAIT);
      /* Push descriptor to Rx free queue */
      push_queue(NUM_FFTC_QUE_RX_FREE, 1, 0, tmp);

  }

/*agc因子，负数表明要右移，正数为左移*/
/*IFFT需要除以2048,再乘以sqrt(2048)（sqrt(2048)=2^5*sqrt(2)，
 根号2采用Q(16,2)定标)）,Q(16,2)*Q(16,1)=Q(32,3)=Q(16,3)<<1=输出定标Q(16,2)则共右移11-5-1=5*/
if((0 == OFDMA_start) ||(7 == OFDMA_start))
{
  vxx_shift_factor[0][OFDMA_start] = (signed short)(*((unsigned int *)pDataBuffer[0] + 3)) - 5;
  vxx_shift_factor[1][OFDMA_start] = (signed short)(*((unsigned int *)pDataBuffer[1] + 3)) - 5;
  temp1 = _sshvl(IFFT_2048_SQRT,vxx_shift_factor[0][OFDMA_start]);
  temp2 = _sshvl(IFFT_2048_SQRT,vxx_shift_factor[1][OFDMA_start]);
  factor0 = _packh2(temp1,temp1);
  factor1 = _packh2(temp2,temp2);
  /*factor0 = -((*((unsigned int *)pDataBuffer[0] + 3)) - 5);
	factor1 = -((*((unsigned int *)pDataBuffer[1] + 3)) - 5);*/

  fftc_instart0 = TSCL;
  fft_output_data_copy(vxx_cell_para.num_antenna_port,
                      (unsigned int* )pDataBuffer[0] + 4,
                      (unsigned int*)pDataBuffer[1] + 4,
                      host_pkt->packet_length/4 - 4,//4是SUPPRESS_SIDE_INFO占用的字节
                      factor0,
                      factor1
                     );
  fftc_incycle0 = TSC_delay_cycle(fftc_instart0);
}
else
{
	length = host_pkt->packet_length/4/(OFDMA_end - OFDMA_start) - 4;//4是SUPPRESS_SIDE_INFO占用的字节
	for(j=OFDMA_start; j<OFDMA_end; j++)
	{
		vxx_shift_factor[0][j] = (signed short)(*((unsigned int *)pDataBuffer[0] + 3 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start))) - 5;
	    vxx_shift_factor[1][j] = (signed short)(*((unsigned int *)pDataBuffer[1] + 3 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start))) - 5;
		temp1 = _sshvl(IFFT_2048_SQRT,vxx_shift_factor[0][j]);
		temp2 = _sshvl(IFFT_2048_SQRT,vxx_shift_factor[1][j]);
		factor0 = _packh2(temp1,temp1);
		factor1 = _packh2(temp2,temp2);
		/*factor0 = -((*((unsigned int *)pDataBuffer[0] + 3 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start))) - 5);
		factor1 = -((*((unsigned int *)pDataBuffer[1] + 3 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start))) - 5);*/

		fftc_instart1 = TSCL;
		fft_output_data_copy(vxx_cell_para.num_antenna_port,
		                      (unsigned int* )pDataBuffer[0] + 4 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start),
		                      (unsigned int* )pDataBuffer[1] + 4 + (4 + 144 + IDFT_SIZE) * (j - OFDMA_start),
		                      length,
		                      factor0,
		                      factor1
		                     );
		fftc_incycle1 = TSC_delay_cycle(fftc_instart1);
	}
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

void BS_DSP0_Core0_OFDM(unsigned char total_symbol,
						unsigned int * fftc_output_data_address)
{
	unsigned int dft_list[6 * NUM_ANTENNA_PORT_4];
	int i=0,ok_flag=0,cp_len;

	for (i = 0; i < 6 * vxx_cell_para.num_antenna_port; i++)/*按照最大的来配置*/
    {
         dft_list[i] = IDFT_SIZE;
    }
	/******************符号0处理********************/
	cp_len = 160;

	fft_config(dft_list,/*输入的DFT/IDFTsize，仅支持DFT_sz_list里面的各种size*/
	           1 ,/*输入的DFT/IDFTsize个数,目前仅支持每次1种DFTSIZE配置*/
	           1204,/*输入数据长度必须是4的倍数*/
	           IDFT_MODE,/*DFT/IDFT*/
	           fftc_output_data_address,/*输出数据地址,使用时注意输出数据要偏移4个int才是真正的结果*/
	           CSL_FFTC_A,
	           ZERO_PAD_VALUE,/*zero-pad value*/
	           VARIABLE_SHIFT_INPUT,/*VARIABLE_SHIFT_INPUT = 1448/2*/
	           cp_len,
	           0,
	           1);


    if(N_SYM_PER_SLOT >= total_symbol)//总符号数<=7时
    {
    	cp_len = 144;
    	ok_flag = fft_config(dft_list,/*输入的DFT/IDFTsize，仅支持DFT_sz_list里面的各种size*/
                         	 (total_symbol - 1),/*输入的DFT/IDFTsize个数,目前仅支持每次1种DFTSIZE配置*/
                         	 1204,/*输入数据长度必须是4的倍数*/
                         	 IDFT_MODE,/*DFT/IDFT*/
							 fftc_output_data_address,/*输出数据地址,使用时注意输出数据要偏移4个int才是真正的结果*/
							 CSL_FFTC_A,
							 ZERO_PAD_VALUE,/*zero-pad value*/
							 VARIABLE_SHIFT_INPUT,/*VARIABLE_SHIFT_INPUT = 1448/2*/
							 cp_len,
							 1,
							 total_symbol);
    }

    else
    {
        cp_len = 144;
        /***************符号1~6***************/
        ok_flag = fft_config(dft_list,/*输入的DFT/IDFTsize，仅支持DFT_sz_list里面的各种size*/
                             6,/*输入的DFT/IDFTsize个数,目前仅支持每次1种DFTSIZE配置*/
                             1204,/*输入数据长度必须是4的倍数*/
                             IDFT_MODE,/*DFT/IDFT*/
                             fftc_output_data_address,/*输出数据地址,使用时注意输出数据要偏移4个int才是真正的结果*/
                             CSL_FFTC_A,
                             ZERO_PAD_VALUE,/*zero-pad value*/
                             VARIABLE_SHIFT_INPUT,/*VARIABLE_SHIFT_INPUT = 1448/2*/
                             cp_len,
                             1,
                             7);

        /***************符号7***************/
        cp_len = 160;
        ok_flag = fft_config(dft_list,/*输入的DFT/IDFTsize，仅支持DFT_sz_list里面的各种size*/
                                           1,/*输入的DFT/IDFTsize个数,目前仅支持每次1种DFTSIZE配置*/
                                           1204,/*输入数据长度必须是4的倍数*/
                                           IDFT_MODE,/*DFT/IDFT*/
                                           fftc_output_data_address,/*输出数据地址,使用时注意输出数据要偏移4个int才是真正的结果*/
                                           CSL_FFTC_A,
                                           ZERO_PAD_VALUE,/*zero-pad value*/
                                           VARIABLE_SHIFT_INPUT,/*VARIABLE_SHIFT_INPUT = 1448/2*/
                                           cp_len,
                                           7,
                                           8);
        /***********符号8~total************/

        cp_len = 144;

        ok_flag = fft_config(dft_list,/*输入的DFT/IDFTsize，仅支持DFT_sz_list里面的各种size*/
                             (total_symbol - 8),/*输入的DFT/IDFTsize个数,目前仅支持每次1种DFTSIZE配置*/
                             1204,/*输入数据长度必须是4的倍数*/
                             IDFT_MODE,/*DFT/IDFT*/
                             fftc_output_data_address,/*输出数据地址,使用时注意输出数据要偏移4个int才是真正的结果*/
                             CSL_FFTC_A,
                             ZERO_PAD_VALUE,/*zero-pad value*/
                             VARIABLE_SHIFT_INPUT,/*VARIABLE_SHIFT_INPUT = 1448/2*/
                             cp_len,
                             8,
                             total_symbol);


    }
    CACHE_wbL2(total_output_data[slot_idx>>1],sizeof(total_output_data[slot_idx>>1]),CACHE_WAIT);
    //memcpy(total_output_data[slot_idx>>1],vxx_layermapping_fft_data.fft_output_data,sizeof(vxx_layermapping_fft_data.fft_output_data));
}

