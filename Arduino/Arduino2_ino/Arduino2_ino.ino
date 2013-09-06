#include <Servo.h>
Servo Pump;
Servo Swing;
char incomingByte;
int full;
int pos;

void setup(){
  Serial.begin(9600);
  Pump.attach(3);
  Swing.attach(4);
  reset();
}

void loop(){

}

void PumpTo(int Position, int Delay){
 int initAlpha = servo.read(); 
 if(initAlpha<Position){
   pos = initAlpha;
 }
 while()
}

void SwingTo(int Position, int Delay){

}

void reset(){
  Swing.write(90);
  Pump.write(40);
}
