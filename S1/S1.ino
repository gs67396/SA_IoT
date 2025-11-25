#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

DHT dht(DHTPIN, DHTTYPE);

//sensor de luz
const byte TRIGGER_PIN = 22;
const byte LED = 19;

const byte LDR_PIN = 34;
const byte ECHO_PIN = 23;

void setup() {

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(LDR_PIN, INPUT);
  pinMode(ECHO_PIN, INPUT);

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

  //Sensor de umidade e temperatura
  Serial.begin(115200);
  dht.begin();

  // distancia
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //sensor de luz
  Serial.begin(115200);

  //inscrever topic
  mqtt.subscribe(SA_SL_Luminosidade);
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

//distancia
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

  //Sensor de umidade e temperatura
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro na leitura do DHT11");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%  Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  delay(2000);

  //sensor de luz
  int leituraLDR = analogRead(LDR_PIN);
  float tensao = (leituraLDR * 3.3) / 4095.0;

  Serial.print("Leitura LDR: ");
  Serial.print(leituraLDR);
  Serial.print(" - Tensão: ");
  Serial.println(tensao);

  if (leituraLDR < 1000) {
    Serial.println("Ambiente escuro");
  } else {
    Serial.println("Ambiente claro");
  }

  delay(500);


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
