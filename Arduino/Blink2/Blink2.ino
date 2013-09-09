#include <Servo.h>

Servo servoPump;
Servo servoSwing;
char incomingByte;
int full;
int pos;

int led = 13;

int foggerIn1 = A1;
int foggerIn2 = A0;
int foggerIn3 = A2;

int foggerOut3 = 10;
int foggerOut2 = 8;
int foggerOut1 = 9;

int fogger1Val = 0;
int fogger2Val = 1;
int fogger3Val = 2;

int f1Ready = 0;
int f2Ready = 0;
int f3Ready = 0;

int fogTrigDelay = 150;

int toggle=LOW;

void setup() {
  // initialize the LED pin as an output:
  setupFoggers();
  Serial.begin(9600);
  //set up servos
  servoPump.attach(3);
  servoSwing.attach(4);

  //go to initial position
  servoSwing.write(100);
  servoPump.write(50); 
}

void setupFoggers(){
  //setup output lines.
  pinMode(foggerOut1, OUTPUT);
  pinMode(foggerOut2, OUTPUT);
  pinMode(foggerOut3, OUTPUT);
  digitalWrite(foggerOut1, LOW);
  digitalWrite(foggerOut2, LOW);
  digitalWrite(foggerOut3, LOW);
}

void triggerFogger(int whichFogger){
  int trigLine;
  if(whichFogger==foggerOut1){
    trigLine = foggerOut1;
  }
  else{
    if(whichFogger==foggerOut2){
      trigLine = foggerOut2;
    }
    else{
      if(whichFogger==foggerOut3){
        trigLine = foggerOut3;
      }
      else{
        Serial.print("Invalid fogger trig call. You called for: ");
        Serial.println(whichFogger,DEC);
      }
    }
  }

  //the fogging process is... turn it on for a short duration (of x ms)
  digitalWrite(trigLine, HIGH);
  delay(fogTrigDelay);
  digitalWrite(trigLine,LOW);
  Serial.println("Fired!!");


}


// the loop routine runs over and over again forever:
void loop() {
  programAnalogRead();
  if(Serial.available()&&f2Ready<10&&f2Ready!=0){
    readChar();
  }
  
  
}

void programAnalogRead(){
  
  fogger1Val = analogRead(foggerIn1);
  if(fogger1Val<200&&f1Ready<100){
    f1Ready++;  
  }
  else{
    f1Ready = 0;
  }
 
  fogger2Val = analogRead(foggerIn2);
  if(fogger2Val<200&&f2Ready<100){
    f2Ready++;
  }
  else{
    f2Ready = 0;
  }
  
  fogger3Val = analogRead(foggerIn3);
  if(fogger3Val<200&&f3Ready<100){  
    f3Ready++;
  }
  else{
    f3Ready = 0;
  }
  
  
//  Serial.print("F1: ");
//  Serial.print(fogger1Val,DEC);
//  Serial.print(" F2: ");
//  Serial.print(fogger2Val,DEC);
//  Serial.print(" F3: ");
//  Serial.println(fogger3Val,DEC);
//  
//  delay(300);
}

void readChar(){
  incomingByte = Serial.read();
  int fogToTrig;
  
  if(incomingByte!=-1){
    if(full==0){
      SuckSmoke();
    }
    else{
      SuckAir();
    }
  } 
  
  switch(incomingByte){
  case 'f':
  case 'F':
    while(!Serial.available()){
      incomingByte = 10;
    }
    incomingByte = Serial.read();
    if(incomingByte=='4'){
      fogToTrig = foggerOut3;
    }
    else{
      if(incomingByte=='2'){
        fogToTrig = foggerOut2;
      }
      else{
         if(incomingByte=='1'){
            fogToTrig = foggerOut1;
          }
        else{
          fogToTrig = 100;
        }
      }
    }
    Serial.print("Firing");
    if(fogToTrig!=100)
      triggerFogger(fogToTrig);
    else{
      Serial.println("Abort");
    }
    
     
    break;
  
  case 'b':
  case 'B':
    Dip(800);
    Big();
    break;
   
  case 'm':
  case 'M':
    Dip(800);
    Medium();
    break;
    
  case 's':
  case 'S':
    Dip(800);
    Small();
    break;
  
  case 't':
  case 'T':
    Dip(800);
    Tiny();
    break;
  }
  if((int)incomingByte>48&&(int)incomingByte<53){
    Dip(800);
    multiBubbles((int)incomingByte-48);
  }
}


void mainProgram1(){
  if(incomingByte!=-1){
    toggle = toggle!=HIGH?HIGH:LOW;
    digitalWrite(led, toggle);   // turn the LED on (HIGH is the voltage level)
  }
  int inByte = digitalRead(11);
  Serial.println(inByte,DEC);
  delay(25);
}



//-------------------------------Generator actions------------------------
void SuckSmoke(){
  //move in position for smoke
  for(pos = servoSwing.Read(); pos<=145; pos++){ 
    servoSwing.write(pos);
    delay(10);
  }
  delay(600);
   //takle air out
  for(pos = servoPump.Read(); pos<=130; pos++){ 
    servoPump.write(pos);
    delay(10);
  }
  delay(550);
  triggerFogger(foggerOut2);
  delay(100);
  
  //suck smoke
  for(pos = servoPump.Read(); pos<=50; pos++){ 
    servoPump.write(pos);
    delay(10);
  }
  delay(1000);
}

void SuckAir(){
  //suck air
  for(pos = servoPump.Read(); pos<=50; pos++){ 
    servoPump.write(pos);
    delay(10);
  }
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
//--------------------------------Bubble creation-------------------------
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
  for(pos = 50; pos<=70; pos++){ 
    servoPump.write(pos);
    delay(45); 
  }
  servoPump.write(79);
  delay(500);

  if(full<40){
    full+=15;
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

