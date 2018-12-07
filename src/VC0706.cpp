#include <cox.h>
#include "VC0706.hpp"

VC0706::VC0706(SerialPort &p, uint8_t compRatio)
  : ratio(compRatio), port(p) {
}

void VC0706::begin() {
  this->port.begin(38400);
  this->port.onReceive([](void *ctx) {
                         ((VC0706 *) ctx)->eventDataReceived();
                       }, this);
  this->port.listen();
}

void VC0706::eventDataReceived() {
  while(this->port.available() > 0) {
    this->data = this->port.read();
//===================== check returnSign & serialNumber ======================
    if ((this->index == 0 && this->data != returnSign) ||
        (this->index == 1 && this->data != serialNumber)) {
        this->index = 0;
        continue;
//============================ check the state ================================
    } else if((this->index == 2) && (data == 0x32)) {
        if (this->imageBuf == NULL) {
          // do something
          this->imageSize = 0;
        } else {
          this->imageIndex = 0;
        }
    }
    //============ check return value and reset the state, index ===================
      else if ((this->state == STATE_RESET) && (this->index == 4) &&
            (this->previousData == 0x00) && (this->data == 0x00)) {
        //Finish the reset !
        this->index = 0;
        this->state = STATE_IDLE;
        continue;
    } else if((this->state == STATE_STOP_FRAME) && (this->index == 4) &&
            (this->previousData == 0x00) && (this->data == 0x00)) {
        //Finish the stopFrame !
        this->index = 0;
        this->state = STATE_IDLE;
        getLen();
        continue;
    } else if((this->state == STATE_RECOVER) && (this->index == 4) &&
            (this->previousData == 0x00) && (this->data == 0x00)) {
        //Finish recoverFrame !
        this->index = 0;
        this->state = STATE_IDLE;
        delay(100);
        if (this->callbackOnRatioSet != nullptr){
          void (*callback)(void *) = this->callbackOnRatioSet;
          this->callbackOnRatioSet = nullptr;
          callback(this->callbackArgOnRatioSet);
        }
        if (this->callbackOnRecoverFrame != nullptr) {
          void (*callback)(void *) = this->callbackOnRecoverFrame;
          this->callbackOnRecoverFrame = nullptr;
          callback(this->callbackArgOnRecoverFrame);
        }
        continue;
    } else if((this->state == STATE_DATA_LEN) && (this->index == 4) &&
            (this->previousData != 0x00) && (this->data != 0x04)) {
        this->index = 0;
        this->state = STATE_IDLE;
        continue;
    } else if((this->state == STATE_IMAGE) && (this->index == 4) &&
            (this->previousData != 0x00) && (this->data != 0x00)) {
        this->index = 0;
        this->state = STATE_IDLE;
        recoverFrame();
        continue;
    } else if((this->state == STATE_MOTION_CTRL) && (this->index == 4) &&
              (this->previousData == 0x00) && (this->data == 0x00)) {
        //Finish the setMotionCtrl !
        this->index = 0;
        this->state = STATE_IDLE;
        continue;
    } else if((this->state == STATE_COMPRESSION) && (this->index == 4) &&
              (this->previousData == 0x00) && (this->data == 0x00)){
        //Finish the setRatio & Start recoverFrame !
        retry.stop();
        this->index = 0;
        this->state = STATE_IDLE;
        recoverFrame();
        continue;
    } else if((this->state == STATE_IMAGE) && (this->index == 6) &&
              (this->previousData != 0xFF) && (this->data != 0xD8)) {
        this->index = 0;
        this->state = STATE_IDLE;
        continue;
    } else if((this->state == STATE_CAPTURE) && (this->index == 6) &&
              (this->previousData == 0x01) && (this->data == 0x11)) {
        this->state = STATE_IDLE;
        this->index = 0;
        if (this->callbackOnCaptured) {
          void (*callback)(void *) = this->callbackOnCaptured;
          this->callbackOnCaptured = nullptr;
          callback(this->callbackArgOnCaptured);
        }
        continue;
    } else if((this->state == STATE_CAPTURE) && (this->index == 6) &&
              (this->previousData == 0x01) && (this->data == 0x01)) {
        this->state = STATE_IDLE;
        this->index = 0;
        continue;
    } else if((this->state == STATE_GET_VER) && (this->index == 15) &&
              (previousData == 0x30) && (this->data == 0x30)) {
        //Finish the get version !
        this->index = 0;
        this->state = STATE_IDLE;
        continue;
    } else if((this->state == STATE_DATA_LEN) && (this->index == 6)) {
        //Take a High 16-bits of image length !
        this->len[3] = this->previousData;
        this->len[2] = this->data;
        size |= (((uint32_t) this->len[3]) << 24);
        size |= (((uint32_t) this->len[2])<<16);
    } else if((this->state == STATE_DATA_LEN) && (this->index == 8)) {
      //Take a Low 16-bits of image length & Finish the getLen !
        this->len[1] = this->previousData;
        this->len[0] = this->data;
        size |= (this->len[1]<<8);
        size |= this->len[0];
        this->imageSize = size;
        this->state = STATE_IDLE;
        this->index = 0;
        getImage(NULL);
        continue;
    }
//============================== save the imageBuf or get the version ============================
      else if (this->state == STATE_IMAGE && (this->index > 4) &&
              (this->index <= (this->imageSize+4))) {

        this->imageBuf[this->imageIndex++] = this->data;

    } else if (this->state == STATE_GET_VER && (this->index > 4) &&
            (this->index <= 14)) {

        this->version[this->versionIndex++] = this->data;

        if(this->versionIndex == 10) {
          //Print the version !
          for (int i=0; i<11; i++){
              printf("%c",this->version[i]);
              if(i==10) {printf("\n" );}
          }
          this->state = STATE_IDLE;
          this->index = 0;
          this->versionIndex = 0;
        }

    } else if (this->state == STATE_IMAGE && ((this->index)==(this->imageSize+6)) &&
              (this->data == 0x00) && (previousData ==0x76)) {
        if (!postTask([](void *ctx) {
                        VC0706 *cam = (VC0706 *) ctx;
                        if (cam->callbackOnPictureTaken) {
                          cam->callbackOnPictureTaken(cam->callbackArgOnPictureTaken,
                                                      cam->imageBuf,
                                                      cam->imageSize);
                          dynamicFree(cam->imageBuf);
                          cam->imageBuf = nullptr;
                        }
                      }, this)) {
          dynamicFree(this->imageBuf);
          this->imageBuf = nullptr;
        }
        //Finish the getImage & Start the recoverFrame !
    } else if (this->state == STATE_IMAGE &&
               this->index == this->imageSize + 9) {
        this->index = 0;
        this->state = STATE_IDLE;
        this->imageIndex = 0;
        this->len[0] = 0;
        this->len[1] = 0;
        this->len[2] = 0;
        this->len[3] = 0;
        size = 0;
        recoverFrame();
        continue;
    }

    this->previousData = this->data;
    index++;
  }
}

