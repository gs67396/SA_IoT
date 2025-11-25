#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

//sensor3
const byte TRIGGER_PIN = 5;
const byte ECHO_PIN = 18;

void setup() {
//wifi
  Serial.begin(115200);
  wifiClient.setInsecure();
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("Conectado com sucesso!");

  // INICIO DO CODIGO: configura o servidor
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  String boardID = "S2-";
  boardID += String(random(0xffff), HEX);

  while (!mqtt.connected()) {                                        
    mqtt.connect(boardID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);  
    Serial.print(".");
    delay(2000);
  }
  
  }

  // INICIO DO CODIGO: configura o servidor
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  String boardID = "Sl-";
  boardID += String(random(0xffff), HEX);

  while (!mqtt.connected(()) {  // nao estiver conectado
    mqtt.connect(userId.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(2000);
  }
  mqtt.subscribe(SA_SP_Presenca); //inscrever topic
  mqtt.subscribe(SA_SP_Presenca2); //inscrever topic
  mqtt.subscribe(SA_SP_Presenca3); //inscrever topic
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");

  //Ultrassom/distancia/presença
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

//Ultrassom/distancia/presença
long lerDistancia() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
}


void loop() {

  //Ultrassom/distancia/presença
  long distancia = lerDistancia();

   Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  if (distancia < 10) {
    Serial.println("Objeto próximo!");
  }
  
  delay(500);
  mqtt.loop();
}


void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }

  Serial.println(mensagemRecebida);

  if (mensagemRecebida == "0") {
    digitalWrite(2, LOW);
    Serial.println("Led desligado: ");
  }

  if (mensagemRecebida == "1") {
    digitalWrite(2, HIGH);
    Serial.println("Led ligado: ");
  }
}
