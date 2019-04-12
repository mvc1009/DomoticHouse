// Inicio Real Time Clock
//=======================

#include <Wire.h>   // Se inicia programa de comunicaciones por bus serie SDA/SCL
#define DS3231_I2C_ADDRESS 0x68  // Se define la direccion de memoria del reloj RTC para acceder
#include <LiquidCrystal_I2C.h>   // Se arranca libreria LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // Se define la direccion del LCD y tamaño 0x27 y 20 columnas 4 filas


String HORA_ACTUAL ="";      // variable texto para almacenar la hora
const byte altavoz = 8;


boolean touch_val = false;
const byte touch = 41;
//VARIABLES KY003 ====================================

const byte KY003_COMEDOR = 44;
const byte KY003_JARDIN = 43;

boolean SENSOR_KY003_COMEDOR = false;
boolean SENSOR_KY003_COMEDOR_ANTES = false;
boolean Config_KY003_COMEDOR = false;

boolean SENSOR_KY003_JARDIN = false;
boolean SENSOR_KY003_JARDIN_ANTES = false;
boolean Config_KY003_JARDIN = false;

String KY003_JARDIN_ON = "PUERTA JARDIN FORZADA";
String KY003_JARDIN_OFF = "PUERTA JARDIN CERRADA";

String KY003_COMEDOR_ON = "PUERTA COMEDOR FORZADA";
String KY003_COMEDOR_OFF = "PUERTA COMEDOR CERRADA";

//VARIABLES IRFLAME=====================================

const byte IRFLAME_COMEDOR = 47;
const byte IRFLAME_COCINA = 48;

boolean SENSOR_IRFLAME_COMEDOR = false;
boolean SENSOR_IRFLAME_COMEDOR_ANTES = false;
boolean Config_IRFLAME_COMEDOR = false;

boolean SENSOR_IRFLAME_COCINA = false;
boolean SENSOR_IRFLAME_COCINA_ANTES = false;
boolean Config_IRFLAME_COCINA = false;

String IRFLAME_COMEDOR_ON = "ALARMA DE FUEGO COMEDOR ON";
String IRFLAME_COMEDOR_OFF = "ALARMA DE FUEGO COMEDOR OFF";

String IRFLAME_COCINA_ON = "ALARMA DE FUEGO COCINA ON";
String IRFLAME_COCINA_OFF = "ALARMA DE FUEGO COCINA OFF";

//VARIABLES IRLIGHT=====================================

const byte IRLIGHT_JARDIN = 49;
const byte FAROLA_LED_R = 25;
const byte FAROLA_LED_V = 26;
const byte FAROLA_LED_A = 27;

boolean SENSOR_IRLIGHT_JARDIN = false;       //variacion del estado del sensor
boolean SENSOR_IRLIGHT_JARDIN_ANTES = false;  
boolean Config_IRLIGHT_JARDIN = false; //configuracion del comando bt on o off

String IRLIGHT_ON = "LUCES JARDIN ENCENDIDAS";
String IRLIGHT_OFF = "LUCES JARDIN APAGADAS";

//VARIABLES HCSR501===================================

const byte led_alarma = 22 ;
const byte HCSR501_COMEDOR = 46;    
const byte HCSR501_JARDIN = 45;

boolean Sensor_mov_comedor = false;
boolean Sensor_mov_antes_comedor = false;
boolean Config_HCSR501_COMEDOR = false;

boolean Sensor_mov_jardin = false;
boolean Sensor_mov_antes_jardin = false;
boolean Config_HCSR501_JARDIN = false;

//VARIABLES LM35==========================================

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

//VARIABLES PRINT LCD======
String Linea0 = "   CASA DOMOTICA    ";  //  pantalla LCD inicial
String Linea1 = "   MARIO VALIENTE   ";
String Linea2 = " TREBALL DE RECERCA ";
String Linea3 = "  SISTEMA INICIADO  ";
String Linea4 = "";


