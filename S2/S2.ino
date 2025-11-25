#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>  ////28/20
#include <ESP32Servo.h>


WiFiClientSecure client;
PubSubClient mqtt(client);

//sensor1
const byte TRIGGER_PIN = 5;
const byte ECHO_PIN = 18;
//sensor2
const byte TRIGGER_PIN2 = 4;
const byte ECHO_PIN2 = 19;

void setup() {
  //Wifi
  pinMode(2, OUTPUT);
  Serial.begin(115200);    
  Client.setInsecure();    
  WiFi.begin(SSID, PASS);
  Serial.println("conectado no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\n conectado com sucesso");

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

  mqtt.subscribe(SA_SP_Luminosidade);  //inscrever topic
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");

  //Ultrassom/distancia/presença
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //Ultrassom2/distancia2/presença2
  Serial.begin(115200);
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
}

//Ultrassom/distancia/presença
long lerDistancia1() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;

  return distancia;
}

//Ultrassom/distancia/presença
long lerDistancia2() {
  digitalWrite(TRIGGER_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);

  long duracao = pulseIn(ECHO_PIN2, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;

  return distancia;
}

void loop() {

  //Ultrassom/distancia/presença
  long distancia1 = lerDistancia1();

  Serial.print("Distância: ");
  Serial.print(distancia1);
  Serial.println(" cm");

  if (distancia1 < 10) {
    mqtt.publish(SA_SP_Presenca1, "detectado");
  }

  //Ultrassom2/distancia2/presença2
  long distancia2 = lerDistancia2();

  Serial.print("Distância: ");
  Serial.print(distancia2);
  Serial.println(" cm");

  if (distancia2 < 10) {
    mqtt.publish(SA_SP_Presenca2, "detectado");
  }

  delay(500);
  mqtt.loop();
}


void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }

  // Exemplo / Abaixo led ligar e desligar, junto com as mensagens de confirmacao.
  Serial.println(mensagemRecebida);
  //Fazer o controle aqui

  if (mensagemRecebida == "0") {
    digitalWrite(2, LOW);
    Serial.println("Led desligado: ");
  }

  if (mensagemRecebida == "1") {
    digitalWrite(2, HIGH);
    Serial.println("Led ligado: ");
  }
}
