#include <cox.h>

#define dataNum 3000

class FS9L6 {
public:

  FS9L6();

  int VpDataH = 0;
  int VpDataL = 0;
  int CurrentDataH = 0;
  int CurrentDataL = 0;
  
  void Maintain();
  void VpData();
  void CurrentData();

private:

  float DataValue = 0;
  float VoltageValue = 0;
  float voltagePeak = 0.0;
  float voltageRMS = 0.0;
  float Current = 0;
};
