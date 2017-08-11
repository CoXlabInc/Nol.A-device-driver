/******************************************************************************
SparkFunBQ27441.hpp
BQ27441 Nol.A-SDK Library Main Header File
Jongsoo Jeong @ CoXlab Inc.
Aug 11, 2017

It is converted for Nol.A-SDK from ...

SparkFunBQ27441.h
BQ27441 Arduino Library Main Header File
Jim Lindblom @ SparkFun Electronics
May 9, 2016
https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library

Definition of the BQ27441 library, which implements all features of the
BQ27441 LiPo Fuel Gauge.

Hardware Resources:
- Arduino Development Board
- SparkFun Battery Babysitter

Development environment specifics:
Arduino 1.6.7
SparkFun Battery Babysitter v1.0
Arduino Uno (any 'duino should do)

The MIT License (MIT)

Copyright (c) 2016 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef SparkFunBQ27441_hpp
#define SparkFunBQ27441_hpp

#include <cox.h>

#define BQ72441_I2C_TIMEOUT 2000

class BQ27441 {
public:
	// Parameters for the current() function, to specify which current to read
	typedef enum {
		AVG,  // Average Current (DEFAULT)
		STBY, // Standby Current
		MAX   // Max Current
	} current_measure;

	// Parameters for the capacity() function, to specify which capacity to read
	typedef enum {
		REMAIN,     // Remaining Capacity (DEFAULT)
		FULL,       // Full Capacity
		AVAIL,      // Available Capacity
		AVAIL_FULL, // Full Available Capacity
		REMAIN_F,   // Remaining Capacity Filtered
		REMAIN_UF,  // Remaining Capacity Unfiltered
		FULL_F,     // Full Capacity Filtered
		FULL_UF,    // Full Capacity Unfiltered
		DESIGN      // Design Capacity
	} capacity_measure;

	// Parameters for the soc() function
	typedef enum {
		FILTERED,  // State of Charge Filtered (DEFAULT)
		UNFILTERED // State of Charge Unfiltered
	} soc_measure;

	// Parameters for the soh() function
	typedef enum {
		PERCENT,  // State of Health Percentage (DEFAULT)
		SOH_STAT  // State of Health Status Bits
	} soh_measure;

	// Parameters for the temperature() function
	typedef enum {
		BATTERY,      // Battery Temperature (DEFAULT)
		INTERNAL_TEMP // Internal IC Temperature
	} temp_measure;

	// Parameters for the setGPOUTFunction() funciton
	typedef enum {
		SOC_INT, // Set GPOUT to SOC_INT functionality
		BAT_LOW  // Set GPOUT to BAT_LOW functionality
	} gpout_function;

	//////////////////////////////
	// Initialization Functions //
	//////////////////////////////
	/**
	    Initializes class variables
	*/
	BQ27441(TwoWire &, uint8_t addr = I2C_ADDRESS);

	/**
	    Initializes I2C and verifies communication with the BQ27441.
		Must be called before using any other functions.

		@return true if communication was successful.
	*/
	bool begin(void);

	/**
	    Configures the design capacity of the connected battery.

		@param capacity of battery (unsigned 16-bit value)
		@return true if capacity successfully set.
	*/
	bool setCapacity(uint16_t capacity);

	/////////////////////////////
	// Battery Characteristics //
	/////////////////////////////
	/**
	    Reads and returns the battery voltage

		@return battery voltage in mV
	*/
	uint16_t voltage(void);

	/**
	    Reads and returns the specified current measurement

		@param current_measure enum specifying current value to be read
		@return specified current measurement in mA. >0 indicates charging.
	*/
	int16_t current(current_measure type = AVG);

	/**
	    Reads and returns the specified capacity measurement

		@param capacity_measure enum specifying capacity value to be read
		@return specified capacity measurement in mAh.
	*/
	uint16_t capacity(capacity_measure type = REMAIN);

	/**
	    Reads and returns measured average power

		@return average power in mAh. >0 indicates charging.
	*/
	int16_t power(void);

	/**
	    Reads and returns specified state of charge measurement

		@param soc_measure enum specifying filtered or unfiltered measurement
		@return specified state of charge measurement in %
	*/
	uint16_t soc(soc_measure type = FILTERED);

	/**
	    Reads and returns specified state of health measurement

		@param soh_measure enum specifying filtered or unfiltered measurement
		@return specified state of health measurement in %, or status bits
	*/
	uint8_t soh(soh_measure type = PERCENT);

	/**
	    Reads and returns specified temperature measurement

		@param temp_measure enum specifying internal or battery measurement
		@return specified temperature measurement in degrees C
	*/
	uint16_t temperature(temp_measure type = BATTERY);

	////////////////////////////
	// GPOUT Control Commands //
	////////////////////////////
	/**
	    Get GPOUT polarity setting (active-high or active-low)

		@return true if active-high, false if active-low
	*/
	bool GPOUTPolarity(void);

	/**
	    Set GPOUT polarity to active-high or active-low

		@param activeHigh is true if active-high, false if active-low
		@return true on success
	*/
	bool setGPOUTPolarity(bool activeHigh);

	/**
	    Get GPOUT function (BAT_LOW or SOC_INT)

		@return true if BAT_LOW or false if SOC_INT
	*/
	bool GPOUTFunction(void);

	/**
	    Set GPOUT function to BAT_LOW or SOC_INT

		@param function should be either BAT_LOW or SOC_INT
		@return true on success
	*/
	bool setGPOUTFunction(gpout_function function);

	/**
	    Get SOC1_Set Threshold - threshold to set the alert flag

		@return state of charge value between 0 and 100%
	*/
	uint8_t SOC1SetThreshold(void);

	/**
	    Get SOC1_Clear Threshold - threshold to clear the alert flag

		@return state of charge value between 0 and 100%
	*/
	uint8_t SOC1ClearThreshold(void);

	/**
	    Set the SOC1 set and clear thresholds to a percentage

		@param set and clear percentages between 0 and 100. clear > set.
		@return true on success
	*/
	bool setSOC1Thresholds(uint8_t set, uint8_t clear);

	/**
	    Get SOCF_Set Threshold - threshold to set the alert flag

		@return state of charge value between 0 and 100%
	*/
	uint8_t SOCFSetThreshold(void);

	/**
	    Get SOCF_Clear Threshold - threshold to clear the alert flag

		@return state of charge value between 0 and 100%
	*/
	uint8_t SOCFClearThreshold(void);

	/**
	    Set the SOCF set and clear thresholds to a percentage

		@param set and clear percentages between 0 and 100. clear > set.
		@return true on success
	*/
	bool setSOCFThresholds(uint8_t set, uint8_t clear);

	/**
	    Check if the SOC1 flag is set in flags()

		@return true if flag is set
	*/
	bool socFlag(void);

	/**
	    Check if the SOCF flag is set in flags()

		@return true if flag is set
	*/
	bool socfFlag(void);

	/**
	    Get the SOC_INT interval delta

		@return interval percentage value between 1 and 100
	*/
	uint8_t sociDelta(void);

	/**
	    Set the SOC_INT interval delta to a value between 1 and 100

		@param interval percentage value between 1 and 100
		@return true on success
	*/
	bool setSOCIDelta(uint8_t delta);

	/**
	    Pulse the GPOUT pin - must be in SOC_INT mode

		@return true on success
	*/
	bool pulseGPOUT(void);

	//////////////////////////
	// Control Sub-commands //
	//////////////////////////

	/**
	    Read the device type - should be 0x0421

		@return 16-bit value read from DEVICE_TYPE subcommand
	*/
	uint16_t deviceType(void);

	/**
	    Enter configuration mode - set userControl if calling from an Arduino
		sketch and you want control over when to exitConfig.

		@param userControl is true if the Arduino sketch is handling entering
		and exiting config mode (should be false in library calls).
		@return true on success
	*/
	bool enterConfig(bool userControl = true);

	/**
	    Exit configuration mode with the option to perform a resimulation

		@param resim is true if resimulation should be performed after exiting
		@return true on success
	*/
	bool exitConfig(bool resim = true);

	/**
	    Read the flags() command

		@return 16-bit representation of flags() command register
	*/
	uint16_t flags(void);

	/**
	    Read the CONTROL_STATUS subcommand of control()

		@return 16-bit representation of CONTROL_STATUS subcommand
	*/
	uint16_t status(void);

