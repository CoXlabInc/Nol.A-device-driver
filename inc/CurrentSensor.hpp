#include <cox.h>

#define dataNum 3000

class CurrentSensor {
public:
  CurrentSensor(int, int, int, int);
  void Sense();
  float voltageRMS = 0.0;
  float Current = 0;

private:
  int Resistor = 0;
  int ADC_Dmax = 0;
  int ADC_Amax = 0;
  int TransformerWinding = 0;

};
