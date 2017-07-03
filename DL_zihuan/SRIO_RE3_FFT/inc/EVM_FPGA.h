

#ifndef __EVM_FPGA_H__
#define __EVM_FPGA_H__


#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <tistdtypes.h>

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

/** @name EVM_FPGA_error_codes
 *  EVM FPGA functions error status codes.
 */
//@{
#define EVM_FPGA_OK                0        ///<  FPGA No Error Code
#define EVM_FPGA_WRITEREG_FAIL     0x1000   ///<  FPGA Write Failure Error
#define EVM_FPGA_READREG_FAIL      0x1002   ///<  FPGA Read Failure Error
//@}


/** @name EVM_FPGA_regs
 *  FPGA register definitions
 */
//@{
    
/** @name EVM_FPGA_Register_0
 *  Reg 0 :  PCB version and board model
 */
//@{
#define EVM_FPGA_REG_PCB_VERSION              (0x00)  ///< PCB version status register
                                        
#define EVM_FPGA_PCB_VERSION_MASK           (0xF0)  ///< PCB Version Mask
#define EVM_FPGA_PCB_VERSION_SHIFT          (4)  ///< PCB Version Shift
#define EVM_FPGA_BOARDMODEL_MASK            (0x0C)  ///< Board model Version Mask
#define EVM_FPGA_BOARDMODEL_SHIFT           (2)  ///< Board model Shift
//@}     
  
//@{
/** @name EVM_FPGA_Register_1
 *  Reg 1 : FPGA version 
 */
//@{
#define EVM_FPGA_REG_FIRMWARWE_VERSION         	(0x01)  ///< FPGA version status register
                                        
#define EVM_FPGA_FIRMWARE_VERSION_MASK        	(0x1F)  ///< PCB Version Mask
#define EVM_FPGA_FIRMWARE_VERSION_SHIFT       	(0)
//@}    

//@{
/** @name EVM_FPGA_Register_2
 *  Reg 2 : LED Control
 */
//@{
#define EVM_FPGA_REG_LED_CONTROL        		(0x02)  ///< LED Control register
                                        
#define EVM_FPGA_OVERRIDELED_MASK	       		(0x10)  ///< Override LED
#define EVM_FPGA_OVERRIDELED_SHIFT	       		(4)
#define EVM_FPGA_OVERRIDELED_DSP	       		(0x10)
#define EVM_FPGA_OVERRIDELED_JTAG	       		(0)

#define EVM_FPGA_LED2_MASK	       				(0x0C)  ///< LED2
#define EVM_FPGA_LED2_SHIFT	       				(2)
#define	EVM_FPGA_LED2_OFF  	                    (0)
#define	EVM_FPGA_LED2_GREEN                     (0x08)
#define	EVM_FPGA_LED2_RED  	                    (0x04)
#define	EVM_FPGA_LED2_YELLOW                    (0x0C)

#define EVM_FPGA_LED1_MASK	       				(0x03)  ///< LED1
#define EVM_FPGA_LED1_SHIFT	       				(0)
#define	EVM_FPGA_LED1_OFF  	                    (0)
#define	EVM_FPGA_LED1_GREEN                     (0x02)
#define	EVM_FPGA_LED1_RED  	                    (0x01)
#define	EVM_FPGA_LED1_YELLOW                    (0x03)
//@}    

//@{
/** @name EVM_FPGA_Register_6
 *  Reg 7 : Dip switch 1
 */
//@{
#define EVM_FPGA_REG_DIP_SW_1		         	(0x06)  ///< 
                                        
#define EVM_FPGA_JTAG_EMULATION_MASK        	(0xE0)  ///< 
#define EVM_FPGA_JTAG_EMULATION_SHIFT		    (5)
#define EVM_FPGA_JTAG_EMULATION_USBDSP1        	(0x00)  ///< 
#define EVM_FPGA_JTAG_EMULATION_USBDAISY	    (0x20)
#define EVM_FPGA_JTAG_EMULATION_USBDSP2        	(0x40)  ///< 
#define EVM_FPGA_JTAG_EMULATION_HEADERSTRAIGHT  (0x80)
#define EVM_FPGA_JTAG_EMULATION_HEADERDAISY     (0xC0)  ///< 

