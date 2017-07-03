################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/C6000 Code Generation Tools 7.4.2

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./common/Keystone_Serdes_init.obj" \
"./common/Keystone_SRIO_drv.obj" \
"./common/KeyStone_common.obj" \
"./common/KeyStone_Timer_Init.obj" \
"./common/KeyStone_SRIO_Init.obj" \
"./common/KeyStone_Multicore_Navigator_drv.obj" \
"./common/KeyStone_DDR_Init.obj" \
"./common/FunctionHook.obj" \
"./common/Exception.obj" \
"./src/timer.obj" \
"./src/time_agc.obj" \
"./src/system_init_var.obj" \
"./src/system_init.obj" \
"./src/srio_debug.obj" \
"./src/rs_basic_gen.obj" \
"./src/qmss_device.obj" \
"./src/precoding.obj" \
"./src/prbmapping.obj" \
"./src/pl_const.obj" \
"./src/phich_data_gen.obj" \
"./src/pdcch_scrambling.obj" \
"./src/para_update.obj" \
"./src/osal.obj" \
"./src/modulation.obj" \
"./src/lte_dl_processmain.obj" \
"./src/layermapping.obj" \
"./src/interleaver.obj" \
"./src/fftc_func.obj" \
"./src/cppi_device.obj" \
"./src/bcp_pdcch_CCEconfig.obj" \
"./src/bcp_lte_pdsch.obj" \
"./src/bcp_lte_pdcch.obj" \
"./src/bcp_lte_pbch.obj" \
"./src/bcp_func.obj" \
"./src/bcp_device.obj" \
"./src/Timer_drv.obj" \
"./src/TSC.obj" \
"./src/SRIO_Test_new.obj" \
"./src/SRIO_PktDMA_Init.obj" \
"./src/SRIO_2DSP_Test.obj" \
"./src/MNav_funct.obj" \
"./src/Interrupt_Handler.obj" \
"./src/Bitscrambling.obj" \
"./src/BS_DSP0_Core0_PHICH.obj" \
"./src/BS_DSP0_Core0_PDSCH.obj" \
"./src/BS_DSP0_Core0_PDCCH.obj" \
"./src/BS_DSP0_Core0_PCFICH.obj" \
"./src/BS_DSP0_Core0_PBCH.obj" \
"./vectors.obj" \
"./srio_discovery_ex.obj" \
"./SRIO_drv.obj" \
"./LTE_Test.obj" \
"./BSLC667X_srio_init.obj" \
-l"libc.a" \
-l"bslc6670.ae66e" \
-l"rts6600e_elf.lib" \
-l"ti.csl.ae66e" \
-l"ti.csl.intc.ae66e" \
-l"ti.drv.bcp.ae66e" \
-l"ti.drv.cppi.ae66e" \
-l"ti.drv.qmss.ae66e" \
"../BSLC667X_C6670update.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include common/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include common/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LTE_Timer_version4.out

