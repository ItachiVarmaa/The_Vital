//MAC ADDRESS 


#include <WIFI.h>

void setup(){
 Serial.begin(115200);
 delay(100);

 WiFi.mode(WIFI_STA);

 Serial.print("devcice mac address : ");
 Serial.println(WiFi.macAdress());

}

void loop(){

}
