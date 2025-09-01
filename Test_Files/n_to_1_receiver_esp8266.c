n_to_1_receiver_esp8266

#include <ESP8266WiFi.h>
#include <espnow.h>

struct SensorData {
  int senderID;
  float temp, pressure, spo2, bpm;
  float accX, accY, accZ;
} data;


void onReceive(uint8_t *mac, uint8_t *incoming, uint8_t len) {
  memcpy(&data, incoming, sizeof(data));

  Serial.printf("Sender %d | Temp: %.2f | Pressure: %.2f | SpO2: %.2f | BPM: %.2f\n",
                data.senderID, data.temp, data.pressure, data.spo2, data.bpm);
  Serial.printf("AccX: %.2f | AccY: %.2f | AccZ: %.2f\n", data.accX, data.accY, data.accZ);
  Serial.println("---------------------------");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver is ready...");
}

void loop() {

}
