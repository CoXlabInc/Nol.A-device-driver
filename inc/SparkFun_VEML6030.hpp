/**
 * @file SparkFun_VEML6030.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#pragma once

#include <TwoWire.hpp>
#include <cox.h>

/** @brief Enable a VEML6030 register feature. */
#define ENABLE        0x01
/** @brief Disable a VEML6030 register feature. */
#define DISABLE       0x00
/** @brief VEML6030 shutdown bit value. */
#define SHUTDOWN      0x01
/** @brief VEML6030 powered-on bit value. */
#define POWER         0x00
/** @brief No decoded VEML6030 interrupt. */
#define NO_INT        0x00
/** @brief Decoded VEML6030 upper-threshold interrupt. */
#define INT_HIGH      0x01
/** @brief Decoded VEML6030 lower-threshold interrupt. */
#define INT_LOW       0x02
/** @brief VEML6030 invalid-setting sentinel, 0xFF. */
#define UNKNOWN_ERROR 0xFF

// 7-Bit address options
/**
 * @brief Default 7-bit sensor address, 0x48.
 */
const uint8_t defAddr = 0x48;
/**
 * @brief Alternative 7-bit sensor address, 0x10.
 */
const uint8_t altAddr = 0x10;

/**
 * @brief VEML6030 register addresses.
 */
enum VEML6030_16BIT_REGISTERS {
  SETTING_REG            = 0x00,
  H_THRESH_REG,
  L_THRESH_REG,
  POWER_SAVE_REG, 
  AMBIENT_LIGHT_DATA_REG,
  WHITE_LIGHT_DATA_REG,
  INTERRUPT_REG

};

/**
 * @brief Masks retaining unrelated VEML6030 register bits.
 */
enum VEML6030_16BIT_REG_MASKS {
  THRESH_MASK            = 0x0,
  GAIN_MASK              = 0xE7FF,
  INTEG_MASK             = 0xFC3F,
  PERS_PROT_MASK         = 0xFFCF,
  INT_EN_MASK            = 0xFFFD,
  SD_MASK                = 0xFFFE,
  POW_SAVE_EN_MASK       = 0x06, // Most of this register is reserved
  POW_SAVE_MASK          = 0x01, // Most of this register is reserved
  INT_MASK               = 0xC000    
  
};

/**
 * @brief Bit positions of VEML6030 register fields.
 */
enum REGISTER_BIT_POSITIONS {
  NO_SHIFT               = 0x00,
  INT_EN_POS             = 0x01,
  PSM_POS                = 0x01,
  PERS_PROT_POS          = 0x04,
  INTEG_POS              = 0x06,
  GAIN_POS               = 0xB,
  INT_POS                = 0xE

};

// Table of lux conversion values depending on the integration time and gain. 
// The arrays represent the all possible integration times and the index of the
// arrays represent the register's gain settings, which is directly analgous to
// their bit representations. 
/**
 * @brief Lux-per-count conversion factors for 800 ms integration, indexed by gain bits.
 */
const float eightHIt[]     = {.0036, .0072, .0288, .0576};
/**
 * @brief Lux-per-count conversion factors for 400 ms integration, indexed by gain bits.
 */
const float fourHIt[]      = {.0072, .0144, .0576, .1152};
/**
 * @brief Lux-per-count conversion factors for 200 ms integration, indexed by gain bits.
 */
const float twoHIt[]       = {.0144, .0288, .1152, .2304};
/**
 * @brief Lux-per-count conversion factors for 100 ms integration, indexed by gain bits.
 */
const float oneHIt[]       = {.0288, .0576, .2304, .4608};
/**
 * @brief Lux-per-count conversion factors for 50 ms integration, indexed by gain bits.
 */
const float fiftyIt[]      = {.0576, .1152, .4608, .9216};
/**
 * @brief Lux-per-count conversion factors for 25 ms integration, indexed by gain bits.
 */
const float twentyFiveIt[] = {.1152, .2304, .9216, 1.8432};

/**
 * @brief VEML6030 ambient and white-light sensor driver.
 * Register reads/writes do not expose transport error status. Configure gain and
 * integration time before setting lux-based interrupt thresholds.
 */
class SparkFun_VEML6030 {
public:

