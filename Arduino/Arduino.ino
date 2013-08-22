#include <Servo.h>

Servo servoPump;
Servo servoSwing;
char incomingByte;
int full;
int pos;

void setup() {                
  Serial.begin(9600);
  servoPump.attach(5);
  servoSwing.attach(4);

  servoSwing.write(90);
  servoPump.write(75);
}

void func1(int n, int s){

    for(int i=0;i<n;i++){
      //slow blow
      for(pos = 38; pos<=78-s; pos++){ 
        servoPump.write(pos);
        delay(45); 
      }
      //fast blow
      for(pos =77-s; pos<=82-s; pos++){ 
        servoPump.write(pos);
        delay(15);
      }
    }
}

void Big(){
  for(pos = 38; pos<=78; pos++){ 
    servoPump.write(pos);
    delay(45); 
  }
  servoPump.write(83);
  delay(500);
  if(full<40){
    full+=10;
  }
  else{
    full=0;
  }
}

void Medium(){
  for(pos = 38; pos<=58; pos++){ 
    servoPump.write(pos);
    delay(45); 
  }
  servoPump.write(63);
  delay(500);
  if(full<40){
    full+=7;
  }
  else{
    full=0;
  }
}

void Small(){
   for(pos = 38; pos<=45; pos++){ 
     servoPump.write(pos);
     delay(45); 
   }
   servoPump.write(48);
   delay(500);
   
   if(full<40){
     full+=4;
   }
   else{
     full=0;
   }
}

void Tiny(){
  servoPump.write(41);
  delay(100);
  if(full<40){
    full+=2;
  }
  else{
    full=0;
  }
}

void multiBubbles(int n){
  if(n>0){
      for(pos = 38; pos<=44; pos++){ 
        servoPump.write(pos);
        delay(40); 
      }
      servoPump.write(48);
      delay(700);
  }
  
  if(n>1){
      for(pos = 49; pos<=56; pos++){ 
        servoPump.write(pos);
        delay(40); 
      }
      servoPump.write(59);
      delay(700);
  }
  
  if(n>2){
      for(pos = 60; pos<=67; pos++){ 
        servoPump.write(pos);
        delay(40); 
      }
      servoPump.write(70);
      delay(700);
  }
  
  if(n>3){
      for(pos = 71; pos<=78; pos++){ 
        servoPump.write(pos);
        delay(40); 
      }
      servoPump.write(81);
      delay(700);
  }
  
  if(full<40){
    full+=5*n;
  }
  else{
    full=0;
  }
}

  void SuckSmoke(){
    //move in position for smoke
    servoSwing.write(130);
    //delay(400);
    delay(2400);
    
    //takle air out
    servoPump.write(75);
    delay(600);
    
    //suck smoke
    servoPump.write(35);
    delay(1000);
  }
  
  void SuckAir(){
    //suck air
    servoPump.write(35);
    delay(100);
  }
  
  void Dip(int d){
    //dip into solution
    servoSwing.write(90);
    delay(700);

    //swing into bubble making position
    servoSwing.write(10);
    delay(800);
  }
  
  void Reset(){
    //return to initial stage
    servoSwing.write(90);
    delay(600);
  }

void loop() {
  incomingByte = Serial.read();
  if(incomingByte!=-1){
    if(full==0){
      SuckSmoke();
    }
    else{
      SuckAir();
    }
  }
  
  if(incomingByte == 'b'){
    Dip(800);
    Big();
  }
  else if(incomingByte == 'm'){
    Dip(800);
    Medium();
  }
  else if(incomingByte == 's'){
    Dip(800);
    Small();
  }
  else if(incomingByte == 't'){
    Dip(800);
    Tiny();
  }
  else if((int)incomingByte>48&&(int)incomingByte<53){
    Dip(800);
    multiBubbles((int)incomingByte-48);
  }
}

