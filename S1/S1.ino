#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

//Inscricoes (recebe)
#define TOPIC "ST/S1/Temperatura"
#define TOPIC "SU/S1/Unidade"
#define TOPIC "SA/S1/Luminosidade"
#define TOPIC "SP1/S1/Presença1"


//publicacoes (envia)


WiFiClientSecure wifiClient;
PubSubClient mqtt(client);

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topico ="CaioA";

const String brokerUser = "";
const String brokerpass = "";

//sensor de luz
const byte LDR_PIN = 34;


void setup() {
  //sensor de luz
  Serial.begin(115200);

  pinMode(2, OUTPUT);

  Serial.begin(115200);
  wifiClient.setInsetcure();
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL , brokerPort);
  Serial.println("Conectando no Broker");

  String boardID = "Sl-";
  boardID += String(random(0xffff), HEX);

  while (!mqtt.connected()) {
    mqtt.connect(userId.c_str(),BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(2000);
  }
  mqtt.subscribe(TOPIC1());
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

void loop() {
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

    mqtt.publish("pipa",mensagem.c_str());
  }
  mqtt.loop(); //mantem a conexão
}

void callback(char* topic, byte* payload, unsigned long length) {
    String mensagemRecebida = "";
    for (int i = 0; i < length; i++) {
      mensagemRecebida += (char) payload[i];
    }

    Serial.println(mensagemRecebida);
    //Fazer o controle aqui

    if(mensagemRecebida == "0") {
      digitalWrite(2,LOW);
      Serial.println("Led desligado: ");
      
    }

    if(mensagemRecebida == "1") {
      digitalWrite(2,HIGH);
       Serial.println("Led ligado: ");
    }

}
