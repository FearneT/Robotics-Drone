#include <ICM_ACCESS.h>

uint8_t data[12];

void setup() {
  Serial.begin(115200);
  initSPI();
}

//Add the datatype these need to be
uint16_t accelX, accelY, accelZ, angVelX, angVelY, angVelZ;

void loop() {
  readIMUSPI(ACCEL_DATA_X1, 12, data);

  accelX = data[0];
  accelY = data[1];
  accelZ = data[2];
  angVelX = data[3];
  angVelY = data[4];
  angVelZ = data[5];


  Serial.print(accelX);  Serial.print("\t");
  Serial.print(accelY);  Serial.print("\t");
  Serial.print(accelZ);  Serial.print("\t");
  Serial.print(angVelX); Serial.print("\t");
  Serial.print(angVelY); Serial.print("\t");
  Serial.print(angVelZ); Serial.print("\n");


  delay(150);
}