String comando_enviado_bt =""; //comando que enviaremos al bluetooth para que salga en la pantalla del mobil
String comando_bt ="";  // comando recivido desde el bluetooth
String HCSR501_ALARM_ON = "INTRUSO DETECTADO ALARMA ACTIVADA";
String HCSR501_ALARM_OFF = "ALARMA DE MOVIMIENTO DESACTIVADA";


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

  
  pinMode (touch, INPUT);
  
  pinMode (HCSR501_COMEDOR, INPUT ); //SENSOR MOVIMIENTO COMEDOR
  pinMode (HCSR501_JARDIN, INPUT); //SENSOR MOVIMIENTO JARDIN
  pinMode (led_alarma, OUTPUT );

  pinMode (IRLIGHT_JARDIN, INPUT ); //SENSOR LUZ
  pinMode (FAROLA_LED_R, OUTPUT ); //FAROLA
  pinMode (FAROLA_LED_V, OUTPUT );
  pinMode (FAROLA_LED_A, OUTPUT );  

  pinMode (KY003_COMEDOR, INPUT ); //SENSOR PUERTA COMEDOR
  pinMode (KY003_JARDIN, INPUT ); //SENSOR PUERTA JARDIN
   
  pinMode (IRFLAME_COMEDOR, INPUT ); //SENSOR FUEGO COMEDOR
  pinMode (IRFLAME_COCINA, INPUT ); //SENSOR FUEGO COCINA

  pinMode (LM35_COMEDOR, INPUT);   //PINES LM35
  pinMode (LED_AA_COMEDOR, OUTPUT );
  pinMode (LED_CC_COMEDOR, OUTPUT );
  pinMode (LM35_HABITACION, INPUT);
  pinMode (LED_AA_HABITACION, OUTPUT );
  pinMode (LED_CC_HABITACION, OUTPUT );

  pinMode (altavoz, OUTPUT);
 
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
  delay (1000); // para parar un poco la impresion en el Serial monitor luego se quitara


touch_val = digitalRead (touch);

