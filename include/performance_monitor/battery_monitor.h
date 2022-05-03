/*
 * battery_monitor.h
 */
#ifndef PERFORMANCE_MONITOR_BATTERY_MONITOR_H_
#define PERFORMANCE_MONITOR_BATTERY_MONITOR_H_
#include <Arduino.h>

#include <esp_task_wdt.h>
#include "performance_monitor/task_manager.h"
class BatteryMonitor
{
public:
  BatteryMonitor(uint8_t adcPin);
  void getCurrentVoltage();
  void setup();
  void loop();
  uint8_t adc_pin_;
  float ADC_value_;
  float voltage_;
  float battery_;
};

#endif  // CONNECT_AWS_CONNECT_AWS_H_
