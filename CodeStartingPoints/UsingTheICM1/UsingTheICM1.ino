#include <SPI.h>
#define NCS_ICM42688 18
#define ICM42688_ID 71

int readIMURegister(int reg) { //This function doesn't do anything yet. Follow the instructions to add the code to carry out an SPI read.
  int regData;
  digitalWrite(18, LOW);
  SPI.transfer(reg | 0x80);
  regData = SPI.transfer(0x00);
  digitalWrite(18,HIGH);
  return regData;
}

void initSPI(){
  pinMode(NCS_ICM42688, OUTPUT);
  digitalWrite(NCS_ICM42688, HIGH);
  SPI.begin(17,15,16,47);
  SPI.setFrequency(20000000);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

void setup() {
  Serial.begin(115200);
  initSPI();
  delay(100);

}

void loop() {
  
  if(readIMURegister(117)!=71){
    Serial.println("Can't find IMU");
    Serial.println(readIMURegister(117));
  }
  else{
    Serial.println("IMU Connected");
  }
  
  delay(10);
}

