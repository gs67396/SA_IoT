#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>  ////28/20
#include <ESP32Servo.h>

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

//sensor1
const byte TRIGGER_PIN = 5;
const byte ECHO_PIN = 18;
//sensor2
const byte TRIGGER_PIN2 = 4;
const byte ECHO_PIN2 = 19;

//sensor de luz
const byte TRIGGER_PIN3 = 22;
const byte LED3 = 19;

const byte LDR_PIN = 34;
const byte ECHO_PIN3 = 23;

//Led RGB
const byte LED_R = 14;
const byte LED_G = 26;
const byte LED_B = 25;

//SERVO
Servo meuServo;
const byte SERVO_PIN = 3;

void setup() {
  //Wifi
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  wifiClient.setInsecure();
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("\n conectado com sucesso");

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

  //presença 1 e 2
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, OUTPUT);
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, OUTPUT);
  //Luz
  pinMode(TRIGGER_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, OUTPUT);

  //servo
  Serial.begin(115200);
  meuServo.attach(SERVO_PIN);

  //Ultrassom/distancia/presença
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //Ultrassom2/distancia2/presença2
  Serial.begin(115200);
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

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

//LED RGB
void definirCor(int r, int g, int b) {
  ledcWrite(0, r);  // Escreve valor no canal 0 (vermelho)
  ledcWrite(1, g);  // Escreve valor no canal 1 (verde)
  ledcWrite(2, b);  // Escreve valor no canal 2 (azul)
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

  // Servo motor
  // Varre de 0 a 180 graus
  for (int pos = 0; pos <= 180; pos += 1) {
    meuServo.write(pos);
    delay(15);

    // Varre de 180 a 0 graus
    for (int pos = 180; pos >= 0; pos -= 1) {
      meuServo.write(pos);
      delay(15);

      mqtt.loop();
    }
  }

  //servo motor
  for (int pos = 0; pos <= 90; pos += 1) {
    meuServo.write(pos);
    delay(15);
  }
  
  // Varre de 90 a 0 graus
  for (int pos = 90; pos >= 0; pos -= 1) {
    meuServo.write(pos);
    delay(15); 
  }
}

void statusLED(byte status) {
  turnOffLEDs();
  switch (status) {
    case 254:  //(Vermelho)
      setLEDColor(255, 0, 0);
      break;
    case 1:  //(Amarelo)
      setLEDColor(150, 255, 0);
      break;
    case 2:  //(Rosa)
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

  // Exemplo / Abaixo led ligar e desligar, junto com as mensagens de confirmacao.
  Serial.println(mensagemRecebida);
  //Fazer o controle aqui

  if (mensagemRecebida == "apagado"){
    digitalWrite(2, LOW);
    Serial.println("Led desligado: ");
  }

  else if (mensagemRecebida == "aceso"){
    digitalWrite(2, HIGH);
    Serial.println("Led ligado: ");
  }

}
