#include <WiFi.h>
#include <PubSubClient.h>

// cria objeto para WiFi
WiFiClient client;
// cria objeto para mqtt
PubSubClient mqtt(client);

//Nome da rede
const String SSID = "FIESC_IOT_EDU";
// define senha
const String PASS = "8120gv08";

//endereço
const String brokerURL = "test.mosquitto.org";
//porta que está
const int brokerPort = 1883;
const String topico = "pipa"; // nome do topico

const String brokerUser = "";  // variavel para o user do broker
const String brokerpass = "";  // variavel para a senha do broker

void setup() {
  pinMode(2, OUTPUT);

  Serial.begin(115200);    //configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS);  //tenta conectar na rede
  Serial.println("conectado no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\n conectado com sucesso");


  // INICIO DO CODIGO: configura o servidor
  mqtt.setServer(brokerURL.c_str(), brokerPort);
  Serial.println("Conectando no Broker");
  //cria um nome que começa com S2
  String boardID = "S2-";
  //junta o S1 com um numero aleatorio
  boardID += String(random(0xffff), HEX);

  // enquando nao estiver conectado mostra "."
  while (!mqtt.connect(boardID.c_str())) {  // nao estiver conectado
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(topico.c_str());
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso!");
}

void loop() {
    String mensagem = "";
    if (Serial.available() > 0) {
      mensagem = Serial.readStringUntil('\n');
      Serial.print("Mensagem digitada: ");
      Serial.println(mensagem);
      mqtt.publish("gabrielTopico", mensagem.c_str());  //envia a mensagem
    }
    mqtt.loop();  // mantém a conexão
}

//abaixo led ligar e desligar, junto com as mensagens de confirmacao.
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

<<<<<<< HEAD
}
=======
}
>>>>>>> 7b0a06d7acbebe57d133ef952cb8a98726d48d51
