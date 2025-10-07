#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client; 
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASS);
  Serial.println("conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("*");
    delay(200);
  }
  Serial.println("\nconectando com sucesso");

  String boardID = "S4- ";
  boardID += String(random(0xffff),HEX);

  mqtt.setServer(brokerURL.c_str(),brokerPort);
  Serial.println("Conectadno no Broker");
  while(mqtt.connect(boardID.c_str())){
    Serial.print("*");
    delay(200);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
