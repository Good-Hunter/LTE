################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/C6000 Code Generation Tools 7.4.2

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./BSLC667X_srio_discovery_ex.obj" \
-l"D:\ti\pdk_C6670_1_1_2_5\packages\ti\csl\lib\ti.csl.ae66e" \
-l"D:\ti\C6000 Code Generation Tools 7.4.2\lib\rts6600e_elf.lib" \
-l"D:\ti\pdk_C6670_1_1_2_5\packages\ti\csl\lib\ti.csl.intc.ae66e" \
-l"D:\CommAgility_Amc2c66xx_V0_4_9\dsp\dsplibs\lib\bslc6670.ae66e" \
-l"D:\ti\C6000 Code Generation Tools 7.4.2\lib\libc.a" \
"../BSLC667X_C6670_TCI6616_cmd.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
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
all: BSLC6670_srio_discovery_ex_1.out

# Tool invocations
BSLC6670_srio_discovery_ex_1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"D:/ti/C6000 Code Generation Tools 7.4.2/bin/cl6x" -mv6600 -g --display_error_number --diag_warning=225 --big_endian --abi=eabi -z -m"BSLC6670_srio_discovery_ex_1.map" --warn_sections -i"D:/ti/C6000 Code Generation Tools 7.4.2/lib" -i"D:/ti/C6000 Code Generation Tools 7.4.2/include" -i"D:/CommAgility_Amc2c66xx_V0_4_9/dsp/dsplibs/lib" -i"D:/ti/pdk_C6670_1_1_2_6/packages/ti/csl/lib" --reread_libs --xml_link_info="BSLC6670_srio_discovery_ex_1_linkInfo.xml" --rom_model -o "BSLC6670_srio_discovery_ex_1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C6000_EXECUTABLE_OUTPUTS__QUOTED) "BSLC6670_srio_discovery_ex_1.out"
	-$(RM) "BSLC667X_srio_discovery_ex.pp" 
	-$(RM) "BSLC667X_srio_discovery_ex.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
