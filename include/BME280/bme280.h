/*
 * bme280.h
 */

#ifndef BME280_BME280_H_
#define BME280_BME280_H_
#include <Arduino.h>

#include <Wire.h>
#include "SparkFunBME280.h"

class BME280Publisher
{
public:
  BME280Publisher();

  static void publishTask(void * pvParameters);
  void setup();
  void loop();
  bool initialize_;
  float humidity_;
  float pressure_;
  float alt_;
  float temp_;
  float discomfort_;

};

#endif // BME280_BME280_H_
