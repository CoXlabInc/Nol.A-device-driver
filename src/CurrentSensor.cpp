#include "CurrentSensor.hpp"
#include "cox.h"

CurrentSensor::CurrentSensor(float ADC_AmaxInput, float ADC_DmaxInput, int ResistorInput, int TransformerWindingInput){
  this->ADC_Amax = ADC_AmaxInput;
  this->ADC_Dmax = ADC_DmaxInput;
  this->Resistor = ResistorInput;
  this->TransformerWinding = TransformerWindingInput;
}

// Measure and Caculate AC
void CurrentSensor::Sense(){
  float DataValue = 0;
  float VoltageValue = 0;

  for (int i=0 ; i<dataNum; i++) {
    DataValue = analogRead(A0);
    if (DataValue > VoltageValue) VoltageValue = DataValue;
  }
    //calculate data
  this->Current = VoltageValue / 1.414 / this->ADC_Dmax / this->Resistor * this->ADC_Amax * this->TransformerWinding;
  this->voltageRMS = VoltageValue / 1.414 / this->ADC_Dmax * this->ADC_Amax;
}
// !Measure and Caculate AC
