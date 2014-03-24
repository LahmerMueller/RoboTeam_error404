// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object   

  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
} 
 
void loop() 
{ 
  while(digitalRead(27) && digitalRead(26)); 
  myservo.write(20);
  while(!digitalRead(27) || !digitalRead(26));
  while(digitalRead(27) && digitalRead(26));
  myservo.write(160);
  while(!digitalRead(27) || !digitalRead(26));
} 
