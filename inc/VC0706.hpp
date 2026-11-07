/**
 * @file VC0706.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#ifndef VC0706_HPP
/** @brief VC0706 hpp protocol constant. */
#define VC0706_HPP

#include <nola-common.h>
#include <Timer.hpp>
#include <SerialPort.hpp>

/** @brief VC0706 response header marker. */
#define returnSign 0x76
/** @brief VC0706 serial protocol device number. */
#define serialNumber 0x00

/**
 * @brief Asynchronous UART JPEG camera with timer-driven command handling.
 * Call begin() before commands and keep the driver/UART alive during callbacks.
 * Commands share a state machine; do not overlap capture and command sequences.
 */
class VC0706 {
public:
  /**
   * @brief Bind the UART and initial compression byte (default 0xFF).
   */
  VC0706(SerialPort &, uint8_t compRatio = 0xFF);

  /**
   * @brief Start the UART at 38400 baud and install the asynchronous receive handler.
   */
  void begin();
  /**
   * @brief Request the version; the response is printed by the receive handler.
   */
  void getVer();
  /**
   * @brief Begin an asynchronous JPEG capture when idle.
   * @param func Receives arg, JPEG bytes, and byte count; failure paths can report
   * nullptr and zero. The image buffer is valid only during the callback.
   * @param arg User context passed to func.
   * A request while busy resets the camera instead of queuing another capture.
   */
  void takePicture(void (*func)(void *, const char *buf, uint32_t size), void *arg);
  /**
   * @brief Freeze the frame and start the internal length/image retrieval sequence.
   */
  void stopFrame();
  /**
   * @brief Request frozen-frame length as part of the capture state machine.
   */
  void getLen();
  /**
   * @brief Allocate a JPEG buffer and request image bytes after length retrieval.
   */
  void getImage();
  /**
   * @brief Discard pending UART bytes and send an asynchronous reset command.
   */
  void reset();
  /**
   * @brief Enter motion-status capture mode with optional periodic polling.
   * @param cycle Poll interval in milliseconds; zero disables automatic polling.
   * @param func Optional callback receiving arg and true on a detected capture event.
   * @param arg User callback context. The stored callback is consumed on notification.
   */
  void startCapture(uint16_t cycle,
                    void (*func)(void *, bool) = nullptr,
                    void *arg = nullptr);
  /**
   * @brief Stop periodic capture polling and reset the parser to idle.
   */
  void endCapture();
  /**
   * @brief Send raw motion-control settings.
   * @param len Command argument length byte.
   * @param motionAttribute Camera motion attribute selector.
   * @param ctrlItme Camera control-item selector; 1 sends both payload bytes.
   * @param secondBit First motion-control payload byte.
   * @param thirdBit Second payload byte, used for control item 1.
   */
  void setMotionCtrl (uint8_t len, uint8_t motionAttribute, uint8_t ctrlItme, uint8_t secondBit, uint8_t thirdBit=0);
  /**
   * @brief Write Len raw command bytes from args to the UART.
   */
  void sendData(char *args, uint8_t Len);

  /**
   * @brief Resume frame acquisition asynchronously.
   * @param func Optional callback receiving arg and success; timeout reports false.
   * @param arg User context passed to the callback.
   */
  void recoverFrame(void (*func)(void *, bool) = nullptr, void *arg = nullptr);
  /**
   * @brief Send a motion-status query, normally used while startCapture() is active.
   */
  void checkMotionStatus();
  /**
   * @brief Reset the local parser to idle and resend the current compression ratio.
   */
  void restart();

  /**
   * @brief Desired compression byte applied by the next capture or restart().
   */
  uint8_t ratio;
private:
  SerialPort &port;
  Timer captureCycle;
  Timer commandTimeout;

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
  uint32_t index = 0;
  uint32_t imageIndex = 0;
  uint32_t motionCycle = 0;
  int16_t prevRatio = -1;
  char *imageBuf = NULL;
  char version[12] = {0,};
  uint8_t checkSetedRatio = 0;
  uint8_t previousRatio = 0;
  uint32_t size = 0;
  uint32_t imageSize = 0;

  void eventDataReceived();
  void (*callbackOnCaptured)(void *, bool success) = nullptr;
  void *callbackArgOnCaptured = nullptr;
  void (*callbackOnRecoverFrame)(void *, bool success) = nullptr;
  void *callbackArgOnRecoverFrame = nullptr;
  void (*callbackOnRatioSet)(void *, bool success) = nullptr;
  void *callbackArgOnRatioSet = nullptr;
  void (*callbackOnPictureTaken)(void *, const char *buf, uint32_t size) = nullptr;
  void *callbackArgOnPictureTaken = nullptr;

  void setRatio(uint8_t compRatio, void (*func)(void *, bool) = nullptr, void *arg = nullptr);
};

#endif //VC0706_HPP

/** @} */
