#include <Servo.h>

Servo servoPump;
Servo servoSwing;
char incomingByte;
int full;
int pos;

void setup() {                
  Serial.begin(9600);
  servoPump.attach(3);
  servoSwing.attach(4);
  //SuckSmoke();
  servoSwing.write(100);
  servoPump.write(50);
   pinMode(8, OUTPUT);  
}

void Big(){
  for(pos = 50; pos<=130; pos++){ 
    servoPump.write(pos);
    delay(55); 
  }
  servoPump.write(175);
  delay(500);
  if(full<40){
    full+=10;
  }
  else{
    full=0;
  }
}

void Medium(){
  for(pos = 50; pos<=90; pos++){ 
    servoPump.write(pos);
    delay(45); 
  }
  servoPump.write(150);
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
  servoPump.write(40);
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
    servoSwing.write(145);
    //delay(400);
    delay(400);
    
    //takle air out
    servoPump.write(75);
    delay(550);
    digitalWrite(8,HIGH);
    delay(50);
    digitalWrite(8,LOW);
    //suck smoke
    servoPump.write(50);
    delay(1000);
  }
  
  void SuckAir(){
    //suck air
    servoPump.write(35);
    delay(500);
  }
  
  void Dip(int d){
    //dip into solution
    servoSwing.write(100);
    delay(1400);

    //swing into bubble making position
    servoSwing.write(20);
    delay(1200);
  }
  
  void Reset(){
    //return to initial stage
    servoSwing.write(90);
    delay(600);
  }

int smokeon = 0;
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

