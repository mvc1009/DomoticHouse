void setup() {
  // put your setup code here, to run once:
 
//Puerto 40 led ROJO
//Puerto 42 led VERDE

pinMode(40, OUTPUT);
pinMode (42, OUTPUT);
}
void loop()
 {
  // put your main code here, to run repeatedly:

digitalWrite(40, HIGH);
delay(500);
digitalWrite(42, HIGH);
delay(500);
digitalWrite(40, LOW);
delay(500);
digitalWrite(42, LOW);
delay(500);
}
