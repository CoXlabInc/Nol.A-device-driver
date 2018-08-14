#include <cox.h>

#define dataNum 3000

class AcCurrentSensor {
public:
  AcCurrentSensor(int, float, float, int, int);
  void Sense();
  float voltageRMS = 0.0;
  float Current = 0;

private:
  float ADC_Dmax = 0;
  float ADC_Amax = 0;
  int DigitalPinNum = -1;
  int Resistor = 0;
  int TransformerWinding = 0;

};
