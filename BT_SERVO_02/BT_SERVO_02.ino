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
      Serial.print("commando_rx : ");
      Serial.println(commando_rx);
   }


if (commando_rx == "*00?") {
    //abrir puerta finca y encender led
mover_puerta (180);
}  

if (commando_rx =="*RR#") {
    //cerrar puerta finca y apagar led
mover_puerta (90);
}

commando_rx=""; // borrar comandos distintos al abrir y cerrar puerta
}


void mover_puerta ( int grados) {
for (int i=0; i <= 5; i++){
  digitalWrite (40, LOW);
  delay(200);
  digitalWrite (40, HIGH);
  delay(200);
   } 
if ( grados == 180 ){
    digitalWrite (40, HIGH);
    }
    else
    {
    digitalWrite (40, LOW);
    }
puerta.write(grados);
commando_rx="";
Serial1.println("ROGER ROGER");
}
  

