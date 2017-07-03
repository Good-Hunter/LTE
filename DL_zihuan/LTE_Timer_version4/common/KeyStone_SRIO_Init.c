
#include "csl_srioAux.h"
#include "Keystone_SRIO_init.h"
#include "KeyStone_Multicore_Navigator_drv.h"


SerdesRegs * srioSerdesRegs;

/*enable globally used blocks including MMR block in SRIO*/
void Keystone_SRIO_GlobalEnable(void)
{
  	srioRegs->RIO_GBL_EN = 1;   
	srioRegs->BLOCK_ENABLE_STATUS[0].RIO_BLK_EN= 1; //MMR_EN

	//wait for enable completion
	while(0x3 != (srioRegs->RIO_GBL_EN_STAT&0x3));

}

/*Enable SRIO blocks*/
void Keystone_SRIO_enable_blocks(
	SRIO_Block_Enable * blockEn)
{
	srioRegs->BLOCK_ENABLE_STATUS[5].RIO_BLK_EN= blockEn->bBLK5_8_Port_Datapath_EN[0];
	srioRegs->BLOCK_ENABLE_STATUS[6].RIO_BLK_EN= blockEn->bBLK5_8_Port_Datapath_EN[1];
	srioRegs->BLOCK_ENABLE_STATUS[7].RIO_BLK_EN= blockEn->bBLK5_8_Port_Datapath_EN[2];
	srioRegs->BLOCK_ENABLE_STATUS[8].RIO_BLK_EN= blockEn->bBLK5_8_Port_Datapath_EN[3];

	srioRegs->BLOCK_ENABLE_STATUS[1].RIO_BLK_EN= blockEn->bBLK1_LSU_EN  ;
	srioRegs->BLOCK_ENABLE_STATUS[2].RIO_BLK_EN= blockEn->bBLK2_MAU_EN  ;
	srioRegs->BLOCK_ENABLE_STATUS[3].RIO_BLK_EN= blockEn->bBLK3_TXU_EN  ;
	srioRegs->BLOCK_ENABLE_STATUS[4].RIO_BLK_EN= blockEn->bBLK4_RXU_EN  ;

	while(srioRegs->BLOCK_ENABLE_STATUS[5].RIO_BLK_EN_STAT != blockEn->bBLK5_8_Port_Datapath_EN[0]);
	while(srioRegs->BLOCK_ENABLE_STATUS[6].RIO_BLK_EN_STAT != blockEn->bBLK5_8_Port_Datapath_EN[1]);
	while(srioRegs->BLOCK_ENABLE_STATUS[7].RIO_BLK_EN_STAT != blockEn->bBLK5_8_Port_Datapath_EN[2]);
	while(srioRegs->BLOCK_ENABLE_STATUS[8].RIO_BLK_EN_STAT != blockEn->bBLK5_8_Port_Datapath_EN[3]);

	while(srioRegs->BLOCK_ENABLE_STATUS[1].RIO_BLK_EN_STAT != blockEn->bBLK1_LSU_EN  );
	while(srioRegs->BLOCK_ENABLE_STATUS[2].RIO_BLK_EN_STAT != blockEn->bBLK2_MAU_EN  );
	while(srioRegs->BLOCK_ENABLE_STATUS[3].RIO_BLK_EN_STAT != blockEn->bBLK3_TXU_EN  );
	while(srioRegs->BLOCK_ENABLE_STATUS[4].RIO_BLK_EN_STAT != blockEn->bBLK4_RXU_EN  );

}

/*Disable all SRIO blocks*/
void Keystone_SRIO_disable_all_blocks()
{
	srioRegs->BLOCK_ENABLE_STATUS[1].RIO_BLK_EN= 0; //LSU_EN  
	srioRegs->BLOCK_ENABLE_STATUS[2].RIO_BLK_EN= 0; //MAU_EN  
	srioRegs->BLOCK_ENABLE_STATUS[3].RIO_BLK_EN= 0; //TXU_EN  
	srioRegs->BLOCK_ENABLE_STATUS[4].RIO_BLK_EN= 0; //RXU_EN  

	srioRegs->BLOCK_ENABLE_STATUS[5].RIO_BLK_EN= 0; //PORT0_EN
	srioRegs->BLOCK_ENABLE_STATUS[6].RIO_BLK_EN= 0; //PORT1_EN
	srioRegs->BLOCK_ENABLE_STATUS[7].RIO_BLK_EN= 0; //PORT2_EN
	srioRegs->BLOCK_ENABLE_STATUS[8].RIO_BLK_EN= 0; //PORT3_EN

	srioRegs->BLOCK_ENABLE_STATUS[0].RIO_BLK_EN= 0; //MMR_EN

	srioRegs->RIO_GBL_EN = 0;

	//wait for disable completion
	while(srioRegs->RIO_GBL_EN_STAT&1);	
	
}

/*soft shutdown and reset SRIO*/
void Keystone_SRIO_soft_reset()
{
	int i, j, k;
	
	/*shut down TXU/RXU transaction*/
	for(i=0; i<SRIO_PKTDMA_MAX_CH_NUM; i++)
	{
		KeyStone_pktDma_RxCh_teardown(srioDmaRxChCfgRegs, i);
		KeyStone_pktDma_TxCh_teardown(srioDmaTxChCfgRegs, i);
	}

	for(i= 0; i<SRIO_MAX_LSU_NUM ; i++)
	{
		/*flash LSU transfer for all Source ID*/
		for(j=0; j< SRIO_MAX_DEVICEID_NUM; j++)
		{
			srioRegs->LSU_CMD[i].RIO_LSU_REG6 = 
				CSL_SRIO_RIO_LSU_REG6_FLUSH_MASK| /*flash*/
				(j<<CSL_SRIO_RIO_LSU_REG6_SCRID_MAP_SHIFT); 

			/*This can take more than one cycle to do the flush. 
			wait for a while*/
			for(k=0; k< 100; k++)
				asm(" nop 5");
		}
	}

	/*disable the PEREN bit of the PCR register to stop all
	new logical layer transactions.*/
	srioRegs->RIO_PCR &= (~CSL_SRIO_RIO_PCR_PEREN_MASK);

	/*Wait one second to finish any current DMA transfer.*/
	for(i=0; i< 100000000; i++)
		asm(" nop 5");

	//reset all logic blocks in SRIO
	Keystone_SRIO_disable_all_blocks();

	//disable Serdes
	Keystone_Serdes_disable(srioSerdesRegs, 4);
	
	//disable SRIO through PSC
	Keystone_disable_PSC_module(CSL_PSC_PD_SRIO, CSL_PSC_LPSC_SRIO);
	Keystone_disable_PSC_Power_Domain(CSL_PSC_PD_SRIO);
}

/*configure SRIO 1x 2x or 4x path mode*/
void Keystone_SRIO_set_1x2x4x_Path(
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control)
{
	/*This register is a global register, even though it can be accessed 
	from any port. So you do not need to program from each port, it is 
	basically a single register.
	The PathID is a RO value, that is driven by the H/W. You cannot modify it*/
	srioRegs->RIO_PLM[0].RIO_PLM_SP_PATH_CTL= 
		(srioRegs->RIO_PLM[0].RIO_PLM_SP_PATH_CTL&(~SRIO_1x2x4x_PATH_CONTROL_MASK))|
		srio_1x2x4x_path_control;

}

