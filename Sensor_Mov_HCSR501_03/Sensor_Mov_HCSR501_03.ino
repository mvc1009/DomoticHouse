#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

boolean Sensor_mov = false;
boolean Sensor_mov_antes = false;

String Linea0 = "   CASA DOMOTICA    ";  //  pantalla LCD inicial
String Linea1 = "   MARIO VALIENTE   ";
String Linea2 = " TREBALL DE RECERCA ";
String Linea3 = "  SISTEMA INICIADO  ";
String Linea4 = "";

//====================================================
void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
pinMode (40, INPUT);  //SENSOR DE MOVIMIENTO
pinMode (22, OUTPUT); //LED ALARMA

lcd.setCursor (0,0);
lcd.print (Linea0);
lcd.setCursor (0,1);
lcd.print (Linea1);   //imprimiendo la pantalla LCD inicial
lcd.setCursor (0,2);
lcd.print (Linea2);
lcd.setCursor (0,3);
lcd.print (Linea3);

delay(1000);
}
//=================================================================
void loop() {
  // put your main code here, to run repeatedly:
Sensor_mov = digitalRead (40);  //LEEMOS EL SENSOR SI HAY MOVIMIENTO O NO

if (Sensor_mov != Sensor_mov_antes){    // comparamos si la variable actual es igual que la anterior
  
  if  (Sensor_mov)
  {
  digitalWrite (22, HIGH);
  Serial.println ("INTRUSO DETECTADO ALARMA ACTIVADA");
  Linea4 = " INTRUSO DETECTADO  ";
  
  }
  else
  {
  digitalWrite (22, LOW);
  Serial.println ("ALARMA DE MOVIMIENTO DESACTIVADA");
  Linea4 = "   ALARM MOV OFF    ";
  }
    scroll_lcd ();
Sensor_mov_antes = Sensor_mov;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
}

}
//============================================

void scroll_lcd() {   // scroll del lcd

lcd.setCursor (0,0);
Linea0 = Linea1;    // funcion que permite que las letras dela pantalla van subiendo hacia arriba
lcd.print (Linea0);

lcd.setCursor (0,1);
Linea1 = Linea2;
lcd.print (Linea1);

lcd.setCursor (0,2);
Linea2 = Linea3;
lcd.print (Linea2);

lcd.setCursor (0,3);
Linea3 = Linea4;
lcd.print (Linea3);
  
}

