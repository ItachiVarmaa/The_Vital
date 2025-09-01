//receiver_1-1_8266

#include <ESP8266WiFi.h>
#include <espnow.h>

struct Message {
  int id;
  float temp;
};

Message data;

void onReceive(uint8_t* mac, uint8_t* incoming, uint8_t len) {
  memcpy(&data, incoming, sizeof(data));
  Serial.printf("ID: %d | Temp: %.2f\n", data.id, data.temp);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver ready");
}

void loop() {

}