/*SRIO timeout configuration in microsecond*/
void Keystone_SRIO_Timeout_Config(SRIO_Config * srio_cfg,
	Uint32 logicalRepsonseTimeout_us, 
	Uint32 physicalPortTimeout_us, 
	Uint32 linkInitSilenceTimeout_us)
{
	Uint32 uiTimeout, uiMaxTimeout;

	/*PRESCALAR_SELECT is used to divide VBUSM clock(normally 333 to 400MHz, 
	here use 350MHz), (VBUS_M clock)/(PRESCALAR_SELECT+1), 
	to get about 50MHz clock with about 20ns period.*/
	srioRegs->RIO_PER_SET_CNTL= (srioRegs->RIO_PER_SET_CNTL
		&(~CSL_SRIO_RIO_PER_SET_CNTL_PRESCALER_SELECT_MASK))
		|((350/50-1)<<CSL_SRIO_RIO_PER_SET_CNTL_PRESCALER_SELECT_SHIFT);

	/*logical layer response timeout
	logicalRepsonseTimeout = 15 x (scaled VBUS_M clock period) x TIMEOUT_VALUE),
	TIMEOUT_VALUE = logicalRepsonseTimeout/(scaled VBUS_M clock period)/15 */
	uiTimeout= logicalRepsonseTimeout_us*1000/20/15;
	uiMaxTimeout=CSL_SRIO_RIO_SP_RT_CTL_TVAL_MASK
		>>CSL_SRIO_RIO_SP_RT_CTL_TVAL_SHIFT;
	if(uiTimeout>uiMaxTimeout)
		uiTimeout= uiMaxTimeout;
	if(0==uiTimeout)
		uiTimeout= 1;
	srioRegs->RIO_SP_RT_CTL= uiTimeout<<CSL_SRIO_RIO_SP_RT_CTL_TVAL_SHIFT;

	/*PRESCALAR_SRV_CLK is used to divide SRIO reference clock to get about 
	10MHz SRV_CLK with about 100ns period. */
	srioRegs->RIO_PRESCALAR_SRV_CLK= 
		(Uint32)srio_cfg->serdes_cfg->commonSetup.inputRefClock_MHz/10;

	/*physical layer response timeout.
	physicalPortTimeout = SRV_CLK period * TIMEOUT_VALUE * 3,
	TIMEOUT_VALUE = physicalPortTimeout/SRV_CLK period/3 */
	uiTimeout= physicalPortTimeout_us*1000/100/3;
	uiMaxTimeout=CSL_SRIO_RIO_SP_LT_CTL_TVAL_MASK
		>>CSL_SRIO_RIO_SP_LT_CTL_TVAL_SHIFT;
	if(uiTimeout>uiMaxTimeout)
		uiTimeout= uiMaxTimeout;
	if(0==uiTimeout)
		uiTimeout= 1;
	srioRegs->RIO_SP_LT_CTL= uiTimeout<<CSL_SRIO_RIO_SP_LT_CTL_TVAL_SHIFT;
	
	/*port silence timeout
	The SRIO starts in the SILENT state. The link output driver is disabled 
	to force the link partner to initialize regardless of its current state. 
	The duration of the SILENT state is controlled by the silence_timer. 
	The duration must be long enough to ensure that the link partner detects 
	the silence (as a loss of lane_sync) and is forced to initialize but short 
	enough that it is readily distinguished from a link break.
	linkInitSilenceTimeout is SRV_CLK period X 410 X SILENCE_TIMER,
	SILENCE_TIMER= linkInitSilenceTimeout/SRV_CLK period/410*/
	uiTimeout= linkInitSilenceTimeout_us*1000/100/410;
	uiMaxTimeout=CSL_SRIO_RIO_PLM_SP_SILENCE_TIMER_SILENCE_TIMER_MASK
		>>CSL_SRIO_RIO_PLM_SP_SILENCE_TIMER_SILENCE_TIMER_SHIFT;
	if(uiTimeout>uiMaxTimeout)
		uiTimeout= uiMaxTimeout;
	if(0==uiTimeout)
		uiTimeout= 1;
	uiTimeout= uiTimeout
		<<CSL_SRIO_RIO_PLM_SP_SILENCE_TIMER_SILENCE_TIMER_SHIFT;
	if(srio_cfg->blockEn.bLogic_Port_EN[0])
		srioRegs->RIO_PLM[0].RIO_PLM_SP_SILENCE_TIMER= uiTimeout;
	if(srio_cfg->blockEn.bLogic_Port_EN[1])
		srioRegs->RIO_PLM[1].RIO_PLM_SP_SILENCE_TIMER= uiTimeout;
	if(srio_cfg->blockEn.bLogic_Port_EN[2])
		srioRegs->RIO_PLM[2].RIO_PLM_SP_SILENCE_TIMER= uiTimeout;
	if(srio_cfg->blockEn.bLogic_Port_EN[3])
		srioRegs->RIO_PLM[3].RIO_PLM_SP_SILENCE_TIMER= uiTimeout;
	
	
}

