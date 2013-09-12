#include <Servo.h>

Servo servoPump;
Servo servoSwing;
char incomingByte;
int full;
int pos;

int led = 13;

//global delays
int moveDelay=3;


//fogger ports in
int foggerIn1 = A1;
int foggerIn2 = A0;
int foggerIn3 = A2;

//fogger ports out
int foggerOut3 = 10; //coffe
int foggerOut2 = 8; //lemon
int foggerOut1 = 9; //coconut

//fogger current value
int fogger1Val = 0;
int fogger2Val = 1;
int fogger3Val = 2;

//fogger low value counter
int f1Ready = 0;
int f2Ready = 0;
int f3Ready = 0;

int fogTrigDelay = 300;

int toggle=LOW;

void setup() {
  // initialize the LED pin as an output:
  setupFoggers();
  Serial.begin(9600);
  //set up servos
  servoPump.attach(3);
  servoSwing.attach(4);

  moveServo('p',65, moveDelay);
    delay(1000);

  //go to initial position
  moveServo('s', 100, moveDelay);
  //moveServo('s', 45, moveDelay);
   
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
  //Serial.println("Fired!!");


}


// the loop routine runs over and over again forever:
void loop() {
  programAnalogRead();
  
  if(Serial.available()&&f3Ready<10&&f3Ready!=0){
    readChar();
  }
}

void programAnalogRead(){
  
  fogger1Val = analogRead(foggerIn1);
  if(fogger1Val<200&&f1Ready<100){
    f1Ready++;  
  }
  else if(fogger1Val>=200){
    f1Ready = 0;
  }
 
  fogger2Val = analogRead(foggerIn2);
  if(fogger2Val<200&&f2Ready<100){
    f2Ready++;
  }
  else if(fogger2Val>=200){
    f2Ready = 0;
  }
  
  fogger3Val = analogRead(foggerIn3);
  if(fogger3Val<200&&f3Ready<100){  
    f3Ready++;
  }
  else if(fogger1Val>=200){
    f3Ready = 0;
  }
  
//Serial.print("F2: ");
//Serial.print(fogger2Val,DEC);
//Serial.print(" ");
//  Serial.println(f2Ready);
////  Serial.print("F1: ");
//  Serial.print(fogger1Val,DEC);
//  Serial.print(" F2: ");
//  Serial.print(fogger2Val,DEC);
//  Serial.print(" F3: ");
//  Serial.println(fogger3Val,DEC);
//  
  delay(500);
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
      //SuckSmoke();
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
    //Serial.print("Firing");
    if(fogToTrig!=100)
      triggerFogger(fogToTrig);
    else{
      Serial.println("Abort");
    }
    
     
    break;
  
  case 'b':
  case 'B':
    Dip();
    Big();
    break;
   
  case 'm':
  case 'M':
    Dip();
    Medium();
    break;
    
  case 's':
  case 'S':
    Dip();
    Small();
    break;
  
  case 't':
  case 'T':
    Dip();
    Tiny();
    break;
  }
  if((int)incomingByte>48&&(int)incomingByte<53){
    Dip();
    multiBubbles((int)incomingByte-48);
  }
}


void mainProgram1(){
  if(incomingByte!=-1){
    toggle = toggle!=HIGH?HIGH:LOW;
    digitalWrite(led, toggle);   // turn the LED on (HIGH is the voltage level)
  }
  int inByte = digitalRead(11);
  //Serial.println(inByte,DEC);
  delay(25);
}
// Smoke tank usage (full/empty)

void incrementSmoke(int amount){
  if(full<40){
    full+=amount;
  }
  else{
    full=0;
  }
}

// ----------------Servo functions----------------------------------------

void moveServo(char s,int nextPos, int Delay){
  int start;
  Servo servo;
  if(s=='s'){
    servo= servoSwing;
  }
  else{
    servo = servoPump;
  }
  start = servo.read();
  if(start<=nextPos){
    for(pos = start; pos<=nextPos; pos++){ 
      servo.write(pos);
      delay(Delay);
    }
  }
  else{
   for(pos = start; pos>=nextPos; pos--){ 
      servo.write(pos);
      delay(Delay);
    }
  }
}

//-------------------------------Generator actions------------------------
void SuckSmoke(){
  //move in position for smoke
  moveServo('s', 145, moveDelay);
  delay(600);
   //takle air out
  moveServo('p', 130, moveDelay);
  delay(550);
  
  //trigger the smoke machine
  triggerFogger(foggerOut3);
  delay(100);
  
  //suck smoke
  moveServo('p', 65, moveDelay);
  delay(1000);
}

void SuckAir(){
  //suck air
  moveServo('p', 65, moveDelay);
  delay(500);
}

void clean(){
  moveServo('s', 45, moveDelay);
  delay(100);
  moveServo('s', 55, moveDelay);
  delay(100);
}

void Dip(){
  //dip into solution
  moveServo('s', 100, moveDelay);
  delay(1400);
  //clean();
  // clean();
  //swing into bubble making position
  moveServo('s', 30, moveDelay);
  delay(1200);
}

void Reset(){
  //return to initial stage
  moveServo('s',90, moveDelay);
  delay(600);
}
//--------------------------------Bubble creation-------------------------
void Big(){
  moveServo('p', 100, 45);
  moveServo('p', 130, 0);
  delay(500);
  incrementSmoke(10);
}

void Medium(){
  moveServo('p', 95, 45);
  moveServo('p', 110, 0);
  delay(500);
  incrementSmoke(20);
}

void Small(){
moveServo('p', 80, 45);
moveServo('p', 87, 0);
  delay(500);

  incrementSmoke(15);
}

void Tiny(){
  moveServo('p', 70, 0);
  delay(100);
  incrementSmoke(2);
}

void multiBubbles(int n){
  if(n>0){
    moveServo('p',75,40);
    moveServo('p', 81, moveDelay);
    delay(500);
  }
  if(n>1){
    moveServo('p',91,45);
    moveServo('p', 97, moveDelay);
    delay(500);
  }

  if(n>2){
    moveServo('p',107,40);
    moveServo('p', 122, moveDelay);
    delay(500);
  }

  if(n>3){
    moveServo('p',122,40);
    moveServo('p', 129, moveDelay);
    delay(500);
  }
  incrementSmoke(5*n);
}

