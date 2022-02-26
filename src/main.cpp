#include <iostream>

#include <performance_monitor/cpu_monitor.h>
#include <ultrasonic/ultrasonic.h>
#include <Connect_AWS/Connect_AWS.h>
#include <OTA/OTA.h>

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define ULTRASONIC_PUBLISH_TOPIC "ultrasonic/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define Trigger_Pin 13
#define Echo_Pin 12
float distance;
const char * hostname = "myesp32";
bool use_ultrasonic;
bool use_cpu_monitoring;
ConnectAWS connect_aws = ConnectAWS(AWS_IOT_PUBLISH_TOPIC, AWS_IOT_SUBSCRIBE_TOPIC);
CpuMonitor cpu_monitor = CpuMonitor();
OTA ota = OTA(hostname);

void ultrasonicTask(void * pvParameters)
{
  while (1) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    UltraSonic ultrasonic = UltraSonic(Trigger_Pin, Echo_Pin, ULTRASONIC_PUBLISH_TOPIC);
    ultrasonic.sendTrigger();
    // distance = ultrasonic.calcDistance();
    StaticJsonDocument<200> doc;
    doc["time"] = millis();
    doc["ultrasonic"] = ultrasonic.calcDistance();
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);  // print to client
    connect_aws._client.publish(ULTRASONIC_PUBLISH_TOPIC, jsonBuffer);
  }
}

void ultrasonic_publisher()
{
  xTaskCreateUniversal(
    ultrasonicTask,
    "ultrasonicTask",
    4096,
    NULL,
    2,
    NULL,
    APP_CPU_NUM
  );
}

void samplePublisher()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sample_message"] = "Hello World";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  connect_aws._client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  use_ultrasonic = true;
  use_cpu_monitoring = false;

  connect_aws.connectToAWS();
  ota.setupOTA();

  if (use_ultrasonic) {
    ultrasonic_publisher();
  }
  if (use_cpu_monitoring) {
    cpu_monitor.cpu_monitor_loop();
  }
}

void loop()
{
  samplePublisher();
  connect_aws._client.loop();
  if (!connect_aws._client.loop()) {
    connect_aws._client.disconnect();
    Serial.println("Lost connection to AWS. Trying again.");
    connect_aws.connectToAWS();
  }
  ota.loopingOTA();
  delay(1000);
}
