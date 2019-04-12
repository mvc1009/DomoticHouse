int valor_actual_ky003;

void setup() {
  // put your setup code here, to run once:
 Serial.begin (9600);

 pinMode (40, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
valor_actual_ky003 = analogRead (A7);
Serial.println (valor_actual_ky003);
delay (100);

}