void VC0706::sendData(char *args, uint8_t Len)
{
  for(int j=0; j<Len; j++,args++){
    this->port.write(*args);
  }
}

void VC0706::takePicture(void (*func)(void *arg, const char *buf, uint32_t size),
                         void *arg) {
  if (this->state == STATE_IDLE) {
    this->callbackOnPictureTaken = func;
    this->callbackArgOnPictureTaken = arg;
    if(this->ratio != this->prevRatio){
      //If you set the compression ratio, Start the setRatio function !
      setRatio(ratio,
               [](void *ctx) {
                 ((VC0706 *) ctx)->stopFrame();
               },
               this);
      this->prevRatio = this->ratio;
    } else {
      stopFrame();
    }
  } else {
    //Busy now !
    reset();
  }
}

void VC0706::stopFrame()
{
  //Captures the current frame
  this->state = STATE_STOP_FRAME;
  char args[] = {0x56, 0x00, 0x36, 0x01, 0x00};
  sendData(args, sizeof(args));
}

void VC0706::getLen()
{
  //Take a length
  this->state = STATE_DATA_LEN;
  char args[] = {0x56, 0x00, 0x34, 0x01, 0x00};
  sendData(args, sizeof(args));
}

void VC0706::getImage(void (*func)(const char *buf, uint32_t size))
{
  //Take a image data
  this->imageBuf = (char *) dynamicMalloc(this->imageSize);

  if( (this->imageBuf == NULL)){
    //Not enough memory
    this->index = 0;
    this->state = STATE_IDLE;
    this->imageIndex = 0;
    this->imageSize = 0;
    this->len[0] = 0;
    this->len[1] = 0;
    this->len[2] = 0;
    this->len[3] = 0;
    size=0;

    if (this->ratio > 0xF6) {
      //Max value of the ratio
      this->ratio = 0xFF;
    }
    else {
      //Increase compression ratio
      this->ratio +=10;
    }

    if (this->previousRatio == this->ratio) {
      //I need more memory
      getVer();
    }

    this->previousRatio = this->ratio;
    setRatio(this->ratio);
  }
  else {
    //Take a image data
    this->state = STATE_IMAGE;
    char args[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00,
                   this->len[3], this->len[2], this->len[1], this->len[0], 0x00, 0x0A};
    sendData(args, sizeof(args));
  }
}

