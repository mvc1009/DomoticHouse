#include <Stepper.h>
// const int stepsPerRevolution = 2048;
// Stepper myStepper (stepsPerRevolution ,8,10,11,9);
// Max speed is 14 RPM

Stepper myStepper(2048, 10,12,11,13);            
 
void setup() {
  myStepper.setSpeed(10);
}
 
void loop() {
  myStepper.setSpeed(10);
 
  myStepper.step(2048);
  delay (200);
 
  myStepper.step(-2048);
  delay (200);
}
