
#ifndef __EVM_H__
#define __EVM_H__


#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <tistdtypes.h>


/****************************************************************************
 *                             Global constants                              *
 ****************************************************************************/
#define EVM_BSLVERSION "V0.1.3.0"   ///< EVM BSL VERSION

/** @name EVM error codes
 *  Evm functions error status codes 
 */
//@{
#define EVM_INITOK               0      ///< No error
#define EVM_INCORRECT_VERSION    0x0501 ///< Incorrect binary bsl version not matching .h
#define EVM_INCORRECT_DSPID      0x0502 ///< This function cannot be run on this DSP
#define EVM_ERRORREAD_VERSION    0x0503 ///< Problem while reading evm versions
#define EVM_INCOMPATIBLE_VERSION 0x0504 ///< Board/firmware version is incompatible with this function
#define EVM_ERRORREAD_PROG       0x0505 ///< Error while reading eeprom programming register
#define EVM_EEPROMPROG_ERR       0x0506 ///< Eeprom configuration programming/verification error
#define EVM_EEPROMPROG_TO        0x0507 ///< Eeprom configuration programming timeout error
#define EVM_ERRORWRITE_EEREG1    0x0508 ///< Problem #1 writing eeprom configuration programming register
#define EVM_ERRORWRITE_EEREG2    0x0509 ///< Problem #2 writing eeprom configuration programming register
#define EVM_INCOMPATIBLE_SAVECFG 0x050A ///< Board/firmware version is incompatible and cannot save in eeprom config
#define EVM_ERRORWRITE_SRIORST1  0x050B ///< Problem cannot release srio reset
#define EVM_ERRORWRITE_SRIORST2  0x050C ///< Problem cannot activates srio reset
#define EVM_ERRORWRITE_CFG2      0x050D ///< Problem cannot write config register #2
#define EVM_ERRORWRITE_CFG1      0x050E ///< Problem cannot write config register #1
#define EVM_ERRORWRITE_BOOTCFG1  0x050F ///< Problem cannot write eeprom boot config register1
#define EVM_ERRORWRITE_BOOTCFG2  0x0510 ///< Problem cannot write eeprom boot config register2
#define EVM_ERRORWRITE_BOOTCFG3  0x0511 ///< Problem cannot write eeprom boot config register3
#define EVM_ERRORWRITE_BOOTCFG4  0x0512 ///< Problem cannot write eeprom boot config register4
#define EVM_TIMEOUT_ERROR        0x0513 ///< Timeout occured in DSP initialization
#define EVM_ERROR_PSC_INIT       0x0514 ///< Timeout occured at DSP PSC initialization
#define EVM_PLLMULT_ERROR        0x0515 ///< Unsupported pll1 multiplier was used
//@}

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 *   NAME : EVM_init(void)
 ************************************************************************//**
 *
 *   Init the evm configuration and resources for modules. This function
 *      must be the first function always use before using any other function
 *      of the BSL. It will initialize also chip CSL interrupt settings, so
 *      it must be called first before using any interruption through
 *      the CSL outside of the BSL to avoid unexpected behaviors.
 *  
 *   @return   Error code
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/ 
Int32 EVM_init(void);


