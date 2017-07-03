



#ifndef _TURBONYQUIST_Atrenta_DSP1_H_
#define _TURBONYQUIST_Atrenta_DSP1_H_



#include "cslr.h"



#define CSL_INTERNAL_RAM_PER_CNT          1
#define CSL_CGEM_0_5_LOCAL_REGISTERS_PER_CNT 1
#define CSL_CP_TRACER_PER_CNT             16
#define CSL_AIF2_PER_CNT                  1
#define CSL_PA_SS_PER_CNT                 1
#define CSL_RAC_PER_CNT                   2
#define CSL_TAC_PER_CNT                   1
#define CSL_TCP3D_PER_CNT                 3
#define CSL_VCP2_PER_CNT                  4
#define CSL_TCP3E_PER_CNT                 1
#define CSL_FFTC_PER_CNT                  3
#define CSL_TIMER_PER_CNT                 8
#define CSL_PLL_CONTROLLER_PER_CNT        1
#define CSL_GPIO_PER_CNT                  1
#define CSL_SMARTREFLEX_PER_CNT           1
#define CSL_PSC_PER_CNT                   1
#define CSL_MPU_PER_CNT                   6
#define CSL_DEBUG_SS_PER_CNT              1
#define CSL_GEM_DEBUG_PER_CNT             1
#define CSL_SEC_CTL_PER_CNT               1
#define CSL_SEC_KEY_MGR_PER_CNT           1
#define CSL_I2C_PER_CNT                   1
#define CSL_UART_PER_CNT                  1
#define CSL_CP_INTC_PER_CNT               3
#define CSL_BOOT_CFG_PER_CNT              1
#define CSL_SEMAPHORE_PER_CNT             1
#define CSL_TPCC_PER_CNT                  3
#define CSL_TPTC_PER_CNT                  10
#define CSL_SRIO_PER_CNT                  1
#define CSL_QM_SS_PER_CNT                 1
#define CSL_MSMC_PER_CNT                  1
#define CSL_DSP0_GLOBAL_PER_CNT           1
#define CSL_DSP1_GLOBAL_PER_CNT           1
#define CSL_DSP2_GLOBAL_PER_CNT           1
#define CSL_DSP3_GLOBAL_PER_CNT           1
#define CSL_BOOT_PER_CNT                  1
#define CSL_SPI_PER_CNT                   1
#define CSL_DDR3EMIF_PER_CNT              1
#define CSL_MCM_PER_CNT                   1
#define CSL_PCIE_PER_CNT                  1
#define CSL_BCP_PER_CNT                   1

/**************************************************************************\
* Peripheral Instance definitions.
\**************************************************************************/

/** @brief Instance number of internal RAM */
#define CSL_INTERNAL_RAM                      (0) 

/** @brief Instance number of CGEM_0_5_LOCAL_REGISTERS */
#define CSL_CGEM_0_5_LOCAL_REGISTERS                      (0)

/** @brief Peripheral Instance numbers of CP_TRACER */
#define CSL_CP_TRACER_0                           (0) 
#define CSL_CP_TRACER_1                           (1) 
#define CSL_CP_TRACER_2                           (2) 
#define CSL_CP_TRACER_3                           (3) 
#define CSL_CP_TRACER_4                           (4) 
#define CSL_CP_TRACER_5                           (5) 
#define CSL_CP_TRACER_6                           (6) 
#define CSL_CP_TRACER_7                           (7) 
#define CSL_CP_TRACER_8                           (8) 
#define CSL_CP_TRACER_9                           (9) 
#define CSL_CP_TRACER_10                          (10) 
#define CSL_CP_TRACER_11                          (11) 
#define CSL_CP_TRACER_12                          (12) 
#define CSL_CP_TRACER_13                          (13) 
#define CSL_CP_TRACER_14                          (14) 
#define CSL_CP_TRACER_15                          (15) 

/** @brief Peripheral Instance for AIF */
#define CSL_AIF                             (0)

/** @brief Peripheral Instance for PA_SS */
#define  CSL_PA_SS                           (0) 

/** @brief Peripheral Instance for RAC */
#define  CSL_RAC_A                           (0) 
#define  CSL_RAC_B                           (1) 

/** @brief Peripheral Instance for TAC */
#define  CSL_TAC                           (0) 

/** @brief Peripheral Instance of TCP3D instances */
#define CSL_TCP3D_A                          (0)
#define CSL_TCP3D_B                          (1)
#define CSL_TCP3D_C                          (2)

/** @brief Peripheral Instance for VCP2 */
#define CSL_VCP2_A                            (0) 
#define CSL_VCP2_B                            (1) 
#define CSL_VCP2_C                            (2) 
#define CSL_VCP2_D                            (3) 

/** @brief Peripheral Instance of TCP3E */
#define CSL_TCP3E                          (0)

/** @brief Peripheral Instance of FFTC */
#define CSL_FFTC_A                          (0)
#define CSL_FFTC_B                          (1)
#define CSL_FFTC_C                          (2)

/** @brief Peripheral Instance of Timer 64p */
#define CSL_TMR_0                           (0)
#define CSL_TMR_1                           (1)
#define CSL_TMR_2                           (2)
#define CSL_TMR_3                           (3)
#define CSL_TMR_4                           (4)
#define CSL_TMR_5                           (5)
#define CSL_TMR_6                           (6)
#define CSL_TMR_7                           (7)

/** @brief Instance number of PLL controller*/
#define CSL_PLLC                          (0) 

/** @brief Peripheral Instance for GPIO */
#define CSL_GPIO				            (0)

/** @brief Peripheral Instance for SMARTREFLEX */
#define CSL_SMARTREFLEX				            (0)

/** @brief Instance number of PSC controller */
#define CSL_PSC                             (0)

/** @brief Instance number of MPU */
#define CSL_MPU_0                             (0)
#define CSL_MPU_1                             (1)
#define CSL_MPU_2                             (2)
#define CSL_MPU_3                             (3)
#define CSL_MPU_4                             (4)
#define CSL_MPU_5                             (5)

/** @brief Instance number of DEBUG_SS */
#define CSL_DEBUG_SS                             (0)

/** @brief Instance number of GEM_DEBUG */
#define CSL_GEM_DEBUG                             (0)

/** @brief Instance number of SEC_CTL */
#define CSL_SEC_CTL                            (0)

/** @brief Instance number of SEC_KEY_MGR */
#define CSL_SEC_KEY_MGR                             (0)

/** @brief Peripheral Instance for I2C */
#define CSL_I2C                             (0) 

/** @brief Peripheral Instance of UART */
#define CSL_UART                            (0)

/** @brief Peripheral Instance numbers of CP_INTC */
#define CSL_CP_INTC_0                           (0) 
#define CSL_CP_INTC_1                           (1) 
#define CSL_CP_INTC_2                           (2) 

/** @brief Peripheral Instance of BOOT_CFG */
#define CSL_BOOT_CFG                            (0)

/** @brief Peripheral Instance of SEMAPHORE */
#define CSL_SEMAPHORE                           (0)

/** @brief Peripheral Instance of TPCC instances */
#define CSL_TPCC_0                           (0)
#define CSL_TPCC_1                           (1)
#define CSL_TPCC_2                           (2)

/** @brief Peripheral Instance of TPTC instances */
#define CSL_TPTC_0                           (0)
#define CSL_TPTC_1                           (1)
#define CSL_TPTC_2                           (2)
#define CSL_TPTC_3                           (3)
#define CSL_TPTC_4                           (4)
#define CSL_TPTC_5                           (5)
#define CSL_TPTC_6                           (6)
#define CSL_TPTC_7                           (7)
#define CSL_TPTC_8                           (8)
#define CSL_TPTC_9                           (9)

/** @brief Peripheral Instance of SRIO */
#define CSL_SRIO                           (0)

/** @brief Peripheral Instance of QM_SS */
#define CSL_QM_SS                           (0)

/** @brief Peripheral Instance of MSMC */
#define CSL_MSMC                           (0)

/** @brief Peripheral Instance of DSP0_GLOBAL */
#define CSL_DSP0_GLOBAL                          (0)

/** @brief Peripheral Instance of DSP1_GLOBAL */
#define CSL_DSP1_GLOBAL                          (0)

/** @brief Peripheral Instance of DSP2_GLOBAL */
#define CSL_DSP2_GLOBAL                          (0)

/** @brief Peripheral Instance of DSP3_GLOBAL */
#define CSL_DSP3_GLOBAL                          (0)

/** @brief Peripheral Instance of BOOT */
#define CSL_BOOT                          (0)

/** @brief Peripheral Instance of SPI */
#define CSL_SPI                          (0)

/** @brief Peripheral Instance for DDR3 */
#define CSL_DDR3                            (0)

/** @brief Instance number of MCM */
#define CSL_MCM                             (0)

/** @brief Peripheral Instance for PCIE_SS */
#define CSL_PCIE_SS		                (0)

/** @brief Instance number of BCP */
#define CSL_BCP                             (0)

/** @brief Instance number of device configuration module */
#define CSL_DEV                             (0) 

/** @TPCC0 Configuration*/

#define CSL_EDMA3_TPCC0_NUM_DMACH 16
#define CSL_EDMA3_TPCC0_NUM_QDMACH 8
#define CSL_EDMA3_TPCC0_NUM_PARAMSETS 128
#define CSL_EDMA3_TPCC0_NUM_INTCH 16
#define CSL_EDMA3_TPCC0_NUM_EVQUE 2
#define CSL_EDMA3_TPCC0_NUM_TC 2
#define CSL_EDMA3_TPCC0_CHMAPEXIST 1
#define CSL_EDMA3_TPCC0_NUM_REGIONS 8
#define CSL_EDMA3_TPCC0_MEMPROTECT 1

/** @TPCC1 Configuration*/

#define CSL_EDMA3_TPCC1_NUM_DMACH 64
#define CSL_EDMA3_TPCC1_NUM_QDMACH 8
#define CSL_EDMA3_TPCC1_NUM_PARAMSETS 512
#define CSL_EDMA3_TPCC1_NUM_INTCH 64
#define CSL_EDMA3_TPCC1_NUM_EVQUE 4
#define CSL_EDMA3_TPCC1_NUM_TC 4
#define CSL_EDMA3_TPCC1_CHMAPEXIST 1
#define CSL_EDMA3_TPCC1_NUM_REGIONS 8
#define CSL_EDMA3_TPCC1_MEMPROTECT 1

/** @TPCC2 Configuration*/

#define CSL_EDMA3_TPCC2_NUM_DMACH 64
#define CSL_EDMA3_TPCC2_NUM_QDMACH 8
#define CSL_EDMA3_TPCC2_NUM_PARAMSETS 512
#define CSL_EDMA3_TPCC2_NUM_INTCH 64
#define CSL_EDMA3_TPCC2_NUM_EVQUE 4
#define CSL_EDMA3_TPCC2_NUM_TC 4
#define CSL_EDMA3_TPCC2_CHMAPEXIST 1
#define CSL_EDMA3_TPCC2_NUM_REGIONS 8
#define CSL_EDMA3_TPCC2_MEMPROTECT 1

