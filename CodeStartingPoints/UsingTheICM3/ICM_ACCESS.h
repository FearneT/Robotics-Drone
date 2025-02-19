#ifndef ICM_ACCESS_h
#define ICM_ACCESS_h
#include <SPI.h>
#define NCS_ICM42688 18
#define ICM42688_ID 71

#define WHO_AM_I 117
#define DEVICE_CONFIG 17
#define PWR_MGMT0 78
#define ACCEL_DATA_X1 31

int readIMURegister(int reg) {
  int regData;  //Effectively the same as readSPI, except it only reads one register and returns it for ease of use

  digitalWrite(NCS_ICM42688, LOW);

  SPI.transfer(reg | 0x80);
  regData = SPI.transfer(0x00);

  digitalWrite(NCS_ICM42688, HIGH);

  return regData;
}

void writeIMUSPI(int reg, int value) {
  digitalWrite(NCS_ICM42688, LOW);  //Pull CS line for the ICM42688 low, beginning the transmittion

  SPI.transfer(reg);  //Send register address

  SPI.transfer(value);  //Send new value

  digitalWrite(NCS_ICM42688, HIGH);  //End the transmittion by pulling the line high again


  delayMicroseconds(1);  //delay required to prevent overlapping commands
}

void readIMUSPI(int reg, int count, byte data[]) {
  digitalWrite(NCS_ICM42688, LOW);  //Pull CS line for the ICM42688 low, beginning the transmittion

  SPI.transfer(reg | 0x80);  //Send the address for the first register to read, with the read bit set high (hence the + 0x80)

  SPI.transferBytes(data, data, 12);

  digitalWrite(NCS_ICM42688, HIGH);  //End the transmittion by pulling the line high again
  SPI.endTransaction();
}

void initSPI(){
  pinMode(NCS_ICM42688, OUTPUT);
  digitalWrite(NCS_ICM42688, HIGH);
  SPI.begin(17,15,16,47);
  SPI.setFrequency(20000000);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

    delay(100);

  while(readIMURegister(WHO_AM_I)!=71){
    Serial.println("Can't find IMU");
    Serial.println(readIMURegister(117));
    delay(100);
  }

  writeIMUSPI(DEVICE_CONFIG, 0x01);  //Reset device
  delay(100);
  writeIMUSPI(PWR_MGMT0, 0x0F);
  delay(100);

}

#endif