/****************************************************************************
 *   NAME : EVM_preinit(Uint32 cachel1d_cfg, Uint32 cachel1p_cfg, Uint32 cachel2_cfg, Uint32 pll1_multv)
 ************************************************************************//**
 *
 *   Basic pre-initialization for DSP (replacing GEL file initialization).
 *   This function is not necessary when the evm is used from CCS
 *   with a JTAG debugger. In this situation, dsp default initialization
 *   would be provided by the GEL file.
 *   If this function is needed, it must be called as the first function of
 *   the BSL before the normal EVM_init call. It will replace GEL initialization
 *   that is normally done first, before evm_init is called in the user program.
 *   List of initializations performed:
 *    1- DSP configure L1D, L1P and L2 cache memory size according to parameters
 *       (values are direct values associated with dsp cache setup registers)
 *    2- DSP Pll1 is set according to pll1_multv parameter (8 for 983 MHz = 8 * 122.88)
 *    3- DSP Turn on all power domains
 *    4- DSP Set DDR3 for 533 MHz
 *    5- DSP Set pass clk (pll3) to 1044 MHz (from external passclk input)
 *    6- Configure some IO and dsp pins settings
 *
 *    WARNINGS:
 *    - This function does not validate the parameters given.
 *    - Using this function while application is running from DDR3 may
 *      give unexpected results (possible program crash)
 *    - Memory for a specific internal memory section must not be
 *      configured as cache if the application program is using it for code/data ram.
 *    - This function is including embedded software loops, and have not been
 *      verified to run outside of an unitialized cpu pll (at 122.88 MHz) and without
 *      cpu cache activated, before calling this function.
 *
 *   Typical example for DSP running at 983 MHz, cache L1D = L1P = 32K, L2 All ram:
 *      EVM_preinit(7,7,0,8);
 *
 *   @param [in] cachel1d_cfg
 *      L1D cache size dsp reg (7 = 32K , 0 = 0K)
 *
 *   @param [in] cachel1p_cfg
 *      L1P cache size dsp reg (7 = 32K , 0 = 0K)
 *
 *   @param [in] cachel2_cfg
 *      L2 cache size dsp reg  (7 = All cache , 0 = ALL RAM)
 *
 *   @param [in] pll1_multv
 *      Core pll dsp multiplier reg +1 (8 for 983 MHz = 8*122.88 MHz)
 *  
 *   @return   Error code: EVM_INITOK if no problem is found, !EVM_INITOK if problem
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/ 
Int32 EVM_preinit(Uint32 cachel1d_cfg, Uint32 cachel1p_cfg, 
                  Uint32 cachel2_cfg, Uint32 pll1_multv);

 
 /****************************************************************************
 *   NAME : EVM_bslversion(void)
 ************************************************************************//**
 *
 *   Get the evm bsl version. It can be used to display the binary library
 *      version of the bsl.
 *      See function EVM_checkbslversion.
 *  
 *   @return   Character string holding the bsl version
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/ 
Int8 *EVM_bslversion(void);


 /****************************************************************************
 *   NAME : EVM_checkbslversion(Int8 *expectedversion)
 ************************************************************************//**
 *
 *   Verify that we have a correct binary bsl version matching .h definitions
 *      Call this function with the constant EVM_BSLVERSION as input
 *      parameters to get a version check status.
 *      EVM_BSLVERSION is in the evm.h file.
 *
 *   @param [in] expectedversion
 *      Character string holding the expected bsl version
 *  
 *   @return   Error code: EVM_INITOK if no problem is found, !EVM_INITOK if problem
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/ 
Int32 EVM_checkbslversion(Int8 *expectedversion);


/****************************************************************************
 *   NAME : EVM_hardwareinfo
 ************************************************************************//**
 * 
 *    Reads the firmware version
 *
 *   @param [out] model
 *      Pointer to the board model read
 *
 *   @param [out] version
 *      Pointer to the board version read
 *
 *   @return
 *      !=EVM_FPGA_READREG_FAIL for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/   
Int32 EVM_hardwareinfo(Uint8* model, Uint8* version);


/****************************************************************************
 *   NAME : EVM_firmwareversion
 ************************************************************************//**
 * 
 *    Reads the firmware version
 *
 *   @param [out] version
 *      Pointer to the firmware version read
 *
 *   @return
 *      !=EVM_FPGA_READREG_FAIL for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/   
Int32 EVM_firmwareversion(Uint8* version);


/****************************************************************************
 *   NAME : EVM_dspid
 ************************************************************************//**
 *
 *    Reads the DSP id
 *
 *   @param [out] )
 *      Pointer to the ID read
 *
 *   @return
 *      !=EVM_FPGA_READREG_FAIL for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM
 *
 ***************************************************************************/   
Int32 EVM_dspid(Uint8* id);


#ifdef __cplusplus
} 
#endif  // extern "C"

#endif //__EVM__H__
   






































