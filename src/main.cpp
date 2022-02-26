#include <iostream>

#include <performance_monitor/cpu_monitor.h>
#include <ultrasonic/ultrasonic.h>
#include <Connect_AWS/Connect_AWS.h>
#include <OTA/OTA.h>
#include <BME280/bme280.h>

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define ULTRASONIC_PUBLISH_TOPIC "ultrasonic/pub"
#define BME280_PUBLISH_TOPIC "BME280/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define Trigger_Pin 13
#define Echo_Pin 12
float distance;
const char * hostname = "myesp32";
bool use_ultrasonic;
bool use_cpu_monitoring;
bool use_bme280;
bool bme280_init_;
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

void bme280Task(void * pvParameters)
{
  while (1) {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    BME280Publisher BME280_publisher = BME280Publisher();
    if (!bme280_init_) {

      BME280_publisher.setup();
      bme280_init_ = true;
      printf("init BME280");
    }
    BME280_publisher.loop();
    StaticJsonDocument<200> doc;
    doc["time"] = millis();
    doc["Humidity"] = BME280_publisher.humidity_;
    // doc["Pressure"] = BME280_publisher.pressure_;
    // doc["Alt"] = BME280_publisher.alt_;
    doc["Temp"] = BME280_publisher.temp_;
    // doc["discomfort"] = BME280_publisher.discomfort_;
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);  // print to client
    connect_aws._client.publish(BME280_PUBLISH_TOPIC, jsonBuffer);
  }
}

void bme280_publisher()
{
  xTaskCreateUniversal(
    bme280Task,
    "bme280Task",
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
  doc["sample_message"] = "test World";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  connect_aws._client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  use_bme280 = true;
  use_ultrasonic = false;
  use_cpu_monitoring = false;

  bme280_init_ = false;

  connect_aws.connectToAWS();
  ota.setupOTA();

  if (use_bme280) {
    bme280_publisher();
  }
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
