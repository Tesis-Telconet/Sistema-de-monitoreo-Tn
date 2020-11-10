

/////LIBRERIAS////////////////////////////
#include <Ethernet.h>
#include <SPI.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include "RestClient.h"
#include <SoftwareSerial.h>

int sensorfire = 0;
int sensorpuerta= 0;
int data = 0; //Serial Data
int data2 = 0; //RX TX Data
SoftwareSerial mySerial(A4, A5); // RX, TX

///////ASIGNACION DE PINES PARA LOS SENSORES////////////////////////////
#define GAS_PIN A0
#define DHT1PIN 3
#define DHT2PIN 5
#define DHT3PIN 6
#define DHT4PIN 7
#define DHT5PIN 8

///////DEFINIR EL TIPO DE SENSOR DHT////////////////////////////
#define DHT1TYPE DHT22
#define DHT2TYPE DHT22
#define DHT3TYPE DHT22
#define DHT4TYPE DHT22
#define DHT5TYPE DHT22

DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);
DHT dht3(DHT3PIN, DHT3TYPE);
DHT dht4(DHT4PIN, DHT4TYPE);
DHT dht5(DHT5PIN, DHT5TYPE);


///////PARAMETROS PARA ENVIAR DATOS A LA APIREST////////////////////////////
#define IP "192.168.254.240" // Server IP
#define PORT 5000     // Server Port
RestClient client = RestClient(IP, PORT);

#include <avr/wdt.h>

void softwareReset( uint8_t prescaller) {
  // start watchdog with the provided prescaller
  wdt_enable( prescaller);
  // wait for the prescaller time to expire
  // without sending the reset signal by using
  // the wdt_reset() method
  while (1) {}
}


void setup() {
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para puerto USB nativo
  }
  Serial.begin(9600);
  mySerial.begin(4800);
  pinMode(2, INPUT_PULLUP);
  //pinMode(3, INPUT_PULLUP);
 
  Serial.println("conectado");
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  byte ip[] = { 192, 168, 254, 242 };
  Ethernet.begin(mac, ip);
}
///////DECLARACIÓN DE VARIABLES////////////////////////////
String texto;
String response;
String login;


void loop()
{

  //Serial.println("iniciando");
  
  
  if (mySerial.available() > 0) {
    data2 = mySerial.read();
    if (data2 == 1) {
a:

      //sensorfire = digitalRead(3);
      //if (sensorfire == HIGH) {
       // texto = "Fuego Detectado";
        //http_POST_log(texto);
      //}

      sensorpuerta = digitalRead(2);
      if (sensorpuerta == HIGH) {
        texto = "Puerta Abierta";
        http_POST_log(texto);
      }
      delay(5000);
      float t1 = dht1.readTemperature();
      login = "T1";
      http_POST_sensor(t1, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h1 = dht1.readHumidity();
      login.replace("T1", "H1");
      http_POST_sensor(h1, "humidity",  "/SensorData/humedad");

      delay(5000);
      float t2 = dht2.readTemperature();
      login.replace("H1", "T2" );
      http_POST_sensor(t2, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h2 = dht2.readHumidity();
      login.replace("T2", "H2");
      http_POST_sensor(h2, "humidity",  "/SensorData/humedad");

      delay(5000);
      float t3 = dht3.readTemperature();
      login.replace("H2", "T3" );
      http_POST_sensor(t3, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h3 = dht3.readHumidity();
      login.replace("T3", "H3");
      http_POST_sensor(h3, "humidity",  "/SensorData/humedad");

      delay(5000);
      float t4 = dht4.readTemperature();
      login.replace("H3", "T4" );
      http_POST_sensor(t4, "temperature", "/SensorData/temperatura");
      
      delay(5000);
      float h4 = dht4.readHumidity();
      login.replace("T4", "H4");
      http_POST_sensor(h4, "humidity",  "/SensorData/humedad"); 

      delay(5000);
      float t5 = dht5.readTemperature();
      login.replace("H4", "T5" );
      http_POST_sensor(t5, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h5 = dht5.readHumidity();
      login.replace("T5", "H5");
      http_POST_sensor(h5, "humidity",  "/SensorData/humedad");
   

      delay(5000);
      float g1  = analogRead(A0);
      login.replace("H5", "MQ1");
      http_POST_sensor(g1, "cant_gas",  "/SensorData/gas");
      if ( g1 > 100 )                                   //establecemos el rango de temperatura
      { texto = "Presencia Humo"; //si la temperaura esta fuera del rango, nos envia un mensaje
        http_POST_log(texto);
      }
      
      delay(5000);
       if ( t1 >= 21 ||t2 >= 21 ||t3 >= 21 ||t4 >= 21 ||t4 >= 21   )
      { texto = "Temperatura alta:Nodo"; //si la temperaura esta fuera del rango, nos envia un mensaje
        http_POST_log(texto);
      }

      delay(5000);
       if ( t1 <= 15 ||t2 <= 15 ||t3 <= 15 ||t4 <= 15 || t5 <= 15   )
      { texto = "Temperatura alta:Nodo"; //si la temperaura esta fuera del rango, nos envia un mensaje
        http_POST_log(texto);
      }

      delay(5000);
      if (isnan(h1) || isnan(t2) || isnan(h3) || isnan(t4)|| isnan(t5)  )                                     //Comprobamos si ha habido algún error en la lectura
      { texto = "Error lectura sensores Nodo";
        http_POST_log(texto);
        //return;
      }
      else {
        texto = "Sistema Nodo OK";
        http_POST_log(texto);
      }
      

      texto = "";
      response = "";
      login = "";
      mySerial.write(1);
      Serial.println("Envio");
      delay(1000);
      mySerial.write(2);
      delay(1000);
      softwareReset( WDTO_4S);
    }

  }
  delay(1000);
}


///////FUNCIONES////////////////////////////////////////////////////////

int http_POST_sensor(float sensor, String tag , const char* dir_recurso) {
  response = "";
  client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
  client.setHeader("Content-Type: application/json");
  StaticJsonBuffer<200> jsonBuffer;
  char json[256];
  JsonObject& root = jsonBuffer.createObject();
  root[tag] = sensor;
  root["login"] = login;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  int statusCode = client.post(dir_recurso, json, &response);
  //Serial.print("Status code from server: ");
  //Serial.println(statusCode);
  //Serial.print("Response body from server: ");
  //Serial.println(response);
  //return statusCode;
  delay(5000);
}


int http_POST_log(String texto) {
  String response = "";
  client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
  client.setHeader("Content-Type: application/json");
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root["log"] = texto;
  root.printTo(Serial);
  char json[270];
  root.printTo(json, sizeof(json));
  int statusCode = client.post("/SensorData/log", json, &response);
  //Serial.print("Status code from server: ");
  //Serial.println(statusCode);
  //Serial.print("Response body from server: ");
  //Serial.println(response);
  //return statusCode;
  delay(5000);
}
