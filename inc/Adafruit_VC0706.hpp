/**
 * @file Adafruit_VC0706.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
/***************************************************
  This is a library for the Adafruit TTL JPEG Camera (VC0706 chipset)

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/397

  These displays use Serial to communicate, 2 pins are required to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#pragma once

#include <SerialPort.hpp>

/** @brief VC0706 reset protocol constant. */
#define VC0706_RESET  0x26
/** @brief VC0706 gen version protocol constant. */
#define VC0706_GEN_VERSION 0x11
/** @brief VC0706 set port protocol constant. */
#define VC0706_SET_PORT 0x24
/** @brief VC0706 read fbuf protocol constant. */
#define VC0706_READ_FBUF 0x32
/** @brief VC0706 get fbuf len protocol constant. */
#define VC0706_GET_FBUF_LEN 0x34
/** @brief VC0706 fbuf ctrl protocol constant. */
#define VC0706_FBUF_CTRL 0x36
/** @brief VC0706 downsize ctrl protocol constant. */
#define VC0706_DOWNSIZE_CTRL 0x54
/** @brief VC0706 downsize status protocol constant. */
#define VC0706_DOWNSIZE_STATUS 0x55
/** @brief VC0706 read data protocol constant. */
#define VC0706_READ_DATA 0x30
/** @brief VC0706 write data protocol constant. */
#define VC0706_WRITE_DATA 0x31
/** @brief VC0706 comm motion ctrl protocol constant. */
#define VC0706_COMM_MOTION_CTRL 0x37
/** @brief VC0706 comm motion status protocol constant. */
#define VC0706_COMM_MOTION_STATUS 0x38
/** @brief VC0706 comm motion detected protocol constant. */
#define VC0706_COMM_MOTION_DETECTED 0x39
/** @brief VC0706 motion ctrl protocol constant. */
#define VC0706_MOTION_CTRL 0x42
/** @brief VC0706 motion status protocol constant. */
#define VC0706_MOTION_STATUS 0x43
/** @brief VC0706 tvout ctrl protocol constant. */
#define VC0706_TVOUT_CTRL 0x44
/** @brief VC0706 osd add char protocol constant. */
#define VC0706_OSD_ADD_CHAR 0x45

/** @brief VC0706 stopcurrentframe protocol constant. */
#define VC0706_STOPCURRENTFRAME 0x0
/** @brief VC0706 stopnextframe protocol constant. */
#define VC0706_STOPNEXTFRAME 0x1
/** @brief VC0706 resumeframe protocol constant. */
#define VC0706_RESUMEFRAME 0x2
/** @brief VC0706 stepframe protocol constant. */
#define VC0706_STEPFRAME 0x3

/** @brief VC0706 640x480 protocol constant. */
#define VC0706_640x480 0x00
/** @brief VC0706 320x240 protocol constant. */
#define VC0706_320x240 0x11
/** @brief VC0706 160x120 protocol constant. */
#define VC0706_160x120 0x22

/** @brief VC0706 motioncontrol protocol constant. */
#define VC0706_MOTIONCONTROL 0x0
/** @brief VC0706 uartmotion protocol constant. */
#define VC0706_UARTMOTION 0x01
/** @brief VC0706 activatemotion protocol constant. */
#define VC0706_ACTIVATEMOTION 0x01

/** @brief VC0706 set zoom protocol constant. */
#define VC0706_SET_ZOOM 0x52
/** @brief VC0706 get zoom protocol constant. */
#define VC0706_GET_ZOOM 0x53

/** @brief Maximum shared Adafruit_VC0706 response-buffer payload size in bytes. */
#define CAMERABUFFSIZ 100
/** @brief Adafruit_VC0706 JPEG read-response delay setting. */
#define CAMERADELAY 10


/**
 * @brief Blocking VC0706 UART camera commands. Call begin() before camera access.
 * Returned buffers belong to the driver and are overwritten by subsequent commands.
 * Boolean command results normally indicate acknowledgement, except setPTZ().
 */
