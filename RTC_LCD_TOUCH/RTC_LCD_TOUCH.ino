// Inicio Real Time Clock
//=======================

#include <Wire.h>   // Se inicia programa de comunicaciones por bus serie SDA/SCL
#define DS3231_I2C_ADDRESS 0x68  // Se define la direccion de memoria del reloj RTC para acceder
#include <LiquidCrystal_I2C.h>   // Se arranca libreria LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // Se define la direccion del LCD y tamaño 0x27 y 20 columnas 4 filas


String HORA_ACTUAL ="";      // variable texto para almacenar la hora

const byte LM35_COMEDOR = A1;
float VR_LM35_COMEDOR = analogRead(LM35_COMEDOR);
float TEMP_COMEDOR = VR_LM35_COMEDOR *500 /1024;
float TEMPERATURA_COMEDOR = TEMP_COMEDOR + "ºC";

const byte LED_AA_COMEDOR = 30;
const byte LED_CC_COMEDOR =31;   //PINES DE LOS LEDS DE CC Y AA


byte TEMP_CONFIG_LM35_COMEDOR;   //CAMBIO DE VALOR SI SE TENDRA QUE APAGAR O ENCENDER
byte TEMP_CONFIG_LM35_COMEDOR_ANTES;
boolean Config_LM35_COMEDOR= false; //BT ON O OFF

String Linea0 = "   CASA DOMOTICA    ";  //  pantalla LCD inicial
String Linea1 = "   MARIO VALIENTE   ";
String Linea2 = " TREBALL DE RECERCA ";
String Linea3 = "  SISTEMA INICIADO  ";
String Linea4 = "";

String comando_enviado_bt =""; //comando que enviaremos al bluetooth para que salga en la pantalla del mobil
String comando_bt ="";  // comando recivido desde el bluetooth



// UTIL RELOJ Conversion de decimal a hexadecimal
//=======================================================
byte decToBcd(byte val)
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
  Serial.begin(9600);

  lcd.init(); 
  lcd.backlight();
  lcd.clear ();

 comando_bt =""; //ponemos en blanco lo recivido

 pinMode (LM35_COMEDOR, INPUT);
 pinMode (LED_AA_COMEDOR, OUTPUT );
 pinMode (LED_CC_COMEDOR, OUTPUT );

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
   if (comando_bt == "1A1") {
      Config_LM35_COMEDOR = true;
      Serial.print ("Sensor Temp COMEDOR ON");
        Linea4 = "SENSOR AA ON COMEDOR";
        scroll_lcd();
     }
     if (comando_bt == "0A1") {
      Config_LM35_COMEDOR = false;
      Serial.print ("Sensor Temp COMEDOR OFF");
        Linea4 = "SENSR AA OFF COMEDOR";
        scroll_lcd();
     }
  //=========SENSOR TEMPERATURA COMEDOR==============
   if (Config_LM35_COMEDOR) {
     if (TEMP_COMEDOR >= 30) {
      TEMP_CONFIG_LM35_COMEDOR = 1
     }
   }
}