#define EVM_FPGA_ENDIANNESS_MASK        		(0x10)  ///< 
#define EVM_FPGA_ENDIANNESS_SHIFT		    	(4)
#define EVM_FPGA_ENDIANNESS_SMALL		       	(0)
#define EVM_FPGA_ENDIANNESS_BIG	       			(0x10)

#define EVM_FPGA_DSP1_RESET_SELECT_MASK         (0x08)  ///< 
#define EVM_FPGA_DSP1_RESET_SELECT_SHIFT	    (3)
#define EVM_FPGA_RESET_SELECT_WRST		       	(0)
#define EVM_FPGA_RESET_SELECT_POR		       	(0x08)

#define EVM_FPGA_DSP1_BOOTMODE_MASK        		(0x07)  ///< 
#define EVM_FPGA_DSP1_BOOTMODE_SHIFT	       	(0)

#define EVM_FPGA_BOOTMODE_SLEEP			       	(0)
#define EVM_FPGA_BOOTMODE_SRIO			       	(1)
#define EVM_FPGA_BOOTMODE_SGMII_CORE	       	(2)
#define EVM_FPGA_BOOTMODE_SGMII_PA		       	(3)
#define EVM_FPGA_BOOTMODE_PCIE			       	(4)
#define EVM_FPGA_BOOTMODE_PCIE2			       	(5)
#define EVM_FPGA_BOOTMODE_SPI			       	(6)
#define EVM_FPGA_BOOTMODE_MCM			       	(7)

//@{
/** @name EVM_FPGA_Register_7
 *  Reg 7 : Dip switch 2
 */
//@{
#define EVM_FPGA_REG_DIP_SW_2		         	(0x07)  ///< 
                                        
#define EVM_FPGA_USER_SWITCH_MASK        		(0x20)  ///< 
#define EVM_FPGA_USER_SWITCH_SHIFT		       	(5)
#define EVM_FPGA_FACTORY_DEFAULT_MASK        	(0x10)  ///< 
#define EVM_FPGA_FACTORY_DEFAULT_SHIFT		    (4)
#define EVM_FPGA_FACTORY_DEFAULT_FACTORY		(0)
#define EVM_FPGA_FACTORY_DEFAULT_FLASH		    (0x10)

#define EVM_FPGA_DSP2_RESET_SELECT_MASK         (0x08)  ///< 
#define EVM_FPGA_DSP2_RESET_SELECT_SHIFT	    (3)
#define EVM_FPGA_DSP2_BOOTMODE_MASK        		(0x07)  ///< 
#define EVM_FPGA_DSP2_BOOTMODE_SHIFT	       	(0)

//@{
/** @name EVM_FPGA_DSP1_GPIO_STATUS_REG1
 *  Reg 8 : DSP1 GPIO status register1
 *          GPIO07-00
 */
//@{
#define EVM_FPGA_DSP1_GPIO_STATUS_REG1        	(0x08)  ///< 
                                        
//@{
/** @name EVM_FPGA_DSP1_GPIO_STATUS_REG2
 *  Reg 9 : DSP1 GPIO status register2
 *          GPIO15-08
 */
//@{
#define EVM_FPGA_DSP1_GPIO_STATUS_REG2        	(0x09)  ///< 
                                        
//@{
/** @name EVM_FPGA_DSP2_GPIO_STATUS_REG1
 *  Reg 8 : DSP2 GPIO status register1
 *          GPIO07-00
 */
//@{
#define EVM_FPGA_DSP2_GPIO_STATUS_REG1        	(0x0a)  ///< 
                                        
//@{
/** @name EVM_FPGA_DSP2_GPIO_STATUS_REG2
 *  Reg 9 : DSP2 GPIO status register2
 *          GPIO15-08
 */
//@{
#define EVM_FPGA_DSP2_GPIO_STATUS_REG2        	(0x0b)  ///< 
                                        

//@}   
//@{
/** @name EVM_FPGA_Register_15
 *  Reg 15 : DSP specific register
 */
//@{
#define EVM_FPGA_REG_DSP_SPECIFIC                  (0x0F)  	///< DSP specific register
                                        
