
#include <stdio.h>
#include "csl_bootcfgaux.h"
#include "KeyStone_DDR_Init.h"
#include "KeyStone_common.h"

CSL_Emif4fRegs * DDR_Regs= (CSL_Emif4fRegs *)CSL_DDR3_EMIF_CONFIG_REGS;

/*configure DDR according to the clock speed
please note, clock_MHz is the clock speed in MHz, not data rate. 
For example, clock speed for 1333.333M data rate is 666.667MHz*/
void Shannon_EVM_DDR_Init(float clock_MHz)
{
	printf("configure DDR at %d MHz\n", (unsigned int)clock_MHz);
	
	CSL_BootCfgUnlockKicker();

	/*Drives CKE low.
	This is a JEDEC requirement that we have 500us delay between reset de-assert 
	and cke assert and then program the correct refresh rate
	The DDR internal clock is divide by 16 before SDCFG write*/
	DDR_Regs->SDRAM_REF_CTRL = 0x80000000|(unsigned int)(500.f*clock_MHz/16.f);

	DDR_Regs->SDRAM_TIM_1 =
		((unsigned int)(13.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RP_SHIFT)|
		((unsigned int)(13.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RCD_SHIFT)|
		((unsigned int)(15*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_WR_SHIFT)|
		((unsigned int)(36*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RAS_SHIFT)|
		((unsigned int)(49.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RC_SHIFT)|
		((unsigned int)(45*clock_MHz/4000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RRD_SHIFT)| 	/*T_RRD = (tFAW/(4*tCK)) 每 1*/
		((unsigned int)(7.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_WTR_SHIFT);
	DDR_Regs->SDRAM_TIM_2   = 
		((unsigned int)(6*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XP_SHIFT)|
		((unsigned int)(120*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSNR_SHIFT)| 	/*T_XSNR = (tXS /tCK)每 1*/
		((512-1)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSRD_SHIFT)| 	/*T_XSRD =tXSDLL每 1*/
		((unsigned int)(7.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_RTP_SHIFT)|
		((unsigned int)(5.625*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_CKE_SHIFT);
	DDR_Regs->SDRAM_TIM_3   = 
		(5<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_PDLL_UL_SHIFT)| 	/*This field must always be programmed to 0x5.*/
		((5)<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_CSTA_SHIFT)| 	/*This field should be set according to PHY requirements as 0x5.*/
		((unsigned int)(5.625*clock_MHz/1000.f+0.9999f)<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_CKESR_SHIFT)|
		((64-1)<<CSL_EMIF4F_SDRAM_TIM_3_REG_ZQ_ZQCS_SHIFT)|
		((unsigned int)(110*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_RFC_SHIFT)|
		(15<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_RAS_MAX_SHIFT); 	/*This field must always be programmed to 0xF.*/

	DDR_Regs->DDR_PHY_CTRL_1  = 0x00100100|
		(12<<CSL_EMIF4F_DDR_PHY_CTRL_1_REG_READ_LATENCY_SHIFT); 	/*between CAS Latency + 1 and CAS Latency + 7*/

	DDR_Regs->ZQ_CONFIG = 
		((0)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_CS1EN_SHIFT)|
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_CS0EN_SHIFT)|
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_DUALCALEN_SHIFT)| 	/*This bit should always be set to 1.*/
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_SFEXITEN_SHIFT)|
		((512/256-1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_ZQINIT_MULT_SHIFT)| 	/*T_ZQ_ZQINIT_MULT = (tZQinit/tZQoper 每 1)*/
		((256/64-1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_ZQCL_MULT_SHIFT)| 	/*T_ZQ_ZQCL_MULT = (tZQoper/tZQCS 每 1)*/
		/*interval between ZQCS commands = ZQCorrection/((TSens x Tdriftrate) + (VSens x Vdriftrate))
		= 0.5/((1.5*1.2)+(0.15*10))= 0.151515151s, this time need be converted to refresh period number*/
		((151515151/(64000000/8192))<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_REFINTERVAL_SHIFT);

	/*map priority 0,1,2,3 to COS0,
	map priority 3,5,6,7 to COS1*/
	DDR_Regs->PRI_COS_MAP = 
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_COS_MAP_EN_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_7_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_6_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_5_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_4_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_3_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_2_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_1_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_0_COS_SHIFT);

	/*master based COS map is disabled*/
	DDR_Regs->MSTID_COS_1_MAP= 0;
	DDR_Regs->MSTID_COS_2_MAP= 0;

	/*LAT_CONFIG*/
	DDR_Regs->VBUSM_CONFIG= 
		(8<<CSL_EMIF4F_VBUSM_CONFIG_REG_COS_COUNT_1_SHIFT)|
		(16<<CSL_EMIF4F_VBUSM_CONFIG_REG_COS_COUNT_2_SHIFT)|
		(32<<CSL_EMIF4F_VBUSM_CONFIG_REG_PR_OLD_COUNT_SHIFT);

	DDR_Regs->ECC_CTRL = 
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_EN_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_PROT_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_2_EN_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_1_EN_SHIFT);

	DDR_Regs->ECC_ADDR_RNG_1= 
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_1_REG_ECC_STRT_ADDR_1_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_1_REG_ECC_END_ADDR_1_SHIFT);

	DDR_Regs->ECC_ADDR_RNG_2= 
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_2_REG_ECC_STRT_ADDR_2_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_2_REG_ECC_END_ADDR_2_SHIFT);

	/* enables DRAM configuration.  It still has the refresh interval 
	programmed to the longer number needed during DRAM initialization.*/
	DDR_Regs->SDRAM_REF_CTRL = (unsigned int)(500.f*clock_MHz/16.f); 

	DDR_Regs->SDRAM_CONFIG = 
		(3<<CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_TYPE_SHIFT)| 	/*Set to 3 for DDR3. All other values reserved.*/
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_POS_SHIFT)|
		(DDR_TERM_RZQ_OVER_6<<CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_TERM_SHIFT)|
		(DDR_DYN_ODT_DISABLED<<CSL_EMIF4F_SDRAM_CONFIG_REG_DYN_ODT_SHIFT)|
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_DISABLE_DLL_SHIFT)|
		(SDRAM_DRIVE_RZQ_OVER_7<<CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_DRIVE_SHIFT)|
		(DDR_CWL_7<<CSL_EMIF4F_SDRAM_CONFIG_REG_CWL_SHIFT)|
		(DDR_BUS_WIDTH_64<<CSL_EMIF4F_SDRAM_CONFIG_REG_NARROW_MODE_SHIFT)|
		(DDR_CL_9<<CSL_EMIF4F_SDRAM_CONFIG_REG_CL_SHIFT)|
		(DDR_ROW_SIZE_13_BIT<<CSL_EMIF4F_SDRAM_CONFIG_REG_ROWSIZE_SHIFT)|
		(DDR_BANK_NUM_8<<CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_SHIFT)|
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_EBANK_SHIFT)|
		(DDR_PAGE_SIZE_10_BIT_1024_WORD<<CSL_EMIF4F_SDRAM_CONFIG_REG_PAGESIZE_SHIFT);

//	DDR_Regs->SDRAM_REF_CTRL    = 64000000/8192/(1000/clock_MHz);
	DDR_Regs->SDRAM_REF_CTRL    = (unsigned int)64000.f*clock_MHz/8192.f;

	//initial vale for leveling
	/*GTLVL_INIT_RATIO*/
	boot_cfg_regs->DDR3_CONFIG_REG[14] = 0xA1;
	boot_cfg_regs->DDR3_CONFIG_REG[15] = 0x9E;
	boot_cfg_regs->DDR3_CONFIG_REG[16] = 0xA7;
	boot_cfg_regs->DDR3_CONFIG_REG[17] = 0xA9;
	boot_cfg_regs->DDR3_CONFIG_REG[18] = 0xCA;
	boot_cfg_regs->DDR3_CONFIG_REG[19] = 0xBE;
	boot_cfg_regs->DDR3_CONFIG_REG[20] = 0xDD;
	boot_cfg_regs->DDR3_CONFIG_REG[21] = 0xDD;
	boot_cfg_regs->DDR3_CONFIG_REG[22] = 0xBA;

	/*WRLVL_INIT_RATIO*/
	boot_cfg_regs->DDR3_CONFIG_REG[2] = 0x20;
	boot_cfg_regs->DDR3_CONFIG_REG[3] = 0x24;
	boot_cfg_regs->DDR3_CONFIG_REG[4] = 0x3A;
	boot_cfg_regs->DDR3_CONFIG_REG[5] = 0x38;
	boot_cfg_regs->DDR3_CONFIG_REG[6] = 0x51;
	boot_cfg_regs->DDR3_CONFIG_REG[7] = 0x5E;
	boot_cfg_regs->DDR3_CONFIG_REG[8] = 0x5E;
	boot_cfg_regs->DDR3_CONFIG_REG[9] = 0x5E;
	boot_cfg_regs->DDR3_CONFIG_REG[10] = 0x44;

	boot_cfg_regs->DDR3_CONFIG_REG[0] |= 0xF; 	// set dll_lock_diff to 15

	/*Invert Clock Out*/
	boot_cfg_regs->DDR3_CONFIG_REG[0] &= ~(0x007FE000);  // clear ctrl_slave_ratio field
	boot_cfg_regs->DDR3_CONFIG_REG[0] |= 0x00200000;     // set ctrl_slave_ratio to 0x100
	boot_cfg_regs->DDR3_CONFIG_REG[12] |= 0x08000000;    // Set invert_clkout = 1
	
	boot_cfg_regs->DDR3_CONFIG_REG[23] |= 0x00000200; //Set bit 9 = 1 to use forced ratio leveling for read DQS

	//enable full leveling, no incremental leveling
	/*Typically you program the ramp window for a higher rate of 
	incremental leveling, meaning the 3 fields in the ramp control 
	register will be smaller than the corresponding fields in the 
	RDWR_LVL_CTRL.*/
	DDR_Regs->RDWR_LVL_RMP_WIN = 4096;
	DDR_Regs->RDWR_LVL_RMP_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVL_EN_SHIFT)|
		(16<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVLINC_RMP_PRE_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDLVLINC_RMP_INT_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDLVLGATEINC_RMP_INT_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_WRLVLINC_RMP_INT_SHIFT);
	DDR_Regs->RDWR_LVL_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLFULL_START_SHIFT)|
		(32<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLINC_PRE_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDLVLINC_INT_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDLVLGATEINC_INT_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_WRLVLINC_INT_SHIFT);


}

/*configure DDR according to the clock speed
please note, clock_MHz is the clock speed in MHz, not data rate. 
For example, clock speed for 1333.333M data rate is 666.667MHz*/
void Nyquist_EVM_DDR_Init(float clock_MHz)
{
	int i;
	
	unsigned int uiDSP_Num= Get_DSP_Number();

	if(0!=uiDSP_Num&&clock_MHz>533.333)
		clock_MHz= 533.333;
		
	printf("configure DDR at %d MHz\n", (unsigned int)clock_MHz);
	
	CSL_BootCfgUnlockKicker();

	/*different DSP on the board has different trace length to the DDR device
	so, the initial leveling value is different*/
	if(0==uiDSP_Num)
	{
		//initial vale for leveling                
		/*WRLVL_INIT_RATIO*/                       
		boot_cfg_regs->DDR3_CONFIG_REG[2]  = 0x0F; 
		boot_cfg_regs->DDR3_CONFIG_REG[3]  = 0x0F; 
		boot_cfg_regs->DDR3_CONFIG_REG[4]  = 0x20; 
		boot_cfg_regs->DDR3_CONFIG_REG[5]  = 0x24; 
		boot_cfg_regs->DDR3_CONFIG_REG[6]  = 0x33; 
		boot_cfg_regs->DDR3_CONFIG_REG[7]  = 0x38; 
		boot_cfg_regs->DDR3_CONFIG_REG[8]  = 0x21; 
		boot_cfg_regs->DDR3_CONFIG_REG[9]  = 0x29; 
		boot_cfg_regs->DDR3_CONFIG_REG[10] = 0x0 ; 
                                                           
		/*GTLVL_INIT_RATIO*/                       
		boot_cfg_regs->DDR3_CONFIG_REG[14] = 0x7B; 
		boot_cfg_regs->DDR3_CONFIG_REG[15] = 0x7B; 
		boot_cfg_regs->DDR3_CONFIG_REG[16] = 0x8D; 
		boot_cfg_regs->DDR3_CONFIG_REG[17] = 0x89; 
		boot_cfg_regs->DDR3_CONFIG_REG[18] = 0xC0; 
		boot_cfg_regs->DDR3_CONFIG_REG[19] = 0xBC; 
		boot_cfg_regs->DDR3_CONFIG_REG[20] = 0xC3; 
		boot_cfg_regs->DDR3_CONFIG_REG[21] = 0xBB; 
		boot_cfg_regs->DDR3_CONFIG_REG[22] = 0x0 ; 
	}
	else
	{
		//initial vale for leveling
		/*GTLVL_INIT_RATIO*/
		boot_cfg_regs->DDR3_CONFIG_REG[22] = 0;
		boot_cfg_regs->DDR3_CONFIG_REG[21] = 0xB8;
		boot_cfg_regs->DDR3_CONFIG_REG[20] = 0xC0;
		boot_cfg_regs->DDR3_CONFIG_REG[19] = 0xBE;
		boot_cfg_regs->DDR3_CONFIG_REG[18] = 0xC2;
		boot_cfg_regs->DDR3_CONFIG_REG[17] = 0x89;
		boot_cfg_regs->DDR3_CONFIG_REG[16] = 0x8C;
		boot_cfg_regs->DDR3_CONFIG_REG[15] = 0x7B;
		boot_cfg_regs->DDR3_CONFIG_REG[14] = 0x7E;

		/*WRLVL_INIT_RATIO*/
		boot_cfg_regs->DDR3_CONFIG_REG[10] = 0;
		boot_cfg_regs->DDR3_CONFIG_REG[9]  = 0x25;
		boot_cfg_regs->DDR3_CONFIG_REG[8]  = 0x1D;
		boot_cfg_regs->DDR3_CONFIG_REG[7]  = 0x38;
		boot_cfg_regs->DDR3_CONFIG_REG[6]  = 0x34;
		boot_cfg_regs->DDR3_CONFIG_REG[5]  = 0x21;
		boot_cfg_regs->DDR3_CONFIG_REG[4]  = 0x1E;
		boot_cfg_regs->DDR3_CONFIG_REG[3]  = 0x0E;
		boot_cfg_regs->DDR3_CONFIG_REG[2]  = 0x0B;
	}
	
	boot_cfg_regs->DDR3_CONFIG_REG[0] |= 0xF; 	// set dll_lock_diff to 15

	/*Invert Clock Out*/
	boot_cfg_regs->DDR3_CONFIG_REG[0] &= ~(0x007FE000);  // clear ctrl_slave_ratio field
	boot_cfg_regs->DDR3_CONFIG_REG[0] |= 0x00200000;     // set ctrl_slave_ratio to 0x100
	boot_cfg_regs->DDR3_CONFIG_REG[12] |= 0x08000000;    // Set invert_clkout = 1
	
	boot_cfg_regs->DDR3_CONFIG_REG[23] |= 0x00000200; //Set bit 9 = 1 to use forced ratio leveling for read DQS
	//boot_cfg_regs->DDR3_CONFIG_REG[23] &= ~(0x00000200); //Clear bit 9 = 1 to use full auto leveling + incremental leveling

	/*the PHY_RESET is pulsed (0 -> 1 -> 0) to latch these 
	leveling configuration values into the PHY logic.*/
	DDR_Regs->DDR_PHY_CTRL_1 &= ~(0x00008000);
	DDR_Regs->DDR_PHY_CTRL_1 |= (0x00008000);
	DDR_Regs->DDR_PHY_CTRL_1 &= ~(0x00008000);


	/*Drives CKE low.
	This is a JEDEC requirement that we have 500us delay between reset de-assert 
	and cke assert and then program the correct refresh rate
	The DDR internal clock is divide by 16 before SDCFG write*/
	DDR_Regs->SDRAM_REF_CTRL = 0x80000000|(unsigned int)(500.f*clock_MHz/16.f);

	DDR_Regs->SDRAM_TIM_1 =
		((unsigned int)(13.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RP_SHIFT)|
		((unsigned int)(13.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RCD_SHIFT)|
		((unsigned int)(15*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_WR_SHIFT)|
		((unsigned int)(36*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RAS_SHIFT)|
		((unsigned int)(49.5*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RC_SHIFT)|
		((unsigned int)(45*clock_MHz/4000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_RRD_SHIFT)| 	/*T_RRD = (tFAW/(4*tCK)) 每 1*/
		(_max2(4-1, (unsigned int)(7.5*clock_MHz/1000.f-0.0001f))<<CSL_EMIF4F_SDRAM_TIM_1_REG_T_WTR_SHIFT);
	DDR_Regs->SDRAM_TIM_2   = 
		(_max2(3-1, (unsigned int)(6*clock_MHz/1000.f-0.0001f))<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XP_SHIFT)|
		(_max2(5-1, (unsigned int)((160+10)*clock_MHz/1000.f-0.0001f))<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSNR_SHIFT)| 	/*T_XSNR = (tXS /tCK)每 1*/
		((512-1)<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSRD_SHIFT)| 	/*T_XSRD =tXSDLL每 1*/
		(_max2(4-1, (unsigned int)(7.5*clock_MHz/1000.f-0.0001f))<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_RTP_SHIFT)|
		(_max2(3-1, (unsigned int)(5.625*clock_MHz/1000.f-0.0001f))<<CSL_EMIF4F_SDRAM_TIM_2_REG_T_CKE_SHIFT);
	DDR_Regs->SDRAM_TIM_3   = 
		(5<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_PDLL_UL_SHIFT)| 	/*This field must always be programmed to 0x5.*/
		((5)<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_CSTA_SHIFT)| 	/*This field should be set according to PHY requirements as 0x5.*/
		(_max2(3, (unsigned int)(5.625*clock_MHz/1000.f+0.9999f))<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_CKESR_SHIFT)|
		((64-1)<<CSL_EMIF4F_SDRAM_TIM_3_REG_ZQ_ZQCS_SHIFT)|
		((unsigned int)(160*clock_MHz/1000.f-0.0001f)<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_RFC_SHIFT)|
		(15<<CSL_EMIF4F_SDRAM_TIM_3_REG_T_RAS_MAX_SHIFT); 	/*This field must always be programmed to 0xF.*/

	DDR_Regs->DDR_PHY_CTRL_1  = 0x00100100|
		(12<<CSL_EMIF4F_DDR_PHY_CTRL_1_REG_READ_LATENCY_SHIFT); 	/*between CAS Latency + 1 and CAS Latency + 7*/

	DDR_Regs->ZQ_CONFIG = 
		((0)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_CS1EN_SHIFT)|
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_CS0EN_SHIFT)|
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_DUALCALEN_SHIFT)| 	/*This bit should always be set to 1.*/
		((1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_SFEXITEN_SHIFT)|
		((512/256-1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_ZQINIT_MULT_SHIFT)| 	/*T_ZQ_ZQINIT_MULT = (tZQinit/tZQoper 每 1)*/
		((256/64-1)<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_ZQCL_MULT_SHIFT)| 	/*T_ZQ_ZQCL_MULT = (tZQoper/tZQCS 每 1)*/
		/*interval between ZQCS commands = ZQCorrection/((TSens x Tdriftrate) + (VSens x Vdriftrate))
		= 0.5/((1.5*1.2)+(0.15*10))= 0.151515151s, this time need be converted to refresh period number*/
		((151515151/(64000000/8192))<<CSL_EMIF4F_ZQ_CONFIG_REG_ZQ_REFINTERVAL_SHIFT);

	/*map priority 0,1,2,3 to COS0,
	map priority 3,5,6,7 to COS1*/
	DDR_Regs->PRI_COS_MAP = 
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_COS_MAP_EN_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_7_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_6_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_5_COS_SHIFT)|
		((1)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_4_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_3_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_2_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_1_COS_SHIFT)|
		((0)<<CSL_EMIF4F_PRI_COS_MAP_REG_PRI_0_COS_SHIFT);

	/*master based COS map is disabled*/
	DDR_Regs->MSTID_COS_1_MAP= 0;
	DDR_Regs->MSTID_COS_2_MAP= 0;

	/*LAT_CONFIG*/
	DDR_Regs->VBUSM_CONFIG= 
		(8<<CSL_EMIF4F_VBUSM_CONFIG_REG_COS_COUNT_1_SHIFT)|
		(16<<CSL_EMIF4F_VBUSM_CONFIG_REG_COS_COUNT_2_SHIFT)|
		(32<<CSL_EMIF4F_VBUSM_CONFIG_REG_PR_OLD_COUNT_SHIFT);

	DDR_Regs->ECC_CTRL = 
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_EN_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_PROT_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_2_EN_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_CTRL_REG_ECC_ADDR_RNG_1_EN_SHIFT);

	DDR_Regs->ECC_ADDR_RNG_1= 
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_1_REG_ECC_STRT_ADDR_1_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_1_REG_ECC_END_ADDR_1_SHIFT);

	DDR_Regs->ECC_ADDR_RNG_2= 
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_2_REG_ECC_STRT_ADDR_2_SHIFT)|
		((0)<<CSL_EMIF4F_ECC_ADDR_RNG_2_REG_ECC_END_ADDR_2_SHIFT);

	/* enables DRAM configuration.  It still has the refresh interval 
	programmed to the longer number needed during DRAM initialization.*/
	DDR_Regs->SDRAM_REF_CTRL = (unsigned int)(500.f*clock_MHz/16.f); 

	DDR_Regs->SDRAM_CONFIG = 
		(3<<CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_TYPE_SHIFT)| 	/*Set to 3 for DDR3. All other values reserved.*/
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_POS_SHIFT)|
		(DDR_TERM_RZQ_OVER_6<<CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_TERM_SHIFT)|
		(DDR_DYN_ODT_DISABLED<<CSL_EMIF4F_SDRAM_CONFIG_REG_DYN_ODT_SHIFT)|
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_DISABLE_DLL_SHIFT)|
		(SDRAM_DRIVE_RZQ_OVER_7<<CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_DRIVE_SHIFT)|
		(DDR_CWL_7<<CSL_EMIF4F_SDRAM_CONFIG_REG_CWL_SHIFT)|
		(DDR_BUS_WIDTH_64<<CSL_EMIF4F_SDRAM_CONFIG_REG_NARROW_MODE_SHIFT)|
		(DDR_CL_9<<CSL_EMIF4F_SDRAM_CONFIG_REG_CL_SHIFT)|
		(DDR_ROW_SIZE_14_BIT<<CSL_EMIF4F_SDRAM_CONFIG_REG_ROWSIZE_SHIFT)|
		(DDR_BANK_NUM_8<<CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_SHIFT)|
		(0<<CSL_EMIF4F_SDRAM_CONFIG_REG_EBANK_SHIFT)|
		(DDR_PAGE_SIZE_10_BIT_1024_WORD<<CSL_EMIF4F_SDRAM_CONFIG_REG_PAGESIZE_SHIFT);

	for(i=0;i<100000;i++)
		asm(" nop"); 	//Wait 600us for HW init to complete

//	DDR_Regs->SDRAM_REF_CTRL    = 64000000/8192/(1000/clock_MHz);
	DDR_Regs->SDRAM_REF_CTRL    = (unsigned int)64000.f*clock_MHz/8192.f;

#if 1
	//enable full leveling, no incremental leveling
	DDR_Regs->RDWR_LVL_RMP_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVL_EN_SHIFT);
	DDR_Regs->RDWR_LVL_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLFULL_START_SHIFT);

	for(i=0;i<1000000;i++)
		asm(" nop"); 	//Wait 3ms for leveling to complete
#else
	//enable full leveling and incremental leveling
	/*Typically program a higher rate of incremental leveling in the ramp window*/
	DDR_Regs->RDWR_LVL_RMP_WIN = 10000/7.8; 	/*10000us*/
	DDR_Regs->RDWR_LVL_RMP_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVL_EN_SHIFT)|
		(0<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVLINC_RMP_PRE_SHIFT)|
		(3<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDLVLINC_RMP_INT_SHIFT)|
		(3<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDLVLGATEINC_RMP_INT_SHIFT)|
		(3<<CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_WRLVLINC_RMP_INT_SHIFT);
	DDR_Regs->RDWR_LVL_CTRL = 
		(1<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLFULL_START_SHIFT)|
		(0x7F<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLINC_PRE_SHIFT)|
		(9<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDLVLINC_INT_SHIFT)|
		(9<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDLVLGATEINC_INT_SHIFT)|
		(9<<CSL_EMIF4F_RDWR_LVL_CTRL_REG_WRLVLINC_INT_SHIFT);

	/*Incremental leveling of at least the read eye sample point must be 
	executed at least 64 times after full automatic leveling to converge 
	it to an initial optimum value.*/
	for(i=0;i<100000000;i++)
		asm(" nop"); 	//Wait 640ms for leveling to complete
#endif
	if(DDR_Regs->STATUS&(CSL_EMIF4F_STATUS_REG_RDLVLGATETO_MASK
		|CSL_EMIF4F_STATUS_REG_RDLVLTO_MASK
		|CSL_EMIF4F_STATUS_REG_WRLVLTO_MASK))
	{
		printf("DDR3 leveling has failed, STATUS = 0x%x", DDR_Regs->STATUS);
		return;
	}
}