private:
	TwoWire &wire;
	uint8_t _deviceAddress;  // Stores the BQ27441-G1A's I2C address
	bool _sealFlag; // Global to identify that IC was previously sealed
	bool _userConfigControl; // Global to identify that user has control over
	                         // entering/exiting config

	/**
	    Check if the BQ27441-G1A is sealed or not.

		@return true if the chip is sealed
	*/
	bool sealed(void);

	/**
	    Seal the BQ27441-G1A

		@return true on success
	*/
	bool seal(void);

	/**
	    UNseal the BQ27441-G1A

		@return true on success
	*/
	bool unseal(void);

	/**
	    Read the 16-bit opConfig register from extended data

		@return opConfig register contents
	*/
	uint16_t opConfig(void);

	/**
	    Write the 16-bit opConfig register in extended data

		@param New 16-bit value for opConfig
		@return true on success
	*/
	bool writeOpConfig(uint16_t value);

	/**
	    Issue a soft-reset to the BQ27441-G1A

		@return true on success
	*/
	bool softReset(void);

	/**
	    Read a 16-bit command word from the BQ27441-G1A

		@param subAddress is the command to be read from
		@return 16-bit value of the command's contents
	*/
	uint16_t readWord(uint16_t subAddress);

	/**
	    Read a 16-bit subcommand() from the BQ27441-G1A's control()

		@param function is the subcommand of control() to be read
		@return 16-bit value of the subcommand's contents
	*/
	uint16_t readControlWord(uint16_t function);

	/**
	    Execute a subcommand() from the BQ27441-G1A's control()

		@param function is the subcommand of control() to be executed
		@return true on success
	*/
	bool executeControlWord(uint16_t function);

	////////////////////////////
	// Extended Data Commands //
	////////////////////////////
	/**
	    Issue a BlockDataControl() command to enable BlockData access

		@return true on success
	*/
	bool blockDataControl(void);

	/**
	    Issue a DataClass() command to set the data class to be accessed

		@param id is the id number of the class
		@return true on success
	*/
	bool blockDataClass(uint8_t id);

	/**
	    Issue a DataBlock() command to set the data block to be accessed

		@param offset of the data block
		@return true on success
	*/
	bool blockDataOffset(uint8_t offset);

	/**
	    Read the current checksum using BlockDataCheckSum()

		@return true on success
	*/
	uint8_t blockDataChecksum(void);

	/**
	    Use BlockData() to read a byte from the loaded extended data

		@param offset of data block byte to be read
		@return true on success
	*/
	uint8_t readBlockData(uint8_t offset);

	/**
	    Use BlockData() to write a byte to an offset of the loaded data

		@param offset is the position of the byte to be written
		       data is the value to be written
		@return true on success
	*/
	bool writeBlockData(uint8_t offset, uint8_t data);

	/**
	    Read all 32 bytes of the loaded extended data and compute a
		checksum based on the values.

		@return 8-bit checksum value calculated based on loaded data
	*/
	uint8_t computeBlockChecksum(void);

	/**
	    Use the BlockDataCheckSum() command to write a checksum value

		@param csum is the 8-bit checksum to be written
		@return true on success
	*/
	bool writeBlockChecksum(uint8_t csum);

	/**
	    Read a byte from extended data specifying a class ID and position offset

		@param classID is the id of the class to be read from
		       offset is the byte position of the byte to be read
		@return 8-bit value of specified data
	*/
	uint8_t readExtendedData(uint8_t classID, uint8_t offset);

	/**
	    Write a specified number of bytes to extended data specifying a
		class ID, position offset.

		@param classID is the id of the class to be read from
		       offset is the byte position of the byte to be read
			   data is the data buffer to be written
			   len is the number of bytes to be written
		@return true on success
	*/
	bool writeExtendedData(uint8_t classID, uint8_t offset, uint8_t * data, uint8_t len);

	/////////////////////////////////
	// I2C Read and Write Routines //
	/////////////////////////////////

	/**
	    Read a specified number of bytes over I2C at a given subAddress

		@param subAddress is the 8-bit address of the data to be read
		       dest is the data buffer to be written to
			   count is the number of bytes to be read
		@return true on success
	*/
	int16_t i2cReadBytes(uint8_t subAddress, uint8_t * dest, uint8_t count);

	/**
	    Write a specified number of bytes over I2C to a given subAddress

		@param subAddress is the 8-bit address of the data to be written to
		       src is the data buffer to be written
			   count is the number of bytes to be written
		@return true on success
	*/
	uint16_t i2cWriteBytes(uint8_t subAddress, uint8_t * src, uint8_t count);

	enum {
		I2C_ADDRESS = 0x55, // Default I2C address of the BQ27441-G1A

		///////////////////////
		// General Constants //
		///////////////////////
		UNSEAL_KEY = 0x8000, // Secret code to unseal the BQ27441-G1A
		DEVICE_ID	 = 0x0421, // Default device ID

		///////////////////////
		// Standard Commands //
		///////////////////////
		// The fuel gauge uses a series of 2-byte standard commands to enable system
		// reading and writing of battery information. Each command has an associated
		// sequential command-code pair.
		COMMAND_CONTROL			    = 0x00, // Control()
		COMMAND_TEMP			      = 0x02, // Temperature()
		COMMAND_VOLTAGE			    = 0x04, // Voltage()
		COMMAND_FLAGS			      = 0x06, // Flags()
		COMMAND_NOM_CAPACITY	  = 0x08, // NominalAvailableCapacity()
		COMMAND_AVAIL_CAPACITY	= 0x0A, // FullAvailableCapacity()
		COMMAND_REM_CAPACITY	  = 0x0C, // RemainingCapacity()
		COMMAND_FULL_CAPACITY	  = 0x0E, // FullChargeCapacity()
		COMMAND_AVG_CURRENT		  = 0x10, // AverageCurrent()
		COMMAND_STDBY_CURRENT	  = 0x12, // StandbyCurrent()
		COMMAND_MAX_CURRENT		  = 0x14, // MaxLoadCurrent()
		COMMAND_AVG_POWER		    = 0x18, // AveragePower()
		COMMAND_SOC				      = 0x1C, // StateOfCharge()
		COMMAND_INT_TEMP		    = 0x1E, // InternalTemperature()
		COMMAND_SOH				      = 0x20, // StateOfHealth()
		COMMAND_REM_CAP_UNFL	  = 0x28, // RemainingCapacityUnfiltered()
		COMMAND_REM_CAP_FIL		  = 0x2A, // RemainingCapacityFiltered()
		COMMAND_FULL_CAP_UNFL	  = 0x2C, // FullChargeCapacityUnfiltered()
		COMMAND_FULL_CAP_FIL	  = 0x2E, // FullChargeCapacityFiltered()
		COMMAND_SOC_UNFL		    = 0x30, // StateOfChargeUnfiltered()

		//////////////////////////
		// Control Sub-commands //
		//////////////////////////
		// Issuing a Control() command requires a subsequent 2-byte subcommand. These
		// additional bytes specify the particular control function desired. The
		// Control() command allows the system to control specific features of the fuel
		// gauge during normal operation and additional features when the device is in
		// different access modes.
		CONTROL_STATUS					= 0x00,
		CONTROL_DEVICE_TYPE			= 0x01,
		CONTROL_FW_VERSION			= 0x02,
		CONTROL_DM_CODE					= 0x04,
		CONTROL_PREV_MACWRITE		= 0x07,
		CONTROL_CHEM_ID					= 0x08,
		CONTROL_BAT_INSERT			= 0x0C,
		CONTROL_BAT_REMOVE			= 0x0D,
		CONTROL_SET_HIBERNATE		= 0x11,
		CONTROL_CLEAR_HIBERNATE	= 0x12,
		CONTROL_SET_CFGUPDATE	  = 0x13,
		CONTROL_SHUTDOWN_ENABLE	= 0x1B,
		CONTROL_SHUTDOWN		    = 0x1C,
		CONTROL_SEALED			    = 0x20,
		CONTROL_PULSE_SOC_INT	  = 0x23,
		CONTROL_RESET						= 0x41,
		CONTROL_SOFT_RESET			= 0x42,
		CONTROL_EXIT_CFGUPDATE	= 0x43,
		CONTROL_EXIT_RESIM			= 0x44,

		///////////////////////////////////////////
		// Control Status Word - Bit Definitions //
		///////////////////////////////////////////
		// Bit positions for the 16-bit data of CONTROL_STATUS.
		// CONTROL_STATUS instructs the fuel gauge to return status information to
		// Control() addresses 0x00 and 0x01. The read-only status word contains status
		// bits that are set or cleared either automatically as conditions warrant or
		// through using specified subcommands.
		STATUS_SHUTDOWNEN	= (1<<15),
		STATUS_WDRESET		= (1<<14),
		STATUS_SS					= (1<<13),
		STATUS_CALMODE		=	(1<<12),
		STATUS_CCA				= (1<<11),
		STATUS_BCA				= (1<<10),
		STATUS_QMAX_UP		=	(1<<9),
		STATUS_RES_UP			= (1<<8),
		STATUS_INITCOMP		= (1<<7),
		STATUS_HIBERNATE	= (1<<6),
		STATUS_SLEEP			= (1<<4),
		STATUS_LDMD				= (1<<3),
		STATUS_RUP_DIS		= (1<<2),
		STATUS_VOK				= (1<<1),

		////////////////////////////////////
		// Flag Command - Bit Definitions //
		////////////////////////////////////
		// Bit positions for the 16-bit data of Flags()
		// This read-word function returns the contents of the fuel gauging status
		// register, depicting the current operating status.
		FLAG_OT					= (1<<15),
		FLAG_UT					= (1<<14),
		FLAG_FC					= (1<<9),
		FLAG_CHG				= (1<<8),
		FLAG_OCVTAKEN		= (1<<7),
		FLAG_ITPOR			= (1<<5),
		FLAG_CFGUPMODE	= (1<<4),
		FLAG_BAT_DET		= (1<<3),
		FLAG_SOC1				= (1<<2),
		FLAG_SOCF				= (1<<1),
		FLAG_DSG				= (1<<0),

		////////////////////////////
		// Extended Data Commands //
		////////////////////////////
		// Extended data commands offer additional functionality beyond the standard
		// set of commands. They are used in the same manner; however, unlike standard
		// commands, extended commands are not limited to 2-byte words.
		EXTENDED_OPCONFIG	  = 0x3A, // OpConfig()
		EXTENDED_CAPACITY	  = 0x3C, // DesignCapacity()
		EXTENDED_DATACLASS	= 0x3E, // DataClass()
		EXTENDED_DATABLOCK  = 0x3F, // DataBlock()
		EXTENDED_BLOCKDATA	= 0x40, // BlockData()
		EXTENDED_CHECKSUM	  = 0x60, // BlockDataCheckSum()
		EXTENDED_CONTROL	  = 0x61, // BlockDataControl()

		////////////////////////////////////////
		// Configuration Class, Subclass ID's //
		////////////////////////////////////////
		// To access a subclass of the extended data, set the DataClass() function
		// with one of these values.
		// Configuration Classes
		ID_SAFETY			     = 2,   // Safety
		ID_CHG_TERMINATION = 36,  // Charge Termination
		ID_CONFIG_DATA		 = 48,  // Data
		ID_DISCHARGE		   = 49,  // Discharge
		ID_REGISTERS		   = 64,  // Registers
		ID_POWER			     = 68,  // Power
		// Gas Gauging Classes
		ID_IT_CFG			     = 80,  // IT Cfg
		ID_CURRENT_THRESH	 = 81,  // Current Thresholds
		ID_STATE			     = 82,  // State
		// Ra Tables Classes
		ID_R_A_RAM			   = 89,  // R_a RAM
		// Calibration Classes
		ID_CALIB_DATA		   = 104, // Data
		ID_CC_CAL			     = 105, // CC Cal
		ID_CURRENT			   = 107, // Current
		// Security Classes
		ID_CODES			     = 112, // Codes

		/////////////////////////////////////////
		// OpConfig Register - Bit Definitions //
		/////////////////////////////////////////
		// Bit positions of the OpConfig Register
		OPCONFIG_BIE       = (1<<13),
		OPCONFIG_BI_PU_EN  = (1<<12),
		OPCONFIG_GPIOPOL   = (1<<11),
		OPCONFIG_SLEEP     = (1<<5),
		OPCONFIG_RMFCC     = (1<<4),
		OPCONFIG_BATLOWEN  = (1<<2),
		OPCONFIG_TEMPS     = (1<<0),
	};
};

#endif //SparkFunBQ27441_hpp