if (touch_val)
{
  noTone (altavoz);
  digitalWrite (led_alarma, LOW);
  
}

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
 while (Serial1.available()>0) {      
 delay(100);
 char comando_bt_recivido = Serial1.read();
 comando_bt = comando_bt + comando_bt_recivido;
 }
    
    if (comando_bt != "") {
         Serial.print ("COMANDO RECIBIDO: ");
         Serial.println (comando_bt);
        }
     if (comando_bt == "145") {
      Config_HCSR501_JARDIN = true;
        Serial.println ("SENSOR DE MOVIMIENTO 2 ACTIVADO");    
     Linea4 = "  SENSOR MOV 2 ON   ";
     scroll_lcd ();
    }
      if (comando_bt == "045") {
      Config_HCSR501_JARDIN = false;
      Serial.println ("SENSOR DE MOVIMIENTO 2 DESACTIVADO");    
         Linea4 = "  SENSOR MOV 2 OFF  "; 
        scroll_lcd();
    }
    if (comando_bt == "146") {
      Config_HCSR501_COMEDOR = true;
        Serial.println ("SENSOR DE MOVIMIENTO 1 ACTIVADO");    
     Linea4 = "  SENSOR MOV 1 ON   ";
     scroll_lcd ();
    }
      if (comando_bt == "046") {
      Config_HCSR501_COMEDOR = false;
      Serial.println ("SENSOR DE MOVIMIENTO 1 DESACTIVADO");    
         Linea4 = "  SENSOR MOV 1 OFF  "; 
        scroll_lcd();
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
    if (comando_bt == "149") {
        Config_IRLIGHT_JARDIN = true;
        Serial.println ("SENSOR DE LUZ ACTIVADO");    
        Linea4 = "  SENSOR LUZ  ON   ";
       scroll_lcd ();
    }
    if (comando_bt == "049") {
       Config_IRLIGHT_JARDIN = false;
       Serial.println ("SENSOR DE LUZ DESACTIVADO");    
        Linea4 = "  SENSOR LUZ   OFF  "; 
        scroll_lcd();
    }
    if (comando_bt == "147") {
      Config_IRFLAME_COMEDOR = true;
        Serial.println ("SENSOR DE FUEGO COMEDOR ACTIVADO");    
     Linea4 = "SENSR FLAME COM ON  ";
     scroll_lcd ();
    }
      if (comando_bt == "047") {
      Config_IRFLAME_COMEDOR = false;
      Serial.println ("SENSOR DE FUEGO COMEDOR DESACTIVADO");    
         Linea4 = "SENSR FLAME COM OFF "; 
        scroll_lcd();
    }
       if (comando_bt == "148") {
      Config_IRFLAME_COCINA = true;
        Serial.println ("SENSOR DE FUEGO COCINA ACTIVADO");    
     Linea4 = "SENSR FLAME COC ON  ";
     scroll_lcd ();
    }
      if (comando_bt == "048") {
      Config_IRFLAME_COCINA = false;
      Serial.println ("SENSOR DE FUEGO COCINA DESACTIVADO");    
         Linea4 = "SENSR FLAME COC OFF "; 
        scroll_lcd();
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
    if (comando_bt == "143") {
      Config_KY003_JARDIN = true;
        Serial.println ("SENSOR PUERTA DEL JARDIN ACTIVADO");    
     Linea4 = " SENSR DOOR JAR ON  ";
     scroll_lcd ();
    }
      if (comando_bt == "043") {
      Config_KY003_JARDIN = false;
      Serial.println ("SENSOR PUERTA DEL JARDIN DESACTIVADO");    
         Linea4 = " SENSR DOOR JAR OFF "; 
        scroll_lcd();
    }
    comando_bt="";  // la limpieza del comando leido tiene que SER DETRAS HABER TODOS LOS POSIBLESS COMANDOS

 //==============IRFLAME COMEDOR=================
    if (Config_IRFLAME_COMEDOR) {
   
    
    SENSOR_IRFLAME_COMEDOR = digitalRead (IRFLAME_COMEDOR);  //LEEMOS EL SENSOR SI HAY FUEGO O NO
    
         if (SENSOR_IRFLAME_COMEDOR != SENSOR_IRFLAME_COMEDOR_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (SENSOR_IRFLAME_COMEDOR) {
            
                    
           Serial.print ( HORA_ACTUAL );
           Serial.println ("ALARMA FUEGO COMEDOR OFF");
           
           comando_enviado_bt = HORA_ACTUAL + IRFLAME_COMEDOR_OFF;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "ALARMA FUEGO COM OFF";
  
           }
             else {
            digitalWrite (led_alarma, HIGH);
            tone (altavoz, 800);
            Serial.print ( HORA_ACTUAL );
            Serial.println ("ALARMA FUEGO COMEDOR ON");

            comando_enviado_bt = HORA_ACTUAL + IRFLAME_COMEDOR_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "ALARMA FUEGO COM ON ";
            }
             scroll_lcd ();
            SENSOR_IRFLAME_COMEDOR_ANTES = SENSOR_IRFLAME_COMEDOR;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
  }
    
//==============IRFLAME COCINA=================
    if (Config_IRFLAME_COCINA) {
   
    
    SENSOR_IRFLAME_COCINA = digitalRead (IRFLAME_COCINA);  //LEEMOS EL SENSOR SI HAY FUEGO O NO
    
         if (SENSOR_IRFLAME_COCINA != SENSOR_IRFLAME_COCINA_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (SENSOR_IRFLAME_COCINA) {
            
           
         
           Serial.print ( HORA_ACTUAL );
           Serial.println ("ALARMA FUEGO COCINA OFF");
           
           comando_enviado_bt = HORA_ACTUAL + IRFLAME_COCINA_OFF;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "ALARMA FUEGO COC OFF";
  
           }
             else {
            digitalWrite (led_alarma, HIGH);
            tone (altavoz, 800);
            Serial.print ( HORA_ACTUAL );
            Serial.println ("ALARMA FUEGO COCINA ON");

            comando_enviado_bt = HORA_ACTUAL + IRFLAME_COCINA_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "ALARMA FUEGO COM ON ";
            }
             scroll_lcd ();
            SENSOR_IRFLAME_COCINA_ANTES = SENSOR_IRFLAME_COCINA;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
  }

  
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

//====================SENSOR MOVIMIENTO COMEDOR=======================================
    if (Config_HCSR501_COMEDOR) {
   
    
    Sensor_mov_comedor = digitalRead (HCSR501_COMEDOR);  //LEEMOS EL SENSOR SI HAY MOVIMIENTO O NO
    
         if (Sensor_mov_comedor != Sensor_mov_antes_comedor){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (Sensor_mov_comedor) {
           digitalWrite (led_alarma, HIGH);
           tone (altavoz, 800);
           Serial.print ( HORA_ACTUAL );
           Serial.println ("INTRUSO DETECTADO ALARMA ACTIVADA");
           
           comando_enviado_bt = HORA_ACTUAL + HCSR501_ALARM_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = " INTRUSO DETECTADO  ";
  
           }
             else {
          
            Serial.print ( HORA_ACTUAL );
            Serial.println ("ALARMA DE MOVIMIENTO DESACTIVADA");

            comando_enviado_bt = HORA_ACTUAL + HCSR501_ALARM_OFF;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "   ALARM MOV OFF    ";
            }
             scroll_lcd ();
            Sensor_mov_antes_comedor = Sensor_mov_comedor;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
    }

         //================SENSOR MOVIMIENTO JARDIN==========================================
          if (Config_HCSR501_JARDIN) {
   
    
    Sensor_mov_jardin = digitalRead (HCSR501_JARDIN);  //LEEMOS EL SENSOR SI HAY MOVIMIENTO O NO
    
         if (Sensor_mov_jardin != Sensor_mov_antes_jardin){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (Sensor_mov_jardin) {
           digitalWrite (led_alarma, HIGH);
           tone (altavoz, 800);
           Serial.print ( HORA_ACTUAL );
           Serial.println ("INTRUSO DETECTADO ALARMA ACTIVADA");
           
           comando_enviado_bt = HORA_ACTUAL + HCSR501_ALARM_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = " INTRUSO DETECTADO  ";
  
           }
             else {
            Serial.print ( HORA_ACTUAL );
            Serial.println ("ALARMA DE MOVIMIENTO DESACTIVADA");

            comando_enviado_bt = HORA_ACTUAL + HCSR501_ALARM_OFF;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "   ALARM MOV OFF    ";
            }
             scroll_lcd ();
            
            Sensor_mov_antes_jardin = Sensor_mov_jardin;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
          }
//======================SENSOR IRLIGHT JARDIN=======================
 if (Config_IRLIGHT_JARDIN) {
   
    
    SENSOR_IRLIGHT_JARDIN = digitalRead (IRLIGHT_JARDIN);  //LEEMOS EL SENSOR SI HAY MOVIMIENTO O NO
    
         if (SENSOR_IRLIGHT_JARDIN != SENSOR_IRLIGHT_JARDIN_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (SENSOR_IRLIGHT_JARDIN) {
            
           digitalWrite (FAROLA_LED_R, HIGH);
           digitalWrite (FAROLA_LED_V, HIGH);
           digitalWrite (FAROLA_LED_A, HIGH);
           
           Serial.print ( HORA_ACTUAL );
           Serial.println ("LUCES DEL JARDIN ENCENDIDAS");
           
           comando_enviado_bt = HORA_ACTUAL + IRLIGHT_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "  LUCES JARDIN ON   ";
  
           }
             else {
            digitalWrite (FAROLA_LED_R, LOW);
            digitalWrite (FAROLA_LED_V, LOW);
            digitalWrite (FAROLA_LED_A, LOW);
            
            Serial.print ( HORA_ACTUAL );
            Serial.println ("LUCES DEL JARDIN APAGADAS");

            comando_enviado_bt = HORA_ACTUAL + IRLIGHT_OFF;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "  LUCES JARDIN OFF  ";
            }
             scroll_lcd ();
            SENSOR_IRLIGHT_JARDIN_ANTES = SENSOR_IRLIGHT_JARDIN;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
  }
//==============SENSOR KY003 JARDIN===================================
      if (Config_KY003_JARDIN) {
   
    
         SENSOR_KY003_JARDIN = digitalRead (KY003_JARDIN);  //LEEMOS EL SENSOR SI HAY EFECTO HALL
    
         if (SENSOR_KY003_JARDIN != SENSOR_KY003_JARDIN_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (!SENSOR_KY003_JARDIN) {
            
                                     
           comando_enviado_bt = HORA_ACTUAL + KY003_JARDIN_OFF;  //enviar al bluetooth un emergente
           Serial.println (comando_enviado_bt);
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "DOOR JARDIN CERRADA ";
  
           }
             else {
            digitalWrite (led_alarma, HIGH);
            tone (altavoz, 800);
                   
            comando_enviado_bt = HORA_ACTUAL + KY003_JARDIN_ON;  //enviar al bluetooth un emergente
           Serial1.println (comando_enviado_bt);
           Serial.println (comando_enviado_bt);
           
            Linea4 = (HORA_ACTUAL);   //imprimir en el LCD
            scroll_lcd ();
            Linea4 = "DOOR JARDIN ABIERTA ";
            }
             scroll_lcd ();
            SENSOR_KY003_JARDIN_ANTES = SENSOR_KY003_JARDIN;  // cambiamos el valor de la variable del sensor para que la lectura actual al siguiente loop sea la anterior
          }
  }
  //==============SENSOR KY003 COMEDOR===============================================
    if (Config_KY003_COMEDOR) {
   
    
    SENSOR_KY003_COMEDOR = digitalRead (KY003_COMEDOR);  //LEEMOS EL SENSOR SI HAY EFECTO HALL
    
         if (SENSOR_KY003_COMEDOR != SENSOR_KY003_COMEDOR_ANTES){    // comparamos si la variable actual es igual que la anterior
         HORA_ACTUAL = readDS3231time(); // HORA_ACTUAL ="DD/MM/AA HH:MM:SS" texto de 17 caracteres
         HORA_ACTUAL = HORA_ACTUAL + "   ";
  
           if  (!SENSOR_KY003_COMEDOR) {
            
           comando_enviado_bt = HORA_ACTUAL + KY003_COMEDOR_OFF;  //enviar al bluetooth un emergente
           Serial.println (comando_enviado_bt);
           Serial1.println (comando_enviado_bt);
           
           Linea4 = (HORA_ACTUAL);    //imprimir en el LCD
           scroll_lcd ();
           Linea4 = "DOOR COMEDOR CERRADA";
  
           }
             else {
            digitalWrite (led_alarma, HIGH);
            tone (altavoz, 800);
                   
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



