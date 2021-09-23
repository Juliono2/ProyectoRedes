//Varias de las lineas de codigo fueron puestas como comentarios, pues eran escenciales para 
//verificar el correcto funcionamiento enviando mensajes, pero la comunicacion entre la ESP32
//y el Arduino Nano, se realiza via el serial y especificamente "Serial.print" por lo tanto 
//estariamos enviando basura al ESP32 que se encarga de la conmunicacion con Blynk y ThingSpeak


#include <SimpleDHT.h>      //Libreria para el control del DHT11
#include <SPI.h>            //Libreria para la comunicacion entre microcontroladores
#include <LiquidCrystal.h>  //Libreria para funcionalidades de pantalla led

//Asignacion de pines analogos
#define humidity_sensor_pin A0
#define ldr_pin A5
#define MQ_pin A1
#define pin_lluvia A2

//Definimos los pines digitales que vamos a usar y variables
int pinDHT11 = 2;
SimpleDHT11 dht11;
int ldr_value = 0;
int water_pump_pin = 3;
int water_pump_speed = 255;
int sensorMQ2value;
int sensor_lluvia_value;
float RS;
float RO;
const float RL = 1.0;
const float CLEAN_AIR_RATIO = 9.80;
String EstadoLluvia;
String EstadoBomba;

//Pines para el uso de la pantalla LCD
const int rs = 12, en = 11, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);           //Usaremos un serial a velocidad de 9600 baudios
  lcd.begin(16, 2);             //Inicializamos la pantalla lcd
  lcd.print("KEAP A BIO-LIFE!");
  //delay(3000);
}

void loop() {
  //Serial.println("*******************************");
  //Serial.println("Muestra DHT11...");
  byte temperature = 0;
  byte humidity_in_air = 0;
  byte data[40] = {0};

  //Determina si se encuentra en funcionamiento el DHT11
  if (dht11.read(pinDHT11, &temperature, &humidity_in_air, data)) {
    Serial.print("Lectura del sensor DHT11 fallida");  //Informa de si deja de funcionar
    return;
  delay(9000);
  }
  
  //Serial.print("Muestra RAW Bits: ");
  //for (int i = 0; i < 40; i++) {
    //Serial.print((int)data[i]);
    //if (i > 0 && ((i + 1) % 4) == 0) {
      //Serial.print(' ');
    //}
  //}

  //CALCULAMOS LA TEMPERATURA, mostramos en serial y en la lcd.
  //Serial.println("");
  lcd.clear();
  //Serial.print("Muestra OK: ");
  //Serial.print("Temperatura: ");
  //Serial.print((int)temperature);
  // Serial.print(" *C, ");
  lcd.println("Temperatura:");
  lcd.print((int)temperature);
  lcd.print("C°");
  //delay(3000);

  //CALCULAMOS LA HUMEDAD DEL AIRE, mostramos en serial y en la lcd.
  lcd.clear();
  //Serial.print("Humedad relativa en aire: ");
  //Serial.print((int)humidity_in_air); 
  //Serial.println(" %");
  lcd.println("H. Relativa:");
  lcd.print((int)humidity_in_air);
  lcd.print("%");
  //delay(3000);

  //CALCULAMOS LA HUMEDAD DEL SUELO, mostramos en serial y en la lcd.
  lcd.clear();
  int ground_humidity_value = map(analogRead(humidity_sensor_pin), 0, 1023, 100, 0);
  //Serial.print("Humedad en suelo: ");
  //Serial.print(ground_humidity_value);
  //Serial.println("%");
  lcd.println("H. Suelo:");
  lcd.print(ground_humidity_value);
  lcd.print("%");
  //delay(3000);

  //CALCULAMOS EL PORCENTAJE DE LUZ, mostramos en serial y en la lcd.
  lcd.clear();
  int ldr_value = map(analogRead(ldr_pin), 1023, 0, 100, 0);
  //Serial.print("Luz: ");
  //Serial.print(ldr_value);
  //Serial.println("%");
  lcd.println("Luz:");
  lcd.print(ldr_value);
  lcd.print("%");
  //delay(3000);

  //CALCULAMOS EL PORCENTAJE DE METANO EN EL AMBIENTE, mostramos en serial y en la lcd.
  lcd.clear();
  sensorMQ2value = analogRead(MQ_pin);
  RS= ((float) RL *  (1023 - sensorMQ2value)/sensorMQ2value);
  //Serial.print("La concentracion de metano en el ambiente es: ");
  //Serial.println(RS);
  lcd.println("Conc. Met:");
  lcd.print(RS);
  //delay(3000);

  //SITUACIONAL DE SI ESTA LLOVIENDO
  lcd.clear();
  sensor_lluvia_value = analogRead(pin_lluvia); 
  if (sensor_lluvia_value < 300){
    EstadoLluvia = "Lluvia Intensa";
    //Serial.println("Lluvia Intensa");
    lcd.println("Estado:");
    lcd.print("Ll.Alt");
    //delay(3000);
    lcd.clear();
  }
  else if (sensor_lluvia_value < 500){
    EstadoLluvia = "Lluvia Moderada";
    //Serial.println("Lluvia Moderada");
    lcd.println("Estado:");
    lcd.print("Ll.Mod");
    //delay(3000);
    lcd.clear();
  }
  else{
    EstadoLluvia = "Lluvia no detectada";
    //Serial.println("Lluvia no detectada");
    lcd.println("Estado:");
    lcd.print("Ll.None");
    //delay(3000);
    lcd.clear();
  }

 //Dadas las condiciones, el sistema inicia el riego o lo detiene
 //Serial.println("*******************************");
 if( ground_humidity_value <= 50 && ldr_value < 30 && temperature < 30) {
 digitalWrite(water_pump_pin, HIGH);
 EstadoBomba = "Irrigando";
 //Serial.println("Irrigación");
 analogWrite(water_pump_pin, water_pump_speed);
 }
 else{
 digitalWrite(water_pump_pin, LOW);
 EstadoBomba = "Detenido";
 //Serial.println("Riego detenido");
 }

 //Preparamos la String que le vamos a enviar al ESP32 via Serial
 String resultadodht = (String)temperature+ "," + (String)humidity_in_air + "," ;
 String resultadoYL = (String)ground_humidity_value + ",";
 String resultadoldr = (String)ldr_value+ ",";
 String resultadoMQ = (String)((int)RS) + ",";
 String resultadoYL69 = EstadoLluvia + ",";
 String resultadoBomba = EstadoBomba + ",";
 String resultados = resultadodht + resultadoYL + resultadoldr + resultadoMQ + resultadoYL69 + EstadoBomba;

 //El ESP32 toma el mensaje del Serial escrito en pantalla y lo considera de tipo String
 Serial.println(resultados);
 delay (5000); 
}