#define EVM_FPGA_DSP_IDENTIFICATION_MASK           (0x80)  	///< DSP identification mask
#define EVM_FPGA_DSP_IDENTIFICATION_SHIFT          (7) 		///< DSP identification shift
#define EVM_FPGA_DSP_LED_MASK                      (0x60)  	///< DSP led mask
#define EVM_FPGA_DSP_LED_SHIFT                     (5)  	///< DSP led mask
#define	EVM_FPGA_DSP_LED_OFF  	                   (0)
#define	EVM_FPGA_DSP_LED_GREEN                     (0x40)
#define	EVM_FPGA_DSP_LED_RED  	                   (0x20)
#define	EVM_FPGA_DSP_LED_YELLOW                    (0x60)
#define EVM_FPGA_MDIO_REQUEST_MASK                 (0x10)   ///< 
#define EVM_FPGA_MDIO_REQUEST_ASK                  (0x10)   ///< 
#define EVM_FPGA_MDIO_REQUEST_RELEASE              (0x00)   ///<
#define EVM_FPGA_MDIO_REQUEST_SHIFT                (4)      ///< 
#define EVM_FPGA_MDIO_ACKNOWLEDGE_MASK             (0x08)   ///< 
#define EVM_FPGA_MDIO_ACKNOWLEDGE_SHIFT            (3)      ///<
#define EVM_FPGA_EEPROM_WP_MASK            		   (0x02)   ///< 
#define EVM_FPGA_EEPROM_WP_SHIFT            	   (1)      ///<  
//@}

//@{
/** @name EVM_FPGA_Register_21
 *  Reg 21 : Misc register
 */
//@{
#define EVM_FPGA_REG_MISC                  	(0x15)   ///< Misc register  
#define EVM_FPGA_NAND_WP_MASK            	(0x01)   ///< 
#define EVM_FPGA_NAND_WP_SHIFT            	(0)      ///<  
//@}  


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 *   NAME : EVM_FPGA_writereg
 ************************************************************************//**
 *
 *   This function writes a value to a register of the FPGA device using the I2C bus.
 *
 *   @param [in] reg
 *      Register offset to write
 *  
 *   @param [in] val
 *      Value (byte) to write
 *
 *   @return
 *      !=EVM_FPGA_OK for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM_FPGA
 *
 ***************************************************************************/ 
Int32 EVM_FPGA_writereg(Uint8 reg, Uint8 val);
 
 
 /****************************************************************************
 *   NAME : EVM_FPGA_readreg
 ************************************************************************//**
 *
 *   This function reads the value of a register of the FPGA device using the I2C bus.
 *
 *   @param [in] reg
 *      Register offset to read
 *  
 *   @param [out] val
 *      Value (byte) read
 *
 *   @return
 *      !=EVM_FPGA_OK for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM_FPGA
 *
 ***************************************************************************/ 
Int32 EVM_FPGA_readreg(Uint8 reg, Uint8 *val );

 
  /****************************************************************************
 *   NAME : EVM_FPGA_readwritereg
 ************************************************************************//**
 *
 *   Read a FPGA register, change a bitfield and then writeback the register.
 *      
 *   Bitmask values and bitval are binary value with 1 set at the bit position
 *   to modify in the register.
 *   Pseudocode operation:
 *       1- Value1 = Read FPGA register regnum
 *       2- Value1 = Value1 & (~bitmask)
 *       3- Value1 = Value1 | (bitmask & bitval)
 *       4- Write ECP register regnum with Value1
 *
 *   @param [in] reg
 *      Register offset (8 bits).
 *
 *   @param [in] bitmask
 *     Bit mask value (8 bits) with bit to keep set as 1.
 *  
 *   @param [in] bitval
 *     Bit values (8 bits) to be set at mask position.
 *
 *   @return
 *      !=EVM_FPGA_OK for failure, EVM_FPGA_OK for success
 *   
 *   @ingroup EVM_FPGA
 *
 ***************************************************************************/   
Int32 EVM_FPGA_readwritereg( Uint8 reg, Uint8 bitmask, Uint8 bitval );


#ifdef __cplusplus
} 
#endif  // extern "C"

#endif //__EVM_FPGA_H__
   






