/* PSC Definitions */
#define CSL_PSC_NUM_PD 19
#define CSL_PSC_NUM_LPSC 31
/* PSC Power Domain Assignment Definitions */
#define CSL_PSC_PD_ALWAYSON 0
#define CSL_PSC_PD_DEBUG_TRC 1
#define CSL_PSC_PD_PASS 2
#define CSL_PSC_PD_PCIEX 3
#define CSL_PSC_PD_SRIO 4
#define CSL_PSC_PD_HYPERBRIDGE 5
#define CSL_PSC_PD_L2SRAM 6
#define CSL_PSC_PD_MSMCSRAM 7
#define CSL_PSC_PD_TAC_RAC 8
#define CSL_PSC_PD_FFTC_AB 9
#define CSL_PSC_PD_AI 10
#define CSL_PSC_PD_TCP3D_A 11
#define CSL_PSC_PD_PD_VCP_BCD 12
#define CSL_PSC_PD_GEM0 13
#define CSL_PSC_PD_GEM1 14
#define CSL_PSC_PD_GEM2 15
#define CSL_PSC_PD_GEM3 16
#define CSL_PSC_PD_TCP3D_B 17
#define CSL_PSC_PD_BCP_FFTC_C_TCP3D_C 18

/* PSC LPSC Module Assignment Definitions */
#define CSL_PSC_LPSC_MODRST0 0
#define CSL_PSC_LPSC_MODSRC3_PWR 1
#define CSL_PSC_LPSC_EMIF4F 2
#define CSL_PSC_LPSC_TCP3E 3
#define CSL_PSC_LPSC_VCP2_A 4
#define CSL_PSC_LPSC_DEBUGSS_TRC 5
#define CSL_PSC_LPSC_TETB_TRC 6
#define CSL_PSC_LPSC_PKTPROC 7
#define CSL_PSC_LPSC_CPGMAC 8
#define CSL_PSC_LPSC_Crypto  9
#define CSL_PSC_LPSC_PCIEX 10
#define CSL_PSC_LPSC_SRIO 11
#define CSL_PSC_LPSC_HYPERBRIDGE 12
#define CSL_PSC_LPSC_L2SRAM 13
#define CSL_PSC_LPSC_MSMCSRAM 14
#define CSL_PSC_LPSC_RAC 15 
#define CSL_PSC_LPSC_TAC 16
#define CSL_PSC_LPSC_FFTC_AB 17
#define CSL_PSC_LPSC_AI 18
#define CSL_PSC_LPSC_TCP3D_A 19
#define CSL_PSC_LPSC_VCP2_B  20
#define CSL_PSC_LPSC_VCP2_C 21
#define CSL_PSC_LPSC_VCP2_D 22
#define CSL_PSC_LPSC_GEM0  23
#define CSL_PSC_LPSC_GEM1 24
#define CSL_PSC_LPSC_RSAx2_1 25
#define CSL_PSC_LPSC_GEM2 26
#define CSL_PSC_LPSC_RSAx2_2 27
#define CSL_PSC_LPSC_GEM3 28
#define CSL_PSC_LPSC_TCP3D_B  29 
#define CSL_PSC_LPSC_BCP_FFTC_C_TCP3D_C 30

/**************************************************************************
* Peripheral Base Address-0.1.6
**************************************************************************/
#define CSL_CGEM0_5_LOCAL_L2_SRAM_REGS    (0x00800000)
#define CSL_CGEM0_5_LOCAL_L1P_SRAM_REGS  (0x00e00000)
#define CSL_CGEM0_5_L1D_SRAM_REGS  (0x00f00000)
#define CSL_CGEM0_5_REG_BASE_ADDRESS_REGS  (0x01800000)
#define CSL_CGEM0_5_POWER_DOWN_CONTROL_REGS  (0x01810000)
#define CSL_CGEM0_5_SECURITY_ID_REGS  (0x01811000)
#define CSL_CGEM0_5_REVISION_ID_REGS  (0x01812000)
#define CSL_CGEM0_5_L1_L2_CONTROL_REGS  (0x01840000)
#define CSL_CGEM0_5_EMULATION_REGS  (0x01bc0000)
#define CSL_TRACER_ICPM_MSMC0_MST_SLV_0_REGS  (0x01d00000)
#define CSL_TRACER_ICPM_MSMC1_MST_SLV_1_REGS  (0x01d08000)
#define CSL_TRACER_ICPM_MSMC2_MST_SLV_2_REGS  (0x01d10000)
#define CSL_TRACER_ICPM_MSMC3_MST_SLV_3_REGS  (0x01d18000)
#define CSL_TRACER_ICPM_QM_VBUSM_MST_SLV_4_REGS  (0x01d20000)
#define CSL_TRACER_ICPM_DDR_MST_SLV_5_REGS  (0x01d28000)
#define CSL_TRACER_ICPM_SM_MST_SLV_6_REGS  (0x01d30000)
#define CSL_TRACER_ICPM_QM_VBUSP_MST_SLV_7_REGS  (0x01d38000)
#define CSL_TRACER_ICPM_CFG_MST_SLV_8_REGS  (0x01d40000)
#define CSL_TRACER_ICPM_L2_0_MST_SLV_9_REGS  (0x01d48000)
#define CSL_TRACER_ICPM_L2_1_MST_SLV_10_REGS  (0x01d50000)
#define CSL_TRACER_ICPM_L2_2_MST_SLV_11_REGS  (0x01d58000)
#define CSL_TRACER_ICPM_L2_3_MST_SLV_12_REGS  (0x01d60000)
#define CSL_TRACER_ICPM_RAC_FEI_MST_SLV_13_REGS  (0x01d68000)
#define CSL_TRACER_ICPM_RAC_CFG_MST_SLV_14_REGS  (0x01d70000)
#define CSL_TRACER_ICPM_TAC_MST_SLV_15_REGS  (0x01d78000)
#define CSL_AIF2_CONTROL_REGS   (0x01f00000)
#define CSL_AIF2_CFG_CPPI_DMA_GLOBAL_CFG_REGS (0x01f14000)
#define CSL_AIF2_CFG_CPPI_DMA_TX_CFG_REGS (0x01f16000)
#define CSL_AIF2_CFG_CPPI_DMA_RX_CFG_REGS (0x01f18000)
#define CSL_AIF2_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x01f17000)
#define CSL_AIF2_CFG_CPPI_DMA_RX_FLOW_CFG_REGS (0x01f1a000)
#define CSL_PA_SS_CFG_REGS     (0x02000000)
#define CSL_PA_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS (0x02004000)
#define CSL_PA_SS_CFG_CPPI_DMA_TX_CFG_REGS (0x02004400)
#define CSL_PA_SS_CFG_CPPI_DMA_RX_CFG_REGS (0x02004800)
#define CSL_PA_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x02004c00)
#define CSL_PA_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS (0x02005000)
#define CSL_PA_SS_CFG_CP_ACE_CFG_REGS (0x020C0000)
#define CSL_RAC_A_FEI_CONTROL_REGS (0x02100000)
#define CSL_RAC_A_BEI_CONTROL_REGS (0x02110000)
#define CSL_RAC_A_GCCP_0_CONTROL_REGS (0x02120000)
#define CSL_RAC_A_GCCP_1_CONTROL_REGS (0x02140000)
#define CSL_RAC_A_DATA_FEI_REGS (0x33200000)
#define CSL_RAC_B_FEI_CONTROL_REGS (0x01f80000)
#define CSL_RAC_B_BEI_CONTROL_REGS (0x01f90000)
#define CSL_RAC_B_GCCP_0_CONTROL_REGS (0x01fa0000)
#define CSL_RAC_B_GCCP_1_CONTROL_REGS (0x01fc0000)
#define CSL_RAC_B_DATA_FEI_REGS (0x20200000)
#define CSL_TAC_FEI_CONTROL_REGS  (0x02180000)
#define CSL_TAC_BEI_CONTROL_REGS  (0x02188000)
#define CSL_TAC_SGCCP_0_CONTROL_REGS  (0x02190000)
#define CSL_TAC_SGCCP_1_CONTROL_REGS  (0x021a0000)
#define CSL_TAC_DATA_BEI_REGS    (0x34c00000)
#define CSL_TCP3D_A_CFG_REGS       (0x021c0000)
#define CSL_TCP3D_A_DATA_REGS      (0x20800000)
#define CSL_TCP3D_B_CFG_REGS       (0x021c8000)
#define CSL_TCP3D_B_DATA_REGS      (0x20600000)
#define CSL_VCP2_A_CFG_REGS      (0x021d0000)
#define CSL_VCP2_B_CFG_REGS      (0x021d4000)
#define CSL_VCP2_C_CFG_REGS      (0x021d8000)
#define CSL_VCP2_D_CFG_REGS      (0x021dc000)
#define CSL_VCP2_A_DATA_REGS     (0x22a00000)
#define CSL_VCP2_B_DATA_REGS     (0x22b00000)
#define CSL_VCP2_C_DATA_REGS     (0x22c00000)
#define CSL_VCP2_D_DATA_REGS     (0x22d00000)
#define CSL_TCP3E_CFG_REGS       (0x021e0000)
#define CSL_TCP3E_DATA_WRITE_PORT_REGS  (0x20900000)
#define CSL_TCP3E_DATA_READ_PORT_REGS  (0x20902000)
#define CSL_FFTC_A_CONFIG_REGS    (0x021f0000)
#define CSL_FFTC_A_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS (0x021f0200)
#define CSL_FFTC_A_CONFIG_CPPI_DMA_TX_CFG_REGS (0x021f0400)
#define CSL_FFTC_A_CONFIG_CPPI_DMA_RX_CFG_REGS (0x021f0500)
#define CSL_FFTC_A_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x021f0300)
#define CSL_FFTC_A_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS (0x021f0600)
#define CSL_FFTC_B_CONFIG_REGS    (0x021f4000)
#define CSL_FFTC_B_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS (0x021f4200)
#define CSL_FFTC_B_CONFIG_CPPI_DMA_TX_CFG_REGS (0x021f4400)
#define CSL_FFTC_B_CONFIG_CPPI_DMA_RX_CFG_REGS (0x021f4500)
#define CSL_FFTC_B_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x021f4300)
#define CSL_FFTC_B_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS (0x021f4600)
#define CSL_TIMER_0_REGS         (0x02200000)
#define CSL_TIMER_1_REGS         (0x02210000)
#define CSL_TIMER_2_REGS         (0x02220000)
#define CSL_TIMER_3_REGS         (0x02230000)
#define CSL_TIMER_4_REGS         (0x02240000)
#define CSL_TIMER_5_REGS         (0x02250000)
#define CSL_TIMER_6_REGS         (0x02260000)
#define CSL_TIMER_7_REGS         (0x02270000)
#define CSL_PLL_CONTROLLER_REGS  (0x02310000)
#define CSL_GPIO_REGS            (0x02320000)
#define CSL_PID_PLUS_RESERVED_REGS (0x02330000)
#define CSL_SR_VPRM_REGS        (0x02330080)
#define CSL_SMARTREFLEX_SENSOR0_REGS (0x02330100)
#define CSL_SMARTREFLEX_SENSOR1_REGS (0x02330180)
#define CSL_SMARTREFLEX_SENSOR2_REGS (0x02330200)
#define CSL_SMARTREFLEX_SENSOR3_REGS (0x02330280)
#define CSL_SR_VP_REGS          (0x02330300)
#define CSL_SR_VOLCON_REGS      (0x02330380)
#define CSL_PSC_REGS             (0x02350000)
#define CSL_MPU_0_REGS           (0x02360000)
#define CSL_MPU_1_REGS           (0x02368000)
#define CSL_MPU_2_REGS           (0x02370000)
#define CSL_MPU_3_REGS           (0x02378000)
#define CSL_MPU_4_REGS           (0x02380000)
#define CSL_DEBUG_SS_CONFIG_REGS  (0x02400000)
#define CSL_TETB8_SYSTEM_TRACE_REGS  (0x02850000)
#define CSL_STM_REGS             (0x20000000)
#define CSL_DEBUG_ADTF_0_REGS    (0x02440000)
#define CSL_DEBUG_ADTF_1_REGS    (0x02450000)
#define CSL_DEBUG_ADTF_2_REGS    (0x02460000)
#define CSL_DEBUG_ADTF_3_REGS    (0x02470000)
#define CSL_TETB_0_REGS          (0x027d0000)
#define CSL_TETB_1_REGS          (0x027e0000)
#define CSL_TETB_2_REGS          (0x027f0000)
#define CSL_TETB_3_REGS          (0x02800000)
#define CSL_SEC_CTL_REGS         (0x02500000)
#define CSL_SEC_KEY_MGR_REGS     (0x02520000)
#define CSL_I2C_DATA_CONTROL_REGS  (0x02530000)
#define CSL_UART_REGS            (0x02540000)
#define CSL_CP_INTC_0_REGS       (0x02600000)
#define CSL_CP_INTC_1_REGS       (0x02604000)
#define CSL_CP_INTC_2_REGS       (0x02608000)
#define CSL_BOOT_CFG_REGS        (0x02620000)
#define CSL_SEMAPHORE_REGS       (0x02640000)
#define CSL_EDMA0CC_REGS   (0x02700000)
#define CSL_EDMA1CC_REGS   (0x02720000)
#define CSL_EDMA2CC_REGS   (0x02740000)
#define CSL_EDMA0TC0_REGS  (0x02760000)
#define CSL_EDMA0TC1_REGS  (0x02768000)
#define CSL_EDMA1TC0_REGS  (0x02770000)
#define CSL_EDMA1TC1_REGS  (0x02778000)
#define CSL_EDMA1TC2_REGS  (0x02780000)
#define CSL_EDMA1TC3_REGS  (0x02788000)
#define CSL_EDMA2TC0_REGS  (0x02790000)
#define CSL_EDMA2TC1_REGS  (0x02798000)
#define CSL_EDMA2TC2_REGS  (0x027a0000)
#define CSL_EDMA2TC3_REGS  (0x027a8000)
#define CSL_SRIO_CONFIG_REGS   (0x02900000)
#define CSL_SRIO_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS (0x02901000)
#define CSL_SRIO_CONFIG_CPPI_DMA_TX_CFG_REGS (0x02901400)
#define CSL_SRIO_CONFIG_CPPI_DMA_RX_CFG_REGS (0x02901800)
#define CSL_SRIO_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x02901c00)
#define CSL_SRIO_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS (0x02902000)
#define CSL_QM_SS_CFG_QUE_PEEK_REGS (0x02a00000)
#define CSL_QM_SS_CFG_QM_QUEUE_DEQUEUE_REGS (0x02a20000)
#define CSL_QM_SS_CFG_PROXY_QUEUE_DEQUEUE_REGS (0x02a40000)
#define CSL_QM_SS_CFG_APDSP1_RAM_REGS (0x02a60000)
#define CSL_QM_SS_CFG_APDSP2_RAM_REGS (0x02a61000)
#define CSL_QM_SS_CFG_QM_STATUS_RAM_REGS (0x02a62000)
#define CSL_QM_SS_CFG_CONFIG_STARVATION_COUNTER_REGS (0x02a68000)
#define CSL_QM_SS_CFG_DESCRIPTION_REGS (0x02a6a000)
#define CSL_QM_SS_CFG_PROXY_CONFIG_REGS (0x02a6b000)
#define CSL_QM_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS (0x02a6c000)
#define CSL_QM_SS_CFG_CPPI_DMA_TX_CFG_REGS (0x02a6c400)
#define CSL_QM_SS_CFG_CPPI_DMA_RX_CFG_REGS (0x02a6c800)
#define CSL_QM_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS (0x02a6d000)
#define CSL_QM_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x02a6cc00)
#define CSL_QM_SS_CFG_ADSP1_REGS (0x02a6e000)
#define CSL_QM_SS_CFG_ADSP2_REGS (0x02a6f000)
#define CSL_QM_SS_CFG_LINKING_RAM_REGS (0x02a80000)
#define CSL_QM_SS_CFG_MCDMA_REGS (0x02a90000)
#define CSL_QM_SS_CFG_INTD_REGS (0x02aa0000)
#define CSL_QM_SS_CFG_TIMER1_REGS (0x02aa8000)
#define CSL_QM_SS_CFG_TIMER2_REGS (0x02aa8800)
#define CSL_QM_SS_CFG_SCRACH_RAM1_REGS (0x02ab8000)
#define CSL_QM_SS_CFG_SCRACH_RAM2_REGS (0x02abc000)
#define CSL_QM_SS_DATA_QM_QUEUE_DEQUEUE_REGS (0x34020000)
#define CSL_QM_SS_DATA_PROXY_QUEUE_DEQUEUE_REGS (0x34040000)
#define CSL_QM_SS_DATA_APDSP1_RAM_REGS (0x34060000)
#define CSL_QM_SS_DATA_APDSP2_RAM_REGS (0x34061000)
#define CSL_XMC_CONFIG_REGS      (0x08000000)
#define CSL_MSMC_CONFIG_REGS     (0x0bc00000)
#define CSL_MSMC_SRAM_REGS       (0x0c000000)
#define CSL_DSP0_L2_SRAM_REGS    (0x10800000)
#define CSL_DSP0_L1P_SRAM_REGS   (0x10e00000)
#define CSL_DSP0_L1D_SRAM_REGS   (0x10f00000)
#define CSL_DSP1_L2_SRAM_REGS    (0x11800000)
#define CSL_DSP1_L1P_SRAM_REGS   (0x11e00000)
#define CSL_DSP1_L1D_SRAM_REGS   (0x11f00000)
#define CSL_DSP2_L2_SRAM_REGS    (0x12800000)
#define CSL_DSP2_L1P_SRAM_REGS   (0x12e00000)
#define CSL_DSP2_L1D_SRAM_REGS   (0x12f00000)
#define CSL_DSP3_L2_SRAM_REGS    (0x13800000)
#define CSL_DSP3_L1P_SRAM_REGS   (0x13e00000)
#define CSL_DSP3_L1D_SRAM_REGS   (0x13f00000)
#define CSL_BOOT_ROM_REGS        (0x20b00000)
#define CSL_SPI_REGS             (0x20bf0000)
#define CSL_DDR3_EMIF_CONFIG_REGS  (0x21000000)
#define CSL_DDR3_DATA_0_REGS     (0x80000000)
#define CSL_DDR3_DATA_1_REGS     (0x90000000)
#define CSL_DDR3_DATA_2_REGS     (0xa0000000)
#define CSL_DDR3_DATA_3_REGS     (0xb0000000)
#define CSL_DDR3_DATA_4_REGS     (0xc0000000)
#define CSL_DDR3_DATA_5_REGS     (0xd0000000)
#define CSL_DDR3_DATA_6_REGS     (0xe0000000)
#define CSL_DDR3_DATA_7_REGS     (0xf0000000)
#define CSL_MCM_CONFIG_REGS      (0x21400000)
#define CSL_MCM_REGS             (0x40000000)
#define CSL_PCIE_CONFIG_REGS     (0x21800000)
#define CSL_PCIE_REGS            (0x60000000)
#define CSL_MPU_5_REGS           (0x35000000)
#define CSL_TCP3D_C_CFG_REGS       (0x35020000)
#define CSL_TCP3D_C_DATA_REGS      (0x35600000)
#define CSL_FFTC_C_CONFIG_REGS    (0x35040000)
#define CSL_FFTC_C_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS (0x35040200)
#define CSL_FFTC_C_CONFIG_CPPI_DMA_TX_CFG_REGS (0x35040400)
#define CSL_FFTC_C_CONFIG_CPPI_DMA_RX_CFG_REGS (0x35040500)
#define CSL_FFTC_C_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x35040300)
#define CSL_FFTC_C_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS (0x35040600)
#define CSL_BCP_CONFIG_REGS    (0x35200000)
#define CSL_BCP_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS (0x35214000)
#define CSL_BCP_CONFIG_CPPI_DMA_TX_CFG_REGS (0x35216000)
#define CSL_BCP_CONFIG_CPPI_DMA_RX_CFG_REGS (0x35218000)
#define CSL_BCP_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS (0x3521a000)
#define CSL_BCP_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS (0x3521c000)