/*configure SRIO standard Command Status Capability registers*/
void Keystone_SRIO_CSR_CAR_Config(SRIO_Config * srio_cfg)
{
	int i;
	Uint32 uiRegisterValue;
	float speed;
	
    srioRegs->RIO_DEV_ID	= (0x009D << CSL_SRIO_RIO_DEV_ID_DEV_ID_SHIFT)
                             |(0x0030 << CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_SHIFT);

	/*The lower 4b should match the 4b from the JTAG Variant field of 
	the DeviceID register.*/
    srioRegs->RIO_DEV_INFO	= boot_cfg_regs->DEVICE_ID_REG0>>28; 
    
    srioRegs->RIO_PE_FEAT	= 
		(0 << CSL_SRIO_RIO_PE_FEAT_BRDG_SHIFT) 	/*PE is not a bridge*/
		|(0 << CSL_SRIO_RIO_PE_FEAT_MEM_SHIFT) 	/*PE is not a memory*/
		|(1 << CSL_SRIO_RIO_PE_FEAT_PROC_SHIFT)	/*PE is a processor*/
		|(0 << CSL_SRIO_RIO_PE_FEAT_SW_SHIFT) 	/*PE is not a switch*/
		|(0 << CSL_SRIO_RIO_PE_FEAT_MULT_P_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_PE_FEAT_FLOW_ARB_SHIFT) //???
		|(0 << CSL_SRIO_RIO_PE_FEAT_MC_SHIFT) 		//???
		|(0 << CSL_SRIO_RIO_PE_FEAT_ERTC_SHIFT) 	//???
		|(1 << CSL_SRIO_RIO_PE_FEAT_SRTC_SHIFT) 	//???
		|(1 << CSL_SRIO_RIO_PE_FEAT_FLOW_CTRL_SHIFT)/*PE supports flow control*/
		|(1 << 6) 	//???
		|(1 << CSL_SRIO_RIO_PE_FEAT_CRF_SHIFT) 		/*PE supports CRF Function*/
		|(1 << CSL_SRIO_RIO_PE_FEAT_CTLS_SHIFT) 	/*supports common transport large systems*/
		|(1 << CSL_SRIO_RIO_PE_FEAT_EXT_FEA_SHIFT)  /*PE has extended features list*/
		|(1 << CSL_SRIO_RIO_PE_FEAT_EXT_AS_SHIFT); 	/*PE supports 34 bit addresses*/

    srioRegs->RIO_SW_PORT    = (0 << CSL_SRIO_RIO_SW_PORT_PORT_TOTAL_SHIFT) //???
                              |(4 << CSL_SRIO_RIO_SW_PORT_PORT_NUM_SHIFT); 	//???
    
    srioRegs->RIO_SRC_OP     = 
		(0 << CSL_SRIO_RIO_SRC_OP_G_READ_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_IREAD_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_READ_OWN_SHIFT)//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_DC_INVALIDATE_SHIFT)	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_CASTOUT_SHIFT) 		//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_DC_FLUSH_SHIFT) 		//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_IO_READ_SHIFT) 		//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_IC_INVALIDATE_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_TLB_INVALIDATE_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_TLB_SYNC_SHIFT) 		//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_RIO_RSVD_10_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_G_RIO_RSVD_11_SHIFT) 	//???
		|(0 << CSL_SRIO_RIO_SRC_OP_DS_TM_SHIFT) 		//???
		|(1 << CSL_SRIO_RIO_SRC_OP_DS_SHIFT) 			//???
		|(0 << CSL_SRIO_RIO_SRC_OP_IMPLEMENT_DEF_SHIFT) //???
		|(1 << CSL_SRIO_RIO_SRC_OP_READ_SHIFT) 		/*support a read operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_WRITE_SHIFT) 	/*support a write operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_STRM_WR_SHIFT) 	/*support a streaming write operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_WR_RES_SHIFT) 	/*support a write with response operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_D_MSG_SHIFT) 	/*support a data message operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_DBELL_SHIFT) 	/*support a doorbell operation*/
		|(0 << CSL_SRIO_RIO_SRC_OP_ACSWAP_SHIFT) 	//???
		|(1 << CSL_SRIO_RIO_SRC_OP_ATSWAP_SHIFT) 	/*support an atomic test-and-swap operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_A_INC_SHIFT) 	/*support an atomic increment operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_A_DEC_SHIFT) 	/*support an atomic decrement operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_A_SET_SHIFT) 	/*support an atomic set operation*/
		|(1 << CSL_SRIO_RIO_SRC_OP_A_CLEAR_SHIFT) 	/*support an atomic clear operation*/
		|(0 << CSL_SRIO_RIO_SRC_OP_A_SWAP_SHIFT) 	//???
		|(1 << CSL_SRIO_RIO_SRC_OP_PORT_WR_SHIFT)	/*support a port-write generation*/
		|(0 << CSL_SRIO_RIO_SRC_OP_IMPLEMENT_DEF2_SHIFT); //???     0x0004FDF4;


    srioRegs->RIO_DEST_OP    = 
		(0 << CSL_SRIO_RIO_DEST_OP_G_READ_SHIFT)       	  //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_IREAD_SHIFT)          //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_READ_OWN_SHIFT)       //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_DC_INVALIDATE_SHIFT)  //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_CASTOUT_SHIFT)        //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_DC_FLUSH_SHIFT)       //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_IO_READ_SHIFT)        //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_IC_INVALIDATE_SHIFT)  //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_TLB_INVALIDATE_SHIFT) //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_TLB_SYNC_SHIFT)       //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_RIO_RSVD_10_SHIFT)    //???
		|(0 << CSL_SRIO_RIO_DEST_OP_G_RIO_RSVD_11_SHIFT)    //???
		|(0 << CSL_SRIO_RIO_DEST_OP_DS_TM_SHIFT)            //???
		|(0 << CSL_SRIO_RIO_DEST_OP_DS_SHIFT)               //???
		|(0 << CSL_SRIO_RIO_DEST_OP_IMPLEMENT_DEF_SHIFT)    //???
		|(1 << CSL_SRIO_RIO_DEST_OP_READ_SHIFT)      /*support a read operation*/                  
		|(1 << CSL_SRIO_RIO_DEST_OP_WRITE_SHIFT)     /*support a write operation*/                 
		|(1 << CSL_SRIO_RIO_DEST_OP_STRM_WR_SHIFT)   /*support a streaming write operation*/       
		|(1 << CSL_SRIO_RIO_DEST_OP_WR_RES_SHIFT)    /*support a write with response operation*/   
		|(1 << CSL_SRIO_RIO_DEST_OP_D_MSG_SHIFT)     /*support a data message operation*/          
		|(1 << CSL_SRIO_RIO_DEST_OP_DBELL_SHIFT)     /*support a doorbell operation*/              
		|(0 << CSL_SRIO_RIO_DEST_OP_ACSWAP_SHIFT)    //???                                         
		|(0 << CSL_SRIO_RIO_DEST_OP_ATSWAP_SHIFT)    /*support an atomic test-and-swap operation*/ 
		|(0 << CSL_SRIO_RIO_DEST_OP_A_INC_SHIFT)     /*support an atomic increment operation*/     
		|(0 << CSL_SRIO_RIO_DEST_OP_A_DEC_SHIFT)     /*support an atomic decrement operation*/     
		|(0 << CSL_SRIO_RIO_DEST_OP_A_SET_SHIFT)     /*support an atomic set operation*/           
		|(0 << CSL_SRIO_RIO_DEST_OP_A_CLEAR_SHIFT)   /*support an atomic clear operation*/         
		|(0 << CSL_SRIO_RIO_DEST_OP_A_SWAP_SHIFT) 	//???
		|(1 << CSL_SRIO_RIO_DEST_OP_PORT_WR_SHIFT) 	/*support a port-write operation*/
		|(0 << CSL_SRIO_RIO_DEST_OP_IMPLEMENT_DEF2_SHIFT); //??? 0x0000FC04;      

	/*PE supports 34 bit addresses*/
	srioRegs->RIO_PE_LL_CTL= (1<<CSL_SRIO_RIO_PE_LL_CTL_EXT_ADDR_CTL_SHIFT);

	/*The host base device ID lock CSR contains the base device ID value for 
	the processing element in the system that is responsible for initializing
	this processing element.*/
  //  srioRegs->RIO_HOST_BASE_ID_LOCK	= 0x00A0;
    	//srio_cfg->device_ID_routing_config[0].idPattern;


	/*Software defined component tag for the PE. 
	Useful for devices without device IDs.*/
	srioRegs->RIO_COMP_TAG	= 0 ;  // not touched
    
    /* port general control */
    srioRegs->RIO_SP_GEN_CTL 		=  // agent, master, undiscovered
		(0 << CSL_SRIO_RIO_SP_GEN_CTL_HOST_SHIFT)
		|(1 << CSL_SRIO_RIO_SP_GEN_CTL_MAST_EN_SHIFT)
		|(0 << CSL_SRIO_RIO_SP_GEN_CTL_DISC_SHIFT);

	/*port control*/
	for(i= 0; i< SRIO_MAX_PORT_NUM; i++)
	{
		if(FALSE==srio_cfg->blockEn.bLogic_Port_EN[i])
			continue;

		uiRegisterValue= 
			(0<<CSL_SRIO_RIO_SP_CTL_PORT_DIS_SHIFT    )
			|(1<<CSL_SRIO_RIO_SP_CTL_OTP_EN_SHIFT      )
			|(1<<CSL_SRIO_RIO_SP_CTL_INP_EN_SHIFT      )
			|(0<<CSL_SRIO_RIO_SP_CTL_STOP_FAIL_EN_SHIFT)
			|(1<<CSL_SRIO_RIO_SP_CTL_PTYP_SHIFT        );
		if(NULL!=srio_cfg->flowControlID)
			uiRegisterValue|= 
				(1<<CSL_SRIO_RIO_SP_CTL_FLOW_CTRL_SHIFT   )
				|(1<<CSL_SRIO_RIO_SP_CTL_FLOW_ARB_SHIFT    );
		srioRegs->RIO_SP[i].RIO_SP_CTL = uiRegisterValue;

		speed= srio_cfg->serdes_cfg->linkSetup[i]->linkSpeed_GHz;
		if(1.25==speed)
			srioRegs->RIO_SP[i].RIO_SP_CTL2 = 
				1<<CSL_SRIO_RIO_SP_CTL2_GB_1P25_EN_SHIFT;
		if(2.5==speed)
			srioRegs->RIO_SP[i].RIO_SP_CTL2 = 
				1<<CSL_SRIO_RIO_SP_CTL2_GB_2P5_EN_SHIFT;
		if(3.125==speed)
			srioRegs->RIO_SP[i].RIO_SP_CTL2 = 
				1<<CSL_SRIO_RIO_SP_CTL2_GB_3P125_EN_SHIFT;
		if(5==speed)
			srioRegs->RIO_SP[i].RIO_SP_CTL2 = 
				1<<CSL_SRIO_RIO_SP_CTL2_GB_5P0_EN_SHIFT;
		if(6.25==speed)
			srioRegs->RIO_SP[i].RIO_SP_CTL2 = 
				1<<CSL_SRIO_RIO_SP_CTL2_GB_6P25_EN_SHIFT;

		/*disable port write*/
		srioRegs->RIO_PLM[i].RIO_PLM_SP_ALL_PW_EN = 0;
	}
	
	/*clear port write dest Id, port write not used*/
    srioRegs->RIO_PW_TGT_ID= 0;
	/*disable port write*/
    srioRegs->RIO_EM_DEV_PW_EN = 0;
    
    /* Register Reset Control 
    Allows the SELF_RST and PWDN_PORT resets to clear sticky register bits 
    in addition to the normal configuration registers.*/
    srioRegs->RIO_REG_RST_CTL = 
    	1 << CSL_SRIO_RIO_REG_RST_CTL_CLEAR_STICKY_SHIFT;


}


