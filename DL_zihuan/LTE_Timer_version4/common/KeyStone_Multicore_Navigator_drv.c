

#include <c6x.h>
#include <stdio.h>
#include <string.h>
#include "cslr_device.h"
#include "csl_cacheAux.h"
#include "KeyStone_common.h"
#include "qmss_qm.h"
#include "qmss_acc.h"
#include "cppi_desc.h"
#include "KeyStone_Multicore_Navigator_drv.h"

/*Queue Manager definition*/
CSL_Qm_configRegs * qmCfgRegs= (CSL_Qm_configRegs *)CSL_QM_SS_CFG_CONFIG_STARVATION_COUNTER_REGS;
QMSS_DescriptorMemoryRegionRegs * qmDescriptorRegions= 
	(QMSS_DescriptorMemoryRegionRegs *)CSL_QM_SS_CFG_DESCRIPTION_REGS;

/*queue access registers through VBUSP configration bus*/
QueueManageRegs * queueManageRegs= 
	(QueueManageRegs *)CSL_QM_SS_CFG_QM_QUEUE_DEQUEUE_REGS;
QueueManageRegs * queueManageProxyRegs= 
	(QueueManageRegs *)CSL_QM_SS_CFG_PROXY_QUEUE_DEQUEUE_REGS;

/*queue access data space through VBUSM data bus*/
QueueManageRegs * queueManageData= 
	(QueueManageRegs *)CSL_QM_SS_DATA_QM_QUEUE_DEQUEUE_REGS;
QueueManageRegs * queueManageProxyData= 
	(QueueManageRegs *)CSL_QM_SS_DATA_PROXY_QUEUE_DEQUEUE_REGS;

QueueStatusConfigRegs * queueStatusConfigRegs= 
	(QueueStatusConfigRegs *)CSL_QM_SS_CFG_QUE_PEEK_REGS;
volatile Uint32 * queueThresholdStatus= (volatile Uint32 *)CSL_QM_SS_CFG_QM_STATUS_RAM_REGS;
CSL_Qm_intdRegs * qmIntRegs= (CSL_Qm_intdRegs *)CSL_QM_SS_CFG_INTD_REGS;

/*PDSP definition*/
volatile Uint32 * qmPdspIRam[2] = 
{
	(volatile Uint32 * )CSL_QM_SS_CFG_APDSP1_RAM_REGS,
	(volatile Uint32 * )CSL_QM_SS_CFG_APDSP2_RAM_REGS
};
CSL_PdspRegs * qmPdspCtrlRegs[2]= 
{
(CSL_PdspRegs * )CSL_QM_SS_CFG_ADSP1_REGS,
(CSL_PdspRegs * )CSL_QM_SS_CFG_ADSP2_REGS
};
volatile Uint32 * qmPdspCmd[2]= 
{
	(volatile Uint32 * )CSL_QM_SS_CFG_SCRACH_RAM1_REGS,
	(volatile Uint32 * )CSL_QM_SS_CFG_SCRACH_RAM2_REGS
};

