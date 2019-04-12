void setup () {
// ACTIVAR PUERTO SERIE POR 9600 BITS/S
Serial.begin(9600);
pinMode(46, INPUT);
pinMode(40, OUTPUT);
pinMode(41, OUTPUT);
pinMode(42, OUTPUT);
pinMode(53, INPUT);
}

void loop() {
 
// ACTIVAR LUZ POR SENSOR

int interruptor = digitalRead(53);
int  sensorValueD = digitalRead(46);
Serial.print (" valor 46 : ");
Serial.println(sensorValueD);
Serial.print (" valor 53 : ");
Serial.println(interruptor);

if ((sensorValueD == 1) || (interruptor == 1)) {
  digitalWrite(40,HIGH);
  digitalWrite(41,HIGH);
  digitalWrite(42,HIGH);
  }
  else {
  digitalWrite(40,LOW);
  digitalWrite(41,LOW);
  digitalWrite(42,LOW);
  }

  delay(500);
 

}
