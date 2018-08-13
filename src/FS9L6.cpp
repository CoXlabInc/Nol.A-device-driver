#include "FS9L6.hpp"
#include "cox.h"

FS9L6::FS9L6(){
  printf("*********** FS9L6 ************\n");
}

// Save VpData
void FS9L6::VpData(){
  VpDataH = this->voltageRMS;
  VpDataL = this->voltageRMS * 1000;
}
// !Save VpData

// Save CurrentData
void FS9L6::CurrentData(){
  CurrentDataH = this->Current;
  CurrentDataL = this->Current * 1000;
}
// !Save CurrentData

// Measure and Caculate AC
void FS9L6::Maintain(){
    this->VoltageValue = 0;

    for (int i=0 ; i<dataNum; i++) {
      this->DataValue = analogRead(A0);
      if (this->DataValue > this->VoltageValue) this->VoltageValue = this->DataValue;
    }
    //calculate data
    this->voltagePeak = this->VoltageValue / 4095.0 * 2.5;
    this->voltageRMS = this->VoltageValue / 1.414 / 4095.0 * 2.5;
    this->Current = this->VoltageValue / 1.414 / 4095.0 / 330 * 2.5 * 600;

    //show data (serial port)
    /*
    printf("Standard Current = ");
    Serial.print(this->StandardCurrent, 3);
    printf(".\n");

    Serial.print("* Vp = ");
    Serial.print(this->voltagePeak, 3);
    Serial.print(", * Vrms = ");
    Serial.print(this->voltageRMS, 3);
    Serial.print(", * Current = ");
    Serial.print(this->Current, 3);
    printf("\n");
    */
    //!show data (serial port)

    this->VpData();
    this->CurrentData();

}
// !Measure and Caculate AC
