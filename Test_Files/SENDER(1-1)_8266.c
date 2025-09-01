//SENDER(1-1)_8266

#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct Message {
  int id;
  float temperature;
};

Message data;

void onSend(uint8_t* mac, uint8_t status) {
  Serial.println(status == 0 ? " Sent successfully" : "send failed");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); 
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onSend);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 0, NULL, 0);
}

void loop() {
  data.id = 1;
  data.temperature = 26.5;

  esp_now_send(broadcastAddress, (uint8_t*)&data, sizeof(data));
  delay(2000); 
}
