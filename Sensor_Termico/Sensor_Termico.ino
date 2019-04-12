void setup() {
  // put your setup code here, to run once:
//Puerto 40 led ROJO
//Puerto 42 led VERDE

float VR;
float T;
Serial.begin(9600);

pinMode (40, OUTPUT);
//pinMode (42, OUTPUT);
//Puerto A0 lectura termostato

pinMode (22, OUTPUT);
pinMode (A0, INPUT);

}

void loop()
{
  // put your main code here, to run repeatedly:

//digitalWrite(40, HIGH);
//delay(500);
//digitalWrite(42, HIGH);
//delay(500);
//digitalWrite(40, LOW);
//delay(500);
//digitalWrite(42, LOW);
delay(500);

float VR = analogRead(A0);
Serial.print ("valorRead ");
Serial.println(VR);

float T = VR*500/1024;
Serial.print("Temp ");
Serial.println(T);

if (T >= 27) {
digitalWrite (40, HIGH);
}

else {
digitalWrite (40, LOW);
}
}
