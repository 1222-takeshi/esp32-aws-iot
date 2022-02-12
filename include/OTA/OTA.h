/*
 * OTA.h
 */
#ifndef OTA_OTA_H_
#define OTA_OTA_H_
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

class OTA
{
public:
  explicit OTA(const char * hostname);
  void setupOTA();
  void loopingOTA();
};

#endif  // OTA_OTA_H_
