/*

Ligar o LED na porta D0 (16).
Sensor do reservatório secundário= porta D2
Sensor do reservatório principal= porta D3
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "****";
const char* password = "*****";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


boolean estadoReservatorio=digitalRead(D3); // Reservatório principal
boolean estadoAbastecimento=digitalRead(D2); //Reservatório secundário, responsável por manter abastecer o outro reservatório
boolean estadoAnterior=!estadoReservatorio;
boolean estadoRele=1;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Comando recebido [");
  Serial.print(topic);
  Serial.print("] ");

  
  String  payloadStr="";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  Serial.println();
  

if(String(topic).equals("reservatorio10/comando")){

  if (payloadStr.equals("ON") && estadoReservatorio==1) {   
    if(estadoAbastecimento==0){
       estadoRele=0;
       snprintf (msg, MSG_BUFFER_SIZE,"Reservatório em abastecimento.");
       Serial.print("Enviado o estado do reservatório: ");
       Serial.println(msg);
       client.publish("reservatorio10/info", msg);  
       digitalWrite(D5, 0); //Relé ligado
      }
      else if(estadoAbastecimento==1){
       Serial.println("LED não foi ligado, pois o reservatório de abastecimento está abaixo do nível"); 
       snprintf (msg, MSG_BUFFER_SIZE,"Quantidade de água insuficiente p/ abastecer");
       Serial.print("Informando que o reservatório de abastecimento está com pouca água:");
       Serial.println(msg);
       client.publish("reservatorio10/info", msg);  
        }

  }
  else if (payloadStr.equals("ON") && estadoReservatorio==0) {  
    Serial.println("LED não foi ligado, pois o reservatório está no nível ideal");
  }
}

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-leonardo7474";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Conexão realizada com sucesso");
      client.subscribe("reservatorio10/comando");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(16, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, 1); //Relé inicia desligado 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  digitalWrite(D5,estadoRele);
  estadoReservatorio=digitalRead(D3);
  estadoAbastecimento=digitalRead(D2);
  
  if(estadoReservatorio!=estadoAnterior){
    if(estadoReservatorio==0){
      estadoAnterior=0;
      estadoRele=1;
      Serial.println(estadoReservatorio);
      digitalWrite(D5, 1); //Relé desligado
      snprintf (msg, MSG_BUFFER_SIZE,"Está no nível ideal");
      Serial.print("Enviado o estado do reservatório: ");
      Serial.println(msg);
      client.publish("reservatorio10/info", msg);  
      }else if(estadoReservatorio==1){
        estadoAnterior=1;
        snprintf (msg, MSG_BUFFER_SIZE,"Abaixo do nível ideal");
        Serial.print("Enviado o estado do reservatório: ");
        Serial.println(msg);
        client.publish("reservatorio10/info", msg);  
      }
      snprintf (msg, MSG_BUFFER_SIZE,"%1d",estadoReservatorio );
      Serial.print("Enviado o estado do reservatório: ");
      Serial.println(msg);
      client.publish("reservatorio10/estado", msg);
  }

  delay(500);
}
