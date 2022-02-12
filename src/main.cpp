#include <iostream>

#include "ultrasonic/ultrasonic.h"
#include "Connect_AWS/Connect_AWS.h"
#include "OTA/OTA.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define ULTRASONIC_PUBLISH_TOPIC "ultrasonic/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define Trigger_Pin 13
#define Echo_Pin 12
float distance;
const char * hostname = "myesp32";
bool use_ultrasonic;
ConnectAWS connect_aws = ConnectAWS(AWS_IOT_PUBLISH_TOPIC, AWS_IOT_SUBSCRIBE_TOPIC);
OTA ota = OTA(hostname);

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sample_message"] = "Hello World";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  connect_aws._client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void ultrasonic_publisher()
{
  UltraSonic ultrasonic = UltraSonic(Trigger_Pin, Echo_Pin);
  ultrasonic.sendTrigger();
  distance = ultrasonic.calcDistance();
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["ultrasonic"] = distance;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client
  connect_aws._client.publish(ULTRASONIC_PUBLISH_TOPIC, jsonBuffer);
}
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  use_ultrasonic = true;

  connect_aws.connectToAWS();
  ota.setupOTA();
}

void loop()
{
  if (use_ultrasonic) {
    ultrasonic_publisher();
  }
  publishMessage();
  connect_aws._client.loop();
  ota.loopingOTA();
  delay(1000);
}