/**************************************************************************
* CHIP LEVEL INTC0 EVENT INPUT-0.1.10
**************************************************************************/

#define INTC0_EVENT_CNT                 208


/**************************************************************************
* CHIP LEVEL INTC0 EVENT DEFINITION
**************************************************************************/
#define CSL_INTC0_CPU_3_1_EDMACC_ERRINT (0x00000000)
#define CSL_INTC0_CPU_3_1_EDMACC_MPINT (0x00000001)
#define CSL_INTC0_CPU_3_1_EDMATC_ERRINT0 (0x00000002)
#define CSL_INTC0_CPU_3_1_EDMATC_ERRINT1 (0x00000003)
#define CSL_INTC0_CPU_3_1_EDMATC_ERRINT2 (0x00000004)
#define CSL_INTC0_CPU_3_1_EDMATC_ERRINT3 (0x00000005)
#define CSL_INTC0_CPU_3_1_EDMACC_GINT (0x00000006)
#define CSL_INTC0_RESERVED_1         (0x00000007)
#define CSL_INTC0_CPU_3_1_TPCCINT0   (0x00000008)
#define CSL_INTC0_CPU_3_1_TPCCINT1   (0x00000009)
#define CSL_INTC0_CPU_3_1_TPCCINT2   (0x0000000a)
#define CSL_INTC0_CPU_3_1_TPCCINT3   (0x0000000b)
#define CSL_INTC0_CPU_3_1_TPCCINT4   (0x0000000c)
#define CSL_INTC0_CPU_3_1_TPCCINT5   (0x0000000d)
#define CSL_INTC0_CPU_3_1_TPCCINT6   (0x0000000e)
#define CSL_INTC0_CPU_3_1_TPCCINT7   (0x0000000f)
#define CSL_INTC0_CPU_3_2_EDMACC_ERRINT (0x00000010)
#define CSL_INTC0_CPU_3_2_EDMACC_MPINT (0x00000011)
#define CSL_INTC0_CPU_3_2_EDMATC_ERRINT0 (0x00000012)
#define CSL_INTC0_CPU_3_2_EDMATC_ERRINT1 (0x00000013)
#define CSL_INTC0_CPU_3_2_EDMATC_ERRINT2 (0x00000014)
#define CSL_INTC0_CPU_3_2_EDMATC_ERRINT3 (0x00000015)
#define CSL_INTC0_CPU_3_2_EDMACC_GINT (0x00000016)
#define CSL_INTC0_RESERVED_2         (0x00000017)
#define CSL_INTC0_CPU_3_2_TPCCINT0   (0x00000018)
#define CSL_INTC0_CPU_3_2_TPCCINT1   (0x00000019)
#define CSL_INTC0_CPU_3_2_TPCCINT2   (0x0000001a)
#define CSL_INTC0_CPU_3_2_TPCCINT3   (0x0000001b)
#define CSL_INTC0_CPU_3_2_TPCCINT4   (0x0000001c)
#define CSL_INTC0_CPU_3_2_TPCCINT5   (0x0000001d)
#define CSL_INTC0_CPU_3_2_TPCCINT6   (0x0000001e)
#define CSL_INTC0_CPU_3_2_TPCCINT7   (0x0000001f)
#define CSL_INTC0_CPU_2_EDMACC_ERRINT (0x00000020)
#define CSL_INTC0_CPU_2_EDMACC_MPINT (0x00000021)
#define CSL_INTC0_CPU_2_EDMATC_ERRINT0 (0x00000022)
#define CSL_INTC0_CPU_2_EDMATC_ERRINT1 (0x00000023)
#define CSL_INTC0_CPU_2_EDMACC_GINT  (0x00000024)
#define CSL_INTC0_RESERVED_3         (0x00000025)
#define CSL_INTC0_CPU_2_TPCCINT0     (0x00000026)
#define CSL_INTC0_CPU_2_TPCCINT1     (0x00000027)
#define CSL_INTC0_CPU_2_TPCCINT2     (0x00000028)
#define CSL_INTC0_CPU_2_TPCCINT3     (0x00000029)
#define CSL_INTC0_CPU_2_TPCCINT4     (0x0000002a)
#define CSL_INTC0_CPU_2_TPCCINT5     (0x0000002b)
#define CSL_INTC0_CPU_2_TPCCINT6     (0x0000002c)
#define CSL_INTC0_CPU_2_TPCCINT7     (0x0000002d)
#define CSL_INTC0_RESERVED_4         (0x0000002e)
#define CSL_INTC0_RESERVED_5         (0x0000002f)
#define CSL_INTC0_PCIEXPRESS_ERR_INT (0x00000030)
#define CSL_INTC0_PCIEXPRESS_PM_INT  (0x00000031)
#define CSL_INTC0_PCIEXPRESS_LEGACY_INTA (0x00000032)
#define CSL_INTC0_PCIEXPRESS_LEGACY_INTB (0x00000033)
#define CSL_INTC0_PCIEXPRESS_LEGACY_INTC (0x00000034)
#define CSL_INTC0_PCIEXPRESS_LEGACY_INTD (0x00000035)
#define CSL_INTC0_SPIINT0            (0x00000036)
#define CSL_INTC0_SPIINT1            (0x00000037)
#define CSL_INTC0_SPIXEVT            (0x00000038)
#define CSL_INTC0_SPIREVT            (0x00000039)
#define CSL_INTC0_I2CINT             (0x0000003a)
#define CSL_INTC0_I2CREVT            (0x0000003b)
#define CSL_INTC0_I2CXEVT            (0x0000003c)
#define CSL_INTC0_KEYMGRINT          (0x0000003d)
#define CSL_INTC0_SECCTLINT          (0x0000003e)
#define CSL_INTC0_TETBHFULLINT       (0x0000003f)
#define CSL_INTC0_TETBFULLINT        (0x00000040)
#define CSL_INTC0_TETBACQINT         (0x00000041)
#define CSL_INTC0_TETBOVFLINT        (0x00000042)
#define CSL_INTC0_TETBUNFLINT        (0x00000043)
#define CSL_INTC0_MDIO_LINK_INTR0    (0x00000044)
#define CSL_INTC0_MDIO_LINK_INTR1    (0x00000045)
#define CSL_INTC0_MDIO_USER_INTR0    (0x00000046)
#define CSL_INTC0_MDIO_USER_INTR1    (0x00000047)
#define CSL_INTC0_MISC_INTR          (0x00000048)
#define CSL_INTC0_CP_TRACER_CORE_0_INTD (0x00000049)
#define CSL_INTC0_CP_TRACER_CORE_1_INTD (0x0000004a)
#define CSL_INTC0_CP_TRACER_CORE_2_INTD (0x0000004b)
#define CSL_INTC0_CP_TRACER_CORE_3_INTD (0x0000004c)
#define CSL_INTC0_CP_TRACER_DDR_INTD (0x0000004d)
#define CSL_INTC0_CP_TRACER_MSMC_0_INTD (0x0000004e)
#define CSL_INTC0_CP_TRACER_MSMC_1_INTD (0x0000004f)
#define CSL_INTC0_CP_TRACER_MSMC_2_INTD (0x00000050)
#define CSL_INTC0_CP_TRACER_MSMC_3_INTD (0x00000051)
#define CSL_INTC0_CP_TRACER_CFG_INTD (0x00000052)
#define CSL_INTC0_CP_TRACER_QM_SS_CFG_INTD (0x00000053)
#define CSL_INTC0_CP_TRACER_QM_SS_DMA_INTD (0x00000054)
#define CSL_INTC0_CP_TRACER_SEM_INTD (0x00000055)
#define CSL_INTC0_PSC_ALLINT         (0x00000056)
#define CSL_INTC0_MSMC_SCRUB_CERROR  (0x00000057)
#define CSL_INTC0_BOOTCFG_INTD       (0x00000058)
#define CSL_INTC0_PO_VCON_SMPSERR_INTR (0x00000059)
#define CSL_INTC0_MPU0_INTD          (0x0000005a)
#define CSL_INTC0_BCP_INTD0          (0x0000005b)
#define CSL_INTC0_MPU1_INTD          (0x0000005c)
#define CSL_INTC0_BCP_INTD1          (0x0000005d)
#define CSL_INTC0_MPU2_INTD          (0x0000005e)
#define CSL_INTC0_BCP_INTD2          (0x0000005f)
#define CSL_INTC0_MPU3_INTD          (0x00000060)
#define CSL_INTC0_BCP_INTD3          (0x00000061)
#define CSL_INTC0_MSMC_DEDC_CERROR   (0x00000062)
#define CSL_INTC0_MSMC_DEDC_NC_ERROR (0x00000063)
#define CSL_INTC0_MSMC_SCRUB_NC_ERROR (0x00000064)
#define CSL_INTC0_RESERVED_6         (0x00000065)
#define CSL_INTC0_MSMC_MPF_ERROR8    (0x00000066)
#define CSL_INTC0_MSMC_MPF_ERROR9    (0x00000067)
#define CSL_INTC0_MSMC_MPF_ERROR10   (0x00000068)
#define CSL_INTC0_MSMC_MPF_ERROR11   (0x00000069)
#define CSL_INTC0_MSMC_MPF_ERROR12   (0x0000006a)
#define CSL_INTC0_MSMC_MPF_ERROR13   (0x0000006b)
#define CSL_INTC0_MSMC_MPF_ERROR14   (0x0000006c)
#define CSL_INTC0_MSMC_MPF_ERROR15   (0x0000006d)
#define CSL_INTC0_DDR3_ERR           (0x0000006e)
#define CSL_INTC0_VUSR_INT_O         (0x0000006f)
#define CSL_INTC0_INTDST0            (0x00000070)
#define CSL_INTC0_INTDST1            (0x00000071)
#define CSL_INTC0_INTDST2            (0x00000072)
#define CSL_INTC0_INTDST3            (0x00000073)
#define CSL_INTC0_INTDST4            (0x00000074)
#define CSL_INTC0_INTDST5            (0x00000075)
#define CSL_INTC0_INTDST6            (0x00000076)
#define CSL_INTC0_INTDST7            (0x00000077)
#define CSL_INTC0_INTDST8            (0x00000078)
#define CSL_INTC0_INTDST9            (0x00000079)
#define CSL_INTC0_INTDST10           (0x0000007a)
#define CSL_INTC0_INTDST11           (0x0000007b)
#define CSL_INTC0_INTDST12           (0x0000007c)
#define CSL_INTC0_INTDST13           (0x0000007d)
#define CSL_INTC0_INTDST14           (0x0000007e)
#define CSL_INTC0_INTDST15           (0x0000007f)
#define CSL_INTC0_RAC_A_DEVENT0      (0x00000080)
#define CSL_INTC0_RAC_A_DEVENT1      (0x00000081)
#define CSL_INTC0_TAC_INTD           (0x00000082)
#define CSL_INTC0_TACDEVENT0         (0x00000083)
#define CSL_INTC0_TACDEVENT1         (0x00000084)
#define CSL_INTC0_AIF_INTD           (0x00000085)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_22 (0x00000086)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_23 (0x00000087)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_24 (0x00000088)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_25 (0x00000089)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_26 (0x0000008a)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_27 (0x0000008b)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_28 (0x0000008c)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_29 (0x0000008d)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_30 (0x0000008e)
#define CSL_INTC0_VCP0INT            (0x0000008f)
#define CSL_INTC0_VCP1INT            (0x00000090)
#define CSL_INTC0_VCP2INT            (0x00000091)
#define CSL_INTC0_VCP3INT            (0x00000092)
#define CSL_INTC0_VCP0REVT           (0x00000093)
#define CSL_INTC0_VCP0XEVT           (0x00000094)
#define CSL_INTC0_VCP1REVT           (0x00000095)
#define CSL_INTC0_VCP1XEVT           (0x00000096)
#define CSL_INTC0_VCP2REVT           (0x00000097)
#define CSL_INTC0_VCP2XEVT           (0x00000098)
#define CSL_INTC0_VCP3REVT           (0x00000099)
#define CSL_INTC0_VCP3XEVT           (0x0000009a)
#define CSL_INTC0_TCP3D_A_INTD       (0x0000009b)
#define CSL_INTC0_TCP3D_B_INTD       (0x0000009c)
#define CSL_INTC0_TCP3D_A_REVT0      (0x0000009d)
#define CSL_INTC0_TCP3D_A_REVT1      (0x0000009e)
#define CSL_INTC0_TCP3E_INTD         (0x0000009f)
#define CSL_INTC0_TCP3EREVT          (0x000000a0)
#define CSL_INTC0_TCP3EWEVT          (0x000000a1)
#define CSL_INTC0_TCP3D_B_REVT0      (0x000000a2)
#define CSL_INTC0_TCP3D_B_REVT1      (0x000000a3)
#define CSL_INTC0_UARTINT            (0x000000a4)
#define CSL_INTC0_URXEVT             (0x000000a5)
#define CSL_INTC0_UTXEVT             (0x000000a6)
#define CSL_INTC0_CP_TRACER_RAC_INTD (0x000000a7)
#define CSL_INTC0_CP_TRACER_RAC_FE_INTD (0x000000a8)
#define CSL_INTC0_CP_TRACER_TAC_INTD (0x000000a9)
#define CSL_INTC0_MSMC_MPF_ERROR4    (0x000000aa)
#define CSL_INTC0_MSMC_MPF_ERROR5    (0x000000ab)
#define CSL_INTC0_MSMC_MPF_ERROR6    (0x000000ac)
#define CSL_INTC0_MSMC_MPF_ERROR7    (0x000000ad)
#define CSL_INTC0_MPU4_INTD          (0x000000ae)
#define CSL_INTC0_QM_INT_PASS_TXQ_PEND_31 (0x000000af)
#define CSL_INTC0_QM_INT_CDMA_0      (0x000000b0)
#define CSL_INTC0_QM_INT_CDMA_1      (0x000000b1)
#define CSL_INTC0_RAPIDIO_INT_CDMA_0 (0x000000b2)
#define CSL_INTC0_PASS_INT_CDMA_0    (0x000000b3)
#define CSL_INTC0_TCP3D_C_MPU5_INTD  (0x000000b4)
#define CSL_INTC0_SMARTREFLEX_INTRREQ0 (0x000000b5)
#define CSL_INTC0_SMARTREFLEX_INTRREQ1 (0x000000b6)
#define CSL_INTC0_SMARTREFLEX_INTRREQ2 (0x000000b7)
#define CSL_INTC0_SMARTREFLEX_INTRREQ3 (0x000000b8)
#define CSL_INTC0_VPNOSMPSACK        (0x000000b9)
#define CSL_INTC0_VPEQVALUE          (0x000000ba)
#define CSL_INTC0_VPMAXVDD           (0x000000bb)
#define CSL_INTC0_VPMINVDD           (0x000000bc)
#define CSL_INTC0_VPINIDLE           (0x000000bd)
#define CSL_INTC0_VPOPPCHANGEDONE    (0x000000be)
#define CSL_INTC0_DFT_PBIST_CPU_INT  (0x000000bf)
#define CSL_INTC0_FFTC_A_INTD0       (0x000000c0)
#define CSL_INTC0_FFTC_A_INTD1       (0x000000c1)
#define CSL_INTC0_FFTC_A_INTD2       (0x000000c2)
#define CSL_INTC0_FFTC_A_INTD3       (0x000000c3)
#define CSL_INTC0_FFTC_B_INTD0       (0x000000c4)
#define CSL_INTC0_FFTC_B_INTD1       (0x000000c5)
#define CSL_INTC0_FFTC_B_INTD2       (0x000000c6)
#define CSL_INTC0_FFTC_B_INTD3       (0x000000c7)
#define CSL_INTC0_RAC_B_DEVENT0      (0x000000c8)
#define CSL_INTC0_RAC_B_DEVENT1      (0x000000c9)
#define CSL_INTC0_TCP3D_C_REVT0      (0x000000ca)
#define CSL_INTC0_TCP3D_C_REVT1      (0x000000cb)
#define CSL_INTC0_FFTC_C_INTD0       (0x000000cc)
#define CSL_INTC0_FFTC_C_INTD1       (0x000000cd)
#define CSL_INTC0_FFTC_C_INTD2       (0x000000ce)
#define CSL_INTC0_FFTC_C_INTD3       (0x000000cf)