/*PacketDMA definition*/
CSL_Cppidma_global_configRegs * qmDmaCfgRegs= 
	(CSL_Cppidma_global_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS;
CSL_Cppidma_tx_channel_configRegs * qmDmaTxChCfgRegs= 
	(CSL_Cppidma_tx_channel_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_TX_CFG_REGS;
CSL_Cppidma_rx_channel_configRegs * qmDmaRxChCfgRegs= 
	(CSL_Cppidma_rx_channel_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_RX_CFG_REGS;
volatile Uint32 * qmDmaTxChPriority = 
	(volatile Uint32 *)CSL_QM_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS;
CSL_Cppidma_rx_flow_configRegs * qmDmaRxFlowCfgRegs= 
	(CSL_Cppidma_rx_flow_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS;
PKT_DMA_Regs qmPktDmaRegs=
{
	(CSL_Cppidma_global_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
	(CSL_Cppidma_tx_channel_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_TX_CFG_REGS,
	(CSL_Cppidma_rx_channel_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_RX_CFG_REGS,
	(volatile Uint32 *)CSL_QM_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
	(CSL_Cppidma_rx_flow_configRegs * )CSL_QM_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS
};

#if 0 	/*for Nyquist only*/
CSL_Cppidma_global_configRegs * aifDmaCfgRegs= 
	(CSL_Cppidma_global_configRegs * )CSL_AIF2_CFG_CPPI_DMA_GLOBAL_CFG_REGS;
CSL_Cppidma_tx_channel_configRegs * aifDmaTxChCfgRegs= 
	(CSL_Cppidma_tx_channel_configRegs * )CSL_AIF2_CFG_CPPI_DMA_TX_CFG_REGS;
CSL_Cppidma_rx_channel_configRegs * aifDmaRxChCfgRegs= 
	(CSL_Cppidma_rx_channel_configRegs * )CSL_AIF2_CFG_CPPI_DMA_RX_CFG_REGS;
volatile Uint32 * aifDmaTxChPriority = 
	(volatile Uint32 *)CSL_AIF2_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS;
CSL_Cppidma_rx_flow_configRegs * aifDmaRxFlowCfgRegs= 
	(CSL_Cppidma_rx_flow_configRegs * )CSL_AIF2_CFG_CPPI_DMA_RX_FLOW_CFG_REGS;
PKT_DMA_Regs aifPktDmaRegs=
{
	(CSL_Cppidma_global_configRegs * )CSL_AIF2_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
	(CSL_Cppidma_tx_channel_configRegs * )CSL_AIF2_CFG_CPPI_DMA_TX_CFG_REGS,
	(CSL_Cppidma_rx_channel_configRegs * )CSL_AIF2_CFG_CPPI_DMA_RX_CFG_REGS,
	(volatile Uint32 *)CSL_AIF2_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
	(CSL_Cppidma_rx_flow_configRegs * )CSL_AIF2_CFG_CPPI_DMA_RX_FLOW_CFG_REGS
};
#endif 

CSL_Cppidma_global_configRegs * srioDmaCfgRegs= 
	(CSL_Cppidma_global_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS;
CSL_Cppidma_tx_channel_configRegs * srioDmaTxChCfgRegs= 
	(CSL_Cppidma_tx_channel_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_TX_CFG_REGS;
CSL_Cppidma_rx_channel_configRegs * srioDmaRxChCfgRegs= 
	(CSL_Cppidma_rx_channel_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_RX_CFG_REGS;
volatile Uint32 * srioDmaTxChPriority = 
	(volatile Uint32 *)CSL_SRIO_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS;
CSL_Cppidma_rx_flow_configRegs * srioDmaRxFlowCfgRegs= 
	(CSL_Cppidma_rx_flow_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS;
PKT_DMA_Regs srioPktDmaRegs=
{
	(CSL_Cppidma_global_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
	(CSL_Cppidma_tx_channel_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_TX_CFG_REGS,
	(CSL_Cppidma_rx_channel_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_RX_CFG_REGS,
	(volatile Uint32 *)CSL_SRIO_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
	(CSL_Cppidma_rx_flow_configRegs * )CSL_SRIO_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS
};

CSL_Cppidma_global_configRegs * paDmaCfgRegs= 
	(CSL_Cppidma_global_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS;
CSL_Cppidma_tx_channel_configRegs * paDmaTxChCfgRegs= 
	(CSL_Cppidma_tx_channel_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_TX_CFG_REGS;
CSL_Cppidma_rx_channel_configRegs * paDmaRxChCfgRegs= 
	(CSL_Cppidma_rx_channel_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_RX_CFG_REGS;
volatile Uint32 * paDmaTxChPriority = 
	(volatile Uint32 *)CSL_PA_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS;
CSL_Cppidma_rx_flow_configRegs * paDmaRxFlowCfgRegs= 
	(CSL_Cppidma_rx_flow_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS;
PKT_DMA_Regs paPktDmaRegs=
{
	(CSL_Cppidma_global_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
	(CSL_Cppidma_tx_channel_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_TX_CFG_REGS,
	(CSL_Cppidma_rx_channel_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_RX_CFG_REGS,
	(volatile Uint32 *)CSL_PA_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
	(CSL_Cppidma_rx_flow_configRegs * )CSL_PA_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS
};

Uint32 linkingEntrySize= 0; 	/*total number of linking entries*/

/*use QMSS internal linking RAM for region 0
if linkingRAM1!=NULL, use linkingRAM1 for linking RAM region 1*/
void KeyStone_QMSS_Linking_RAM_init(unsigned long long * linkingRAM1,
	Uint32 linkingRMA1EntrySize)
{
	int i;
	
	/*use QMSS internal linking RAM for region 0*/
	qmCfgRegs->LINKING_RAM_REGION_0_BASE_ADDRESS_REG= QMSS_LINKING_RAM_OFFSET;
	qmCfgRegs->LINKING_RAM_REGION_0_SIZE_REG= QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE;
	linkingEntrySize= QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE+1;

	/*configure linking RAM region 1*/
	if(NULL!=linkingRAM1&&linkingRMA1EntrySize)
	{
		qmCfgRegs->LINKING_RAM_REGION_1_BASE_ADDRESS_REG= GLOBAL_ADDR(linkingRAM1);
		linkingEntrySize+= linkingRMA1EntrySize;
	}
	
	//empty all queues
	for(i=0; i<8192 ; i++)
		queueManageRegs[i].REG_D_Descriptor= 0;
	
}

/*initialize descriptor regions according to "descMemRegionCfg",
"uiDescRegionNum" is the number of regions to be initialized*/
void KeyStone_QMSS_Descriptor_Regions_init(
	Qmss_DescMemRegionCfg * descMemRegionCfg, Uint32 uiDescRegionNum)
{
	int i;
	Uint32 uiStartIndex= 0;

	/*check the region configuration*/
	for(i=0; i< uiDescRegionNum-1; i++)
	{
		if((descMemRegionCfg[i].descBase + 
			descMemRegionCfg[i].descNum*descMemRegionCfg[i].descSize) >
			descMemRegionCfg[i+1].descBase)
		{
			printf("!!!descriptor region %d RAM overlap with descriptor region %d RAM\n", 
				i, i+1);
			return;
		}
	}

	//set registers with configuration values
	for(i=0; i< uiDescRegionNum; i++)
	{
		if(uiStartIndex+descMemRegionCfg[i].descNum>linkingEntrySize)
		{
			printf("!!!descriptor region %d last index %d exceeds linking RAM entry size %d\n",
				i, uiStartIndex+descMemRegionCfg[i].descNum-1, linkingEntrySize);
			return;
		}

		if(descMemRegionCfg[i].descSize&0xf)
		{
			printf("!!!size of descriptor region %d is %d, not multiple 16 bytes\n",
				i, descMemRegionCfg[i].descSize);
			return;
		}
		if(descMemRegionCfg[i].descSize>(128*1024))
		{
			printf("!!!size of descriptor region %d is %d > 128K\n",
				i, descMemRegionCfg[i].descSize);
			return;
		}

		if(descMemRegionCfg[i].descNum<32)
		{
			printf("!!!number of descriptors in region %d is %d < 32\n",
				i, descMemRegionCfg[i].descNum);
			return;
		}

		if(descMemRegionCfg[i].descNum>(1024*1024))
		{
			printf("!!!number of descriptors in region %d is %d > 1M\n",
				i, descMemRegionCfg[i].descNum);
			return;
		}
		if((_dotpu4(_bitc4(descMemRegionCfg[i].descNum), 0x01010101))>1)
		{
			printf("!!!number of descriptors in region %d is %d, not power of 2\n",
				i, descMemRegionCfg[i].descNum);
			return;
		}

		qmDescriptorRegions[i].BASE_ADDRESS_REG=
			descMemRegionCfg[i].descBase;
		qmDescriptorRegions[i].START_INDEX_REG= uiStartIndex;
		qmDescriptorRegions[i].DESCRIPTOR_SETUP_REG=
			(((descMemRegionCfg[i].descSize>>4)-1)<<16)|
			((31-_lmbd(descMemRegionCfg[i].descNum, 1)-5));

		uiStartIndex+= descMemRegionCfg[i].descNum;
	}	

}

/*initialize host descriptors and queues according to "hostQuCfg",
"uiQuCfgNumber" is number of Free Descriptor queues to be initalized*/
void KeyStone_Host_Descriptor_Queues_init(
	FreeHostQueueCfg *hostQuCfg, Uint32 uiQuCfgNumber)
{
	int i, j;
	Uint32 uiTempletSize, uiBufferAddress, uiDescriptorAddress;
	HostPacketDescriptor descriptorTemplet, * descriptor;

	uiTempletSize= sizeof(descriptorTemplet);
	memset(&descriptorTemplet, 0, uiTempletSize); 

	descriptorTemplet.type_id= Cppi_DescType_HOST;
	descriptorTemplet.ret_push_policy= 1 ; //return to queue head
	descriptorTemplet.return_policy= 1 ; //each descriptor separately goes to pkt_return_qnum

	for(j=0; j<uiQuCfgNumber; j++)
	{
		/*initialze buffer size*/
		descriptorTemplet.packet_length= hostQuCfg[j].uiBufferSize;
		descriptorTemplet.buffer_len= hostQuCfg[j].uiBufferSize;
		descriptorTemplet.orig_buff0_len= hostQuCfg[j].uiBufferSize;

		/*initialize this free queue as return queue number*/
		descriptorTemplet.pkt_return_qmgr= hostQuCfg[j].uiFreeQuNum>>12;
		descriptorTemplet.pkt_return_qnum= hostQuCfg[j].uiFreeQuNum&0xFFF;

		uiBufferAddress= hostQuCfg[j].uiBufferAddress;
		uiDescriptorAddress= hostQuCfg[j].uiDescriptorAddress;
		for(i=0; i<hostQuCfg[j].uiDescriptorNumber; i++)
		{
			/*copy contents in templet*/
			memcpy((void *)uiDescriptorAddress, 
				(void *)&descriptorTemplet, uiTempletSize);

			/*initialize buffer address*/
			descriptor = (HostPacketDescriptor *)uiDescriptorAddress;
			descriptor->buffer_ptr= uiBufferAddress;
			descriptor->orig_buff0_ptr= uiBufferAddress;

			/*push this descriptor to free queue*/
			queueManageRegs[hostQuCfg[j].uiFreeQuNum].REG_D_Descriptor= 
				uiDescriptorAddress|FETCH_SIZE_32;
			
			/*point to next descriptor and buffer*/
			uiDescriptorAddress+= hostQuCfg[j].uiDescriptorSize;
			uiBufferAddress+= hostQuCfg[j].uiBufferSize;
		
		}
	}
	CACHE_wbAllL2(CACHE_WAIT); 	//write back all descriptors in cache
}

/*initialize monolithic descriptors and queues according to "monoQuCfg",
"uiQuCfgNumber" is number of Free Descriptor queues to be initalized*/
void KeyStone_Mono_Descriptor_Queues_init(
	FreeMonoQueueCfg *monoQuCfg, Uint32 uiQuCfgNumber)
{
	int i, j;
	Uint32 uiTempletSize, uiDescriptorAddress;
	MonolithicPacketDescriptor descriptorTemplet;

	uiTempletSize= sizeof(descriptorTemplet);
	memset(&descriptorTemplet, 0, uiTempletSize); 

	descriptorTemplet.type_id= Cppi_DescType_MONOLITHIC;
	descriptorTemplet.ret_push_policy= 1 ; //return to queue head
	descriptorTemplet.data_offset= uiTempletSize;

	for(j=0; j<uiQuCfgNumber; j++)
	{
		descriptorTemplet.packet_length= 
			monoQuCfg[j].uiDescriptorSize- uiTempletSize;

		/*initialize this free queue as return queue number*/
		descriptorTemplet.pkt_return_qmgr= monoQuCfg[j].uiFreeQuNum>>12;
		descriptorTemplet.pkt_return_qnum= monoQuCfg[j].uiFreeQuNum&0xFFF;

		uiDescriptorAddress= monoQuCfg[j].uiDescriptorAddress;
		for(i=0; i<monoQuCfg[j].uiDescriptorNumber; i++)
		{
			/*copy contents in templet*/
			memcpy((void *)uiDescriptorAddress, 
				(void *)&descriptorTemplet, uiTempletSize);

			/*push this descriptor to free queue*/
			queueManageRegs[monoQuCfg[j].uiFreeQuNum].REG_D_Descriptor= 
				uiDescriptorAddress|FETCH_SIZE_16;
			
			/*point to next descriptor and buffers*/
			uiDescriptorAddress+= monoQuCfg[j].uiDescriptorSize;
		
		}
	}
	CACHE_wbAllL2(CACHE_WAIT); 	//write back all descriptors in cache
}

/*Downloads the PDSP firmware to PDSP. */
void KeyStone_Qmss_Download_Firmware (
	Qmss_PdspId pdspId, void *image, Uint32 size)
{
    Uint32            i, count;
    volatile Uint32   enable;
    Uint32            *data = (Uint32 *) image;

    /* Reset the PDSP */
    qmPdspCtrlRegs[pdspId]->PDSP_CONTROL_REG &= 
    	~CSL_PDSP_PDSP_CONTROL_REG_PDSP_ENABLE_MASK;

    /* Confirm PDSP has halted */
    do
    { 
        enable = qmPdspCtrlRegs[pdspId]->PDSP_CONTROL_REG & 
        	CSL_PDSP_PDSP_CONTROL_REG_PDSP_STATE_MASK;
    }while (enable);

    count = size / 4;

    /* Download the firmware */
    for(i = 0; i < count; i++)
        qmPdspIRam[pdspId][i] = data[i];

    /* Use the command register to sync the PDSP */
    *qmPdspCmd[pdspId] = 0xFFFFFFFF;

    /* Wait to the memory write to land */
    for(i = 0; i < 20000 && *qmPdspCmd[pdspId] != 0xFFFFFFFF; i++);
   
    /* Reset program counter to zero */
	qmPdspCtrlRegs[pdspId]->PDSP_CONTROL_REG &= 
		~CSL_PDSP_PDSP_CONTROL_REG_PCOUNTER_RST_VAL_MASK;
    
	/* Set soft reset to zero to load the program counter */
	qmPdspCtrlRegs[pdspId]->PDSP_CONTROL_REG &=
		~CSL_PDSP_PDSP_CONTROL_REG_SOFT_RST_N_MASK;
    
    /* Enable the PDSP */
    qmPdspCtrlRegs[pdspId]->PDSP_CONTROL_REG |=
    	CSL_PDSP_PDSP_CONTROL_REG_PDSP_ENABLE_MASK;

    /* Wait for the command register to clear */
    while (*qmPdspCmd[pdspId]);

    return;
}

/*This function programs the accumulator channel with values passed in the cfg structure*/
Uint32 KeyStone_Qmss_Config_Acc_Channel (
	Qmss_PdspId pdspId, Qmss_AccCmdCfg *cfg)
{
    Uint32         cmd[5];
    volatile Uint32   *cmdPtr, *reg;
    Uint32            index;
    Uint8             result;

    memset ((void *) &cmd, 0, 20);
    CSL_FINSR (cmd[0], 7, 0, cfg->channel);
    CSL_FINSR (cmd[0], 15, 8, cfg->command);
    cmd[1] = cfg->queueEnMask;
    cmd[2] = cfg->listAddress;
    CSL_FINSR (cmd[3], 15, 0, cfg->queMgrIndex);
    CSL_FINSR (cmd[3], 31, 16, cfg->maxPageEntries);
    CSL_FINSR (cmd[4], 15, 0, cfg->timerLoadCount);
    CSL_FINSR (cmd[4], 17, 16, cfg->interruptPacingMode);
    CSL_FINSR (cmd[4], 19, 18, cfg->listEntrySize);
    CSL_FINSR (cmd[4], 20, 20, cfg->listCountMode);
    CSL_FINSR (cmd[4], 21, 21, cfg->multiQueueMode);
    
    /* Point to the accumulator command register's last word */
    reg = (Uint32 *) ((Uint8 *) qmPdspCmd[pdspId] + 4 * 4);

    /* Write command word last */
    cmdPtr = ((Uint32 *) &cmd) + 4;

    for (index = 0; index < 5; index++)
        *reg-- = *cmdPtr--;

    /* wait for the command to clear */
    reg++;
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

	/*clear interrupts*/
	qmIntRegs->INTCNT_REG[cfg->channel]= 0 ;
	qmIntRegs->EOI_REG= cfg->channel+2;

    return (Uint32) (CSL_FEXTR (*reg, 31, 24));
}

/*This function disables the accumulator functionality for the specified channel*/
Uint32 KeyStone_Qmss_disable_Acc_Channel (
	Qmss_PdspId pdspId, Uint8 channel)
{
    Uint32         cmd[5];
    volatile Uint32   *cmdPtr, *reg;
    Uint32            index;
    Uint8             result;

    memset ((void *) &cmd, 0, 20);
    CSL_FINSR (cmd[0], 7, 0, channel);
    CSL_FINSR (cmd[0], 15, 8, Qmss_AccCmd_DISABLE_CHANNEL);

    /* Point to the accumulator command register's last word */
    reg = (Uint32 *) ((Uint8 *) qmPdspCmd[pdspId] + 4 * 4);

    /* Write command word last */
    cmdPtr = ((Uint32 *) &cmd) + 4;

    for (index = 0; index < 5; index++)
        *reg-- = *cmdPtr--;

    /* Wait for the command to clear */
    reg++;
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    return (Uint32) (CSL_FEXTR (*reg, 31, 24));
}

/**
 *      This function programs the timer constant used by the firmware to generate the timer tick.
 *
 *      The Accumulator time "tick" is controlled by a local timer connected to the PDSP core. 
 *      This timer has a programmable count based on the sub-system clock. When this count expires, 
 *      a local "tick" is registered in the firmware. The tick is used when timing channel interrupts 
 *      based on the "Timer Load Count" value supplied in the timer configuration.
 *
 *      The value of "Timer Constant" is the number of QM sub-system clocks divided by 2 that 
 *      comprise a single "tick" in the accumulator firmware.
 *
 *      For example, if the QM sub-system clock is 350MHz, and the desired firmware "tick" is 20us, 
 *      the proper Timer Constant for this command is computed as follows:
 *      
 *      Timer Constant = (350,000,000 cycles/sec) * (0.000020 sec) / (2 cycles)
 *      Timer Constant = 3,500
 *      
 *      The firmware initializes with a default constant value of 4375. This corresponds to firmware tick of 25us.
 */
Uint32 KeyStone_Qmss_config_Acc_Timer (
	Qmss_PdspId pdspId, Uint16 timerConstant)
{

    volatile Uint32   *reg;
    Uint8             result;

    /* Point to the accumulator command register's last word */
    reg = (Uint32 *) ((Uint8 *) qmPdspCmd[pdspId] + 4);
    *reg-- = timerConstant;
   
    *reg = Qmss_AccCmd_CONFIG_TIMER_CONSTANT << 8; 

    /* wait for the command to clear */
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    return (Uint32) (CSL_FEXTR (*reg, 31, 24));
}


/**
 *      The Accumulator firmware also includes an optional reclamation queue which can be used for 
 *      packet discards. Any descriptor placed on the reclamation queue will be recycled in the 
 *      same manner as if it had been submitted to CDMA. 
 *
 *      The descriptor packet information field is used to determine the return queue and the 
 *      return handling, including options to unlink host descriptors and push to either the 
 *      front or the back of the return queue.
 *      Setting queue to zero disables the reclamation feature
 */
Uint32 KeyStone_Qmss_Config_Reclaim_Queue (
	Qmss_PdspId pdspId, Uint32 uiQueueNumber)
{
    volatile Uint32   *reg;
    Uint8             result;

    /* Point to the accumulator command register's last word */
    reg = (Uint32 *) ((Uint8 *) qmPdspCmd[pdspId] + 4);
    *reg-- = uiQueueNumber;
    *reg = Qmss_AccCmd_CONFIG_RECLAIM_QUEUE << 8;

    /* wait for the command to clear */
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    return (Uint32) (CSL_FEXTR (*reg, 31, 24));
}

/*configure the global control registers of a packet DMA*/
void KeyStone_pktDma_Global_Control (
	CSL_Cppidma_global_configRegs * pktDmaCfgRegs,
	Uint32 uiStarvationWaitCycles, Uint32 rxPriority, Uint32 txPriority)
{
	pktDmaCfgRegs->PERF_CONTROL_REG = (pktDmaCfgRegs->PERF_CONTROL_REG&
		(~CSL_CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_TIMEOUT_CNT_MASK))|
		(uiStarvationWaitCycles<<
		CSL_CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_TIMEOUT_CNT_SHIFT);

	pktDmaCfgRegs->PRIORITY_CONTROL_REG = 
		(rxPriority<<
		CSL_CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_RX_PRIORITY_SHIFT)|
		(txPriority<<
		CSL_CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_TX_PRIORITY_SHIFT);

	pktDmaCfgRegs->QM_BASE_ADDRESS_REG[0]= 0x34020000; 	/*queue 0~4095*/
	pktDmaCfgRegs->QM_BASE_ADDRESS_REG[1]= 0x34030000; 	/*queue 4096~8191*/
}

void KeyStone_pktDma_TxCh_config(PKT_DMA_Regs * pktDmaRegs, Uint32 uiChNum,
	PktDma_TxChCfg * pktDmaTxChCfg)
{
	pktDmaRegs->txChPriority[uiChNum]= pktDmaTxChCfg->priority;

	pktDmaRegs->txChCfgRegs->TX_CHANNEL_GLOBAL_CONFIG[uiChNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B= 
		(pktDmaTxChCfg->filterEINFO<<
		CSL_CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_EINFO_SHIFT)|
		(pktDmaTxChCfg->filterPS<<
		CSL_CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_PSWORDS_SHIFT)|
		(pktDmaTxChCfg->aifMonoMode<<
		CSL_CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_AIF_MONO_MODE_SHIFT);
}


void KeyStone_pktDma_configureRxFlow (
	CSL_Cppidma_rx_flow_configRx_flow_configRegs * rxFlowCfgRegs, 
	PktDma_RxFlowCfg * cfg, Uint32 uiFlowNum)
{
	int i;
    Uint32      reg, temp;

	for(i=0; i<uiFlowNum; i++)
	{
	    reg = 0;
		/* Rx flow configuration register A */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QNUM, cfg[i].rx_dest_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QMGR, cfg[i].rx_dest_qnum>>12);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_SOP_OFFSET, cfg[i].rx_sop_offset);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PS_LOCATION, cfg[i].rx_ps_location);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DESC_TYPE, cfg[i].rx_desc_type);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_ERROR_HANDLING, cfg[i].rx_error_handling);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PSINFO_PRESENT, cfg[i].rx_psinfo_present);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_EINFO_PRESENT, cfg[i].rx_einfo_present);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_A = reg;

	    reg = 0;
	    /* Rx flow configuration register B */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_LO, cfg[i].rx_dest_tag_lo);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_HI, cfg[i].rx_dest_tag_hi);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_LO, cfg[i].rx_src_tag_lo);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_HI, cfg[i].rx_src_tag_hi);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_B = reg;

	    reg = 0;
	    /* Rx flow configuration register C */		
	    temp = ((cfg[i].rx_size_thresh0_en) | (cfg[i].rx_size_thresh1_en << 1) | (cfg[i].rx_size_thresh2_en << 2));

	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SIZE_THRESH_EN, temp);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_LO_SEL, cfg[i].rx_dest_tag_lo_sel);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_HI_SEL, cfg[i].rx_dest_tag_hi_sel);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_LO_SEL, cfg[i].rx_src_tag_lo_sel);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_HI_SEL, cfg[i].rx_src_tag_hi_sel);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_C = reg;

	    reg = 0;
		/* Rx flow configuration register D */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QNUM, cfg[i].rx_fdq1_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QMGR, cfg[i].rx_fdq1_qnum>>12);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QNUM, cfg[i].rx_fdq0_sz0_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QMGR, cfg[i].rx_fdq0_sz0_qnum>>12);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_D = reg;

	    reg = 0;
		/* Rx flow configuration register E */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QNUM, cfg[i].rx_fdq3_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QMGR, cfg[i].rx_fdq3_qnum>>12);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QNUM, cfg[i].rx_fdq2_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QMGR, cfg[i].rx_fdq2_qnum>>12);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_E = reg;

	    reg = 0;
		/* Rx flow configuration register F */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH1, (cfg[i].rx_size_thresh1 >> 5));
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH0, (cfg[i].rx_size_thresh0  >> 5));
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_F = reg;
	    
	    reg = 0;
	    /* Rx flow configuration register G */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QNUM, cfg[i].rx_fdq0_sz1_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QMGR, cfg[i].rx_fdq0_sz1_qnum>>12);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_SIZE_THRESH2, (cfg[i].rx_size_thresh2) >> 5);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_G = reg;

	    reg = 0;
	    /* Rx flow configuration register H */
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QNUM, cfg[i].rx_fdq0_sz3_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QMGR, cfg[i].rx_fdq0_sz3_qnum>>12);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QNUM, cfg[i].rx_fdq0_sz2_qnum);
	    CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QMGR, cfg[i].rx_fdq0_sz2_qnum>>12);
	    rxFlowCfgRegs[i].RX_FLOW_CONFIG_REG_H = reg;
	}
}

