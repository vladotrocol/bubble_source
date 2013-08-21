#include <Servo.h> 

Servo servoPump;
Servo servoSwing;
char incomingByte = 0;
int led = 13;
void setup() {                
  Serial.begin(9600);
  servoPump.attach(5);
  servoSwing.attach(4);

  servoSwing.write(90);
  servoPump.write(75);
}


int pos = 0;

//38-77 sp
//85 fp
//45 pr
//pr2 a10

void create(int n,int s){
 //move in position for smoke
    servoSwing.write(130);
    delay(400);
   // delay(2400);
   

    //takle air out
    servoPump.write(75);
    delay(600);
    
    
    //suck smoke
    servoPump.write(35);
    delay(1000);

    //dip into solution
    servoSwing.write(90);
    delay(1000);

    //swing into bubble making position
    servoSwing.write(10);
    delay(1400);
  
    for(int i=0;i<n;i++){
      //slow blow
      for(pos = 38; pos<=76-s; pos++){ 
        servoPump.write(pos);
        delay(45); 
      }
      //fast blow
      for(pos =77-s; pos<=82-s; pos++){ 
        servoPump.write(pos);
        delay(18);
      }
    }

 delay(600);   
    servoSwing.write(90);
    
    
    
    //return to initial stage
    
    delay(600);
}

void loop() {
  incomingByte = Serial.read();
  char a=incomingByte;
  if(incomingByte == 'b'){
    create(1,15);
  }
  else if(incomingByte == 's'){
    create(1,24);
  }
  else if(incomingByte == 'n'){
    create(1,30);
  }
  else if(incomingByte == 't'){
    create(1,39);
  }
}

