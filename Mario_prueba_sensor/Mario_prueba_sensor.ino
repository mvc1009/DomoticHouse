
boolean valor_sensor;


void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
pinMode (28, OUTPUT); //RELÉ 1
pinMode (29, OUTPUT); //RELÉ 2
pinMode (22, OUTPUT); //led 
pinMode (41, INPUT);  // touchpad

digitalWrite (22, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
valor_sensor = digitalRead (41);


Serial.print ("Valor Digital:"  );
Serial.println (valor_sensor);


if (valor_sensor) {
  digitalWrite (28, HIGH);
  digitalWrite (29, HIGH);
  Serial.println( "alto");
}
else {
  digitalWrite (28, LOW);
  digitalWrite (29, LOW);
    Serial.println( "bajo");
}
delay (5000);
}
