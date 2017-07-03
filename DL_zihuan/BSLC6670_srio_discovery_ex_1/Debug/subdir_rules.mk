################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
BSLC667X_srio_discovery_ex.obj: ../BSLC667X_srio_discovery_ex.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/ti/C6000 Code Generation Tools 7.4.2/bin/cl6x" -mv6600 -g --include_path="D:/ti/C6000 Code Generation Tools 7.4.2/include" --include_path="D:/CommAgility_Amc2c66xx_V0_4_9/dsp/dsplibs/inc" --include_path="D:/ti/pdk_C6670_1_1_2_6/packages" --include_path="D:/ti/pdk_C6670_1_1_2_5/packages/ti/csl" --include_path="D:/ti/pdk_C6670_1_1_2_5/packages" --include_path="D:/CommAgility_Amc2c66xx_V0_4_9/dsp/dsplibs/inc" --display_error_number --diag_warning=225 --big_endian --abi=eabi --preproc_with_compile --preproc_dependency="BSLC667X_srio_discovery_ex.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


