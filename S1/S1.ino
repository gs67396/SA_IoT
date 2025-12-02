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

//Led RGB
const byte LED_R = 14;
const byte LED_G = 26;
const byte LED_B = 25;

void setup() {

  //luz sensor
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

  //LED RGB
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  ledcAttach(LED_R, 5000, 8);
  ledcAttach(LED_G, 5000, 8);
  ledcAttach(LED_B, 5000, 8);

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

//LED RGB
void definirCor(int r, int g, int b) {
  ledcWrite(0, r);  // Escreve valor no canal 0 (vermelho)
  ledcWrite(1, g);  // Escreve valor no canal 1 (verde)
  ledcWrite(2, b);  // Escreve valor no canal 2 (azul)
}

void loop() {

  //distancia
  long distancia = lerDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia < 10) {
    mqtt.publish(SA_SP_Presenca4, "detectado");
  }

  delay(500);
  mqtt.loop();

  //Sensor de umidade e temperatura
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro na leitura do DHT11");
    return;
  }

  // Array de caracteres para armazenar a string convertida 
  char tempStr[8]; 
  char humStr[8];

  //converte

  dtostrf(temperatura, 4, 1, tempStr);
  dtostrf(umidade, 4, 1, humStr);

  Serial.print("Umidade: ");
  Serial.print(umidade);
  mqtt.publish(SA_SP_Umidade, humStr);
  Serial.print("%  Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");
  mqtt.publish(SA_SP_Temperadura, tempStr);
 

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
    mqtt.publish(SA_SP_Luminosidade, "apagado");
  } else {
    Serial.println("Ambiente claro");
    mqtt.publish(SA_SP_Luminosidade, "aceso");
  }

  delay(500);


mqtt.loop();  //mantem a conexão
}

void statusLED(byte status) {
	turnOffLEDs();
	switch (status) {
	case 254: //(Vermelho)
    	setLEDColor(255, 0, 0);
    	break;
	case 1: //(Amarelo)
    	setLEDColor(150, 255, 0);
    	break;
	case 2: //(Rosa)
    	setLEDColor(150, 0, 255);
    	break;
	case 3:  //(Verde)
    	setLEDColor(0, 255, 0);
    	break;
	case 4:  //(Ciano)
    	setLEDColor(0, 255, 255);
    	break;
	default:
    	for (byte i = 0; i < 4; i++) {
        	setLEDColor(0, 0, 255);  //(pisca azul)
        	delay(100);
        	turnOffLEDs();
        	delay(100);
    	}
    	break;
	}
}


void turnOffLEDs() {
      setLEDColor(0, 0, 0); 
}

//led RGB
void setLEDColor(byte r, byte g, byte b) {
	ledcWrite(LED_R, r);
	ledcWrite(LED_G, g);
	ledcWrite(LED_B, b);
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
