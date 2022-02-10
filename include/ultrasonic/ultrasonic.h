/*
 * ultrasonic.h
 */
#ifndef ULTRASONIC_ULTRASONIC_H_
#define ULTRASONIC_ULTRASONIC_H_
#include <Arduino.h>

class UltraSonic
{
public:
  UltraSonic(uint8_t Trigger_Pin, uint8_t Echo_Pin);
  void sendTrigger();
  float calcDistance();

private:
  uint8_t _trigger_pin;
  uint8_t _echo_pin;
  float _V;
  float _distance;

};

#endif // ULTRASONIC_ULTRASONIC_H_
