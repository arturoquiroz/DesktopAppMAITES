#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>                
#include <DallasTemperature.h>
#include <SD.h>
#include <BH1750.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

OneWire ourWire(8);
DallasTemperature sensores(&ourWire);
DHT dht(DHTPIN, DHTTYPE);

File Archivo, Archivoconf;
RTC_DS1307 RTC;
BH1750 luxometro;
LiquidCrystal_I2C lcd(0x3F,20,4);



//VARIABLES PARA ALARMA
int segundopantalla = 0;
int segundo = 0;
int minuto = 0;
int hora = 0;
int aumento = 1;
boolean NoDisparada = true;
//VARIABLES PARA SENSOR DE CO2
const int LecturaCo2=0;
int Co2;
//VARIABLES PARA LUXOMETRO
uint16_t lux;
//VARIABLES PARA LEDS
int LedRojo = 3;
int LedVerde = 6;
//VARIABLES PARA SENSORES DE HUMEDAD Y TEMPERATURA
int humedad = 0;
float h_a = 0;
float t_a = 0;
float temperatura = 0;
//VARIABLES PARA AUTOMATIZACION
const char separador = ',';
int modo = 1;
int Co2min = 0;
int Co2max = 0;
int luxmin = 0;
int luxmax = 0;
float h_amin = 0;
float h_amax = 0;
float t_amax = 0;
float t_amin = 0;
float temperaturamax = 0;
float temperaturamin = 0;
int humedadmax = 0;
int humedadmin = 0;
int amanecer = 6;
int anochecer = 19;
String cadena;

boolean sdnoiniciada = true;

//CARACTERES ESPECIALES
      byte pala[8] = {
      B01110,
      B01010,
      B00100,
      B00100,
      B11111,
      B11111,
      B01110,
      B00100,
      };
      byte aire[8] = {
      B01001,
      B10010,
      B10010,
      B01001,
      B01001,
      B10010,
      B10010,
      B01001,
      };
      byte campana[8] = {
      B00100,
      B01110,
      B01110,
      B01110,
      B11111,
      B00000,
      B00100,
      B00000,
      };
      byte campanarota[8] = {
      B00100,
      B01010,
      B01010,
      B01010,
      B11011,
      B00000,
      B00100,
      B00000,
      };
      byte memoria[8] = {
      B11100,
      B11110,
      B11111,
      B11111,
      B11110,
      B11111,
      B11111,
      B11111,
      };
      byte memoriarota[8] = {
      B11100,
      B11110,
      B11101,
      B11011,
      B10110,
      B01111,
      B10111,
      B11011,
      };
      byte modoauto[8] = {
      B00111,
      B01110,
      B11100,
      B11111,
      B00011,
      B00110,
      B01100,
      B11000,
      };
      byte modomanual[8] = {
      B01110,
      B00100,
      B11111,
      B00100,
      B00100,
      B01010,
      B10001,
      B10001,
      };

void setup() {
  Wire.begin();
  RTC.begin();
  dht.begin();
  sensores.begin();
  lcd.init();
  lcd.backlight();
  DateTime now = RTC.now();
  hora = now.hour();
  hora+=aumento;
  segundopantalla = now.second();
  segundopantalla+=2;
  if(hora >= 24){
    hora-=24;
  }
  if(segundopantalla>=60){
    segundopantalla-=60;
  }
  luxometro.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  pinMode(LedRojo, OUTPUT);
  pinMode(LedVerde, OUTPUT);
      lcd.createChar(0, pala);
      lcd.createChar(1, aire);
      lcd.createChar(2, campana);
      lcd.createChar(3, campanarota);
      lcd.createChar(4, memoria);
      lcd.createChar(5, memoriarota);
      lcd.createChar(6, modoauto);
      lcd.createChar(7, modomanual);
      lcd.clear();
  if (!SD.begin(4)) {
      lcd.setCursor(0,0);
      lcd.write(byte(5));
      }else{
      lcd.setCursor(0,0);
      lcd.write(byte(4));
      Archivo = SD.open("lecturas.txt", FILE_WRITE);
      analogWrite(LedVerde, 130);  
      }
   lcd.setCursor(2,0);
   lcd.write(byte(7));
}