//enable error capture for SRIO
void Keystone_SRIO_Error_Capture_Enable(SRIO_Block_Enable * blockEn)
{
	Int32 i;

	/*Enable specific error reporting, save and lock capture information 
	in the appropriate Logical/Transport Layer Capture CSRs*/
    srioRegs->RIO_ERR_EN= 
		(1 << CSL_SRIO_RIO_ERR_EN_IO_ERR_RESP_EN_SHIFT) 	/*"ERROR" response for an direct IO*/
		|(1 << CSL_SRIO_RIO_ERR_EN_MSG_ERR_RESP_EN_SHIFT) 	/*"ERROR" response for an message*/
		|(0 << CSL_SRIO_RIO_ERR_EN_GSM_ERR_RESP_EN_SHIFT) 	
		|(1 << CSL_SRIO_RIO_ERR_EN_MSG_FMT_ERR_EN_SHIFT) 	/*received message with invalid fromat*/
		|(1 << CSL_SRIO_RIO_ERR_EN_ILL_TRANS_DECODE_EN_SHIFT) 	/*Illegal transaction decode*/
		|(1 << CSL_SRIO_RIO_ERR_EN_ILL_TRANS_TGT_ERR_EN_SHIFT) 	/*Illegal transaction target error*/
		|(1 << CSL_SRIO_RIO_ERR_EN_MSG_REQ_TIMEOUT_EN_SHIFT) 	/*Message Request Time-out*/
		|(1 << CSL_SRIO_RIO_ERR_EN_PKT_RESP_TIMEOUT_EN_SHIFT) 	/*Packet Response Time-out*/
		|(1 << CSL_SRIO_RIO_ERR_EN_UNSOLICITED_RESP_EN_SHIFT) 	/*unsolicited/unexpected Response*/
		|(1 << CSL_SRIO_RIO_ERR_EN_UNSUPPORTED_TRANS_EN_SHIFT) 	/*Unsupported Transaction*/
		|(1 << CSL_SRIO_RIO_ERR_EN_PDU_LEN_ERR_EN_SHIFT) 		/*Data streaming PDU length error*/
		|(1 << CSL_SRIO_RIO_ERR_EN_SHORT_STREAM_SEG_EN_SHIFT) 	/*Short data streaming segment*/
		|(1 << CSL_SRIO_RIO_ERR_EN_LONG_STREAM_SEG_EN_SHIFT) 	/*Long data streaming segment*/
		|(1 << CSL_SRIO_RIO_ERR_EN_OPEN_STREAM_CONTEXT_EN_SHIFT) 	/*Open existing data streaming context*/
		|(1 << CSL_SRIO_RIO_ERR_EN_MISSING_STREAM_CONTEXT_EN_SHIFT) /*Missing data streaming context*/
		|(1 << CSL_SRIO_RIO_ERR_EN_CPPI_SECURITY_VIOLATION_EN_SHIFT)/*RX CPPI Security Violation*/
		|(1 << CSL_SRIO_RIO_ERR_EN_RX_DMA_ERR_EN_SHIFT); 			/*RX I/O DMA Access Error*/

	/*Error detect register, write 0 to clear it.*/
    srioRegs->RIO_ERR_DET= 0 ;

	/*clear error capture registers*/
    srioRegs->RIO_H_ADDR_CAPT= 0;
    srioRegs->RIO_ADDR_CAPT= 0 ;
    srioRegs->RIO_ID_CAPT= 0 ;
    srioRegs->RIO_CTRL_CAPT= 0;

	//enable physical layer error capture
	for(i=0; i< SRIO_MAX_PORT_NUM; i++)
	{
		if(FALSE == blockEn->bLogic_Port_EN[i])
			continue;

		/*Enable error rate counting*/
		srioRegs->RIO_SP_ERR[i].RIO_SP_RATE_EN= 
			(1<<CSL_SRIO_RIO_SP_RATE_EN_IMP_SPEC_EN_SHIFT     )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_CS_CRC_EN_SHIFT       )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_CS_ILL_ID_EN_SHIFT    )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_CS_NOT_ACC_EN_SHIFT   )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_PKT_ILL_ACKID_EN_SHIFT)
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_PKT_CRC_ERR_EN_SHIFT  )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_PKT_ILL_SIZE_EN_SHIFT )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_DSCRAM_LOS_EN_SHIFT   )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_LR_ACKID_ILL_EN_SHIFT )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_PROT_ERR_EN_SHIFT     )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_DELIN_ERR_EN_SHIFT    )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_CS_ACK_ILL_EN_SHIFT   )
			|(1<<CSL_SRIO_RIO_SP_RATE_EN_LINK_TO_EN_SHIFT      );

		/*the threshold value for reporting an error condition*/
		srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_THRESH= 
			(0xF<<CSL_SRIO_RIO_SP_ERR_THRESH_ERR_RFT_SHIFT)
			|(0x4<<CSL_SRIO_RIO_SP_ERR_THRESH_ERR_RDT_SHIFT);

		/*Error detect register, write 0 to clear it.*/
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_DET= 0;

		/*clear error capture registers*/
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_ATTR_CAPT= 0;
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_0= 0;
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_1= 0;
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_2= 0;
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_3= 0;

		/*clear error rate counter*/
	    srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_RATE= 0;

		/*read this register to clear the error count in it*/
		i = srioRegs->RIO_LANE[i].RIO_LANE_STAT0;
