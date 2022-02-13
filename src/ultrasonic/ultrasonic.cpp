/*
 * ultrasonic.cpp
 */
#include "ultrasonic/ultrasonic.h"

UltraSonic::UltraSonic(uint8_t Trigger_Pin, uint8_t Echo_Pin, String pub_topic)
{
  _trigger_pin = Trigger_Pin;
  _echo_pin = Echo_Pin;
  _pub_topic = pub_topic;
  _V = 340.0;
  _distance = 0.0;
  pinMode(_trigger_pin, OUTPUT);
  pinMode(_echo_pin, INPUT);
  digitalWrite(_trigger_pin, LOW);
}

void UltraSonic::sendTrigger()
{
  digitalWrite(_trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger_pin, LOW);
}

float UltraSonic::calcDistance()
{
  while (!digitalRead(_echo_pin)) {
  }
  unsigned long t1 = micros();

  while (digitalRead(_echo_pin)) {
  }
  unsigned long t2 = micros();
  unsigned long t = t2 - t1;
  _distance = _V * t / 20000;
  Serial.print(_distance);
  Serial.println("cm");
  delay(100);  //10Hz
  return _distance;
}
