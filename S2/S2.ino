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

const String brokerUser = ""; // variavel para o user do broker
const String brokerpass = ""; // variavel para a senha do broker

void setup() {
  Serial.begin(115200); //configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //tenta conectar na rede
  Serial.println("conectado no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nconectado com sucesso");


  // INICIO DO CODIGO: configura o servidor
  mqtt.setServer(brokerURL.c_str(),brokerPort);
  Serial.println("Conectando no Broker");
  //cria um nome que começa com S2
  String boardID = "S2-";
  //junta o S1 com um numero aleatorio
  boardID += String(random(0xffff),HEX);

  // enquando nao estiver conectado mostra "."
  while(!mqtt.connect(boardID.c_str())) { // nao estiver conectado
    Serial.print(".");
    delay(200);
      }
}

void loop() {
  // put your main code here, to run repeatedly:

}
