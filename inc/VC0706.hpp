#include <cox.h>

#define returnSign 0x76
#define serialNumber 0x00

class VC0706 {
public:
  VC0706(SerialPort &, uint8_t compRatio = 0xFF);

  void begin();
  void getVer();
  void takePicture(void (*func)(void *, const char *buf, uint32_t size), void *arg);
  void stopFrame();
  void getLen();
  void getImage();
  void getImage(void (*func)(const char *buf, uint32_t size));
  void reset();
  void startCapture(uint16_t cycle,
                    void (*func)(void *) = nullptr,
                    void *arg = nullptr);
  void endCapture();
  void setMotionCtrl (uint8_t len, uint8_t motionAttribute, uint8_t ctrlItme, uint8_t secondBit, uint8_t thirdBit=0);
  void sendData(char *args, uint8_t Len);

  void recoverFrame(void (*func)(void *) = nullptr, void *arg = nullptr);
  void checkMotionStatus();
  void restart();

  uint8_t ratio;
private:
  SerialPort &port;
  Timer captureCycle;
  Timer retry;

  enum state { STATE_IDLE = 0,
               STATE_STOP_FRAME = 1,
               STATE_DATA_LEN = 2,
               STATE_IMAGE = 3,
               STATE_GET_VER = 4,
               STATE_RESET = 5,
               STATE_RECOVER = 6,
               STATE_MOTION_CTRL = 7,
               STATE_CAPTURE = 8,
               STATE_COMPRESSION = 9 };

  state state = STATE_IDLE;
  uint8_t versionIndex = 0;
  uint8_t len[4] = {0,};
  uint8_t data = 0;
  uint8_t previousData = 0;
  uint16_t index = 0;
  uint16_t imageIndex = 0;
  uint32_t motionCycle = 0;
  int16_t prevRatio = -1;
  char *imageBuf = NULL;
  char version[12] = {0,};
  uint8_t checkSetedRatio = 0;
  uint8_t previousRatio = 0;
  uint32_t size = 0;
  uint32_t imageSize = 0;

  void eventDataReceived();
  void (*callbackOnCaptured)(void *) = nullptr;
  void *callbackArgOnCaptured = nullptr;
  void (*callbackOnRecoverFrame)(void *) = nullptr;
  void *callbackArgOnRecoverFrame = nullptr;
  void (*callbackOnRatioSet)(void *) = nullptr;
  void *callbackArgOnRatioSet = nullptr;
  void (*callbackOnPictureTaken)(void *, const char *buf, uint32_t size) = nullptr;
  void *callbackArgOnPictureTaken = nullptr;

  void setRatio(uint8_t compRatio, void (*func)(void *) = nullptr, void *arg = nullptr);
};
