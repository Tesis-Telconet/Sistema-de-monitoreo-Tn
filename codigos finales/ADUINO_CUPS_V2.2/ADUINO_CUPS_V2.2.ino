/////LIBRERIAS////////////////////////////
#include <Ethernet.h>++++++++++,,+
#include <SPI.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include "RestClient.h"
#include <SoftwareSerial.h>

int data = 0; //Serial Data
int data2 = 0; //RX TX Data
SoftwareSerial mySerial(8, 9); // RX, TX

///////ASIGNACION DE PINES PARA LOS SENSORES////////////////////////////
#define DHT1PIN 7
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
void softwareReset( uint8_t prescaller) {    // iniciar el perro guardián con el prescaller proporcionado
  wdt_enable( prescaller);                   // espere a que expire el tiempo de prescaller sin enviar la señal de reinicio usando el método wdt_reset ()
  while (1) {}}


void setup() {
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para puerto USB nativo
  }
  Serial.begin(9600);
  mySerial.begin(4800);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  analogReference(INTERNAL);
//  Serial.println("connect to network");
  dht1.begin();
  dht2.begin();
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  byte ip[] = { 192, 168, 254, 246 };
  Ethernet.begin(mac, ip);
}

///////DECLARACIÓN DE VARIABLES////////////////////////////
String texto;
String response;
String login;

void loop()
{
 //Serial.println("iniciando");
//  if (Serial.available() > 0) {
//    data = Serial.parseInt();
//    if (data == 1) {
//      goto a;}
//  }
  
  if (mySerial.available() > 0) {
    data2 = mySerial.read();
    if (data2 == 1) {
a:
      delay(5000);
      int sensorfire = digitalRead(3);
      if (sensorfire == HIGH) {
        texto = "Fuego C.UPS";
        http_POST_log(texto);
      }
      delay(5000);
      int sensorhmo = digitalRead(2);
      if (sensorhmo == LOW) {
        texto = "Smoke Alarm U";
        http_POST_log(texto);
      }
      delay(5000);
      float t6 = dht1.readTemperature();
      login = "T6";
      http_POST_sensor(t6, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h6 = dht1.readHumidity();
      login.replace("T6", "H6");
      http_POST_sensor(h6, "humidity",  "/SensorData/humedad");

      delay(5000);
      float t7 = dht2.readTemperature();
      login.replace("H6", "T7" );
      http_POST_sensor(t7, "temperature", "/SensorData/temperatura");

      delay(5000);
      float h7 = dht2.readHumidity();
      login.replace("T7", "H7");
      http_POST_sensor(h7, "humidity",  "/SensorData/humedad");
      
      
      ///// VERIFICACIÓN DE VALORES//////
    
      delay(5000);
      if (isnan(h6) || isnan(t7) )                                       //Comprobamos si ha habido algún error en la lectura
      { texto = "Error sensores C-UPS";
        http_POST_log(texto);
      //return;
      }
      else {
        delay(5000); 
        texto = "Sensores C-UPS OK";
        http_POST_log(texto);
           
       // if ( t6 >= 25 ||t7 >= 25    )
      //{ texto = "Temperatura alta: C-UPS"; //si la temperaura esta fuera del rango, nos envia un mensaje
       // http_POST_log(texto);}  
      }



   
      delay(5000);
      float pin = A0;
      float I1 = get_corriente(pin); //Corriente eficaz (A)
      login.replace("H7", "U1-L1");
      http_POST_sensor(I1, "corriente",  "/SensorData/corriente");
      

      delay(5000);
      float pin1 = A1;
      float I2 = get_corriente(pin1); //Corriente eficaz (A)
      login.replace("U1-L1", "U1-L2");
      http_POST_sensor(I2, "corriente",  "/SensorData/corriente");
      

      delay(5000);
        if (I1 < 1 ||I2 < 1  )                                        //Comprobamos si ha habido algún error en la lectura
      { texto = " UPS1 off";
        http_POST_log(texto);
        delay(5000);
      }

      float pin2 = A2;
      float I3 = get_corriente(pin2); //Corriente eficaz (A)
      login.replace("U1-L2", "U2-L1");
      http_POST_sensor(I3, "corriente",  "/SensorData/corriente");
      

      delay(5000);
      float pin3 = A3;
      float I4 = get_corriente(pin3); //Corriente eficaz (A)
      login.replace("U2-L1", "U2-L2");
      http_POST_sensor(I4, "corriente",  "/SensorData/corriente");
       

         delay(5000);
         if (I3 < 1 ||I4 < 1  )                                        //Comprobamos si ha habido algún error en la lectura
      { texto = "UPS2 off";
        http_POST_log(texto);
        delay(5000);//return;
      }

      
      float pin4 = A4;
      float I5 = get_corriente(pin4); //Corriente eficaz (A)
      login.replace("U2-L2", "U3-L1");
      http_POST_sensor(I5, "corriente",  "/SensorData/corriente");
;

      delay(5000);
      float pin5 = A5;
      float I6 = get_corriente(pin5); //Corriente eficaz (A)
      login.replace("U3-L1", "U3-L2");
      http_POST_sensor(I6, "corriente",  "/SensorData/corriente");
 ;
     
            delay(5000);
          if (I5 < 0.8 ||I6 < 1  )                                        //Comprobamos si ha habido algún error en la lectura
      { texto = "UPS3 off";
        http_POST_log(texto);
        delay(5000);//return;
      }
       
      



      texto = "";
      response = "";
      login = "";
      mySerial.write(1);
      Serial.println("Envio de dato");
      delay(1000);
      mySerial.write(1);
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
  //Serial.println(json);
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
    corriente = voltajeSensor * 100.0; //corriente=VoltajeSensor*(30A/1V)
    Sumatoria = Sumatoria + sq(corriente); //Sumatoria de Cuadrados
    N = N + 1;
    delay(1);
  }
  Sumatoria = Sumatoria * 2; //Para compensar los cuadrados de los semiciclos negativos.
  corriente = sqrt((Sumatoria) / N); //ecuación del RMS
  return (corriente);
}
