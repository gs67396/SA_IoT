#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>  ////28/20
#include <ESP32Servo.h>

// le e envia ao topico

// cria objeto para WiFi
WiFiClientSecure WiFiClient;
// cria objeto para mqtt
PubSubClient mqtt(client);

//sensor1
const byte TRIGGER_PIN = 5;
const byte ECHO_PIN = 18;
//sensor2
const byte TRIGGER_PIN2 = 4;
const byte ECHO_PIN2 = 19;

int byte redPin = 16;
int byte greenPin = 17;
int byte bluePin = 21;

void setup() {
  //led rgb
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(2, OUTPUT);
  Serial.begin(115200);      //configura a placa para mostrar na tela
  WiFiClient.SetInsecure();  ////28/20
  WiFi.begin(SSID, PASS);    //tenta conectar na rede
  Serial.println("conectado no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\n conectado com sucesso");


  // INICIO DO CODIGO: configura o servidor
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  //cria um nome que começa com S2
  String boardID = "S2-";
  //junta o S1 com um numero aleatorio
  boardID += String(random(0xffff), HEX);

  // enquando nao estiver conectado mostra "."
  while (!mqtt.connected(()) {  // nao estiver conectado
    mqtt.connect(userId.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(2000);
  }
  mqtt.subscribe(SA/SL/Luminosidade); //inscrever topic
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

long lerDistancia2() {
  digitalWrite(TRIGGER_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);

  long duracao = pulseIn(ECHO_PIN2, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;

  return distancia2;
}

void loop() {

  // led rgb
  //cor vermelha
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(1000);  // Espera 1 segundo

  // Cor Verde
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);

  // Cor Azul
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);

  // Cor Amarelo
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);

  float presença1 = 1 float presença2 = 2

  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //Ultrassom/distancia/presença

  long distancia = lerDistancia();
  
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  if (distancia < 10) {
    mqtt.publish(SA_SP_Presenca1, "detectado");

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

// LED iluminação

// LED status (RGB)
}

 
<<<<<<< HEAD
}
== == == =
}
>>>>>>> 7b0a06d7acbebe57d133ef952cb8a98726d48d51

/*
S1 

Temperatura publica; 
Umidade 
Luminosidade publica; para S2, S3, S1.
Presença 1  se inscreve no S2

S2

Presença 2
Presença 4

Do S1 recebe de Luminosidade

S3

Presença 3 usa o 

Do S1 recebe de Luminosidade

*/