class Adafruit_VC0706 {
 public:
  /**
   * @brief Bind the camera UART, which must outlive the driver.
   */
  Adafruit_VC0706(SerialPort &);
  /**
   * @brief Start the UART at baud and reset the camera; return reset acknowledgement.
   */
  boolean begin(uint16_t baud = 38400);
  /**
   * @brief Send a camera reset and return whether its response was acknowledged.
   */
  boolean reset(void);
  /**
   * @brief Enable TV output; return command acknowledgement.
   */
  boolean TVon(void);
  /**
   * @brief Disable TV output; return command acknowledgement.
   */
  boolean TVoff(void);
  /**
   * @brief Freeze the current frame and reset the internal read offset; return acknowledgement.
   */
  boolean takePicture(void);
  /**
   * @brief Read a JPEG chunk into the shared camera buffer.
   * @param n JPEG byte count; keep n + 5 within CAMERABUFFSIZ.
   * @param offset Absolute read offset, or 0xFFFF to continue from the current offset.
   * @return Borrowed buffer pointer, or nullptr on command/receive failure.
   * The driver advances its 16-bit read offset by n on success.
   */
  uint8_t *readPicture(uint8_t n, uint16_t offset = 0xFFFF);
  /**
   * @brief Resume frame acquisition after a frozen picture; return acknowledgement.
   */
  boolean resumeVideo(void);
  /**
   * @brief Return the frozen JPEG frame length in bytes, or zero on command failure.
   */
  uint32_t frameLength(void);
  /**
   * @brief Return the camera version in the shared response buffer, or nullptr on failure.
   */
  char *getVersion(void);
  /**
   * @brief Return the byte count from the most recent response read, including protocol bytes.
   */
  uint8_t available();
  /**
   * @brief Return the downsize register value, or 0xFF on command failure.
   */
  uint8_t getDownsize(void);
  /**
   * @brief Set the camera downsize register byte; return acknowledgement.
   */
  boolean setDownsize(uint8_t);
  /**
   * @brief Return a VC0706_* image-size code, or 0xFF on failure.
   */
  uint8_t getImageSize();
  /**
   * @brief Set a VC0706_640x480, VC0706_320x240, or VC0706_160x120 size code.
   */
  boolean setImageSize(uint8_t);
  /**
   * @brief Return the motion-detection enable response; false also indicates command failure.
   */
  boolean getMotionDetect();
  /**
   * @brief Query a motion-control selector; returns command success (0/1), not the response payload.
   */
  uint8_t getMotionStatus(uint8_t);
  /**
   * @brief Read and validate a motion-detected notification; return false on timeout or mismatch.
   */
  boolean motionDetected();
  /**
   * @brief Enable or disable motion detection and UART motion reporting; return acknowledgement.
   */
  boolean setMotionDetect(boolean f);
  /**
   * @brief Write motion-control selector x and raw payload bytes d1/d2.
   */
  boolean setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2);
  /**
   * @brief Send a VC0706 frame-buffer control command; return acknowledgement.
   */
  boolean cameraFrameBuffCtrl(uint8_t command);
  /**
   * @brief Return the compression response byte; command failure is not separately reported.
   */
  uint8_t getCompression();
  /**
   * @brief Write the compression byte c; return command acknowledgement.
   */
  boolean setCompression(uint8_t c);

  /**
   * @brief Read image width/height, zoom dimensions, pan, and tilt into the reference arguments.
   * @return True on command acknowledgement, false on failure.
   */
  boolean getPTZ(uint16_t &w, uint16_t &h, uint16_t &wz, uint16_t &hz, uint16_t &pan, uint16_t &tilt);
  /**
   * @brief Set zoom width/height and pan/tilt offsets in camera pixel coordinates.
   * @return False on acknowledgement, true on failure (inverted in this implementation).
   */
  boolean setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt);

  /**
   * @brief Send an on-screen text command at x/y; chipset support is not guaranteed.
   */
  void OSD(uint8_t x, uint8_t y, char *s); // isnt supported by the chip :(

  /**
   * @brief Request camera UART baud 9600; return shared response buffer or nullptr.
   * The host UART baud is not changed by this method.
   */
  char* setBaud9600();
  /**
   * @brief Request camera UART baud 19200; return shared response buffer or nullptr.
   * The host UART baud is not changed by this method.
   */
  char* setBaud19200();
  /**
   * @brief Request camera UART baud 38400; return shared response buffer or nullptr.
   * The host UART baud is not changed by this method.
   */
  char* setBaud38400();
  /**
   * @brief Request camera UART baud 57600; return shared response buffer or nullptr.
   * The host UART baud is not changed by this method.
   */
  char* setBaud57600();
  /**
   * @brief Request camera UART baud 115200; return shared response buffer or nullptr.
   * The host UART baud is not changed by this method.
   */
  char* setBaud115200();

 private:
  uint8_t  serialNum;
  uint8_t  camerabuff[CAMERABUFFSIZ+1];
  uint8_t  bufferLen;
  uint16_t frameptr;

  SerialPort &serial;

  void common_init(void);
  boolean runCommand(uint8_t cmd, uint8_t args[], uint8_t argn, uint8_t resp, boolean flushflag = true);
  void sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn);
  uint8_t readResponse(uint8_t numbytes, uint8_t timeout);
  boolean verifyResponse(uint8_t command);
  void printBuff(void);
};

/** @} */
