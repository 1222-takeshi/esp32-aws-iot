/*
 * ultrasonic.h
 */
#ifndef ULTRASONIC_ULTRASONIC_H_
#define ULTRASONIC_ULTRASONIC_H_
#include <Arduino.h>

class UltraSonic
{
public:
  UltraSonic(uint8_t Trigger_Pin, uint8_t Echo_Pin, String pub_topic);
  void sendTrigger();
  float calcDistance();

private:
  float _V;
  float _distance;
  uint8_t _trigger_pin;
  uint8_t _echo_pin;
  String _pub_topic;

};
#endif // ULTRASONIC_ULTRASONIC_H_