#if 0
		srioRegs->RIO_PORT_OPTION[i].SP_CTL_INDEP= srioRegs->PORT_OPTION[i].SP_CTL_INDEP|
			(CSL_SRIO_SP_CTL_INDEP_ILL_TRANS_EN_ENABLE<<CSL_SRIO_SP_CTL_INDEP_ILL_TRANS_EN_SHIFT)|
			(CSL_SRIO_SP_CTL_INDEP_MAX_RETRY_EN_ENABLE<<CSL_SRIO_SP_CTL_INDEP_MAX_RETRY_EN_SHIFT)|
			(255<<CSL_SRIO_SP_CTL_INDEP_MAX_RETRY_THR_SHIFT)|
			(CSL_SRIO_SP_CTL_INDEP_IRQ_EN_ENABLE<<CSL_SRIO_SP_CTL_INDEP_IRQ_EN_SHIFT);
#endif
	}
}

/*
port write feature should be disabled with code like below:
    srioRegs->SP_IP_MODE |=  
                        CSL_FMKT(SRIO_SP_IP_MODE_PW_DIS, DISABLE) |
*/

/*configure SRIO device ID*/
void Keystone_SRIO_set_device_ID(
	SRIO_Device_ID_Routing_Config * device_id_routing_config,
	Uint32 uiDeviceIdNum)
{
	int i;

	/*The TLM_SP(n)_BRR_x_PATTERN_MATCH registers hold the 15 allowable DestIDs, 
	note that the first register is not used.  We use the RIO_BASE_ID register 
	to hold the first ID */
	srioRegs->RIO_BASE_ID= device_id_routing_config[0].idPattern| 	/*Large ID*/
		((device_id_routing_config[0].idPattern&0xFF)<<16); 		/*small ID*/

	/*The host base device ID lock CSR contains the base device ID value for 
	the processing element in the system that is responsible for initializing
	this processing element.*/
    srioRegs->RIO_HOST_BASE_ID_LOCK	= device_id_routing_config[0].idPattern;
    	//srio_cfg->device_ID_routing_config[0].idPattern;	
	uiDeviceIdNum= _min2(SRIO_MAX_DEVICEID_NUM, uiDeviceIdNum);
	for(i= 1; i<uiDeviceIdNum; i++)
	{
		/*please note, SRIO block 5~8 must be eanbled for corresponding
		RIO_TLM[0:3] taking effect*/
	    srioRegs->RIO_TLM[i/4].brr[i&3].RIO_TLM_SP_BRR_CTL = 
			(device_id_routing_config[i].routeMaintenance<<
				CSL_SRIO_RIO_TLM_SP_BRR_1_CTL_ROUTE_MR_TO_LLM_SHIFT)|
			(0<<CSL_SRIO_RIO_TLM_SP_BRR_1_CTL_PRIVATE_SHIFT)|
			(1<<CSL_SRIO_RIO_TLM_SP_BRR_1_CTL_ENABLE_SHIFT);

	    srioRegs->RIO_TLM[i/4].brr[i&3].RIO_TLM_SP_BRR_PATTERN_MATCH = 
			(device_id_routing_config[i].idPattern<<
				CSL_SRIO_RIO_TLM_SP_BRR_1_PATTERN_MATCH_PATTERN_SHIFT)|
			(device_id_routing_config[i].idMatchMask<<
				CSL_SRIO_RIO_TLM_SP_BRR_1_PATTERN_MATCH_MATCH_SHIFT);

	}	
}

/*configure the map between message and PacketDMA flow and queue*/
void Keystone_map_SRIO_RX_message(SRIO_RX_Message_Map * srio_message_map,
	Uint32 uiNumMessageMap)
{
	int i;
	
	uiNumMessageMap= _min2(SRIO_MAX_MSG_MAP_ENTRY_NUM, uiNumMessageMap);
	for(i=0; i< uiNumMessageMap; i++)
	{
		srioRegs->RXU_MAP[i].RIO_RXU_MAP_L= 
			(srio_message_map[i].ltrMask<<CSL_SRIO_RIO_RXU_MAP_L_LTR_MASK_SHIFT)|
			(srio_message_map[i].mbxMask<<CSL_SRIO_RIO_RXU_MAP_L_MBX_MASK_SHIFT)|
			(srio_message_map[i].ltr<<CSL_SRIO_RIO_RXU_MAP_L_LTR_SHIFT)|
			(srio_message_map[i].mbx<<CSL_SRIO_RIO_RXU_MAP_L_MBX_SHIFT)|
			(srio_message_map[i].srcId<<CSL_SRIO_RIO_RXU_MAP_L_SRCID_SHIFT);

		srioRegs->RXU_MAP[i].RIO_RXU_MAP_H= 
			(srio_message_map[i].dstId<<CSL_SRIO_RIO_RXU_MAP_H_DEST_ID_SHIFT)|
			(srio_message_map[i].dstProm<<CSL_SRIO_RIO_RXU_MAP_H_DEST_PROM_SHIFT)|
			(srio_message_map[i].tt<<CSL_SRIO_RIO_RXU_MAP_H_TT_MASK)|
			(srio_message_map[i].srcProm<<CSL_SRIO_RIO_RXU_MAP_H_SRC_PROM_SHIFT)|
			(srio_message_map[i].segMap<<CSL_SRIO_RIO_RXU_MAP_H_SEG_MAP_SHIFT);

		srioRegs->RXU_TYPE9_MAP[i].RIO_RXU_TYPE9_MAP0= 
			(srio_message_map[i].cosMask<<CSL_SRIO_RIO_RXU_TYPE9_MAP0_COS_MASK_SHIFT)|
			(srio_message_map[i].cos<<CSL_SRIO_RIO_RXU_TYPE9_MAP0_COS_SHIFT)|
			(srio_message_map[i].srcId<<CSL_SRIO_RIO_RXU_TYPE9_MAP0_SRCID_SHIFT);

		srioRegs->RXU_TYPE9_MAP[i].RIO_RXU_TYPE9_MAP1= 
			(srio_message_map[i].dstId<<CSL_SRIO_RIO_RXU_TYPE9_MAP1_DEST_ID_SHIFT)|
			(srio_message_map[i].dstProm<<CSL_SRIO_RIO_RXU_TYPE9_MAP1_DEST_PROM_SHIFT)|
			(srio_message_map[i].tt<<CSL_SRIO_RIO_RXU_TYPE9_MAP1_TT_SHIFT)|
			(srio_message_map[i].srcProm<<CSL_SRIO_RIO_RXU_TYPE9_MAP1_SRC_PROM_SHIFT);

		srioRegs->RXU_TYPE9_MAP[i].RIO_RXU_TYPE9_MAP2= 
			(srio_message_map[i].streamMask<<CSL_SRIO_RIO_RXU_TYPE9_MAP2_STRM_MASK_SHIFT)|
			(srio_message_map[i].streamId<<CSL_SRIO_RIO_RXU_TYPE9_MAP2_STRM_ID_SHIFT);
			
		srioRegs->RXU_MAP[i].RIO_RXU_MAP_QID =
			(srio_message_map[i].flowId<<CSL_SRIO_RIO_RXU_MAP_QID_FLOWID_SHIFT)|
			(srio_message_map[i].destQuID<<CSL_SRIO_RIO_RXU_MAP_QID_DEST_QID_SHIFT);

	}
}

