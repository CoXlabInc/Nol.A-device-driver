#include "I2CIOExpander.hpp"
#include "cox.h"
 I2CIOExpander::I2CIOExpander(TwoWire &wire) : wire(wire){
}

  void I2CIOExpander::begin(uint8_t lcd_Addr, uint8_t dotsize){
      Wire.begin();
_Addr = lcd_Addr;
_backlightval = LCD_NOBACKLIGHT;
//Wait for mort than 40ms after Vcc rises to 2.7V
      delay(50);
//ready for 4 bit Interface
       write4bits(0x03 << 4);
       delayMicroseconds(4500);
       write4bits(0x03 << 4);
       delayMicroseconds(150);
       write4bits(0x03 << 4);
//function set
       write4bits(0x02 << 4);
       command(0x28); // Function set
       command(0x0F); // Display on/off control
       command(0x04); // Entry mode set
       backlight();   // backlight on
       command(0x01); // Clear display
       delayMicroseconds(2000); //this command takes a long time!
  }
//============================================================

inline void I2CIOExpander::command(uint8_t value) {
  // send Instruction
	send(value, 0);
}

void I2CIOExpander::send(uint8_t value, uint8_t mode) {
//for 4bit interface, mode(Rs,Rw,En)
  uint8_t high4bits=value&0xf0;
	uint8_t low4bits=(value<<4)&0xf0;
  write4bits((high4bits|mode));
	write4bits((low4bits|mode));
}

void I2CIOExpander::write4bits(uint8_t data){
//4-bit data Transfer Timing Sequence
  expanderWrite(data);
  pulseEnable(data);
}
void I2CIOExpander::expanderWrite(uint8_t data){
	Wire.beginTransmission(_Addr);
  Wire.write((data) | _backlightval);
	Wire.endTransmission();
}

void I2CIOExpander::pulseEnable(uint8_t _data){
	expanderWrite((_data |En));	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns
	expanderWrite((_data & ~En));	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

void I2CIOExpander::setline(){
     // cursor is positioned at the head of the second line
     command(0xC0);
}

void I2CIOExpander::print(const char c[]){
//print text
 uint8_t a=strlen(c);
for(int i=0;i<a;i++){
   send(c[i],Rs);
  }
}
void I2CIOExpander::backlight(void) {
  //backlight on
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
