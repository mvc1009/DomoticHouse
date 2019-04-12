void setup() {
  // put your setup code here, to run once:
 
//Puerto 40 led AZUL
//Puerto 41 led VERDE
//Puerto 42 led ROJO

pinMode(40, OUTPUT);
pinMode(41, OUTPUT);
pinMode(42, OUTPUT);
}
void loop()
 {
  // put your main code here, to run repeatedly:

digitalWrite(40, HIGH);
delay(500);
digitalWrite(40, LOW);
digitalWrite(41, HIGH);
delay(500);
digitalWrite (41, LOW);
digitalWrite(42, HIGH);
delay(500);
digitalWrite(42, LOW);
digitalWrite(40, HIGH);
digitalWrite(41, HIGH);
delay(500);
digitalWrite (40, LOW);
digitalWrite(42, HIGH);
delay(500);
digitalWrite(41, LOW);
digitalWrite(40, HIGH);
delay(500);
digitalWrite(40, HIGH);
digitalWrite(41, HIGH);
digitalWrite(42, HIGH);
delay(500);
digitalWrite(40, LOW);
digitalWrite(41, LOW);
digitalWrite(42, LOW);

}