/**************************************************************************
* CHIP LEVEL INTC1 EVENT INPUT
**************************************************************************/

#define INTC1_EVENT_CNT                 160


/**************************************************************************
* CHIP LEVEL INTC1 EVENT DEFINITION
**************************************************************************/
#define CSL_INTC1_GPsigned char             (0x00000000)
#define CSL_INTC1_GPINT9             (0x00000001)
#define CSL_INTC1_GPINT10            (0x00000002)
#define CSL_INTC1_GPINT11            (0x00000003)
#define CSL_INTC1_GPINT12            (0x00000004)
#define CSL_INTC1_GPINT13            (0x00000005)
#define CSL_INTC1_GPINT14            (0x00000006)
#define CSL_INTC1_GPINT15            (0x00000007)
#define CSL_INTC1_TETBHFULLINT       (0x00000008)
#define CSL_INTC1_TETBFULLINT        (0x00000009)
#define CSL_INTC1_TETBACQINT         (0x0000000a)
#define CSL_INTC1_TETBHFULLINT0      (0x0000000b)
#define CSL_INTC1_TETBFULLINT0       (0x0000000c)
#define CSL_INTC1_TETBACQINT0        (0x0000000d)
#define CSL_INTC1_TETBHFULLINT1      (0x0000000e)
#define CSL_INTC1_TETBFULLINT1       (0x0000000f)
#define CSL_INTC1_TETBACQINT1        (0x00000010)
#define CSL_INTC1_TETBHFULLINT2      (0x00000011)
#define CSL_INTC1_TETBFULLINT2       (0x00000012)
#define CSL_INTC1_TETBACQINT2        (0x00000013)
#define CSL_INTC1_TETBHFULLINT3      (0x00000014)
#define CSL_INTC1_TETBFULLINT3       (0x00000015)
#define CSL_INTC1_TETBACQINT3        (0x00000016)
#define CSL_INTC1_DFT_PBIST_CPU_INT  (0x00000017)
#define CSL_INTC1_QM_INT_HIGH_16     (0x00000018)
#define CSL_INTC1_QM_INT_HIGH_17     (0x00000019)
#define CSL_INTC1_QM_INT_HIGH_18     (0x0000001a)
#define CSL_INTC1_QM_INT_HIGH_19     (0x0000001b)
#define CSL_INTC1_QM_INT_HIGH_20     (0x0000001c)
#define CSL_INTC1_QM_INT_HIGH_21     (0x0000001d)
#define CSL_INTC1_QM_INT_HIGH_22     (0x0000001e)
#define CSL_INTC1_QM_INT_HIGH_23     (0x0000001f)
#define CSL_INTC1_QM_INT_HIGH_24     (0x00000020)
#define CSL_INTC1_QM_INT_HIGH_25     (0x00000021)
#define CSL_INTC1_QM_INT_HIGH_26     (0x00000022)
#define CSL_INTC1_QM_INT_HIGH_27     (0x00000023)
#define CSL_INTC1_QM_INT_HIGH_28     (0x00000024)
#define CSL_INTC1_QM_INT_HIGH_29     (0x00000025)
#define CSL_INTC1_QM_INT_HIGH_30     (0x00000026)
#define CSL_INTC1_QM_INT_HIGH_31     (0x00000027)
#define CSL_INTC1_MDIO_LINK_INTR0    (0x00000028)
#define CSL_INTC1_MDIO_LINK_INTR1    (0x00000029)
#define CSL_INTC1_MDIO_USER_INTR0    (0x0000002a)
#define CSL_INTC1_MDIO_USER_INTR1    (0x0000002b)
#define CSL_INTC1_MISC_INTR          (0x0000002c)
#define CSL_INTC1_CP_TRACER_CORE_0_INTD (0x0000002d)
#define CSL_INTC1_CP_TRACER_CORE_1_INTD (0x0000002e)
#define CSL_INTC1_CP_TRACER_CORE_2_INTD (0x0000002f)
#define CSL_INTC1_CP_TRACER_CORE_3_INTD (0x00000030)
#define CSL_INTC1_CP_TRACER_DDR_INTD (0x00000031)
#define CSL_INTC1_CP_TRACER_MSMC_0_INTD (0x00000032)
#define CSL_INTC1_CP_TRACER_MSMC_1_INTD (0x00000033)
#define CSL_INTC1_CP_TRACER_MSMC_2_INTD (0x00000034)
#define CSL_INTC1_CP_TRACER_MSMC_3_INTD (0x00000035)
#define CSL_INTC1_CP_TRACER_CFG_INTD (0x00000036)
#define CSL_INTC1_CP_TRACER_QM_SS_CFG_INTD (0x00000037)
#define CSL_INTC1_CP_TRACER_QM_SS_DMA_INTD (0x00000038)
#define CSL_INTC1_CP_TRACER_SEM_INTD (0x00000039)
#define CSL_INTC1_SEMERR0            (0x0000003a)
#define CSL_INTC1_SEMERR1            (0x0000003b)
#define CSL_INTC1_SEMERR2            (0x0000003c)
#define CSL_INTC1_SEMERR3            (0x0000003d)
#define CSL_INTC1_BOOTCFG_INTD       (0x0000003e)
#define CSL_INTC1_PASS_INT_CDMA_0    (0x0000003f)
#define CSL_INTC1_MPU0_INTD          (0x00000040)
#define CSL_INTC1_MSMC_SCRUB_CERROR  (0x00000041)
#define CSL_INTC1_MPU1_INTD          (0x00000042)
#define CSL_INTC1_RAPIDIO_INT_CDMA_0 (0x00000043)
#define CSL_INTC1_MPU2_INTD          (0x00000044)
#define CSL_INTC1_QM_INT_CDMA_0      (0x00000045)
#define CSL_INTC1_MPU3_INTD          (0x00000046)
#define CSL_INTC1_QM_INT_CDMA_1      (0x00000047)
#define CSL_INTC1_MSMC_DEDC_CERROR   (0x00000048)
#define CSL_INTC1_MSMC_DEDC_NC_ERROR (0x00000049)
#define CSL_INTC1_MSMC_SCRUB_NC_ERROR (0x0000004a)
#define CSL_INTC1_RESERVED_5 (0x0000004b)
#define CSL_INTC1_MSMC_MPF_ERROR0    (0x0000004c)
#define CSL_INTC1_MSMC_MPF_ERROR1    (0x0000004d)
#define CSL_INTC1_MSMC_MPF_ERROR2    (0x0000004e)
#define CSL_INTC1_MSMC_MPF_ERROR3    (0x0000004f)
#define CSL_INTC1_MSMC_MPF_ERROR4    (0x00000050)
#define CSL_INTC1_MSMC_MPF_ERROR5    (0x00000051)
#define CSL_INTC1_MSMC_MPF_ERROR6    (0x00000052)
#define CSL_INTC1_MSMC_MPF_ERROR7    (0x00000053)
#define CSL_INTC1_MSMC_MPF_ERROR8    (0x00000054)
#define CSL_INTC1_MSMC_MPF_ERROR9    (0x00000055)
#define CSL_INTC1_MSMC_MPF_ERROR10   (0x00000056)
#define CSL_INTC1_MSMC_MPF_ERROR11   (0x00000057)
#define CSL_INTC1_MSMC_MPF_ERROR12   (0x00000058)
#define CSL_INTC1_MSMC_MPF_ERROR13   (0x00000059)
#define CSL_INTC1_MSMC_MPF_ERROR14   (0x0000005a)
#define CSL_INTC1_MSMC_MPF_ERROR15   (0x0000005b)
#define CSL_INTC1_RESERVED_1         (0x0000005c)
#define CSL_INTC1_INTDST0            (0x0000005d)
#define CSL_INTC1_INTDST1            (0x0000005e)
#define CSL_INTC1_INTDST2            (0x0000005f)
#define CSL_INTC1_INTDST3            (0x00000060)
#define CSL_INTC1_INTDST4            (0x00000061)
#define CSL_INTC1_INTDST5            (0x00000062)
#define CSL_INTC1_INTDST6            (0x00000063)
#define CSL_INTC1_INTDST7            (0x00000064)
#define CSL_INTC1_INTDST8            (0x00000065)
#define CSL_INTC1_INTDST9            (0x00000066)
#define CSL_INTC1_INTDST10           (0x00000067)
#define CSL_INTC1_INTDST11           (0x00000068)
#define CSL_INTC1_INTDST12           (0x00000069)
#define CSL_INTC1_INTDST13           (0x0000006a)
#define CSL_INTC1_INTDST14           (0x0000006b)
#define CSL_INTC1_INTDST15           (0x0000006c)
#define CSL_INTC1_INTDST16           (0x0000006d)
#define CSL_INTC1_INTDST17           (0x0000006e)
#define CSL_INTC1_INTDST18           (0x0000006f)
#define CSL_INTC1_INTDST19           (0x00000070)
#define CSL_INTC1_INTDST20           (0x00000071)
#define CSL_INTC1_INTDST21           (0x00000072)
#define CSL_INTC1_INTDST22           (0x00000073)
#define CSL_INTC1_INTDST23           (0x00000074)
#define CSL_INTC1_AIF_INTD           (0x00000075)
#define CSL_INTC1_RESERVED_2         (0x00000076)
#define CSL_INTC1_VCP0INT            (0x00000077)
#define CSL_INTC1_VCP1INT            (0x00000078)
#define CSL_INTC1_VCP2INT            (0x00000079)
#define CSL_INTC1_VCP3INT            (0x0000007a)
#define CSL_INTC1_TCP3D_A_INTD       (0x0000007b)
#define CSL_INTC1_TCP3D_B_INTD       (0x0000007c)
#define CSL_INTC1_TCP3E_INTD         (0x0000007d)
#define CSL_INTC1_FFTC_B_INTD0       (0x0000007e)
#define CSL_INTC1_FFTC_B_INTD1       (0x0000007f)
#define CSL_INTC1_GPINT4             (0x00000080)
#define CSL_INTC1_GPINT5             (0x00000081)
#define CSL_INTC1_GPINT6             (0x00000082)
#define CSL_INTC1_GPINT7             (0x00000083)
#define CSL_INTC1_CP_TRACER_RAC_INTD (0x00000084)
#define CSL_INTC1_CP_TRACER_RAC_FE_INTD (0x00000085)
#define CSL_INTC1_CP_TRACER_TAC_INTD (0x00000086)
#define CSL_INTC1_MPU4_INTD          (0x00000087)
#define CSL_INTC1_RESERVED_3         (0x00000088)
#define CSL_INTC1_QM_INT_HIGH_0      (0x00000089)
#define CSL_INTC1_QM_INT_HIGH_1      (0x0000008a)
#define CSL_INTC1_QM_INT_HIGH_2      (0x0000008b)
#define CSL_INTC1_QM_INT_HIGH_3      (0x0000008c)
#define CSL_INTC1_QM_INT_HIGH_4      (0x0000008d)
#define CSL_INTC1_QM_INT_HIGH_5      (0x0000008e)
#define CSL_INTC1_QM_INT_HIGH_6      (0x0000008f)
#define CSL_INTC1_QM_INT_HIGH_7      (0x00000090)
#define CSL_INTC1_QM_INT_HIGH_8      (0x00000091)
#define CSL_INTC1_QM_INT_HIGH_9      (0x00000092)
#define CSL_INTC1_QM_INT_HIGH_10     (0x00000093)
#define CSL_INTC1_QM_INT_HIGH_11     (0x00000094)
#define CSL_INTC1_QM_INT_HIGH_12     (0x00000095)
#define CSL_INTC1_QM_INT_HIGH_13     (0x00000096)
#define CSL_INTC1_QM_INT_HIGH_14     (0x00000097)
#define CSL_INTC1_QM_INT_HIGH_15     (0x00000098)
#define CSL_INTC1_FFTC_A_INTD0       (0x00000099)
#define CSL_INTC1_FFTC_A_INTD1       (0x0000009a)
#define CSL_INTC1_FFTC_A_INTD2       (0x0000009b)
#define CSL_INTC1_FFTC_A_INTD3       (0x0000009c)
#define CSL_INTC1_FFTC_B_INTD2       (0x0000009d)
#define CSL_INTC1_FFTC_B_INTD3       (0x0000009e)
#define CSL_INTC1_RESERVED_4         (0x0000009f)