# Tool invocations
LTE_Timer_version4.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"D:/ti/C6000 Code Generation Tools 7.4.2/bin/cl6x" -mv6600 --big_endian --abi=eabi -Ooff -g --display_error_number --diag_warning=225 --diag_wrap=off -z -m"LTE_Timer_version4.map" -i"D:/CommAgility_Amc2c66xx_V0_4_9/dsp/dsplibs/inc" -i"D:/CommAgility_Amc2c66xx_V0_4_9/dsp/dsplibs/lib" -i"D:/ti/C6000 Code Generation Tools 7.4.2/lib" -i"D:/ti/pdk_C6670_1_1_2_5/packages/ti/csl/lib" -i"D:/work1/DL_zihuan/LTE_Timer_version4/lib" -i"D:/ti/C6000 Code Generation Tools 7.4.2/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --rom_model -o "LTE_Timer_version4.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C6000_EXECUTABLE_OUTPUTS__QUOTED) "LTE_Timer_version4.out"
	-$(RM) "BSLC667X_srio_init.pp" "LTE_Test.pp" "SRIO_drv.pp" "srio_discovery_ex.pp" "src\BS_DSP0_Core0_PCFICH.pp" "src\BS_DSP0_Core0_PDCCH.pp" "src\BS_DSP0_Core0_PHICH.pp" "src\Bitscrambling.pp" "src\Interrupt_Handler.pp" "src\MNav_funct.pp" "src\SRIO_2DSP_Test.pp" "src\SRIO_PktDMA_Init.pp" "src\SRIO_Test_new.pp" "src\TSC.pp" "src\Timer_drv.pp" "src\bcp_device.pp" "src\bcp_func.pp" "src\bcp_lte_pbch.pp" "src\bcp_lte_pdcch.pp" "src\bcp_lte_pdsch.pp" "src\bcp_pdcch_CCEconfig.pp" "src\cppi_device.pp" "src\fftc_func.pp" "src\interleaver.pp" "src\layermapping.pp" "src\lte_dl_processmain.pp" "src\modulation.pp" "src\osal.pp" "src\para_update.pp" "src\pdcch_scrambling.pp" "src\phich_data_gen.pp" "src\pl_const.pp" "src\prbmapping.pp" "src\precoding.pp" "src\qmss_device.pp" "src\rs_basic_gen.pp" "src\srio_debug.pp" "src\system_init.pp" "src\system_init_var.pp" "src\time_agc.pp" "src\timer.pp" "common\Exception.pp" "common\FunctionHook.pp" "common\KeyStone_DDR_Init.pp" "common\KeyStone_Multicore_Navigator_drv.pp" 
	-$(RM) "common\KeyStone_SRIO_Init.pp" "common\KeyStone_Timer_Init.pp" "common\KeyStone_common.pp" "common\Keystone_SRIO_drv.pp" "common\Keystone_Serdes_init.pp" 
	-$(RM) "src\BS_DSP0_Core0_PBCH.pp" "src\BS_DSP0_Core0_PDSCH.pp" 
	-$(RM) "BSLC667X_srio_init.obj" "LTE_Test.obj" "SRIO_drv.obj" "srio_discovery_ex.obj" "vectors.obj" "src\BS_DSP0_Core0_PBCH.obj" "src\BS_DSP0_Core0_PCFICH.obj" "src\BS_DSP0_Core0_PDCCH.obj" "src\BS_DSP0_Core0_PDSCH.obj" "src\BS_DSP0_Core0_PHICH.obj" "src\Bitscrambling.obj" "src\Interrupt_Handler.obj" "src\MNav_funct.obj" "src\SRIO_2DSP_Test.obj" "src\SRIO_PktDMA_Init.obj" "src\SRIO_Test_new.obj" "src\TSC.obj" "src\Timer_drv.obj" "src\bcp_device.obj" "src\bcp_func.obj" "src\bcp_lte_pbch.obj" "src\bcp_lte_pdcch.obj" "src\bcp_lte_pdsch.obj" "src\bcp_pdcch_CCEconfig.obj" "src\cppi_device.obj" "src\fftc_func.obj" "src\interleaver.obj" "src\layermapping.obj" "src\lte_dl_processmain.obj" "src\modulation.obj" "src\osal.obj" "src\para_update.obj" "src\pdcch_scrambling.obj" "src\phich_data_gen.obj" "src\pl_const.obj" "src\prbmapping.obj" "src\precoding.obj" "src\qmss_device.obj" "src\rs_basic_gen.obj" "src\srio_debug.obj" "src\system_init.obj" "src\system_init_var.obj" "src\time_agc.obj" "src\timer.obj" 
	-$(RM) "common\Exception.obj" "common\FunctionHook.obj" "common\KeyStone_DDR_Init.obj" "common\KeyStone_Multicore_Navigator_drv.obj" "common\KeyStone_SRIO_Init.obj" "common\KeyStone_Timer_Init.obj" "common\KeyStone_common.obj" "common\Keystone_SRIO_drv.obj" "common\Keystone_Serdes_init.obj" 
	-$(RM) "vectors.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
