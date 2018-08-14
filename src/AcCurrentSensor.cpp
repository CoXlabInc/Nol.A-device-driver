#include "AcCurrentSensor.hpp"
#include "cox.h"

AcCurrentSensor::AcCurrentSensor(int PinNumInput, float ADC_AmaxInput, float ADC_DmaxInput, int ResistorInput, int TransformerWindingInput){
  this->PinNum = PinNumInput;
  this->ADC_Amax = ADC_AmaxInput;
  this->ADC_Dmax = ADC_DmaxInput;
  this->Resistor = ResistorInput;
  this->TransformerWinding = TransformerWindingInput;
}

// Measure and Caculate AC
void AcCurrentSensor::Sense(){
  int DataValue = 0;
  int VoltageValue = 0;

  for (int i=0 ; i<dataNum; i++) {
    DataValue = analogRead(PinNum);
    if (DataValue > VoltageValue) VoltageValue = DataValue;
  }
    //calculate data
  this->Current = VoltageValue / 1.414 / this->ADC_Dmax / this->Resistor * this->ADC_Amax * this->TransformerWinding;
  this->voltageRMS = VoltageValue / 1.414 / this->ADC_Dmax * this->ADC_Amax;
}
// !Measure and Caculate AC
