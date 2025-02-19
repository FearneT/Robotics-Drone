#include <ICM_ACCESS.h>

//Find the number of these registers
#define WHO_AM_I 117
#define DEVICE_CONFIG 17
#define PWR_MGMT0 78
#define ACCEL_DATA_X1 31 

uint8_t data[12];

void setup() {
  Serial.begin(115200);
  initSPI();
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

void loop() {
  readIMUSPI(ACCEL_DATA_X1, 12, data);
  for (int i = 0; i<12; i++){
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(150);
}

