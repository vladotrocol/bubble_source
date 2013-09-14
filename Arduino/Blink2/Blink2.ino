#include <Servo.h>
//------------------------------------Important!-------------------
//Wait about 20-30 seconds before starting the experiment

//Make SURE you have the correct PORT (usually COM6 for the bubble arduino)

//Make sure you reset or turn off the machine before leaving it unattended 
//or else it might just start making bubbles on it's own and fill the room with smoke

Servo servoPump;
Servo servoSwing;
char incomingByte;
int full=-1;
int pos;

int led = 13;
int scentStudy = 0;
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

int fogTrigDelay = 200;

int currentFogger = foggerOut1;
int toggle=LOW;

int startAnglePump = 55;

void setup() {
  // initialize the LED pin as an output:
  setupFoggers();
  Serial.begin(9600);
  //set up servos
  servoPump.attach(3);
  servoSwing.attach(4);

  moveServo('p',startAnglePump, moveDelay);
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

int getReadyFogger(){
  if(currentFogger==foggerOut1){
    return f1Ready;
  }
  else if(currentFogger == foggerOut2){
    return f2Ready;
  }
  else if(currentFogger == foggerOut3){
    return f3Ready;
  }
  return -1;
}


// the loop routine runs over and over again forever:
void loop() {
  programAnalogRead();
  if(scentStudy==0){
        if(full==0){
          if(Serial.available()&&(getReadyFogger()<10&&getReadyFogger()>0)){   //change fogger (ready xcondition)
            readChar();
          }
            
        }
        else
        {
          if(Serial.available()){
            readChar();
          }
        }
        }
  else{
        if(Serial.available()&&(getReadyFogger()<10&&getReadyFogger()>0)){   //change fogger (ready xcondition)
          readChar();
        }
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
    if(full==0||full==-1){
      SuckSmoke();
    }
    else{
      if(scentStudy==0){
      SuckAir();
      }
      else{
      SuckSmoke();//for scent study
      }
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
void clearSmoke(){
//takle air out
  moveServo('p', 130, moveDelay);
  delay(250);
  //takle air out
  moveServo('p', 30, moveDelay);
  delay(250);
  //takle air out
  moveServo('p', 130, moveDelay);
  delay(250);
}

void SuckSmoke(){
  //move in position for smoke
  
  moveServo('s', 145, moveDelay);
  delay(600);
  if(scentStudy==1){
  clearSmoke(); 
  }     //takle air out
  moveServo('p', 130, moveDelay);
  delay(550);
  
  //trigger the smoke machine
  triggerFogger(currentFogger);                    //Set fogger
  delay(100);
  
  //suck smoke
  moveServo('p', startAnglePump, moveDelay);
  delay(1000);
}

void SuckAir(){
  //suck air
  moveServo('p', startAnglePump, moveDelay);
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
  moveServo('p', 85, 45);
  moveServo('p', 100, 50);
  moveServo('p', 125, 0);
  delay(500);
  incrementSmoke(20);
}

void Medium(){
  moveServo('p', 83, 45);
  moveServo('p', 93, 0);
  delay(500);
  incrementSmoke(20);
}

void Small(){
  moveServo('p', 68, 45);
  moveServo('p', 75, 0);
  delay(500);

  incrementSmoke(10);
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

