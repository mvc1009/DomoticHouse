boolean valor_actual_mov1;
void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
pinMode (40, OUTPUT);
pinMode (48, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
valor_actual_mov1 = digitalRead (48);
Serial.println (valor_actual_mov1);

  if  (valor_actual_mov1)
  {
  digitalWrite (40, HIGH);
  }
  else
  {
  digitalWrite (40, LOW);
  }
  }

