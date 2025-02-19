#include <Wire.h>
#include "VL53.h"

const int address = 0x29;
int wireErrorVL53;

void writeReg(uint16_t reg, uint8_t value){
  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  Wire.write(value);
  wireErrorVL53 = Wire.endTransmission();
}

uint8_t readReg(uint16_t reg){
  uint8_t val;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  wireErrorVL53 = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)1);
  val = Wire.read();

  return val;
}

uint16_t readReg16(uint16_t reg){
  uint16_t val;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  wireErrorVL53 = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)2);
  val  = (uint16_t)Wire.read() << 8; // value high byte
  val |=           Wire.read();      // value low byte

  return val;
}

int getRange(){
	uint16_t distance;

  distance = readReg16(VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0);
	return distance;
}

void startRanging(){
	writeReg(SYSTEM__MODE_START, 0x40);	//LiDAR time :D
}

void stopRanging(){
	writeReg(SYSTEM__MODE_START, 0x00);	//LiDAR become no :(
}

void clearInterrupt(){ //Turn off the interrupt GPIO pin, which may or may not be used. Either way, I connected it on the FC
  writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01);
}

int getIntPolarity(){
  int value;
  value = readReg(GPIO_HV_MUX__CTRL);
  value &= 0x10;
  return !(value>>4); 
}

boolean dataReady(){
  int intPolarity;
  int intStatus;
  intPolarity = getIntPolarity();
  intStatus = readReg(GPIO__TIO_HV_STATUS);
  
		if ((intStatus & 1) == intPolarity)
			return 1;
		else
			return 0;
	

}

uint8_t getRangeStatus(){
	uint8_t RgSt, rangeStatus;

	rangeStatus = 255;
  RgSt = readReg(VL53L1_RESULT__RANGE_STATUS);
	RgSt = RgSt & 0x1F;
	if (RgSt < 24)
		rangeStatus = status_rtn[RgSt];
	return rangeStatus;
}

void initLiDAR(){
	uint8_t reg = 0x00, tmp;

	for (reg = 0x2D; reg <= 0x87; reg++){
		writeReg(reg, VL51L1X_DEFAULT_CONFIGURATION[reg - 0x2D]);
	}
	startRanging();
	tmp  = 0;
	while(!dataReady()){;}
	clearInterrupt();
	stopRanging();
	writeReg(VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	writeReg(0x0B, 0); /* start VHV from the previous temperature */

  delay(100);

  startRanging();

}

int bootState(){
	uint8_t booted;

	booted = readReg(VL53L1_FIRMWARE__SYSTEM_STATUS);

  Serial.println(booted);
	
	return booted & 0x01;
}

void setup() {
  Serial.begin(115200);
  pinMode(4, INPUT); //This pin can be used to check for new data, which is faster than using the function

  Wire.begin(36,35);
  Wire.setClock(400000);

  bool booted = false;
  while (!booted) {
    //Put something here to set booted to true when the VL53 is booted
    booted = bootState();
    delay(10);
  }

  //Flash the LiDAR's firmware here
  initLiDAR();

  delay(100);
}

uint16_t range, status;
int startTime, endTime;
void loop(){
  while (!dataReady()){ //Make this check for new data, and exit the loop when there is new data
    delay(50);
  }

  startTime = micros();
  range = getRange(); //Set this to the actual range
  status = getRangeStatus(); //Set this to the status
  
  endTime = micros();

  Serial.print(range);
  Serial.print(" ");
  Serial.print(status);
  Serial.print(" ");
  Serial.println(endTime-startTime);

  clearInterrupt();

}
