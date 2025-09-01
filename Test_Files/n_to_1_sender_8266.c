//n-1_8266

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_ADXL345_U.h>

#define SENDER_ID 1

Adafruit_BMP085 bmp;
PulseOximeter pox;
Adafruit_ADXL345_Unified accel(12345);

struct SensorData {
  int senderID;
  float temp, pressure, spo2, bpm;
  float accX, accY, accZ;
} data;

uint8_t receiverMac[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void OnDataSent(uint8_t *mac, uint8_t status) {
  Serial.println(status == 0 ? "Sent OK" : "Send Failed");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  bmp.begin();
  accel.begin();
  pox.begin();
}

void loop() {
  data.senderID = SENDER_ID;
  data.temp = bmp.readTemperature();
  data.pressure = bmp.readPressure();

  pox.update();
  data.spo2 = pox.getSpO2();
  data.bpm = pox.getHeartRate();

  sensors_event_t e;
  accel.getEvent(&e);
  data.accX = e.acceleration.x;
  data.accY = e.acceleration.y;
  data.accZ = e.acceleration.z;

  esp_now_send(receiverMac, (uint8_t *)&data, sizeof(data));
  delay(1000);
}

