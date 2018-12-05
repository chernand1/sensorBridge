#include "Sensor.h"
#include "Wire.h"

Sensor mySensor;

void setup() {
 
  Serial.begin(9600);
  Serial.print("Hello world");
  Wire.begin(12);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEvent);
  //mySensor.sonarSetup(2, 3);
  mySensor.tempHumidSetup(4);
  
}

// I2C SDA = A4, SCL = A5
void receiveEvent(int howMany) {
  
  while (1 < Wire.available()) { // loop through all but the last
    mySensor.registerAddressRequest = Wire.read(); // receive byte as a character
    Serial.println(mySensor.registerAddressRequest);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  
  Serial.println(x);         // print the integer
  Serial.print("How many = ");
  Serial.println(howMany);
  
}

void sendEvent() {
  
  Wire.write(mySensor.readI2CRegisters(mySensor.registerAddressRequest));
  
}

void loop() {
  
  Serial.print("Sensor Temp is =  ");
  Serial.println(mySensor.tempHumidData(1));
  Serial.println(mySensor.returnRegisterValue(1));
  Serial.print("Sensor humid is =  ");
  Serial.println(mySensor.returnHumid());
  Serial.println(mySensor.returnRegisterValue(2));
  delay(5000);
 
}