void VC0706::getVer()
{
  //Take a Version
  this->state = STATE_GET_VER;
  char args[] = {0x56, 0x00, 0x11, 0x00};
  sendData(args, sizeof(args));
}

void VC0706::reset()
{
  //System reset
  while(this->port.available()>0){
    this->data = this->port.read();
  }
  
  this->state = STATE_RESET;
  this->data = 0;
  this->index = 0;
  char args[] = {0x56, 0x00, 0x26, 0x00};
  sendData(args, sizeof(args));
}

void VC0706::setRatio(uint8_t compRatio, void (*func)(void *), void *arg) {
  retry.onFired([](void *ctx) {
                  ((VC0706 *) ctx)->restart();
                }, this);
  retry.startOneShot(10000);
  this->state = STATE_COMPRESSION;
  this->callbackOnRatioSet = func;
  this->callbackArgOnRatioSet = arg;
  this->ratio = compRatio;
  char args[] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, this->ratio};
  this->sendData(args, sizeof(args));
}

void VC0706::recoverFrame(void (*func)(void *), void *ctx) {
  this->callbackOnRecoverFrame = func;
  this->callbackArgOnRecoverFrame = ctx;
  this->state = STATE_RECOVER;
  char args[] = {0x56, 0x00, 0x36, 0x01, 0x03};
  this->sendData(args, sizeof(args));
}

void VC0706::setMotionCtrl(uint8_t len, uint8_t motionAttribute, uint8_t ctrlItme, uint8_t firstBit, uint8_t secondBit)
{
  //Set the motion control
  this->state = STATE_MOTION_CTRL;
  if(ctrlItme==1 ) {
    char args[] = {0x56, 0x00, 0x42, len, motionAttribute, ctrlItme, firstBit, secondBit};
    this->motionCycle |= (firstBit<<8);
    this->motionCycle |= secondBit;
    sendData(args, sizeof(args));
  } else{
    char args[] = {0x56, 0x00, 0x42, len, motionAttribute, ctrlItme, firstBit};
    sendData(args, sizeof(args));
  }
}

void VC0706::startCapture(uint16_t periodMillis, void (*func)(void *), void *ctx) {
  //Start a capture
  this->state = STATE_CAPTURE;
  this->callbackOnCaptured = func;
  this->callbackArgOnCaptured = ctx;
  if (periodMillis > 0) {
    captureCycle.onFired([](void *ctx) {
                           ((VC0706 *) ctx)->checkMotionStatus();
                         }, this);
    captureCycle.startPeriodic(periodMillis);
  }
}

void VC0706::checkMotionStatus() {
  char args[] = {0x56, 0x00, 0x43, 0x01, 0x00};
  this->sendData(args, sizeof(args));
}

void VC0706::endCapture()
{
  //if you want to exit startCapture, run the endCapture()
  this->state = STATE_IDLE;
  this->index = 0;
  captureCycle.stop();
}

void VC0706::restart() {
  this->state = STATE_IDLE;
  this->index = 0;
  this->setRatio(this->ratio);
}
