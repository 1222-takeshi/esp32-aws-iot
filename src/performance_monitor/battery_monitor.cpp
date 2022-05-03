/*
 * battery_monitor.cpp
 */
#include "performance_monitor/battery_monitor.h"

const int R1 = 10000;
const int R2 = 10000;
const float threshold = 3.06; //1750
const float max_voltage = 3.3;

BatteryMonitor::BatteryMonitor(uint8_t adcPin)
{
  adc_pin_ = adcPin;
}

void BatteryMonitor::getCurrentVoltage()
{
  ADC_value_ = analogRead(adc_pin_);
  voltage_ = ADC_value_ * (R1 + R2) / R2 * (3.6 / 4095);
  battery_ = voltage_ / max_voltage * 100;
  if (voltage_ > threshold) {
    Serial.print("ADC:");
    Serial.print(ADC_value_);
    Serial.print(" Voltage:");
    Serial.println(voltage_);
  } else {
    Serial.println("you should change battery. as soon as possible.");
  }
}

void BatteryMonitor::setup()
{
  esp_task_wdt_init(60, true);    // time in seconds
  enableLoopWDT();

  delay(100);
  BatteryMonitor::getCurrentVoltage();
}

void BatteryMonitor::loop()
{
  BatteryMonitor::getCurrentVoltage();
}
