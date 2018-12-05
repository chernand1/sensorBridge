#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

class Sensor
{
  public:
    void sonarSetup(int trig, int echo);
    float sonarMeasure();
    float sonarDistance;
    void tempHumidSetup(int dataHtPin);
    float tempHumidData(int isTemp);
    float returnHumid();
    float returnTemp();
    byte readHTByte();
    float returnRegisterValue(int regAddress);
    byte readI2CRegisters(int regAdress);
    
    int registerAddressRequest;
    int byteCounter;
    int numberOfLocationsToReturn;

  private:
  	int _trigPin;
  	int _echoPin;
    int _dataHtPin;

    // Registers
    // 
    // 0 Temp + Temp decimal
    // 1 Humid Data + Humid decimal
    int _sonarOffset = 0;
    int _tempOffset = 1;
    float _sensorRegisters[10];
    int _regCounter = 0;

};

void Sensor::sonarSetup(int trig, int echo)
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  _trigPin = trig;
  _echoPin = echo;
  
  digitalWrite(_trigPin, LOW); 
  sonarDistance = 0.0;

  registerAddressRequest = 0;
  numberOfLocationsToReturn = 1;
}

float Sensor::sonarMeasure()
{
  unsigned long startTimer = 0;
  unsigned long endTimer = 0;
  long duration = 0;
  float distance_cm = 0.0;
  
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);
  
  duration = pulseIn(_echoPin, HIGH);
  
  sonarDistance = duration * (0.034/2);

  _sensorRegisters[_sonarOffset] = sonarDistance;
  
  return sonarDistance;
  
}

void Sensor::tempHumidSetup(int dataHtPin)
{
  pinMode(dataHtPin, OUTPUT);
  _dataHtPin = dataHtPin;
  digitalWrite(_dataHtPin, HIGH); 
}

float Sensor::tempHumidData(int isTemp)
{
long duration;

  pinMode(_dataHtPin, OUTPUT);
  digitalWrite(_dataHtPin, HIGH);
  digitalWrite(_dataHtPin, LOW);
  delay(30);
  digitalWrite(_dataHtPin, HIGH);
  delayMicroseconds(40);
  pinMode(_dataHtPin, INPUT);

  //Wait for device acknowledge
  pulseIn(_dataHtPin, HIGH);
  
  int arrayByte[40];
  int i;
  int byteNo;
  int Temperature[5];

  for (byteNo=0; byteNo<5; byteNo++)
  {
    Temperature[byteNo] = readHTByte();
  }

  _sensorRegisters[_tempOffset] = Temperature[2]*1.0 + Temperature[3]*1.0/100;
  _sensorRegisters[_tempOffset+1] = Temperature[0]*1.0 + Temperature[1]*1.0/100;

  digitalWrite(_dataHtPin, HIGH);
  pinMode(_dataHtPin, OUTPUT);
  //digitalWrite(_dataHtPin, LOW);
  digitalWrite(_dataHtPin, HIGH);
  
  if (isTemp != 0)
    return _sensorRegisters[_tempOffset];
  else
    return _sensorRegisters[_tempOffset+1];
}

byte Sensor::readHTByte()
{
byte bytePad;

bytePad = 0;

  for (int i=0;i<8;i++)
  {
   if (pulseIn(_dataHtPin, HIGH) > 30)
    bytePad = bytePad + (1 << (7-i));
  }
 
 return bytePad;
}

float Sensor::returnHumid()
{
  return _sensorRegisters[_tempOffset+1];
}

float Sensor::returnTemp()
{
  return _sensorRegisters[_tempOffset];
}

float Sensor::returnRegisterValue(int regAddress)
{
  return _sensorRegisters[regAddress];
}

byte Sensor::readI2CRegisters(int regAdress)
{
  float regFloatVal = returnRegisterValue(regAdress);
  byte *regFloatByte = (byte *)&regFloatVal;
  byte byteToReturn = regFloatByte[_regCounter];
  
  _regCounter++;

  if (_regCounter == 3)
    _regCounter = 0;
  
  return byteToReturn;
  
}
#endif
