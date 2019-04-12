#include <Servo.h>

Servo puerta;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
puerta.attach(9);
puerta.write(0);


}

void loop() {
  // put your main code here, to run repeatedly:
puerta.write(90);
delay(3000);
puerta.write(0);
delay(1000);

}
