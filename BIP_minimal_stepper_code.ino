# include <Arduino.h>

#define EN        8       // stepper motor enable, low level effective
#define X_DIR     2       //X axis, stepper motor direction control 
#define X_STP     5       //x axis, stepper motor control


void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(2000);  
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(2000);  
  }
}
void setup(){// set the IO pins for the stepper motors as output 
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}
void loop(){
  step(false, X_DIR, X_STP, 200); // x axis motor rotates CCW for 1 circle, as in 200 steps
  delay(5000);
}

