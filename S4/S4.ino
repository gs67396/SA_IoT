#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

const byte TRIGGER_PIN = 22;
const byte ECHO_PIN = 23;
const byte LED = 2;

//Ponte-H
const byte in1 = 21;
const byte in2 = 18;

//Led RGB
const byte LED_R = 14;
const byte LED_G = 26;
const byte LED_B = 25;

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

  //Ponte-H
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  ledcAttach(in1, 5000, 8);
  ledcAttach(in2, 5000, 8);

  pinMode(LED, OUTPUT);

  // distancia
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //LED RGB
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  ledcAttach(LED_R, 5000, 8);
  ledcAttach(LED_G, 5000, 8);
  ledcAttach(LED_B, 5000, 8);

  //inscrever topic
  mqtt.subscribe(SA_VT_VELOCIDADETREM);
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

//LED RGB
void definirCor(int r, int g, int b) {
  ledcWrite(0, r);  // Escreve valor no canal 0 (vermelho)
  ledcWrite(1, g);  // Escreve valor no canal 1 (verde)
  ledcWrite(2, b);  // Escreve valor no canal 2 (azul)
}

void loop() {
  //ponte-H

  //frente
  ledcWrite(in1, 255);
  ledcWrite(in2, 0);
  //trás
  ledcWrite(in1, 0);
  ledcWrite(in2, 255);

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