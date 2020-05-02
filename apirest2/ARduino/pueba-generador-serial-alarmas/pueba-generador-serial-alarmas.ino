/////LIBRERIAS////////////////////////////
#include <Ethernet.h>
#include <SPI.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include "RestClient.h"
#include <SoftwareSerial.h>

int data = 0; //Serial Data
int data2 = 0; //RX TX Data
SoftwareSerial mySerial(8, 9); // RX, TX

///////ASIGNACION DE PINES PARA LOS SENSORES////////////////////////////
#define DHT1PIN 5
#define DHT2PIN 6

///////DEFINIR EL TIPO DE SENSOR DHT////////////////////////////
#define DHT1TYPE DHT22
#define DHT2TYPE DHT22


DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);


///////PARAMETROS PARA ENVIAR DATOS A LA APIREST////////////////////////////
#define IP "192.168.254.240" // Server IP
#define PORT 5000     // Server Port
RestClient client = RestClient(IP, PORT);

#include <avr/wdt.h>

void softwareReset( uint8_t prescaller) {
  // iniciar watchdog con el precalculador proporcionado
  wdt_enable( prescaller);
  // espere a que expire el tiempo del pre-llamador
  // sin enviar la señal de reinicio usando
  // el método wdt_reset ()
  while (1) {}
}


void setup() {
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para puerto USB nativo
  }
  Serial.begin(9600);
  mySerial.begin(4800);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  analogReference(INTERNAL);
  Serial.println("connectado");
  dht1.begin();
  dht2.begin();
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
  Serial.println("iniciando");
  if (Serial.available() > 0) {
    data = Serial.parseInt();
    if (data == 1) {
      goto a;
    }
  }

  if (mySerial.available() > 0) {
    data2 = mySerial.read();
    if (data2 == 1) {
a:

      int sensorfire = digitalRead(3);
      if (sensorfire == HIGH) {
        texto = "Fire Alarm";
        http_POST_log(texto);
      }

      int sensorhmo = digitalRead(2);
      if (sensorhmo == LOW) {
        texto = "Smoke Alarm";
        http_POST_log(texto);
      }

      delay(1000);
      float pin = A0;
      float I1 = get_corriente(pin); //Corriente eficaz (A)
      login = "R-LR";
      http_POST_sensor(I1, "corriente",  "/SensorData/corriente");
      float V1 = I1 * 0.55 ;
      http_POST_sensor(V1, "voltaje",  "/SensorData/voltaje");

      delay(1000);
      float pin1 = A1;
      float I2 = get_corriente(pin1); //Corriente eficaz (A)
      login.replace("R-LR", "R-LS");
      http_POST_sensor(I2, "corriente",  "/SensorData/corriente");
      float V2 = I2 * 0.55 ;
      http_POST_sensor(V2, "voltaje",  "/SensorData/voltaje");

      delay(1000);
      float pin2 = A2;
      float I3 = get_corriente(pin2); //Corriente eficaz (A)
      login.replace("R-LS", "R-LT");
      http_POST_sensor(I3, "corriente",  "/SensorData/corriente");
      float V3 = I3 * 0.55 ;
      http_POST_sensor(V3, "voltaje",  "/SensorData/voltaje");
          
          
          if (I1 < 1 ||I2 < 1 ||I3 < 1  )                                        //Comprobamos si ha habido algún error en la lectura
      { texto = "Voltaje RED OFF";
        http_POST_log(texto);
        
       delay(1000);
      float pin3 = A3;
      float I4 = get_corriente(pin3); //Corriente eficaz (A)
      login.replace("R-LT", "G-LR");
      http_POST_sensor(I4, "corriente",  "/SensorData/corriente");
      float V4 = I4 * 0.55 ;
      http_POST_sensor(V4, "voltaje",  "/SensorData/voltaje");

      delay(1000);
      float pin4 = A4;
      float I5 = get_corriente(pin4); //Corriente eficaz (A)
      login.replace("G-LR", "G-LS");
      http_POST_sensor(I5, "corriente",  "/SensorData/corriente");
      float V5 = I5 * 0.55 ;
      http_POST_sensor(V5, "voltaje",  "/SensorData/voltaje");

      delay(1000);
      float pin5 = A5;
      float I6 = get_corriente(pin5); //Corriente eficaz (A)
      login.replace("G-LS", "G-LT");
      http_POST_sensor(I6, "corriente",  "/SensorData/corriente");
      float V6 = I6 * 0.55 ;
      http_POST_sensor(V6, "voltaje",  "/SensorData/voltaje");
      delay(1000);
      texto = "Generador ON";
        http_POST_log(texto);
      }else{
        texto = "RED EEQ OK";
        http_POST_log(texto);
      }


      texto = "";
      response = "";
      login = "";
      mySerial.write(1);
      Serial.println("Envio de dato");
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
  delay(1000);
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
  delay(1000);
}

float get_corriente(float pin)
{
  float voltajeSensor;
  float corriente = 0;
  float Sumatoria = 0;
  long tiempo = millis();
  int N = 0;
  while (millis() - tiempo < 500) //Duración 0.5 segundos(Aprox. 30 ciclos de 60Hz)
  {
    voltajeSensor = analogRead(pin) * (1.1 / 1024.0);////voltaje del sensor
    corriente = voltajeSensor * 200.0; //corriente=VoltajeSensor*(30A/1V)
    Sumatoria = Sumatoria + sq(corriente); //Sumatoria de Cuadrados
    N = N + 1;
    delay(1);
  }
  Sumatoria = Sumatoria * 2; //Para compensar los cuadrados de los semiciclos negativos.
  corriente = sqrt((Sumatoria) / N); //ecuación del RMS
  return (corriente);
}