/**************************************************************************
* CHIP LEVEL INTC2 EVENT INPUT
**************************************************************************/

#define INTC2_EVENT_CNT                 64


/**************************************************************************
* CHIP LEVEL INTC2 EVENT DEFINITION
**************************************************************************/
#define CSL_INTC2_GPINT0             (0x00000000)
#define CSL_INTC2_GPINT1             (0x00000001)
#define CSL_INTC2_GPINT2             (0x00000002)
#define CSL_INTC2_GPINT3             (0x00000003)
#define CSL_INTC2_GPINT4             (0x00000004)
#define CSL_INTC2_GPINT5             (0x00000005)
#define CSL_INTC2_GPINT6             (0x00000006)
#define CSL_INTC2_GPINT7             (0x00000007)
#define CSL_INTC2_GPsigned char             (0x00000008)
#define CSL_INTC2_GPINT9             (0x00000009)
#define CSL_INTC2_GPINT10            (0x0000000a)
#define CSL_INTC2_GPINT11            (0x0000000b)
#define CSL_INTC2_GPINT12            (0x0000000c)
#define CSL_INTC2_GPINT13            (0x0000000d)
#define CSL_INTC2_GPINT14            (0x0000000e)
#define CSL_INTC2_GPINT15            (0x0000000f)
#define CSL_INTC2_TETBHFULLINT       (0x00000010)
#define CSL_INTC2_TETBFULLINT        (0x00000011)
#define CSL_INTC2_TETBACQINT         (0x00000012)
#define CSL_INTC2_TETBHFULLINT0      (0x00000013)
#define CSL_INTC2_TETBFULLINT0       (0x00000014)
#define CSL_INTC2_TETBACQINT0        (0x00000015)
#define CSL_INTC2_TETBHFULLINT1      (0x00000016)
#define CSL_INTC2_TETBFULLINT1       (0x00000017)
#define CSL_INTC2_TETBACQINT1        (0x00000018)
#define CSL_INTC2_TETBHFULLINT2      (0x00000019)
#define CSL_INTC2_TETBFULLINT2       (0x0000001a)
#define CSL_INTC2_TETBACQINT2        (0x0000001b)
#define CSL_INTC2_TETBHFULLINT3      (0x0000001c)
#define CSL_INTC2_TETBFULLINT3       (0x0000001d)
#define CSL_INTC2_TETBACQINT3        (0x0000001e)
#define CSL_INTC2_CP_TRACER_CORE_0_INTD (0x0000001f)
#define CSL_INTC2_CP_TRACER_CORE_1_INTD (0x00000020)
#define CSL_INTC2_CP_TRACER_CORE_2_INTD (0x00000021)
#define CSL_INTC2_CP_TRACER_CORE_3_INTD (0x00000022)
#define CSL_INTC2_CP_TRACER_DDR_INTD (0x00000023)
#define CSL_INTC2_CP_TRACER_MSMC_0_INTD (0x00000024)
#define CSL_INTC2_CP_TRACER_MSMC_1_INTD (0x00000025)
#define CSL_INTC2_CP_TRACER_MSMC_2_INTD (0x00000026)
#define CSL_INTC2_CP_TRACER_MSMC_3_INTD (0x00000027)
#define CSL_INTC2_CP_TRACER_CFG_INTD (0x00000028)
#define CSL_INTC2_CP_TRACER_QM_SS_CFG_INTD (0x00000029)
#define CSL_INTC2_CP_TRACER_QM_SS_DMA_INTD (0x0000002a)
#define CSL_INTC2_CP_TRACER_SEM_INTD (0x0000002b)
#define CSL_INTC2_VUSR_INT_O         (0x0000002c)
#define CSL_INTC2_CP_TRACER_RAC_INTD (0x0000002d)
#define CSL_INTC2_CP_TRACER_RAC_FE_INTD (0x0000002e)
#define CSL_INTC2_CP_TRACER_TAC_INTD (0x0000002f)
#define CSL_INTC2_TCP3D_C_MPU5_INTD  (0x00000030)
#define CSL_INTC2_TINT4L             (0x00000031)
#define CSL_INTC2_TINT4H             (0x00000032)
#define CSL_INTC2_TINT5L             (0x00000033)
#define CSL_INTC2_TINT5H             (0x00000034)
#define CSL_INTC2_TINT6L             (0x00000035)
#define CSL_INTC2_TINT6H             (0x00000036)
#define CSL_INTC2_TINT7L             (0x00000037)
#define CSL_INTC2_TINT7H             (0x00000038)
#define CSL_INTC2_RESERVED_1         (0x00000039)
#define CSL_INTC2_RESERVED_2         (0x0000003a)
#define CSL_INTC2_RESERVED_3         (0x0000003b)
#define CSL_INTC2_RESERVED_4         (0x0000003c)
#define CSL_INTC2_DDR3_ERR           (0x0000003d)
#define CSL_INTC2_PO_VP_SMPSACK_INTR (0x0000003e)
#define CSL_INTC2_RESERVED_5         (0x0000003f)