void Keystone_SRIO_Datastreaming_init(SRIO_Datastreaming_Cfg * datastreaming_cfg)
{

	if(NULL==datastreaming_cfg)
		return;
		
    srioRegs->RIO_DS_INFO = 
		(datastreaming_cfg->MaxPDU << CSL_SRIO_RIO_DS_INFO_MAX_PDU_SHIFT)
		|(datastreaming_cfg->SegSupport << CSL_SRIO_RIO_DS_INFO_SEG_SUPPORT_SHIFT);  

    srioRegs->RIO_DS_LL_CTL = 
    	datastreaming_cfg->MTU << CSL_SRIO_RIO_DS_LL_CTL_MTU_SHIFT;

    srioRegs->RIO_PER_SET_CNTL1 = (srioRegs->RIO_PER_SET_CNTL1
    	&(~(CSL_SRIO_RIO_PER_SET_CNTL1_COS_EN_MASK 	/*clear COS_EN bit*/
    	|CSL_SRIO_RIO_PER_SET_CNTL1_CRF_MASK))) 	/*Cleare CRF bit*/
    	|(datastreaming_cfg->COS_EN<<CSL_SRIO_RIO_PER_SET_CNTL1_COS_EN_SHIFT);
}

void Keystone_SRIO_TX_Queue_Cfg(
	SRIO_TX_Queue_Sch_Info * TX_Queue_Sch_Info,
	Uint32 uiNumTxQueue)
{
	int i;
	Uint32 uiMask, uiShift, uiRegIndex;

	/*For SRIO, priority 3 is highest, 0 is lowest
	For PktDMA channel, priority 0 is highest, 3 is lowest*/
	Uint32 mapSrioPriToTxChPri[4]={3,2,1,0};
	
	uiNumTxQueue= _min2(SRIO_PKTDMA_MAX_CH_NUM, uiNumTxQueue);
	for(i=0; i< uiNumTxQueue; i++)
	{
		uiRegIndex= i/4;
		uiShift= (i&3)*8;
		uiMask= 0xFF<<uiShift;
		srioRegs->RIO_TX_QUEUE_SCH_INFO[uiRegIndex] =
			(srioRegs->RIO_TX_QUEUE_SCH_INFO[uiRegIndex]&(~uiMask)) 	/*clear the field*/
			|((TX_Queue_Sch_Info[i].CRF
			|(TX_Queue_Sch_Info[i].outputPort<<4))<<uiShift);

		/*PRIO field in TX_QUEUE_SCH_INFOx is read only,
		the priority information comes from the PKTDMA TX channel 
		actually takes effect*/
		srioDmaTxChPriority[i] = 
			mapSrioPriToTxChPri[TX_Queue_Sch_Info[i].priority];
	}
}

/*Setup garbage queue for error packet*/
void Keystone_SRIO_Garbage_Queue_Cfg(
	SRIO_Message_Cfg * msg_cfg)
{
	if(msg_cfg)	
	{
		srioRegs->RIO_GARBAGE_COLL_QID0=
			(msg_cfg->rx_size_error_garbage_Q
				<<CSL_SRIO_RIO_GARBAGE_COLL_QID0_GARBAGE_QID_LEN_SHIFT)
			|(msg_cfg->rx_timeout_garbage_Q
				<<CSL_SRIO_RIO_GARBAGE_COLL_QID0_GARBAGE_QID_TOUT_SHIFT);
			
		srioRegs->RIO_GARBAGE_COLL_QID1=
			(msg_cfg->tx_excessive_retries_garbage_Q
				<<CSL_SRIO_RIO_GARBAGE_COLL_QID1_GARBAGE_QID_RETRY_SHIFT)
			|(msg_cfg->tx_error_garbage_Q
				<<CSL_SRIO_RIO_GARBAGE_COLL_QID1_GARBAGE_QID_TRANS_ERR_SHIFT);

		srioRegs->RIO_GARBAGE_COLL_QID2=
			(msg_cfg->tx_size_error_garbage_Q
				<<CSL_SRIO_RIO_GARBAGE_COLL_QID2_GARBAGE_QID_SSIZE_SHIFT);
	}
}

/* configure SRIO packet forwarding */
void Keystone_SRIO_packet_forwarding_Cfg(
	SRIO_PktForwarding_Cfg * PktForwardingEntry_cfg,
	Uint32 pktForwardingEntryNum)
{
    int i = 0;

	pktForwardingEntryNum= _min2(SRIO_MAX_FORWARDING_ENTRY_NUM, 
		pktForwardingEntryNum);
    for(i=0; i<pktForwardingEntryNum; i++)
    {
		srioRegs->PF_CNTL[i].RIO_PF_16B_CNTL =
			(PktForwardingEntry_cfg[i].forwardingID_up_16 
				<< CSL_SRIO_RIO_PF_16B_CNTL_DEVID_16B_UP_SHIFT)
			|(PktForwardingEntry_cfg[i].forwardingID_lo_16 
				<< CSL_SRIO_RIO_PF_16B_CNTL_DEVID_16B_LO_SHIFT);

		srioRegs->PF_CNTL[i].RIO_PF_8B_CNTL = 
			(PktForwardingEntry_cfg[i].forwardingID_up_8 
				<< CSL_SRIO_RIO_PF_8B_CNTL_DEVID_8B_UP_SHIFT)
			|(PktForwardingEntry_cfg[i].forwardingID_lo_8 
				<< CSL_SRIO_RIO_PF_8B_CNTL_DEVID_8B_LO_SHIFT)
			|(PktForwardingEntry_cfg[i].outport 
				<< CSL_SRIO_RIO_PF_8B_CNTL_OUT_PORT_SHIFT);
    }  
}


/* configure SRIO packet forwarding */
void Keystone_SRIO_MulticastID_Cfg(
    SRIO_Multicast_ID_Cfg * multicastID,
    Uint32 uiNumMulticastID)
{
    int i = 0;

	uiNumMulticastID= _min2(SRIO_MAX_MULTICAST_ID_NUM, uiNumMulticastID);
	for(i=0; i<uiNumMulticastID; i++)
	{
		srioRegs->PF_CNTL[i].RIO_PF_16B_CNTL =
			(multicastID[i].multicast_8b_ID
				<< CSL_SRIO_RIO_MULTIID_REG_16B_NODEID_SHIFT)
			|(multicastID[i].multicast_8b_ID
				<< CSL_SRIO_RIO_MULTIID_REG_8B_NODEID_SHIFT);
	}  
}

