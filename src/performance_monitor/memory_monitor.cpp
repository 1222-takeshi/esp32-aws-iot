// /*
//  * Connect_AWS.cpp
//  */
// #include "Connect_AWS/Connect_AWS.h"

// WiFiClientSecure net = WiFiClientSecure();
// MQTTClient _client = MQTTClient(256);

// void messageHandler(String & topic, String & payload)
// {
//   Serial.println("incoming: " + topic + " - " + payload);

// //  StaticJsonDocument<200> doc;
// //  deserializeJson(doc, payload);
// //  const char* message = doc["message"];
// }

// ConnectAWS::ConnectAWS(String pub_topic, String sub_topic)
// {
//   _pub_topic = pub_topic;
//   _sub_topic = sub_topic;
// }
// void ConnectAWS::connectToAWS()
// {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("Connected to Wi-Fi");
//   // Configure WiFiClientSecure to use the AWS IoT device credentials
//   net.setCACert(AWS_CERT_CA);
//   net.setCertificate(AWS_CERT_CRT);
//   net.setPrivateKey(AWS_CERT_PRIVATE);

//   // Connect to the MQTT broker on the AWS endpoint we defined earlier
//   _client.begin(AWS_IOT_ENDPOINT, 8883, net);

//   // Create a message handler
//   _client.onMessage(messageHandler);

//   Serial.print("Connecting to AWS IOT");
//   Serial.println(THINGNAME);

//   while (!_client.connect(THINGNAME)) {
//     Serial.print(".");
//     delay(100);
//   }

//   if (!_client.connected()) {
//     Serial.println("AWS IoT Timeout!");
//     return;
//   }

//   // Subscribe to a topic
//   _client.subscribe(_sub_topic);

//   Serial.println("AWS IoT Connected!");
// }
