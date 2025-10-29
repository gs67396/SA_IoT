#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>  ////28/20
#include <Servo.h>

// Inscricoes (recebe)

#define TOPIC "SP2/S2/Presença2"
#define TOPIC "SP4/S2/Presença4"

//publicacoes (envia)
#endif 

// cria objeto para WiFi
WiFiClientSecure WiFiClient;
// cria objeto para mqtt
PubSubClient mqtt(client);

//endereço = porta que está
const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

//topicos que S2 usara
const String topico = "SP2";  // temperatura /28/20
const String topico = "SP2";  // temperatura /28/20

const String brokerUser = "";  // variavel para o user do broker
const String brokerpass = "";  // variavel para a senha do broker

void setup() {
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
  mqtt.subscribe(TOPIC1);
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

void loop() {
  String mensagem = "";
  if (Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mqtt.publish("SP2", mensagem.c_str());  //envia a mensagem
  }
  mqtt.loop();  // mantém a conexão
}


void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }

  /* Exemplo / Abaixo led ligar e desligar, junto com as mensagens de confirmacao.
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
  */

// Servo 3
  if (Sensor_de_distancia = HIGH) {
  delay(1000); // Espera 1 segundo
  meuServo.write(180);
} else () {
  delay(1000);
  meuServo.write(o);
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
