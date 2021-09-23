//#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
//#define BLYNK_DEVICE_NAME           "Device"

#define BLYNK_PRINT Serial

#include "ThingSpeak.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define RXD2 16
#define TXD2 17
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"


char auth[] = "################################";

//Nombre y contraseña de la red a la que se va a conectar
char ssid[] = "##############";
char pass[] = "##############";

WidgetLED led0(V0);
WidgetLED led1(V1);

BlynkTimer timer;
int ledStatusV0 = 1;
bool ledStatusV1 = false;


//Canal asignado en ThingSpeak y clave de escritura en la API
unsigned long channelID = 1510484;
const char* WriteAPIKey = "################";

void blinkLedWidget()
{
  if (ledStatusV1) {
    led1.setColor(BLYNK_RED);
    Serial.println("LED on V1: red");
    ledStatusV1 = false;
  } else {
    led1.setColor(BLYNK_GREEN);
    Serial.println("LED on V1: green");
    ledStatusV1 = true;
  }
}

void blinkColorLedWidget()
{
  if (ledStatusV0 == 1) {
    led1.setColor(BLYNK_GREEN);
    Serial.println("LED on V0: green");
  } else if (ledStatusV0 == 2) {
    led1.setColor(BLYNK_YELLOW);
    Serial.println("LED on V0: yellow");
  } else {
    led1.setColor(BLYNK_RED);
    Serial.println("LED on V0: red");
  }
}

//Creamos el cliente
WiFiClient cliente;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Comunicacion serial con el Arduino Nano, dado la velocidad en baudios, protocolo de comunicacion, pines de rx y tx
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  //Realizamos la conexion a la red y comprobamos su estado.
  //WiFi.begin(ssid,password);
    //if (WiFi.status() != WL_CONNECTED){
      //delay(500);
      //Serial.print(".");
    //}
  //Serial.println("WiFi Conectado");
  
  //Iniciamos Blynk
  Blynk.begin(auth, ssid, pass); 
  led1.on();
  led0.on();
  timer.setInterval(1000L,blinkLedWidget);
  timer.setInterval(1500L,blinkColorLedWidget);
  //Iniciamos ThingSpeak con el cliente
  ThingSpeak.begin(cliente);
  
  
}

//Funcion util para separar la String tomada del Arduino, retorna otra String
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {

  Blynk.run();//Colocamos en funcionamiento Blynk

  //Para ingresar datos al ThingSpeak
  ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.println("Datos enviados a ThingSpeak");
  delay(2000);
  
  //Tomamos la cadena enviada desde el Arduino
  String myString(Serial2.readString());

  //Separamos por variables de tipo String
  String temperaturaString = getValue(myString, ',', 0);
  String humidity_in_air_String = getValue(myString, ',', 1);
  String ground_humidity_value_String = getValue(myString, ',', 2);
  String ldr_value_String = getValue(myString, ',', 3);
  String metano_String = getValue(myString, ',', 4);
  String estado_Lluvia = getValue(myString, ',', 5);
  String estado_bomba = getValue(myString, ',', 6);

  if(estado_Lluvia == "Lluvia Intensa"){
    ledStatusV0 = 1;
  }

  //Convertimos las variables a tipo entero para poder graficar
  int temperatura = temperaturaString.toInt();
  int humidity_in_air = humidity_in_air_String.toInt();
  int ground_humidity_value = ground_humidity_value_String.toInt();
  int ldr_value= ldr_value_String.toInt();
  int metano = metano_String.toInt();

  //Imprimimos en el monitor serial de la ESP32
  Serial.println(temperatura);
  Serial.println(humidity_in_air);
  Serial.println(ground_humidity_value);
  Serial.println(ldr_value);
  Serial.println(metano);
  Serial.println(estado_Lluvia);
  Serial.println(estado_bomba);

  Serial.println("-----------------------");

  //Asignamos los campos segun el orden de graficado en ThingSpeak
  ThingSpeak.setField (1,temperatura);
  ThingSpeak.setField (2,humidity_in_air);
  ThingSpeak.setField (3,ground_humidity_value);
  ThingSpeak.setField (4,ldr_value);
  ThingSpeak.setField (5,metano);
  
  delay(5000);

  
}
