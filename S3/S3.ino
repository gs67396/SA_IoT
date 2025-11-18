#include <WiFi.h>
#include <PubSubClient.h>

WiFiClientSecure WiFiClient;
PubSubClient mqtt(client);

void setup() {
  //servo motor 1
  meuServo.attach(9);
  //servo motor 2
  meuServo.attach(13);

  //led rgb
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(2, OUTPUT);
  Serial.begin(115200);      //
  WiFiClient.SetInsecure();  //
  WiFi.begin(SSID, PASS);    //
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
  mqtt.subscribe(SA/SP/Presença); //inscrever topic
  mqtt.subscribe(SA/SP/Presença2); //inscrever topic
  mqtt.subscribe(SA/SP/Presença3); //inscrever topic
  Serial.println("conectado com sucesso!");
}

void loop() {

  // led rgb 1
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

  //ler ultrassonico
  //verificar se distancia < 10
  // mqtt.publish("SA/SP/Presença3", "presença");  //envia a mensagem (publica)
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
