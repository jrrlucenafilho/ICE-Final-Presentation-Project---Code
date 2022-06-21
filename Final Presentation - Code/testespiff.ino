
// Import bibliotecas
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT; 
#define DHT11_PIN 26 //define o pino do DHT
#define pinLed 14 // pino do led
// credenciais da rede
const char* ssid = "Tenda_B0EAC8";
const char* password = "";
//variavies de status
String humidade;
int temperatura = 0;
int sensorLigou = 0;
int statusTemp = 0;
String releStatatus1 = "OFF";
String releStatatus2 = "OFF";
String releStatatus3 = "OFF";
String releStatatus4 = "OFF";
//define o pino dos rele
int rele1 = 27;
int rele2 = 32;
int rele3 = 33;
int rele4 = 25;

// Cria o webserve na porta 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Função de envianr dados para a web
String processor(const String& var){
  if(var == "temperatura"){
    return String(temperatura);
  }
  else if(var == "humidade"){
    return String(humidade);
  }
  else if(var == "rele1"){
    return String(releStatatus1);
  }
  else if(var == "rele2"){
    return String(releStatatus2);
  }
    else if(var == "rele3"){
    return String(releStatatus3);
  }
  else if(var == "rele4"){
    return String(releStatatus4);
  }
  return String();
}

void setup(){
  //star no serial
  Serial.begin(115200);
  //start nos rele
  pinMode(pinLed, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  DHT.read(DHT11_PIN);
  //faz a primeira leitura dos sensores
  humidade = DHT.humidity;
  temperatura = DHT.temperature;
  // Start SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // conexão com o Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(14, LOW);
    delay(100);
    digitalWrite(14, HIGH);
    delay(100);
  }

  // Print ESP32 Local IP
  Serial.println(WiFi.localIP());

  // Inicializa a web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Inicializa a web
  server.on("/cover.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/cover.css", "text/css");
  });
    server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/js.js", "text/js");
  });

  // espera a chamada da pagina
  server.on("/on1", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele1, HIGH); 
    releStatatus1 = "ON";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/on2", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele2, HIGH); 
    releStatatus2 = "ON";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/on3", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele3, HIGH); 
    releStatatus3 = "ON";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/on4", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele4, HIGH); 
    releStatatus4 = "ON";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // espera a chamada da pagina
  server.on("/off1", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele1, LOW); 
    releStatatus1 = "OFF";   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/off2", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele2, LOW); 
    releStatatus2 = "OFF";   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
      server.on("/off3", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele3, LOW); 
    releStatatus3 = "OFF";   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
      server.on("/off4", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(rele4, LOW); 
    releStatatus4 = "OFF";   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //inicia um evento entre a pagina e a esp
   events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second     events.send(String(humidade).c_str(),"humidade",millis());
    client->send("hello!", NULL, millis(), 10000);
    events.send(String(temperatura).c_str(),"temperatura",millis());
    events.send(String(humidade).c_str(),"humidade",millis());
  });
  server.addHandler(&events);
  // Start server
  server.begin();
}
 
void loop(){
  DHT.read(DHT11_PIN);
  if(DHT.humidity != humidade.toInt() || DHT.temperature != temperatura ){
     humidade = DHT.humidity;
     temperatura = DHT.temperature;
     Serial.println(humidade);
     events.send(String(humidade).c_str(),"humidade",millis());
     events.send(String(temperatura).c_str(),"temperatura",millis());
  }
  if(temperatura > 30 && releStatatus1 == "OFF"){
    digitalWrite(rele1, HIGH);
    releStatatus1 = "ON";
    sensorLigou = 1;
    events.send(String(releStatatus1).c_str(),"rele1",millis());
  } 
  if(temperatura < 30 && sensorLigou == 1){
      digitalWrite(rele1, LOW);
      releStatatus1 = "OFF";
      sensorLigou = 0;
      events.send(String(releStatatus1).c_str(),"rele1",millis());
  }
  delay(5000);
  
}