  /**
   * @brief Bind an initialized I2C bus and the 7-bit sensor address (defAddr or altAddr).
   */
  SparkFun_VEML6030(TwoWire &, uint8_t address); // I2C Constructor

  /**
   * @brief Power on the sensor and probe its address; return true on acknowledgement.
   * Initialize the I2C bus before calling this method.
   */
  bool begin();

  // REG0x00, bits [12:11]
  // This function sets the gain for the Ambient Light Sensor. Possible values
  // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
  // sensors is behind dark glass, where as the lowest setting should be used in
  // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
  /**
   * @brief Set gainVal to 0.125, 0.25, 1, or 2; other values leave the setting unchanged.
   */
  void setGain(float gainVal);

  // REG0x00, bits [12:11]
  // This function reads the gain for the Ambient Light Sensor. Possible values
  // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
  // sensors is behind dark glass, where as the lowest setting should be used in
  // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
  /**
   * @brief Return the gain multiplier, or UNKNOWN_ERROR for an unrecognized setting.
   */
  float readGain();

  // REG0x00, bits[9:6]
  // This function sets the integration time (the saturation time of light on the
  // sensor) of the ambient light sensor. Higher integration time leads to better
  // resolution but slower sensor refresh times. 
  /**
   * @brief Set integration time to 25, 50, 100, 200, 400, or 800 milliseconds.
   */
  void setIntegTime(uint16_t time);

  // REG0x00, bits[9:6]
  // This function reads the integration time (the saturation time of light on the
  // sensor) of the ambient light sensor. Higher integration time leads to better
  // resolution but slower sensor refresh times. 
  /**
   * @brief Return integration time in milliseconds, or UNKNOWN_ERROR for an unrecognized setting.
   */
  uint16_t readIntegTime();

  // REG0x00, bits[5:4]
  // This function sets the persistence protect number i.e. the number of
  // values needing to crosh the interrupt thresholds.
  /**
   * @brief Set the persistence count to 1, 2, 4, or 8 threshold-crossing samples.
   */
  void setProtect(uint8_t protVal);

  // REG0x00, bits[5:4]
  // This function reads the persistence protect number i.e. the number of 
  // values needing to crosh the interrupt thresholds.
  /**
   * @brief Return the configured persistence count (1, 2, 4, or 8).
   */
  uint8_t readProtect();

  // REG0x00, bit[1]
  // This function enables the Ambient Light Sensor's interrupt. 
  /**
   * @brief Enable the sensor interrupt output.
   */
  void enableInt();

  // REG0x00, bit[1]
  // This function disables the Ambient Light Sensor's interrupt. 
  /**
   * @brief Disable the sensor interrupt output.
   */
  void disableInt();

  // REG0x00, bit[1]
  // This function checks if the interrupt is enabled or disabled. 
  /**
   * @brief Return the interrupt-enable bit (1 enabled, 0 disabled).
   */
  uint8_t readIntSetting();

  // REG0x00, bit[0]
  // This function powers down the Ambient Light Sensor. The light sensor will
  // hold onto the last light reading which can be acessed while the sensor is 
  // shut down. 0.5 micro Amps are consumed while shutdown. 
  /**
   * @brief Enter shutdown while retaining the last measurement registers.
   */
  void shutDown();

  // REG0x00, bit[0]
  // This function powers up the Ambient Light Sensor. The last value that was
  // read during shut down will be overwritten on the sensor's subsequent read.
  // After power up, a small 4ms delay is applied to give time for the internal
  // osciallator and signal processor to power up.   
  /**
   * @brief Leave shutdown and wait 4 milliseconds for startup.
   */
  void powerOn();

  // REG0x03, bit[0]
  // This function enables the current power save mode value and puts the Ambient
  // Light Sensor into power save mode. 
  /**
   * @brief Enable the configured power-saving mode.
   */
  void enablePowSave();

  // REG0x03, bit[0]
  // This function disables the current power save mode value and pulls the Ambient
  // Light Sensor out of power save mode. 
  /**
   * @brief Disable power-saving mode.
   */
  void disablePowSave();

  // REG0x03, bit[0]
  // This function checks to see if power save mode is enabled or disabled. 
  /**
   * @brief Return the power-saving enable bit (1 enabled, 0 disabled).
   */
  uint8_t readPowSavEnabled();

