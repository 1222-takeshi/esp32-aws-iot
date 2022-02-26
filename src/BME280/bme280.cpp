/*
 * bme280.cpp
 */
#include "BME280/bme280.h"

BME280 mySensor;
BME280Publisher::BME280Publisher()
{
  initialize_ = false;
  humidity_ = 0.0;
  pressure_ = 0.0;
  alt_ = 0.0;
  temp_ = 0.0;
  discomfort_ = 0.0;
}

void BME280Publisher::setup()
{
  Serial.println("Reading basic values from BME280");

  Wire.begin();
  mySensor.setI2CAddress(0x77);
  if (mySensor.beginI2C() == false) { //Begin communication over I2C
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1) { //Freeze
    }
  }
}

void BME280Publisher::loop()
{
  humidity_ = mySensor.readFloatHumidity();
  pressure_ = mySensor.readFloatPressure();
  alt_ = mySensor.readFloatAltitudeMeters();
  temp_ = mySensor.readTempC();
  discomfort_ = 0.81 * mySensor.readTempC() + 0.01 * mySensor.readFloatHumidity() *
    (0.99 * mySensor.readTempC() - 14.3) + 46.3;
  printf("looping BME280 module\n");
  Serial.print("Humidity: ");
  Serial.print(mySensor.readFloatHumidity());

  Serial.print(" Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 2);

  Serial.print(" Alt: ");
  Serial.print(mySensor.readFloatAltitudeMeters(), 2);
  //Serial.print(mySensor.readFloatAltitudeFeet(), 1);
  Serial.print(" Temp: ");
  Serial.print(mySensor.readTempC());
  //Serial.print(mySensor.readTempF(), 2);
  // Serial.print(" discomfort: ");
  // Serial.print(
  //   0.81 * mySensor.readTempC() + 0.01 * mySensor.readFloatHumidity() *
  //   (0.99 * mySensor.readTempC() - 14.3) + 46.3, 4);
  Serial.println();
}
