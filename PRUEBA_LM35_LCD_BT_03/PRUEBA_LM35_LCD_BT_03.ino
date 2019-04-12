// Inicio Real Time Clock
//=======================

#include <Wire.h>   // Se inicia programa de comunicaciones por bus serie SDA/SCL
#define DS3231_I2C_ADDRESS 0x68  // Se define la direccion de memoria del reloj RTC para acceder
#include <LiquidCrystal_I2C.h>   // Se arranca libreria LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // Se define la direccion del LCD y tamaño 0x27 y 20 columnas 4 filas


String HORA_ACTUAL ="";      // variable texto para almacenar la hora

const byte LM35_COMEDOR = A1;
int VR_LM35_COMEDOR;  // valor analogedread que va de 0 a 1024
float TEMP_COMEDOR; //si no utilizamos decimales podria de byte o int, con int nos aseguramos de que no fallamos
String TEMPERATURA_COMEDOR ="";
String BT_TEMP_COMEDOR ="";

const byte LM35_HABITACION = A2;
int VR_LM35_HABITACION;  // valor analogedread que va de 0 a 1024
float TEMP_HABITACION; //si no utilizamos decimales podria de byte o int, con int nos aseguramos de que no fallamos
String TEMPERATURA_HABITACION ="";
String BT_TEMP_HABITACION ="";

const byte LED_AA_COMEDOR = 30;
const byte LED_CC_COMEDOR =31;   //PINES DE LOS LEDS DE CC Y AA
const byte LED_AA_HABITACION = 34;
const byte LED_CC_HABITACION =35;   //PINES DE LOS LEDS DE CC Y AA


byte TEMP_CONFIG_LM35_COMEDOR;   //CAMBIO DE VALOR SI SE TENDRA QUE APAGAR O ENCENDER
byte TEMP_CONFIG_LM35_COMEDOR_ANTES;
boolean Config_LM35_COMEDOR = false; //BT ON O OFF
byte TEMP_CONFIG_LM35_HABITACION;   //CAMBIO DE VALOR SI SE TENDRA QUE APAGAR O ENCENDER
byte TEMP_CONFIG_LM35_HABITACION_ANTES;
boolean Config_LM35_HABITACION = false; //BT ON O OFF

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
  Serial1.begin(9600);

  lcd.init(); 
  lcd.backlight();
  lcd.clear ();

 comando_bt =""; //ponemos en blanco lo recivido
 HORA_ACTUAL = readDS3231time();
 HORA_ACTUAL = HORA_ACTUAL + "   ";

 pinMode (LM35_COMEDOR, INPUT);
 pinMode (LED_AA_COMEDOR, OUTPUT );
 pinMode (LED_CC_COMEDOR, OUTPUT );
 pinMode (LM35_HABITACION, INPUT);
 pinMode (LED_AA_HABITACION, OUTPUT );
 pinMode (LED_CC_HABITACION, OUTPUT );

// imprimiendo la pantalla LCD inicial
// ===================================
lcd.setCursor (0,0);
lcd.print (Linea0);
lcd.setCursor (0,1);
lcd.print (Linea1);   
lcd.setCursor (0,2);
lcd.print (Linea2);
lcd.setCursor (0,3);
lcd.print (Linea3);
  
  delay (1000); // dejar un tiempo de espera antes de ejecutar LOOP
}