/* Rx Mode configuration */
void Keystone_SRIO_RxMode_Setup(SRIO_RX_Mode * rxMode)
{
	int i;
	
	if(rxMode)
	{
		srioRegs->RIO_PER_SET_CNTL = (srioRegs->RIO_PER_SET_CNTL&
			(~CSL_SRIO_RIO_PER_SET_CNTL_LOG_TGT_ID_DIS_MASK))|
			(rxMode->accept_data_with_any_ID
				<<CSL_SRIO_RIO_PER_SET_CNTL_LOG_TGT_ID_DIS_SHIFT);

		/*set RX mode for all ports*/
		for(i=0; i<SRIO_MAX_PORT_NUM; i++)
		{
			srioRegs->RIO_TLM[i].RIO_TLM_SP_CONTROL = 
				(srioRegs->RIO_TLM[i].RIO_TLM_SP_CONTROL&
				(~(CSL_SRIO_RIO_TLM_SP_CONTROL_TGT_ID_DIS_MASK
				|CSL_SRIO_RIO_TLM_SP_CONTROL_MTC_TGT_ID_DIS_MASK)))
				|(rxMode->port_rx_mode[i].accept_maintenance_with_any_ID
				<<CSL_SRIO_RIO_TLM_SP_CONTROL_MTC_TGT_ID_DIS_SHIFT)
				|(rxMode->port_rx_mode[i].support_multicast_forwarding
				<<CSL_SRIO_RIO_TLM_SP_CONTROL_TGT_ID_DIS_SHIFT);
		}
	}
}


void Keystone_SRIO_little_endian_swap(
	SRIO_Little_Endian_Swap_Mode leSwapMode)
{
#ifdef _LITTLE_ENDIAN
	//??? not sure how endian match with another processor in big-endian
	CSL_SRIO_SetLSUSwappingMode(srioRegs, leSwapMode);
	CSL_SRIO_SetTXURXUSwappingMode(srioRegs, leSwapMode);
#endif
}

void Keystone_SRIO_Interrupt_init(
	SRIO_Interrupt_Cfg * interrupt_cfg)
{
    Uint32 i;
    Uint32 reg, shift;
	volatile Uint32 * ICRR= (volatile Uint32 *)srioRegs->DOORBELL_ICRR;

	if(NULL == interrupt_cfg)
		return;
		
    /* Clear all the interrupts */
    for(i=0; i<2; i++)
    {
        srioRegs->LSU_ICSR_ICCR[i].RIO_LSU_ICCR	= 0xFFFFFFFF ;  
    }
    for(i=0; i<4; i++)
    {
        srioRegs->DOORBELL_ICSR_ICCR[i].RIO_DOORBELL_ICCR = 0xFFFFFFFF;        
    }
	srioRegs->RIO_ERR_RST_EVNT_ICCR = 0xFFFFFFFF;

    if(NULL != interrupt_cfg->interrupt_map)
    {
	    for(i=0; i<interrupt_cfg->uiNumInterruptMap; i++)
	    {
	        /* Get register index for the interrupt source*/
	        reg = interrupt_cfg->interrupt_map[i].interrupt_event >> 16;

	        /* Get shift value for the interrupt source*/
	        shift = interrupt_cfg->interrupt_map[i].interrupt_event & 0x0000FFFF;

	    	ICRR[reg]= (ICRR[reg]&(~(0xF<<shift))) 	/*clear the field*/
	    		|(interrupt_cfg->interrupt_map[i].INTDST_number<<shift);

	    }
	}

	srioRegs->RIO_INTERRUPT_CTL = interrupt_cfg->doorbell_route_ctl;

	/*disable interrupt rate control*/
	srioRegs->RIO_INTDST_RATE_DIS= 0xFFFF;
	for(i= 0; i< 16; i++)
	{
		srioRegs->RIO_INTDST_RATE_CNT[i]= 0;
	}
	
	if(NULL != interrupt_cfg->interrupt_rate)
	{
		/*setup interrupt rate for specific INTDST*/
		for(i= 0; i<interrupt_cfg->uiNumInterruptRateCfg; i++)
		{
			/*enable rate control for this INTDST*/
			srioRegs->RIO_INTDST_RATE_DIS &= 
				~(1<<interrupt_cfg->interrupt_rate[i].INTDST_number);

			/*set interrupt rate counter for this INTDST*/
			srioRegs->RIO_INTDST_RATE_CNT[i]= 
				interrupt_cfg->interrupt_rate[i].interrupt_rate_counter;
		}
	}
	
    return;
}

void Keystone_SRIO_Prioirity_Permission_Setup(
	SRIO_priority_permission * priority_permission)
{
	if(NULL != priority_permission)
	{
		srioRegs->RIO_PER_SET_CNTL = (srioRegs->RIO_PER_SET_CNTL
			&(~(CSL_SRIO_RIO_PER_SET_CNTL_CBA_TRANS_PRI_MASK))) 	/*clear the field*/
			|(priority_permission->uiVbusPriority
				<<CSL_SRIO_RIO_PER_SET_CNTL_CBA_TRANS_PRI_SHIFT);
		srioRegs->RIO_SUPERVISOR_ID = 
			(priority_permission->supervisorHostID_8b<<
				CSL_SRIO_RIO_SUPERVISOR_ID_8B_SUPRVSR_ID_SHIFT)
			|(priority_permission->supervisorHostID_16b<<
				CSL_SRIO_RIO_SUPERVISOR_ID_16B_SUPRVSR_ID_SHIFT);
	}
}

void Keystone_SRIO_Flow_Control(
    SRIO_Flow_Control_ID * flowControlID,
    Uint32 uiNumFlowControlID)
{
	int i;
	Uint32 uiFlowMask = 0;

	if(NULL != flowControlID)
	{
		uiNumFlowControlID= _min2(14, uiNumFlowControlID);
		for(i= 0; i< uiNumFlowControlID; i++)
			srioRegs->RIO_FLOW_CNTL[i]= 
				(flowControlID[i].tt<<CSL_SRIO_RIO_FLOW_CNTL_TT_SHIFT)
				|flowControlID[i].flow_control_ID;

		uiFlowMask= _set(uiFlowMask, 0, uiNumFlowControlID-1);
		uiFlowMask= _pack2(uiFlowMask, uiFlowMask);

		/*enable flow control in all TX modules*/
		for(i= 0; i< SRIO_MAX_LSU_NUM/2; i++)
			srioRegs->RIO_LSU_FLOW_MASKS[i]= uiFlowMask;

		for(i= 0; i< SRIO_PKTDMA_MAX_CH_NUM/2; i++)
			srioRegs->RIO_TX_CPPI_FLOW_MASKS[i]= uiFlowMask;
	}		
}

