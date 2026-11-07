/**
 * @file SparkFun_CCS811.hpp
 * @brief Public driver declarations and configuration constants.
 */
#ifndef SPARKFUN_CCS811_HPP
#define SPARKFUN_CCS811_HPP

#include <nola-common.h>
#include <TwoWire.hpp>

//Register addresses
/** @brief CCS811 void register address. */
#define CSS811_void 0x00
/** @brief CCS811 meas mode register address. */
#define CSS811_MEAS_MODE 0x01
/** @brief CCS811 alg result data register address. */
#define CSS811_ALG_RESULT_DATA 0x02
/** @brief CCS811 raw data register address. */
#define CSS811_RAW_DATA 0x03
/** @brief CCS811 env data register address. */
#define CSS811_ENV_DATA 0x05
/** @brief CCS811 ntc register address. */
#define CSS811_NTC 0x06
/** @brief CCS811 thresholds register address. */
#define CSS811_THRESHOLDS 0x10
/** @brief CCS811 baseline register address. */
#define CSS811_BASELINE 0x11
/** @brief CCS811 hw id register address. */
#define CSS811_HW_ID 0x20
/** @brief CCS811 hw version register address. */
#define CSS811_HW_VERSION 0x21
/** @brief CCS811 fw boot version register address. */
#define CSS811_FW_BOOT_VERSION 0x23
/** @brief CCS811 fw app version register address. */
#define CSS811_FW_APP_VERSION 0x24
/** @brief CCS811 error id register address. */
#define CSS811_ERROR_ID 0xE0
/** @brief CCS811 app start register address. */
#define CSS811_APP_START 0xF4
/** @brief CCS811 sw reset register address. */
#define CSS811_SW_RESET 0xFF

/**
 * @brief Low-level CCS811 I2C register transport.
 * The current register helpers return SENSOR_SUCCESS without validating transfers.
 */
class CCS811Core{
public:
	// Return values
  /** @brief Status codes returned by register and sensor operations. */
	typedef enum{
		SENSOR_SUCCESS,
		SENSOR_ID_ERROR,
		SENSOR_I2C_ERROR,
		SENSOR_INTERNAL_ERROR,
		SENSOR_GENERIC_ERROR
 /**
  * @brief Status type used by CCS811 operations; SENSOR_SUCCESS is zero.
  */
	}status;

 /**
  * @brief Bind the I2C bus and 7-bit sensor address.
  */
	CCS811Core(TwoWire &, uint8_t addr);
 /**
  * @brief Destroy the transport wrapper without taking ownership of the bus.
  */
	~CCS811Core() = default;
 /**
  * @brief Legacy initialization declaration without an implementation; use CCS811::begin().
  */
	status beginCore( void );
 /**
  * @brief Read one register into outputPointer; storage must be non-null.
  */
	status readRegister( uint8_t offset, uint8_t* outputPointer);
 /**
  * @brief Request four register bytes and store at most length bytes in outputPointer.
The current implementation always requests four bytes, regardless of length.
  */
	status multiReadRegister(uint8_t offset, uint8_t *outputPointer, uint8_t length);
 /**
  * @brief Write dataToWrite to the register at offset.
  */
	status writeRegister(uint8_t offset, uint8_t dataToWrite);
 /**
  * @brief Write length bytes from inputPointer starting at offset.
  */
	status multiWriteRegister(uint8_t offset, uint8_t *inputPointer, uint8_t length);

protected:
	TwoWire &Wire;
	uint8_t I2CAddress;
};

/**
 * @brief CCS811 gas sensor with cached eCO2 and TVOC results.
 * Call begin(), allow measurements to accumulate, then readAlgorithmResults()
 * before reading the cached getters. The driver does not report transport failures.
 */
class CCS811: public CCS811Core{
public:
 /**
  * @brief Bind the I2C bus and 7-bit sensor address.
  */
	CCS811(TwoWire &, uint8_t addr);

 /**
  * @brief Initialize I2C, start the application, and select drive mode 1.
  * @return SENSOR_SUCCESS in the current implementation; no device ID validation.
  */
	status begin( void );
 /**
  * @brief Set the measurement mode field; values above 4 are clamped to 4.
  */
	status setDriveMode( uint8_t mode );
 /**
  * @brief Read algorithm registers and update cached eCO2/TVOC.
  * @return SENSOR_SUCCESS in the current implementation.
  */
	status readAlgorithmResults( void );

 /**
  * @brief Legacy readiness declaration without an implementation; do not call.
  */
	bool dataAvailable( void );

 /**
  * @brief Return cached total volatile organic compounds in ppb.
  */
	uint16_t getTVOC( void );
 /**
  * @brief Return cached equivalent CO2 concentration in ppm.
  */
	uint16_t getCO2e( void );

private:
	float refResistance;
	float resistance;
	float temperature;
	uint16_t tVOC;
	uint16_t CO2e;
};

#endif //SPARKFUN_CCS811_HPP
