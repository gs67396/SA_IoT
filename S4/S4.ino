#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client; 
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topico ="gabrielTopico";

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("*");
    delay(200);
  }
  Serial.println("conectado com sucesso");

   mqtt.setServer(brokerURL.c_str(),brokerPort);
  Serial.println("Conectando no Broker...");
  String boardID = "S4-";
  boardID += String(random(0xffff),HEX);


  while(!mqtt.connect(boardID.c_str())) { 
    Serial.print(".");
    delay(200);
  }
  
  mqtt.subscribe(topico.c_str());
  mqtt.setCallback(callback);
  Serial.println("conectado com sucesso");
}

void loop() {
  
  String mensagem = "";
  if(Serial.available() > 0){
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Gabriel: " + mensagem;
    mqtt.publish("pipa",mensagem.c_str());
  }
  mqtt.loop();

}

void callback(char* topic,byte* payload, unsigned long length){
  String mensagemRecebida = "";
  for(int i = 0;i <length; i++){
    mensagemRecebida += (char) payload[i];
  }
  Serial.println(mensagemRecebida);

  if(mensagemRecebida=="1"){
    digitalWrite(2,HIGH);
    mensagem = "LED_GABRIEL: LIGADO";
  }
  if(mensagemRecebida=="0"){
    digitalWrite(2,LOW);
  }
}


