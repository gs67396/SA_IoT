#include <WiFi.h>
const String SSID = "nome da rede";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASS);
  Serial.println("conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("*");
    delay(200);
  }
  Serial.println("conectando com sucesso");
}

void loop() {
  // put your main code here, to run repeatedly:

}
