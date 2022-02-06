#include "WiFi.h"
#include <WiFiMulti.h>
#include <PubSubClient.h>

#include "WiFiUtils.h"
#include "settings.h"
unsigned long int previousMillis = 0;
int prevStatus = -1; // not LOW or HIGH
WiFiClient client;
PubSubClient mqttClient(client);
WiFiMulti wifiMulti;

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA); //be only wifi client, not station

  WiFiUtils::printNetworks();
  WiFi.hostname("Movement_Sensor");

  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.printf("Attempting to connect to WPA SSID %s\n", WIFI_SSID);
    delay(1200);
  }

  WiFiUtils::printCurrentNet();
  WiFiUtils::printWifiData();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  unsigned long currentMillis = millis();
  int val = digitalRead(PIR_PIN); //read state of the PIR
  if (val == prevStatus) {
    delay(100);
    return;
  }
  prevStatus = val;
  Serial.printf("Motion: %d\n", val);


  while (wifiMulti.run() != WL_CONNECTED){
    Serial.println("Reconnecting wifi!");
    delay(6000);
  }

  bool connected = true;
  if (!mqttClient.connected()) {
    connected = false;
    if (mqttClient.connect("ArduinoClient")) {
      Serial.println("MQTT reconnected");
      connected = true;
    } else {
      connected = false;
      Serial.print("MQTT failed, rc=");
      Serial.print(mqttClient.state());
      prevStatus=-2;
    }
  }
  if (connected) {
    char dataString[5];       // number of digits + 1 for null terminator
    itoa(val, dataString, 10);  // int value, pointer to string, base number
    mqttClient.publish(MOVE_TOPIC, dataString, true);
    Serial.println("MQTT request sent");
  }
  else {
    Serial.println("MQTT not connected");
  }
}
