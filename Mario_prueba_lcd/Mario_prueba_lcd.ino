#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


String linea0 ="  CASA DOMOTICA     ";
String linea1 ="  M. VALIENTE       ";
String linea2 ="  TREBALL RECERCA   ";
String linea3 ="  SISTEMA INICIADO  ";
String linea4 ="";

boolean FUEGO_AHORA = true;
boolean FUEGO_ANTES = true;

//====================================
void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear ();

pinMode (48,INPUT);

lcd.setCursor (0,0);
lcd.print (linea0);
lcd.setCursor (0,1);
lcd.print (linea1);
lcd.setCursor (0,2);
lcd.print (linea2);
lcd.setCursor (0,3);
lcd.print (linea3);

delay(1000);

}

//======================================
void loop() {
FUEGO_AHORA = digitalRead (48);
if (FUEGO_AHORA != FUEGO_ANTES) {
   if (!FUEGO_AHORA){
   linea4 = "ALARMA FUEGO ROOM1  ";
   }
   else {
   linea4 = "FUEGO EXTINGUIDO    ";
   }
  FUEGO_ANTES = FUEGO_AHORA;
   
  scrolllcd ();
  }
}

//=========================================
void scrolllcd (){
  lcd.setCursor (0,0);
linea0 = linea1;
lcd.print (linea0);

lcd.setCursor (0,1);
linea1 = linea2;
lcd.print (linea1);

lcd.setCursor (0,2);
linea2 = linea3;
lcd.print (linea2);


lcd.setCursor (0,3);
linea3 = linea4;
lcd.print (linea3);
}

