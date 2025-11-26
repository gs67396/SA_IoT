#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

const byte TRIGGER_PIN = 22;
const byte ECHO_PIN = 23;
const byte LED = 2;

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
  String boardID = "Sl-";
  boardID += String(random(0xffff), HEX);

  while (!mqtt.connected()) {
    mqtt.connect(boardID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(2000);
  } 

  pinMode(LED, OUTPUT);

  // distancia
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //inscrever topic
  mqtt.subscribe(SA_VT_VELOCIDADETREM);
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

void loop() {
mqtt.loop();  //mantem a conexão
}

void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }

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