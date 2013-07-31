#include <Servo.h> 

Servo servoPump;
Servo servoSwing;
int incomingByte = 0;
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
    if(incomingByte == 'g'){ //'g'
      
      //move in position for smoke
      servoSwing.write(40);
      delay(1400);
      
      //suck smoke
      servoPump.write(20);
      delay(600);
      
      //dip into solution
      servoSwing.write(90);
      delay(1400);
      
      //swing into bubble making position
      servoSwing.write(140);
      delay(140);
      
      //slow blow
      for(pos = 21; pos<=180; pos+=1){ 
        servoPump.write(pos);
        delay(30);
      }
      
      //fast blow
      servoPump.write(145);
      delay(600);
      
      //return to initial stage
      servoSwing.write(90);
      delay(600);
      servoPump.write(145);
      delay(600);
    }
  }
}