void loop() {
DateTime now = RTC.now();
sensores.requestTemperatures();
Co2 = analogRead(LecturaCo2);
lux = luxometro.readLightLevel();
h_a = dht.readHumidity();
t_a = dht.readTemperature();
temperatura = sensores.getTempCByIndex(0);
humedad = analogRead(A1);
if(hora == now.hour()){
        if(minuto == now.minute()){
          if(NoDisparada){
            NoDisparada = false;
            analogWrite(LedRojo, 150);
             hora += aumento;
             //minuto += aumento;
          if(hora>=24){
            hora -= 24;
          }
          //if(minuto>=60){
            //minuto-=60;
         // }
        Archivo = SD.open("lecturas.txt", FILE_WRITE);
      if (Archivo) {
        Archivo.println("valores");
        Archivo.print("Co2: ");
        Archivo.println(Co2);
        Archivo.print("Luxes: ");
        Archivo.println(lux);
        Archivo.print("Temp aire: ");
        Archivo.println(t_a);
        Archivo.print("Temp suelo: ");
        Archivo.println(temperatura);
        Archivo.print("Hum aire: ");
        Archivo.println(h_a);
        Archivo.print("Hum suelo: ");
        Archivo.println(humedad);
        Archivo.print(now.year());
        Archivo.print("/"); 
        Archivo.print(now.month());
        Archivo.print("/");
        Archivo.println(now.day());
        Archivo.print(now.hour());
        Archivo.print(":");
        Archivo.println(now.minute());     
        Archivo.close();
        
        }  
      }
    }
  }if(hora > now.hour() || hora == 0){
    if(now.minute() > minuto){
     NoDisparada = true;
      analogWrite(LedRojo, 0); 
     } 
    }
  if(segundopantalla == now.second()){
      segundopantalla+=2;
      lcd.setCursor(0,1);
      lcd.print(" Co2 : ");
      lcd.print(Co2);
      lcd.print(" Lux : ");
      lcd.print(lux);
      lcd.setCursor(1,2);
      lcd.print("Hum : ");
      lcd.print(h_a);
      lcd.write(byte(1));
      lcd.print(" ");
      lcd.print(humedad);
      lcd.write(byte(0));
      lcd.setCursor(1,3);
      lcd.print("Temp: ");
      lcd.print(t_a);
      lcd.write(byte(1));
      lcd.print(" ");
      lcd.print(temperatura);
      lcd.write(byte(0));
      if(segundopantalla>=60){
        segundopantalla-=60;
      }
      if(NoDisparada){
        lcd.setCursor(4,0);
        lcd.write(byte(2));
      }else{
        lcd.setCursor(4,0);
        lcd.write(byte(3));
      }
      lcd.setCursor(15,0);
      lcd.print(now.hour());
      lcd.print(":");
      if(now.minute()<10){
        lcd.print("0");
      }
      lcd.print(now.minute());
  }
}



void CargarVariables(){
  Archivoconf = SD.open("conf.txt");
  if(Archivoconf){
    while (Archivoconf.available()) {
      cadena  = Archivoconf.readStringUntil('\n');
  }
  Archivoconf.close();
  int vector[15];
   int contador=0;
   for(int i = 0; i < cadena.length(); i ++){
    String auxiliar  =  cadena.substring(i, i+1);
    if(auxiliar.equals(",")){
      vector[contador]=i;
      contador++;
    }
   }
 modo = (cadena.substring(0, vector[0])).toInt();
 aumento = (cadena.substring(vector[0]+1, vector[1])).toInt();
 h_amax = (cadena.substring(vector[1]+1, vector[2])).toFloat();
 h_amin = (cadena.substring(vector[2]+1, vector[3])).toFloat();
 humedadmax = (cadena.substring(vector[3]+1, vector[4])).toInt();
 humedadmin = (cadena.substring(vector[4]+1, vector[5])).toInt();
 t_amax = (cadena.substring(vector[5]+1, vector[6])).toFloat();
 t_amin = (cadena.substring(vector[6]+1, vector[7])).toFloat();
 temperaturamax = (cadena.substring(vector[7]+1, vector[8])).toFloat();
 temperaturamin = (cadena.substring(vector[8]+1, vector[9])).toFloat();
 luxmin = (cadena.substring(vector[9]+1, vector[10])).toInt();;
 luxmax = (cadena.substring(vector[10]+1, vector[11])).toInt();;
 Co2min = (cadena.substring(vector[11]+1, vector[12])).toInt();
 Co2max = (cadena.substring(vector[12]+1, vector[13])).toInt();
 amanecer = (cadena.substring(vector[13]+1, vector[14])).toInt();;
 anochecer = cadena.substring(vector[14]+1).toInt();
 }else{
 }
}


void automatizacion(){
  if(Co2 > Co2max){
    //ENCENDER ALGO
  }
  if(Co2 < Co2min){
    //ENCENDER OTRA COSA
  }
}

