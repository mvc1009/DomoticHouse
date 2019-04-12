#include <Servo.h>

Servo puerta;


 

String commando_rx;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
commando_rx="";

puerta.attach(9);
puerta.write(90);  

pinMode(40,OUTPUT);

}

void loop() {

  // bt --> monitor
  while (Serial1.available()>0) {
    delay(100);
    char lectura = Serial1.read();
    commando_rx = commando_rx + lectura;
    }

   if (commando_rx != "") {
       Serial.print("commando_rx");
      Serial.println(commando_rx);
   }


if (commando_rx == "*00?") {
    //encender led
digitalWrite (40, HIGH);
delay(200);
digitalWrite (40, LOW);
delay(200);
digitalWrite (40, HIGH);
delay(200);
puerta.write(180);
commando_rx="";
}  
if (commando_rx =="*RR#") {
    // apagar led
digitalWrite (40, LOW);
delay(200);
digitalWrite (40, HIGH);
delay(200);
digitalWrite (40, LOW);
delay(200);
puerta.write(90);
    commando_rx="";
}

}
  
