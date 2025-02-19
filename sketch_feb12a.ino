const int brightness = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


float i;

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  i = i + 0.1;
  long startTime, endTime;  
  startTime = micros();

  neopixelWrite(48, 255, 20, 147);
  neopixelWrite(48, 0, 255, 0);
  neopixelWrite(48, 128, 0, 160);
  neopixelWrite(48, 0, 0, 255);
  endTime = micros();
  Serial.println(startTime);
  Serial.println(endTime);
  
}
