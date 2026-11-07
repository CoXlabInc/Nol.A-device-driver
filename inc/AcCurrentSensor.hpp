/**
 * @file AcCurrentSensor.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#include <cox.h>

/** @brief Number of analog samples acquired by one AcCurrentSensor::Sense() call. */
#define dataNum 3000

/**
 * @brief Peak-sampling AC current estimator. Call Sense() to update the public readings.
 */
class AcCurrentSensor {
public:
  /**
   * @brief Bind an analog input and its conversion factors.
   * The arguments are the analog pin, ADC full-scale voltage in volts, ADC full-scale
   * count, burden resistance in ohms, and transformer turns ratio, respectively.
   * The count and resistance must be nonzero; no pin configuration is performed.
   */
  AcCurrentSensor(int, float, float, int, int);
  /**
   * @brief Sample dataNum readings synchronously and estimate RMS from the peak.
   * Updates voltageRMS and Current using a sinusoidal peak-to-RMS factor of 1.414.
   */
  void Sense();
  /**
   * @brief Last estimated RMS voltage in volts; initially zero.
   */
  float voltageRMS = 0.0;
  /**
   * @brief Last estimated RMS current in amperes; initially zero.
   */
  float Current = 0;

private:
  float ADC_Dmax = 0;
  float ADC_Amax = 0;
  int DigitalPinNum = -1;
  int Resistor = 0;
  int TransformerWinding = 0;

};

/** @} */
