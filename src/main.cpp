#include <iostream>

#include "ultrasonic/ultrasonic.h"
#include "Connect_AWS/Connect_AWS.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define Trigger_Pin 13
#define Echo_Pin 12
float distance;
UltraSonic ultrasonic = UltraSonic(Trigger_Pin, Echo_Pin);
ConnectAWS connect_aws = ConnectAWS(AWS_IOT_PUBLISH_TOPIC, AWS_IOT_SUBSCRIBE_TOPIC);

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["ultrasonic"] = distance;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  connect_aws._client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  connect_aws.connectToAWS();
}

void loop()
{
  ultrasonic.sendTrigger();
  distance = ultrasonic.calcDistance();
  publishMessage();
  connect_aws._client.loop();
  delay(1000);
}
