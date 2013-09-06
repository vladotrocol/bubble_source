
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int foggerIn1 = A0;
int foggerIn2 = A1;
int foggerIn3 = A2;

int foggerOut3 = 10;
int foggerOut2 = 8;
int foggerOut1 = 9;

int fogger1Val = 0;
int fogger2Val = 1;
int fogger3Val = 2;

int fogTrigDelay = 50;

int incomingByte;
int toggle=LOW;

void setup() {
  // initialize the LED pin as an output:
  setupFoggers();
  Serial.begin(115200);
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
}


// the loop routine runs over and over again forever:
void loop() {
  
  if(Serial.available()){
    readChar();
  }
  programAnalogRead();
}

void programAnalogRead(){
  fogger3Val = analogRead(foggerIn3);
  Serial.println(fogger3Val,DEC);
  delay(50);
}

void readChar(){
  char ch = Serial.read();
  switch(ch){
    case 'f':
    case 'F':
      triggerFogger(foggerOut3);
      break;
  
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