/**************************************************************************
* TPCC0 (CPU/2) EVENT INPUT
**************************************************************************/

#define TPCC0_EVENT_CNT                 16


/**************************************************************************
* TPCC0 (CPU/2) EVENT DEFINITION
**************************************************************************/
#define CSL_TPCC0_TCP3D_C_REVT0     (0x00000000)
#define CSL_TPCC0_TCP3D_C_REVT1     (0x00000001)
#define CSL_TPCC0_FFTC_C_INTD0      (0x00000002)
#define CSL_TPCC0_FFTC_C_INTD1      (0x00000003)
#define CSL_TPCC0_FFTC_C_INTD2      (0x00000004)
#define CSL_TPCC0_FFTC_C_INTD3      (0x00000005)
#define CSL_TPCC0_INTC2_OUT40       (0x00000006)
#define CSL_TPCC0_INTC2_OUT41       (0x00000007)
#define CSL_TPCC0_INTC2_OUT0        (0x00000008)
#define CSL_TPCC0_INTC2_OUT1        (0x00000009)
#define CSL_TPCC0_INTC2_OUT2        (0x0000000a)
#define CSL_TPCC0_INTC2_OUT3        (0x0000000b)
#define CSL_TPCC0_INTC2_OUT4        (0x0000000c)
#define CSL_TPCC0_INTC2_OUT5        (0x0000000d)
#define CSL_TPCC0_INTC2_OUT6        (0x0000000e)
#define CSL_TPCC0_INTC2_OUT7        (0x0000000f)

/**************************************************************************
* TPCC1 (CPU/3) EVENT INPUT
**************************************************************************/

#define TPCC1_EVENT_CNT                 64


