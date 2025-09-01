//Fullduplex_n-n_8266


#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t mac2[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};
uint8_t mac3[] = {0x24, 0x6F, 0x28, 0xDD, 0xEE, 0xFF};

const int btn = 0;     
const int led = 2;     

struct Msg {
  int id;
  int state;
};

void onSend(uint8_t *mac, uint8_t status) {
  Serial.println(status == 0 ? "Sent OK" : "Send Fail");
}

void onReceive(uint8_t *mac, uint8_t *incoming, uint8_t len) {
  Msg msg;
  memcpy(&msg, incoming, sizeof(msg));
  if (msg.id == 3) digitalWrite(led, msg.state);  // ESP3 controls ESP1 LED
}

void setup() {
  Serial.begin(115200);
  pinMode(btn, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) return;

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(onSend);
  esp_now_register_recv_cb(onReceive);

  esp_now_add_peer(mac2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(mac3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  Msg msg = {1, digitalRead(btn) == LOW ? 1 : 0}; 
  esp_now_send(mac3, (uint8_t *)&msg, sizeof(msg));
  delay(100);
}