//============== INICIALIZACION DEL PROGRAMA "LOOP" ============================
void loop()
{

// lectura de sensor temperatura comedor
// ====================================
delay (1000); // para parar un poco la impresion en el Serial monitor luego se quitara

VR_LM35_COMEDOR = analogRead(LM35_COMEDOR);
Serial.print ( "VALOR LEIDO  COMEDOR : ");
Serial.print (VR_LM35_COMEDOR );
TEMP_COMEDOR = (VR_LM35_COMEDOR *500 / 1024 ) ; // si dividimos entre 1024 resultado es INT, entre 1024.0 resultado es LONG
Serial.print ( "  -->  Temperatura COMEDOR : ");
Serial.print (TEMP_COMEDOR);
Serial.println ( " C");
TEMPERATURA_COMEDOR = String(TEMP_COMEDOR);
TEMPERATURA_COMEDOR = TEMPERATURA_COMEDOR + " C";
Serial.println (TEMPERATURA_COMEDOR);

// lectura de sensor temperatura habitacion
// ========================================
VR_LM35_HABITACION = analogRead(LM35_HABITACION);
Serial.print ( "VALOR LEIDO HABITACION : ");
Serial.print (VR_LM35_HABITACION );
TEMP_HABITACION = (VR_LM35_HABITACION *500 / 1024 ) ; // si dividimos entre 1024 resultado es INT, entre 1024.0 resultado es LONG
Serial.print ( "  -->  Temperatura  HABITACION: ");
Serial.print (TEMP_HABITACION);
Serial.println ( " C");
TEMPERATURA_HABITACION = String(TEMP_HABITACION);
TEMPERATURA_HABITACION = TEMPERATURA_HABITACION + " C";
Serial.println (TEMPERATURA_HABITACION);

// lectura de comando BT y comprobacion de comando
// ===============================================
while (Serial1.available() > 0)  {      
    delay(100);
    char comando_bt_recivido = Serial1.read();
    comando_bt = comando_bt + comando_bt_recivido;
    }  

if (comando_bt != "")  {
    Serial.print ("COMANDO RECIBIDO: ");
    Serial.println (comando_bt);
    }
if (comando_bt == "1A1") {
    Config_LM35_COMEDOR = true;
    // le decimos que el estdo anterior es reposo para que empiece a comprobar 
    TEMP_CONFIG_LM35_COMEDOR_ANTES = 2 ;
    Serial.println ("Sensor Temp COMEDOR ON");
    Linea4 = "SENS TMP ON COMEDOR ";
    scroll_lcd();
    }
 
if (comando_bt == "0A1") {
    Config_LM35_COMEDOR = false;
    // apagamos los leds por si acaso estan encendidos
    digitalWrite(LED_AA_COMEDOR, LOW);
    digitalWrite(LED_CC_COMEDOR, LOW);
    Serial.println ("Sensor Temp COMEDOR OFF");
    Linea4 = "SENS TMP OFF COMEDOR";
    scroll_lcd();
    }
if (comando_bt == "1A2") {
    Config_LM35_HABITACION = true;
    // le decimos que el estdo anterior es reposo para que empiece a comprobar 
    TEMP_CONFIG_LM35_HABITACION_ANTES = 2 ;
    Serial.println ("Sensor Temp HABITACION ON");
    Linea4 = "SENS TMP ON HABITAC ";
    scroll_lcd();
    }
 
if (comando_bt == "0A2") {
    Config_LM35_HABITACION = false;
    // apagamos los leds por si acaso estan encendidos
    digitalWrite(LED_AA_HABITACION, LOW);
    digitalWrite(LED_CC_HABITACION, LOW);
    Serial.println ("Sensor Temp HABITACION OFF");
    Linea4 = "SENS TMP OFF HABITAC";
    scroll_lcd();
    }
comando_bt="";  // la limpieza del comando leido tiene que SER DETRAS HABER TODOS LOS POSIBLESS COMANDOS
  
// TEMPERATURA COMEDOR actuar AA+CF
// ================================
if (Config_LM35_COMEDOR) {
      if (TEMP_COMEDOR >= 30) {
          TEMP_CONFIG_LM35_COMEDOR = 1; // 1 encender AA  2 dentro de margen NADA  3 encender CF
          }
      if ((TEMP_COMEDOR > 15) && (TEMP_COMEDOR < 30)) {
          TEMP_CONFIG_LM35_COMEDOR = 2;
          }
      if (TEMP_COMEDOR <= 15) {
          TEMP_CONFIG_LM35_COMEDOR = 3;
          }
       Serial.print ("CONFIGURACION ANTES: ");
       Serial.print (TEMP_CONFIG_LM35_COMEDOR_ANTES);
       Serial.print ("  //  CONFIGURACION AHORA: ");
       Serial.println (TEMP_CONFIG_LM35_COMEDOR);

      if (TEMP_CONFIG_LM35_COMEDOR != TEMP_CONFIG_LM35_COMEDOR_ANTES) {
        HORA_ACTUAL = readDS3231time();
        
        if (TEMP_CONFIG_LM35_COMEDOR == 1) {
          BT_TEMP_COMEDOR = HORA_ACTUAL + "AA COMEDOR ON";
          digitalWrite(LED_AA_COMEDOR, HIGH);
          digitalWrite(LED_CC_COMEDOR, LOW);
      
          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_COMEDOR);
          Serial.println ("  Aire Acondicionado COMEDOR ON");
      
          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "    AA COMEDOR ON   ";
          Serial1.println (BT_TEMP_COMEDOR);
          }
       if (TEMP_CONFIG_LM35_COMEDOR == 2) {
          digitalWrite (LED_AA_COMEDOR, LOW);
          digitalWrite (LED_CC_COMEDOR, LOW);

          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_COMEDOR);
          Serial.println ("  TEMPERATURA ESTABILIZADA COMEDOR: AA + CC OFF");
     
          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "TEMP ESTABILIZDA COM";
          }
      if (TEMP_CONFIG_LM35_COMEDOR == 3) {
          BT_TEMP_COMEDOR = HORA_ACTUAL + "CC COMEDOR ON";
          digitalWrite (LED_AA_COMEDOR, LOW);
          digitalWrite (LED_CC_COMEDOR, HIGH);
     
          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_COMEDOR);
          Serial.println ("  Calefaccion COMEDOR ON");

          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "    CC COMEDOR ON   ";
          Serial1.println (BT_TEMP_COMEDOR);
          }
      scroll_lcd();
      BT_TEMP_COMEDOR = "";
      TEMP_CONFIG_LM35_COMEDOR_ANTES = TEMP_CONFIG_LM35_COMEDOR;
      
      } // =====  final del (TEMP_CONFIG_LM35_COMEDOR != TEMP_CONFIG_LM35_COMEDOR_ANTES)
      
      } // =====  final del (Config_LM35_COMEDOR)
      