/**************************************************************************
* TPCC1 (CPU/3) EVENT DEFINITION
**************************************************************************/
#define CSL_TPCC1_SPIINT0            (0x00000000)
#define CSL_TPCC1_SPIINT1            (0x00000001)
#define CSL_TPCC1_SPIXEVT            (0x00000002)
#define CSL_TPCC1_SPIREVT            (0x00000003)
#define CSL_TPCC1_I2CREVT            (0x00000004)
#define CSL_TPCC1_I2CXEVT            (0x00000005)
#define CSL_TPCC1_GPINT0             (0x00000006)
#define CSL_TPCC1_GPINT1             (0x00000007)
#define CSL_TPCC1_GPINT2             (0x00000008)
#define CSL_TPCC1_GPINT3             (0x00000009)
#define CSL_TPCC1_AIF_SEVT0          (0x0000000a)
#define CSL_TPCC1_AIF_SEVT1          (0x0000000b)
#define CSL_TPCC1_AIF_SEVT2          (0x0000000c)
#define CSL_TPCC1_AIF_SEVT3          (0x0000000d)
#define CSL_TPCC1_AIF_SEVT4          (0x0000000e)
#define CSL_TPCC1_AIF_SEVT5          (0x0000000f)
#define CSL_TPCC1_AIF_SEVT6          (0x00000010)
#define CSL_TPCC1_AIF_SEVT7          (0x00000011)
#define CSL_TPCC1_SEMINT0            (0x00000012)
#define CSL_TPCC1_SEMINT1            (0x00000013)
#define CSL_TPCC1_SEMINT2            (0x00000014)
#define CSL_TPCC1_SEMINT3            (0x00000015)
#define CSL_TPCC1_TINT4L             (0x00000016)
#define CSL_TPCC1_TINT4H             (0x00000017)
#define CSL_TPCC1_TINT5L             (0x00000018)
#define CSL_TPCC1_TINT5H             (0x00000019)
#define CSL_TPCC1_TINT6L             (0x0000001a)
#define CSL_TPCC1_TINT6H             (0x0000001b)
#define CSL_TPCC1_TINT7L             (0x0000001c)
#define CSL_TPCC1_TINT7H             (0x0000001d)
#define CSL_TPCC1_RAC_A_INT0         (0x0000001e)
#define CSL_TPCC1_RAC_A_INT1         (0x0000001f)
#define CSL_TPCC1_RAC_A_INT2         (0x00000020)
#define CSL_TPCC1_RAC_A_INT3         (0x00000021)
#define CSL_TPCC1_RAC_A_DEVENT0      (0x00000022)
#define CSL_TPCC1_RAC_A_DEVENT1      (0x00000023)
#define CSL_TPCC1_TAC_INTD           (0x00000024)
#define CSL_TPCC1_TACDEVENT0         (0x00000025)
#define CSL_TPCC1_TACDEVENT1         (0x00000026)
#define CSL_TPCC1_RAC_B_INT0         (0x00000027)
#define CSL_TPCC1_RAC_B_INT1         (0x00000028)
#define CSL_TPCC1_RAC_B_INT2         (0x00000029)
#define CSL_TPCC1_RAC_B_INT3         (0x0000002a)
#define CSL_TPCC1_RAC_B_DEVENT0      (0x0000002b)
#define CSL_TPCC1_RAC_B_DEVENT1      (0x0000002c)
#define CSL_TPCC1_INTC1_OUT2         (0x0000002d)
#define CSL_TPCC1_INTC1_OUT3         (0x0000002e)
#define CSL_TPCC1_INTC1_OUT4         (0x0000002f)
#define CSL_TPCC1_INTC1_OUT5         (0x00000030)
#define CSL_TPCC1_INTC1_OUT6         (0x00000031)
#define CSL_TPCC1_INTC1_OUT7         (0x00000032)
#define CSL_TPCC1_INTC1_OUT8         (0x00000033)
#define CSL_TPCC1_INTC1_OUT9         (0x00000034)
#define CSL_TPCC1_INTC1_OUT10        (0x00000035)
#define CSL_TPCC1_INTC1_OUT11        (0x00000036)
#define CSL_TPCC1_INTC1_OUT12        (0x00000037)
#define CSL_TPCC1_INTC1_OUT13        (0x00000038)
#define CSL_TPCC1_INTC1_OUT14        (0x00000039)
#define CSL_TPCC1_INTC1_OUT15        (0x0000003a)
#define CSL_TPCC1_INTC1_OUT16        (0x0000003b)
#define CSL_TPCC1_INTC1_OUT17        (0x0000003c)
#define CSL_TPCC1_INTC1_OUT18        (0x0000003d)
#define CSL_TPCC1_INTC1_OUT19        (0x0000003e)
#define CSL_TPCC1_INTC1_OUT20        (0x0000003f)


/**************************************************************************
* TPCC2 (CPU/3) EVENT INPUT
**************************************************************************/

#define TPCC2_EVENT_CNT                 64


/**************************************************************************
* TPCC2 (CPU/3) EVENT DEFINITION
**************************************************************************/
#define CSL_TPCC2_TCP3D_A_REVT0      (0x00000000)
#define CSL_TPCC2_TCP3D_A_REVT1      (0x00000001)
#define CSL_TPCC2_TCP3EREVT          (0x00000002)
#define CSL_TPCC2_TCP3EWEVT          (0x00000003)
#define CSL_TPCC2_URXEVT             (0x00000004)
#define CSL_TPCC2_UTXEVT             (0x00000005)
#define CSL_TPCC2_GPINT0             (0x00000006)
#define CSL_TPCC2_GPINT1             (0x00000007)
#define CSL_TPCC2_GPINT2             (0x00000008)
#define CSL_TPCC2_GPINT3             (0x00000009)
#define CSL_TPCC2_VCP0REVT           (0x0000000a)
#define CSL_TPCC2_VCP0XEVT           (0x0000000b)
#define CSL_TPCC2_VCP1REVT           (0x0000000c)
#define CSL_TPCC2_VCP1XEVT           (0x0000000d)
#define CSL_TPCC2_VCP2REVT           (0x0000000e)
#define CSL_TPCC2_VCP2XEVT           (0x0000000f)
#define CSL_TPCC2_VCP3REVT           (0x00000010)
#define CSL_TPCC2_VCP3XEVT           (0x00000011)
#define CSL_TPCC2_SEMINT0            (0x00000012)
#define CSL_TPCC2_SEMINT1            (0x00000013)
#define CSL_TPCC2_SEMINT2            (0x00000014)
#define CSL_TPCC2_SEMINT3            (0x00000015)
#define CSL_TPCC2_TINT4L             (0x00000016)
#define CSL_TPCC2_TINT4H             (0x00000017)
#define CSL_TPCC2_TINT5L             (0x00000018)
#define CSL_TPCC2_TINT5H             (0x00000019)
#define CSL_TPCC2_TINT6L             (0x0000001a)
#define CSL_TPCC2_TINT6H             (0x0000001b)
#define CSL_TPCC2_TINT7L             (0x0000001c)
#define CSL_TPCC2_TINT7H             (0x0000001d)
#define CSL_TPCC2_SPIXEVT            (0x0000001e)
#define CSL_TPCC2_SPIREVT            (0x0000001f)
#define CSL_TPCC2_I2CREVT            (0x00000020)
#define CSL_TPCC2_I2CXEVT            (0x00000021)
#define CSL_TPCC2_TCP3D_B_REVT0      (0x00000022)
#define CSL_TPCC2_TCP3D_B_REVT1      (0x00000023)
#define CSL_TPCC2_INTC1_OUT23        (0x00000024)
#define CSL_TPCC2_INTC1_OUT24        (0x00000025)
#define CSL_TPCC2_INTC1_OUT25        (0x00000026)
#define CSL_TPCC2_INTC1_OUT26        (0x00000027)
#define CSL_TPCC2_INTC1_OUT27        (0x00000028)
#define CSL_TPCC2_INTC1_OUT28        (0x00000029)
#define CSL_TPCC2_INTC1_OUT29        (0x0000002a)
#define CSL_TPCC2_INTC1_OUT30        (0x0000002b)
#define CSL_TPCC2_INTC1_OUT31        (0x0000002c)
#define CSL_TPCC2_INTC1_OUT32        (0x0000002d)
#define CSL_TPCC2_INTC1_OUT33        (0x0000002e)
#define CSL_TPCC2_INTC1_OUT34        (0x0000002f)
#define CSL_TPCC2_INTC1_OUT35        (0x00000030)
#define CSL_TPCC2_INTC1_OUT36        (0x00000031)
#define CSL_TPCC2_INTC1_OUT37        (0x00000032)
#define CSL_TPCC2_INTC1_OUT38        (0x00000033)
#define CSL_TPCC2_INTC1_OUT39        (0x00000034)
#define CSL_TPCC2_INTC1_OUT40        (0x00000035)
#define CSL_TPCC2_INTC1_OUT41        (0x00000036)
#define CSL_TPCC2_INTC1_OUT42        (0x00000037)
#define CSL_TPCC2_INTC1_OUT43        (0x00000038)
#define CSL_TPCC2_INTC1_OUT44        (0x00000039)
#define CSL_TPCC2_TCP3D_C_REVT0      (0x0000003a)
#define CSL_TPCC2_TCP3D_C_REVT1      (0x0000003b)
#define CSL_TPCC2_FFTC_C_INTD0       (0x0000003c)
#define CSL_TPCC2_FFTC_C_INTD1       (0x0000003d)
#define CSL_TPCC2_FFTC_C_INTD2       (0x0000003e)
#define CSL_TPCC2_FFTC_C_INTD3       (0x0000003f)


/**************************************************************************
* MCM EVENT INPUT
**************************************************************************/

#define CSL_MCM_EVENT_CNT                   64


/**************************************************************************
* MCM EVENT DEFINITION
**************************************************************************/
#define CSL_MCM_INTC2_OUT8           (0x00000000)
#define CSL_MCM_INTC2_OUT9           (0x00000001)
#define CSL_MCM_INTC2_OUT10          (0x00000002)
#define CSL_MCM_INTC2_OUT11          (0x00000003)
#define CSL_MCM_INTC2_OUT12          (0x00000004)
#define CSL_MCM_INTC2_OUT13          (0x00000005)
#define CSL_MCM_INTC2_OUT14          (0x00000006)
#define CSL_MCM_INTC2_OUT15          (0x00000007)
#define CSL_MCM_INTC2_OUT16          (0x00000008)
#define CSL_MCM_INTC2_OUT17          (0x00000009)
#define CSL_MCM_INTC2_OUT18          (0x0000000a)
#define CSL_MCM_INTC2_OUT19          (0x0000000b)
#define CSL_MCM_INTC2_OUT20          (0x0000000c)
#define CSL_MCM_INTC2_OUT21          (0x0000000d)
#define CSL_MCM_INTC2_OUT22          (0x0000000e)
#define CSL_MCM_INTC2_OUT23          (0x0000000f)
#define CSL_MCM_INTC2_OUT24          (0x00000010)
#define CSL_MCM_INTC2_OUT25          (0x00000011)
#define CSL_MCM_INTC2_OUT26          (0x00000012)
#define CSL_MCM_INTC2_OUT27          (0x00000013)
#define CSL_MCM_INTC2_OUT28          (0x00000014)
#define CSL_MCM_INTC2_OUT29          (0x00000015)
#define CSL_MCM_INTC2_OUT30          (0x00000016)
#define CSL_MCM_INTC2_OUT31          (0x00000017)
#define CSL_MCM_INTC2_OUT32          (0x00000018)
#define CSL_MCM_INTC2_OUT33          (0x00000019)
#define CSL_MCM_INTC2_OUT34          (0x0000001a)
#define CSL_MCM_INTC2_OUT35          (0x0000001b)
#define CSL_MCM_INTC2_OUT36          (0x0000001c)
#define CSL_MCM_INTC2_OUT37          (0x0000001d)
#define CSL_MCM_INTC2_OUT38          (0x0000001e)
#define CSL_MCM_INTC2_OUT39          (0x0000001f)
#define CSL_MCM_QUE_PENDING_864      (0x00000020)
#define CSL_MCM_QUE_PENDING_865      (0x00000021)
#define CSL_MCM_QUE_PENDING_866      (0x00000022)
#define CSL_MCM_QUE_PENDING_867      (0x00000023)
#define CSL_MCM_QUE_PENDING_868      (0x00000024)
#define CSL_MCM_QUE_PENDING_869      (0x00000025)
#define CSL_MCM_QUE_PENDING_870      (0x00000026)
#define CSL_MCM_QUE_PENDING_871      (0x00000027)
#define CSL_MCM_QUE_PENDING_872      (0x00000028)
#define CSL_MCM_QUE_PENDING_873      (0x00000029)
#define CSL_MCM_QUE_PENDING_874      (0x0000002a)
#define CSL_MCM_QUE_PENDING_875      (0x0000002b)
#define CSL_MCM_QUE_PENDING_876      (0x0000002c)
#define CSL_MCM_QUE_PENDING_877      (0x0000002d)
#define CSL_MCM_QUE_PENDING_878      (0x0000002e)
#define CSL_MCM_QUE_PENDING_879      (0x0000002f)
#define CSL_MCM_QUE_PENDING_880      (0x00000030)
#define CSL_MCM_QUE_PENDING_881      (0x00000031)
#define CSL_MCM_QUE_PENDING_882      (0x00000032)
#define CSL_MCM_QUE_PENDING_883      (0x00000033)
#define CSL_MCM_QUE_PENDING_884      (0x00000034)
#define CSL_MCM_QUE_PENDING_885      (0x00000035)
#define CSL_MCM_QUE_PENDING_886      (0x00000036)
#define CSL_MCM_QUE_PENDING_887      (0x00000037)
#define CSL_MCM_QUE_PENDING_888      (0x00000038)
#define CSL_MCM_QUE_PENDING_889      (0x00000039)
#define CSL_MCM_QUE_PENDING_890      (0x0000003a)
#define CSL_MCM_QUE_PENDING_891      (0x0000003b)
#define CSL_MCM_QUE_PENDING_892      (0x0000003c)
#define CSL_MCM_QUE_PENDING_893      (0x0000003d)
#define CSL_MCM_QUE_PENDING_894      (0x0000003e)
#define CSL_MCM_QUE_PENDING_895      (0x0000003f)


