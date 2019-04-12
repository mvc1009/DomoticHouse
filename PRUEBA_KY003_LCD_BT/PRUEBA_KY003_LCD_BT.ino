// Inicio Real Time Clock
//=======================

#include <Wire.h>   // Se inicia programa de comunicaciones por bus serie SDA/SCL
#define DS3231_I2C_ADDRESS 0x68  // Se define la direccion de memoria del reloj RTC para acceder
#include <LiquidCrystal_I2C.h>   // Se arranca libreria LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // Se define la direccion del LCD y tamaño 0x27 y 20 columnas 4 filas


String HORA_ACTUAL ="";      // variable texto para almacenar la hora


const byte KY003_COMEDOR = 44;
const byte led_alarma = 22;


boolean SENSOR_KY003_COMEDOR = false;
boolean SENSOR_KY003_COMEDOR_ANTES = false;
boolean Config_KY003_COMEDOR = false;

String Linea0 = "   CASA DOMOTICA    ";  //  pantalla LCD inicial
String Linea1 = "   MARIO VALIENTE   ";
String Linea2 = " TREBALL DE RECERCA ";
String Linea3 = "  SISTEMA INICIADO  ";
String Linea4 = "";

String KY003_COMEDOR_ON = "PUERTA COMEDOR FORZADA";
String KY003_COMEDOR_OFF = "PUERTA COMEDOR CERRADA";


String comando_enviado_bt =""; //comando que enviaremos al bluetooth para que salga en la pantalla del mobil
String comando_bt ="";  // comando recivido desde el bluetooth


// UTIL RELOJ Conversion de decimal a hexadecimal
//=======================================================
byte decToBcd (byte val)
{   return( (val/10*16) + (val%10) ); }

// UTIL RELOJ Conversion de hexadecimal a decimal
//=======================================================
byte bcdToDec(byte val)
{   return( (val/16*10) + (val%16) ); }

// UTIL RELOJ Poner en hora el reloj , ejemplo setDS3231time(59, 14, 12, 1, 23, 8, 15);
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
//==============================================================================================================
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

// UTIL RELOJ Pedir hora a reloj , ejemplo HORA_ACTUAL = readDS3231time(); equivale a "23/08/15 12:14:59"
//=======================================================================================================
String readDS3231time()
{
 byte  *second ;
 byte  *minute ;
 byte  *hour ;
 byte  *dayOfWeek ;
 byte  *dayOfMonth ;
 byte  *month ;
 byte  *year ;

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
*second = bcdToDec(Wire.read() & 0x7f);
*minute = bcdToDec(Wire.read());
*hour = bcdToDec(Wire.read() & 0x3f);
*dayOfWeek = bcdToDec(Wire.read());
*dayOfMonth = bcdToDec(Wire.read());
*month = bcdToDec(Wire.read());
*year = bcdToDec(Wire.read());

String fechayhora = "";
if (*dayOfMonth<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*dayOfMonth) + "/" ;
if (*month<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*month) + "/" ;
if (*year<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*year) ;

fechayhora = fechayhora + " ";

if (*hour<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*hour) + ":" ;
if (*minute<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*minute) + ":" ;
if (*second<10) { fechayhora = fechayhora + "0" ; }
fechayhora = fechayhora + String(*second) ;

return fechayhora;
}


//============== INICIALIZACION DEL ARDUINO ============================
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);

  comando_bt ="";  //ponemos en blanco lo recivido
      // Si se desea inicializar la hora
      // DS3231 seconds, minutes, hours, day, date, month, year
      // setDS3231time(05,01,15,3,18,8,15);
  lcd.init(); 
  lcd.backlight();
  lcd.clear ();

  pinMode (KY003_COMEDOR, INPUT ); //SENSOR FUEGO
  pinMode (led_alarma, OUTPUT);
 

  lcd.setCursor (0,0);
  lcd.print (Linea0);
  lcd.setCursor (0,1);
  lcd.print (Linea1);   //imprimiendo la pantalla LCD inicial
  lcd.setCursor (0,2);
  lcd.print (Linea2);
  lcd.setCursor (0,3);
  lcd.print (Linea3);
  
  delay (1000); // dejar un tiempo de espera antes de ejecutar LOOP
}


//============== INICIALIZACION DEL PROGRAMA "LOOP" ============================
void loop()
{
 while (Serial1.available()>0) {      
 delay(100);
 char comando_bt_recivido = Serial1.read();
 comando_bt = comando_bt + comando_bt_recivido;
 }
    
    if (comando_bt != "") {
         Serial.print ("COMANDO RECIBIDO: ");
         Serial.println (comando_bt);
        }
    if (comando_bt == "144") {
      Config_KY003_COMEDOR = true;
        Serial.println ("SENSOR PUERTA DEL COMEDOR ACTIVADO");    
     Linea4 = " SENSR DOOR COM ON  ";
     scroll_lcd ();
    }
      if (comando_bt == "044") {
      Config_KY003_COMEDOR = false;
      Serial.println ("SENSOR PUERTA DEL COMEDOR DESACTIVADO");    
         Linea4 = " SENSR DOOR COM OFF "; 
        scroll_lcd();
    }
    

    if (Config_KY003_COMEDOR) {
   
    
    SENSOR_KY003_COMEDOR = digitalRead (KY003_COMEDOR);  //LEEMOS EL SENSOR SI HAY EFECTO HALL
    
         if (SENSOR_KY003_COMEDOR != SENSOR_KY003_COMEDOR_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (!SENSOR_KY003_COMEDOR) {
            
           digitalWrite (led_alarma, LOW);
                             
           comando_enviado_bt = HORA_ACTUAL + KY003_COMEDOR_OFF;  //enviar al bluetooth un emergente
           Serial.println (comando_enviado_bt);
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "DOOR COMEDOR CERRADA";
  
           }
             else {
            digitalWrite (led_alarma, HIGH);
                   
            comando_enviado_bt = HORA_ACTUAL + KY003_COMEDOR_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           Serial.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "DOOR COMEDOR ABIERTA";
            }
             scroll_lcd ();
            SENSOR_KY003_COMEDOR_ANTES = SENSOR_KY003_COMEDOR;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
  }
 
comando_bt= ""; //borramos comando recivido para poder enviar otro
}
//============================================

void scroll_lcd () {   // scroll del lcd

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