// TEMPERATURA HABITACION actuar AA+CF
// ================================
if (Config_LM35_HABITACION) {
      if (TEMP_HABITACION >= 30) {
          TEMP_CONFIG_LM35_HABITACION = 1; // 1 encender AA  2 dentro de margen NADA  3 encender CF
          }
      if ((TEMP_HABITACION > 15) && (TEMP_HABITACION < 30)) {
          TEMP_CONFIG_LM35_HABITACION = 2;
          }
      if (TEMP_HABITACION <= 15) {
          TEMP_CONFIG_LM35_HABITACION = 3;
          }
       Serial.print ("CONFIGURACION ANTES: ");
       Serial.print (TEMP_CONFIG_LM35_HABITACION_ANTES);
       Serial.print ("  //  CONFIGURACION AHORA: ");
       Serial.println (TEMP_CONFIG_LM35_HABITACION);

      if (TEMP_CONFIG_LM35_HABITACION != TEMP_CONFIG_LM35_HABITACION_ANTES) {
        HORA_ACTUAL = readDS3231time();
        
        if (TEMP_CONFIG_LM35_HABITACION == 1) {
          BT_TEMP_HABITACION = HORA_ACTUAL + "AA HABITACION ON";
          digitalWrite(LED_AA_HABITACION, HIGH);
          digitalWrite(LED_CC_HABITACION, LOW);
      
          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_HABITACION);
          Serial.println ("  Aire Acondicionado HABITACION ON");
      
          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "  AA HABITACION ON  ";
          Serial1.println (BT_TEMP_HABITACION);
          }
       if (TEMP_CONFIG_LM35_HABITACION == 2) {
          digitalWrite (LED_AA_HABITACION, LOW);
          digitalWrite (LED_CC_HABITACION, LOW);

          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_HABITACION);
          Serial.println ("  TEMPERATURA ESTABILIZADA HABITACION: AA + CC OFF");
     
          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "TEMP ESTABILIZDA HAB";
          }
      if (TEMP_CONFIG_LM35_HABITACION == 3) {
          BT_TEMP_HABITACION = HORA_ACTUAL + "CC HABITACION ON";
          digitalWrite (LED_AA_HABITACION, LOW);
          digitalWrite (LED_CC_HABITACION, HIGH);
     
          Serial.print ( HORA_ACTUAL);
          Serial.print (TEMPERATURA_HABITACION);
          Serial.println ("  Calefaccion HABITACION ON");

          Linea4 = HORA_ACTUAL;
          scroll_lcd();
          Linea4 = "  CC HABITACION ON  ";
          Serial1.println (BT_TEMP_HABITACION);
          }
      scroll_lcd();
      BT_TEMP_HABITACION= "";
      TEMP_CONFIG_LM35_HABITACION_ANTES = TEMP_CONFIG_LM35_HABITACION;
      
      } // =====  final del (TEMP_CONFIG_LM35_HABITACION != TEMP_CONFIG_LM35_HABITACION_ANTES)
      
      } // =====  final del (Config_LM35_HABITACION)
} // final del LOOP


// funcion que permite que las letras dela pantalla van subiendo hacia arriba
//===========================================================================
void scroll_lcd () 
{ 
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