/**************************************************************************
* GEM EVENT INPUT
**************************************************************************/

#define GEM_EVENT_CNT                   128


/**************************************************************************
* GEM EVENT DEFINITION
**************************************************************************/
#define CSL_GEM_EVT0                 (0x00000000)
#define CSL_GEM_EVT1                 (0x00000001)
#define CSL_GEM_EVT2                 (0x00000002)
#define CSL_GEM_EVT3                 (0x00000003)
#define CSL_GEM_TETBHFULLINTN        (0x00000004)
#define CSL_GEM_TETBFULLINTN         (0x00000005)
#define CSL_GEM_TETBACQINTN          (0x00000006)
#define CSL_GEM_TETBOVFLINTN         (0x00000007)
#define CSL_GEM_TETBUNFLINTN         (0x00000008)
#define CSL_GEM_EMU_DTDMA            (0x00000009)
#define CSL_GEM_MSMC_MPF_ERRORN      (0x0000000a)
#define CSL_GEM_EMU_RTDXRX           (0x0000000b)
#define CSL_GEM_EMU_RTDXTX           (0x0000000c)
#define CSL_GEM_IDMA0                (0x0000000d)
#define CSL_GEM_IDMA1                (0x0000000e)
#define CSL_GEM_SEMERRN              (0x0000000f)
#define CSL_GEM_SEMINTN              (0x00000010)
#define CSL_GEM_PCIEXPRESS_MSI_INTN  (0x00000011)
#define CSL_GEM_PCIEXPRESS_MSI_INTN_PLUS_4 (0x00000012)
#define CSL_GEM_RAC_A_INTN              (0x00000013)
#define CSL_GEM_INTDST_N_PLUS_16     (0x00000014)
#define CSL_GEM_INTDST_N_PLUS_20     (0x00000015)
#define CSL_GEM_INTC0_OUT_64_PLUS_10_MUL_N (0x00000016)
#define CSL_GEM_INTC0_OUT_65_PLUS_10_MUL_N (0x00000017)
#define CSL_GEM_INTC0_OUT_66_PLUS_10_MUL_N (0x00000018)
#define CSL_GEM_INTC0_OUT_67_PLUS_10_MUL_N (0x00000019)
#define CSL_GEM_INTC0_OUT_68_PLUS_10_MUL_N (0x0000001a)
#define CSL_GEM_INTC0_OUT_69_PLUS_10_MUL_N (0x0000001b)
#define CSL_GEM_INTC0_OUT_70_PLUS_10_MUL_N (0x0000001c)
#define CSL_GEM_INTC0_OUT_71_PLUS_10_MUL_N (0x0000001d)
#define CSL_GEM_INTC0_OUT_72_PLUS_10_MUL_N (0x0000001e)
#define CSL_GEM_INTC0_OUT_73_PLUS_10_MUL_N (0x0000001f)
#define CSL_GEM_QM_INT_LOW_0         (0x00000020)
#define CSL_GEM_QM_INT_LOW_1         (0x00000021)
#define CSL_GEM_QM_INT_LOW_2         (0x00000022)
#define CSL_GEM_QM_INT_LOW_3         (0x00000023)
#define CSL_GEM_QM_INT_LOW_4         (0x00000024)
#define CSL_GEM_QM_INT_LOW_5         (0x00000025)
#define CSL_GEM_QM_INT_LOW_6         (0x00000026)
#define CSL_GEM_QM_INT_LOW_7         (0x00000027)
#define CSL_GEM_QM_INT_LOW_8         (0x00000028)
#define CSL_GEM_QM_INT_LOW_9         (0x00000029)
#define CSL_GEM_QM_INT_LOW_10        (0x0000002a)
#define CSL_GEM_QM_INT_LOW_11        (0x0000002b)
#define CSL_GEM_QM_INT_LOW_12        (0x0000002c)
#define CSL_GEM_QM_INT_LOW_13        (0x0000002d)
#define CSL_GEM_QM_INT_LOW_14        (0x0000002e)
#define CSL_GEM_QM_INT_LOW_15        (0x0000002f)
#define CSL_GEM_QM_INT_HIGH_N        (0x00000030)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_4 (0x00000031)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_8 (0x00000032)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_12 (0x00000033)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_16 (0x00000034)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_20 (0x00000035)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_24 (0x00000036)
#define CSL_GEM_QM_INT_HIGH__N_PLUS_28 (0x00000037)
#define CSL_GEM_INTC0_OUT0           (0x00000038)
#define CSL_GEM_INTC0_OUT1           (0x00000039)
#define CSL_GEM_INTC0_OUT2           (0x0000003a)
#define CSL_GEM_INTC0_OUT3           (0x0000003b)
#define CSL_GEM_INTC0_OUT4           (0x0000003c)
#define CSL_GEM_INTC0_OUT5           (0x0000003d)
#define CSL_GEM_INTC0_OUT6           (0x0000003e)
#define CSL_GEM_INTC0_OUT7           (0x0000003f)
#define CSL_GEM_TINTLN               (0x00000040)
#define CSL_GEM_TINTHN               (0x00000041)
#define CSL_GEM_TINT4L               (0x00000042)
#define CSL_GEM_TINT4H               (0x00000043)
#define CSL_GEM_TINT5L               (0x00000044)
#define CSL_GEM_TINT5H               (0x00000045)
#define CSL_GEM_TINT6L               (0x00000046)
#define CSL_GEM_TINT6H               (0x00000047)
#define CSL_GEM_TINT7L               (0x00000048)
#define CSL_GEM_TINT7H               (0x00000049)
#define CSL_GEM_INTC0_OUT_8_PLUS_16_MUL_N (0x0000004a)
#define CSL_GEM_INTC0_OUT_9_PLUS_16_MUL_N (0x0000004b)
#define CSL_GEM_INTC0_OUT_10_PLUS_16_MUL_N (0x0000004c)
#define CSL_GEM_INTC0_OUT_11_PLUS_16_MUL_N (0x0000004d)
#define CSL_GEM_GPINT4               (0x0000004e)
#define CSL_GEM_GPINT5               (0x0000004f)
#define CSL_GEM_GPINT6               (0x00000050)
#define CSL_GEM_GPINT7               (0x00000051)
#define CSL_GEM_GPsigned char               (0x00000052)
#define CSL_GEM_GPINT9               (0x00000053)
#define CSL_GEM_GPINT10              (0x00000054)
#define CSL_GEM_GPINT11              (0x00000055)
#define CSL_GEM_GPINT12              (0x00000056)
#define CSL_GEM_GPINT13              (0x00000057)
#define CSL_GEM_GPINT14              (0x00000058)
#define CSL_GEM_GPINT15              (0x00000059)
#define CSL_GEM_IPC_LOCAL            (0x0000005a)
#define CSL_GEM_GPINTN               (0x0000005b)
#define CSL_GEM_INTC0_OUT_12_PLUS_16_MUL_N (0x0000005c)
#define CSL_GEM_INTC0_OUT_13_PLUS_16_MUL_N (0x0000005d)
#define CSL_GEM_INTC0_OUT_14_PLUS_16_MUL_N (0x0000005e)
#define CSL_GEM_INTC0_OUT_15_PLUS_16_MUL_N (0x0000005f)
#define CSL_GEM_INTERR               (0x00000060)
#define CSL_GEM_EMC_IDMAERR          (0x00000061)
#define CSL_GEM_PBISTINT             (0x00000062)
#define CSL_GEM_RAC_B_INTN          (0x00000063)
#define CSL_GEM_EFIINTA              (0x00000064)
#define CSL_GEM_EFIINTB              (0x00000065)
#define CSL_GEM_AIF_EVENT0         (0x00000066)
#define CSL_GEM_AIF_EVENT1           (0x00000067)
#define CSL_GEM_AIF_EVENT2          (0x00000068)
#define CSL_GEM_AIF_EVENT3           (0x00000069)
#define CSL_GEM_AIF_EVENT4           (0x0000006a)
#define CSL_GEM_AIF_EVENT5           (0x0000006b)
#define CSL_GEM_AIF_EVENT6          (0x0000006c)
#define CSL_GEM_AIF_EVENT7           (0x0000006d)
#define CSL_GEM_MDMAERREVT         (0x0000006e)
#define CSL_GEM_RESERVED_111         (0x0000006f)
#define CSL_GEM_CPU_2_TPCC_AET         (0x00000070)
#define CSL_GEM_PMC_ED               (0x00000071)
#define CSL_GEM_CPU_3_1_TPCC_AET         (0x00000072)
#define CSL_GEM_CPU_3_2_TPCC_AET         (0x00000073)
#define CSL_GEM_UMC_ED1              (0x00000074)
#define CSL_GEM_UMC_ED2              (0x00000075)
#define CSL_GEM_PDC_INT              (0x00000076)
#define CSL_GEM_SYS_CMPA         (0x00000077)
#define CSL_GEM_PMC_CMPA             (0x00000078)
#define CSL_GEM_PMC_DMPA             (0x00000079)
#define CSL_GEM_DMC_CMPA             (0x0000007a)
#define CSL_GEM_DMC_DMPA             (0x0000007b)
#define CSL_GEM_UMC_CMPA             (0x0000007c)
#define CSL_GEM_UMC_DMPA             (0x0000007d)
#define CSL_GEM_EMC_CMPA             (0x0000007e)
#define CSL_GEM_EMC_BUSERR           (0x0000007f)

/**************************************************************************
* EOI Values
**************************************************************************/
#define CSL_MPU_EOI_VALUE             0
#define CSL_TRACER_EOI_VALUE          0
#define CSL_BOOTCFG_EOI_VALUE         0
#define CSL_TCP3D_EOI_VALUE           0
#define CSL_TCP3E_EOI_VALUE           0
#define CSL_TAC_EOI_VALUE             0
#define CSL_AIF_EOI_VALUE             0
#define CSL_TCP3D_EOI_VALUE           0
#define CSL_TCP3E_EOI_VALUE           0
#define CSL_FFTC_INTD0_EOI_VALUE      0
#define CSL_FFTC_INTD1_EOI_VALUE      1
#define CSL_FFTC_INTD2_EOI_VALUE      2
#define CSL_FFTC_INTD3_EOI_VALUE      3
#define CSL_BCP_INTD0_EOI_VALUE       0
#define CSL_BCP_INTD1_EOI_VALUE       1
#define CSL_BCP_INTD2_EOI_VALUE       2
#define CSL_BCP_INTD3_EOI_VALUE       3

#endif

