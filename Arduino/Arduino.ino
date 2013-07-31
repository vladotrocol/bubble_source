#include <Servo.h> 

Servo servoPump;
Servo servoSwing;
char incomingByte = 0;
int led = 13;
void setup() {                
  Serial.begin(9600);
  servoPump.attach(4);
  servoSwing.attach(5);
  pinMode(led, OUTPUT);
  
  servoSwing.write(90);
  servoPump.write(145);
}


int pos = 0;

void loop() {
  digitalWrite(led, LOW);
  delay(1000);
  
  if (Serial.available() >0) {
    incomingByte = Serial.read();
    digitalWrite(led, HIGH);
    delay(1000); 
    //Serial.println(incomingByte); 
    if(incomingByte == 103){ //'g'
      
      //move in position for smoke
      servoSwing.write(40);
      delay(1400);
      
      //suck smoke
      servoPump.write(120);
      delay(600);
      
      //dip into solution
      servoSwing.write(90);
      delay(1400);
      
      //swing into bubble making position
      servoSwing.write(140);
      delay(140);
      
      //slow blow
      for(pos = 121; pos<=150; pos++){ 
        servoPump.write(pos);
        delay(30);
      }
      
      //fast blow
      servoPump.write(180);
      delay(600);
      
      //return to initial stage
      servoSwing.write(90);
      delay(600);
      servoPump.write(145);
      delay(600);
    }
  }
}