void Keystone_SRIO_Init(SRIO_Config * srio_cfg)
{
    Uint32 cfgValue = 0;
    Uint32 i = 0;
    
	srioSerdesRegs = (SerdesRegs *)&boot_cfg_regs->SRIO_SERDES_CFGPLL;

	if(srioRegs->RIO_PCR&CSL_SRIO_RIO_PCR_PEREN_MASK)
		Keystone_SRIO_soft_reset();	//soft reset SRIO if it is already enabled

	//enable SRIO power and clock domain is if it not enabled
	Keystone_enable_PSC_module(CSL_PSC_PD_SRIO, CSL_PSC_LPSC_SRIO);

    /*Clear BOOT_COMPLETE bit*/
    srioRegs->RIO_PER_SET_CNTL &= (~(1 << CSL_SRIO_RIO_PER_SET_CNTL_BOOT_COMPLETE_SHIFT));

	if(srio_cfg->msg_cfg)
	{
		/*These registers can only be written to while the peripheral is disabled.*/
		Keystone_SRIO_TX_Queue_Cfg(srio_cfg->msg_cfg->TX_Queue_Sch_Info,
			srio_cfg->msg_cfg->uiNumTxQueue);
	}

   // bringing SRIO from reset isolation
    srioRegs->RIO_PCR = 0x11; 
	/*enable globally used blocks including MMR block in SRIO*/
    Keystone_SRIO_GlobalEnable();

    /*The LSU setup registers are only programmable
    while the LSU is disabled while the peripheral is enabled.*/
	if(srio_cfg->lsu_cfg)
	{
	    /*setup the shadow registers allocation between LSU*/
	    srioRegs->RIO_LSU_SETUP_REG0 = 
	    	(srio_cfg->lsu_cfg->lsuGrp0ShadowRegsSetup << CSL_SRIO_RIO_LSU_SETUP_REG0_SHADOW_GRP0_SHIFT)|
	        (srio_cfg->lsu_cfg->lsuGrp1ShadowRegsSetup << CSL_SRIO_RIO_LSU_SETUP_REG0_SHADOW_GRP1_SHIFT);

	    /*setup LSU interrupt based on LSU number or Source ID*/
	    cfgValue = 0;
	    for(i=0; i<SRIO_MAX_LSU_NUM; i++)
	    {
	        cfgValue |= srio_cfg->lsu_cfg->lsuIntSetup[i] << i;         
	    }
	    srioRegs->RIO_LSU_SETUP_REG1 = cfgValue;
	}

	/*enable other optional blocks*/
	Keystone_SRIO_enable_blocks(&srio_cfg->blockEn);

	if(SRIO_SERDES_LOOPBACK==srio_cfg->loopback_mode)
	{
		if(srio_cfg->serdes_cfg->linkSetup[0])
			srio_cfg->serdes_cfg->linkSetup[0]->loopBack = SERDES_LOOPBACK_ENABLE;
		if(srio_cfg->serdes_cfg->linkSetup[1])
			srio_cfg->serdes_cfg->linkSetup[1]->loopBack = SERDES_LOOPBACK_ENABLE;
		if(srio_cfg->serdes_cfg->linkSetup[2])
			srio_cfg->serdes_cfg->linkSetup[2]->loopBack = SERDES_LOOPBACK_ENABLE;
		if(srio_cfg->serdes_cfg->linkSetup[3])
			srio_cfg->serdes_cfg->linkSetup[3]->loopBack = SERDES_LOOPBACK_ENABLE;
	}

	Keystone_SRIO_HyperLink_Serdes_init(srio_cfg->serdes_cfg, srioSerdesRegs);
	Wait_SRIO_PLL_Lock();

	Keystone_SRIO_set_1x2x4x_Path(srio_cfg->srio_1x2x4x_path_control);

	Keystone_SRIO_set_device_ID(srio_cfg->device_ID_routing_config, 
		srio_cfg->uiNumDeviceId);

    Keystone_SRIO_CSR_CAR_Config(srio_cfg);

	/*Allocates ingress Data Nodes and Tags based on priority.
	These registers must only be changed while boot_complete is 
	deasserted or while the port is held in reset.*/
	for(i=0;i<SRIO_MAX_PORT_NUM;i++)
	{
		if(FALSE == srio_cfg->blockEn.bBLK5_8_Port_Datapath_EN[i])
			continue;

		/*maximum data nodes and tags are 72 (0x48).
		 Each data node stores 32 bytes of data*/
		srioRegs->RIO_PBM[i].RIO_PBM_SP_IG_WATERMARK0 = 
			(36<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK0_PRIO0_WM_SHIFT)
			|(32<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK0_PRIO0CRF_WM_SHIFT);
		srioRegs->RIO_PBM[i].RIO_PBM_SP_IG_WATERMARK1 = 
			(28<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK1_PRIO1_WM_SHIFT)
			|(24<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK1_PRIO1CRF_WM_SHIFT);
		srioRegs->RIO_PBM[i].RIO_PBM_SP_IG_WATERMARK2 = 
			(20<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK2_PRIO2_WM_SHIFT)
			|(16<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK2_PRIO2CRF_WM_SHIFT);
		srioRegs->RIO_PBM[i].RIO_PBM_SP_IG_WATERMARK3 = 
			(12<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK3_PRIO3_WM_SHIFT)
			|(8<<CSL_SRIO_RIO_PBM_SP_IG_WATERMARK3_PRIO3CRF_WM_SHIFT);
	}
	
	/*setup timeout value in microsecond*/
	Keystone_SRIO_Timeout_Config(srio_cfg, 500000, 500000, 100);

	if(srio_cfg->msg_cfg)
	{
	    Keystone_SRIO_Datastreaming_init(srio_cfg->msg_cfg->datastreaming_cfg);

		/*Setup garbage queue for error packet*/
		Keystone_SRIO_Garbage_Queue_Cfg(srio_cfg->msg_cfg);

		/*The mapping is programmable and must be configured after device reset.*/
		Keystone_map_SRIO_RX_message(srio_cfg->msg_cfg->message_map, 
			srio_cfg->msg_cfg->uiNumMessageMap);
	}
	
    Keystone_SRIO_RxMode_Setup(srio_cfg->rxMode);
    
    Keystone_SRIO_packet_forwarding_Cfg(srio_cfg->PktForwardingEntry_cfg,
        srio_cfg->uiNumPktForwardingEntry);

    Keystone_SRIO_MulticastID_Cfg(srio_cfg->multicastID,
        srio_cfg->uiNumMulticastID);

    Keystone_SRIO_Flow_Control(srio_cfg->flowControlID,
    	srio_cfg->uiNumFlowControlID);
    
	Keystone_SRIO_CSR_CAR_Config(srio_cfg);
	
	Keystone_SRIO_Prioirity_Permission_Setup(
		srio_cfg->priority_permission);

    Keystone_SRIO_Interrupt_init(srio_cfg->interrupt_cfg);

	if(SRIO_DIGITAL_LOOPBACK==srio_cfg->loopback_mode)
	{
		srioRegs->RIO_PER_SET_CNTL1 |=
			(0xF<<CSL_SRIO_RIO_PER_SET_CNTL1_LOOPBACK_SHIFT);
	}
	else if(SRIO_EXTERNAL_LINE_LOOPBACK==srio_cfg->loopback_mode)
	{
		for(i=0; i<SRIO_MAX_PORT_NUM; i++)
		{
			srioRegs->RIO_PLM[i].RIO_PLM_SP_IMP_SPEC_CTL=
				(1<<CSL_SRIO_RIO_PLM_SP_IMP_SPEC_CTL_LLB_EN_SHIFT);
		}
	}

    /*Set BOOT_COMPLETE bit*/
    srioRegs->RIO_PER_SET_CNTL |= (1 << CSL_SRIO_RIO_PER_SET_CNTL_BOOT_COMPLETE_SHIFT);

    /*This should be the last enable bit to toggle when bringing the 
    device out of reset to begin normal operation.*/
    srioRegs->RIO_PCR|= CSL_SRIO_RIO_PCR_PEREN_MASK;

	/*---------wait all enabled ports OK-------------*/
	for(i=0; i<SRIO_MAX_PORT_NUM; i++)
	{
		if(srio_cfg->blockEn.bLogic_Port_EN[i])
		{
			while(0==(srioRegs->RIO_SP[i].RIO_SP_ERR_STAT&
				CSL_SRIO_RIO_SP_ERR_STAT_PORT_OK_MASK));
		}
	}

	//for debug
    Keystone_SRIO_Error_Capture_Enable(&srio_cfg->blockEn);

}