  // REG0x03, bit[2:1]
  // This function sets the power save mode value. It takes a value of 1-4. Each
  // incrementally higher value descreases the sampling rate of the sensor and so
  // increases power saving. The datasheet suggests enabling these modes when
  // continually sampling the sensor. 
  /**
   * @brief Select power-saving mode 1 through 4, with higher modes sampling less frequently.
   */
  void setPowSavMode(uint16_t modeVal);

  // REG0x03, bit[2:1]
  // This function reads the power save mode value. The function above takes a value of 1-4. Each
  // incrementally higher value descreases the sampling rate of the sensor and so
  // increases power saving. The datasheet suggests enabling these modes when
  // continually sampling the sensor. 
  /**
   * @brief Return the selected power-saving mode, 1 through 4.
   */
  uint8_t readPowSavMode();

  // REG0x06, bits[15:14]
  // This function reads the interrupt register to see if an interrupt has been
  // triggered. There are two possible interrupts: a lower limit and upper limit 
  // threshold, both set by the user.  
  /**
   * @brief Return NO_INT, INT_HIGH, or INT_LOW; other register combinations return UNKNOWN_ERROR.
   */
  uint8_t readInterrupt();

  // REG0x02, bits[15:0]
  // This function sets the lower limit for the Ambient Light Sensor's interrupt. 
  // It takes a lux value as its paramater.
  /**
   * @brief Set the lower interrupt threshold in lux using the current gain/integration settings.
   */
  void setIntLowThresh(uint32_t luxVal);
    
  // REG0x02, bits[15:0]
  // This function reads the lower limit for the Ambient Light Sensor's interrupt. 
  /**
   * @brief Return the lower interrupt threshold in lux.
   */
  uint32_t readLowThresh();

  // REG0x01, bits[15:0]
  // This function sets the upper limit for the Ambient Light Sensor's interrupt. 
  // It takes a lux value as its paramater.
  /**
   * @brief Set the upper interrupt threshold in lux using the current gain/integration settings.
   */
  void setIntHighThresh(uint32_t luxVal);

  // REG0x01, bits[15:0]
  // This function reads the upper limit for the Ambient Light Sensor's interrupt. 
  /**
   * @brief Return the upper interrupt threshold in lux.
   */
  uint32_t readHighThresh();

  // REG[0x04], bits[15:0]
  // This function gets the sensor's ambient light's lux value. The lux value is
  // determined based on current gain and integration time settings. If the lux
  // value exceeds 1000 then a compensation formula is applied to it. 
  /**
   * @brief Return ambient illuminance in integer lux, with high-light compensation above 1000 lux.
   */
  uint32_t readLight();

  // REG[0x05], bits[15:0]
  // This function gets the sensor's ambient light's lux value. The lux value is
  // determined based on current gain and integration time settings. If the lux
  // value exceeds 1000 then a compensation formula is applied to it. 
  /**
   * @brief Return white-channel illuminance in integer lux using the gain/integration conversion.
   */
  uint32_t readWhiteLight();

private:
  TwoWire &_i2cPort;
  const uint8_t _address;
    
  // This function compensates for lux values over 1000. From datasheet:
  // "Illumination values higher than 1000 lx show non-linearity. This
  // non-linearity is the same for all sensors, so a compensation forumla..."
  // etc. etc. 
  uint32_t _luxCompensation(uint32_t _luxVal);

  // The lux value of the Ambient Light sensor depends on both the gain and the
  // integration time settings. This function determines which conversion value
  // to use by using the bit representation of the gain as an index to look up
  // the conversion value in the correct integration time array. It then converts 
  // the value and returns it.  
  uint32_t _calculateLux(uint16_t _lightBits);

  // This function does the opposite calculation then the function above. The interrupt
  // threshold values given by the user are dependent on the gain and
  // intergration time settings. As a result the lux value needs to be
  // calculated with the current settings and this function accomplishes
  // that.  
  uint16_t _calculateBits(uint32_t _luxVal);

  // This function writes to a 16 bit register. Paramaters include the register's address, a mask 
  // for bits that are ignored, the bits to write, and the bits' starting
  // position.
  void _writeRegister(uint8_t _wReg, uint16_t _mask, uint16_t _bits, uint8_t _startPosition);

  // This function reads a 16 bit register. It takes the register's
  // address as its' parameter.
  uint16_t _readRegister(uint8_t _reg);
};

/** @} */
