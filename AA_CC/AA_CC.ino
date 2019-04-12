void setup() {
  // put your setup code here, to run once:
  
//Puerto 40 led ROJO
//Puerto 42 led VERDE

float VR;
float T;
float temp_config_sala1;

Serial.begin(9600);

pinMode (40, OUTPUT);
pinMode (42, OUTPUT);

//Puerto A0 lectura termostato

pinMode (A0, INPUT);
pinMode (A7, INPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:

delay(500);

float temp_config_sala = 27;
float temp_config_sala1 = 18;

float VR = analogRead(A0);
float T = VR*500/1024;

Serial.print("Temp sala 1 ");
Serial.println(T);

if (T >= temp_config_sala) {
digitalWrite (42, HIGH);
}

else {
digitalWrite (42, LOW);
}

if (T >= temp_config_sala1) {
digitalWrite (40, LOW);
}

else {
digitalWrite (40, HIGH);
}
}
