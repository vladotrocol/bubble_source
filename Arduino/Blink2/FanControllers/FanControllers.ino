/*
  Written by Abhijit Karnik Template program
  Serial command control/response
 */
char progVer[50]  =  "Prog ver: Fans!";
char startMsg[50] =  "Send 'q' to stop all fans";
const int SER_COMM_SPEED = 9600;
int run=HIGH;
int blipCnt=0;
int pin13=13;
int pinstate=LOW;

int fan1 = 5;
int fan2 = 6;
int fan3 = 10;
int fan4 = 11;

int fan1speed = 255;
int fan2speed = 255;
int fan3speed = 255;
int fan4speed = 255;

void setup()
{

  //  Wire.begin(); //setup I2C
    Serial.begin(SER_COMM_SPEED); //check this...          
    Serial.println(progVer);
    Serial.println(startMsg);
    initFans();
}

void initFans(){
  analogWrite(fan1, fan1speed);
  analogWrite(fan2, fan2speed);
  analogWrite(fan3, fan3speed);
  analogWrite(fan4, fan4speed);
}



// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()
{
     if(Serial.available() > 0)
     {
       readMsg();
     }
     if(run==HIGH)
     {//running mode
       mainProgram();
     }
     else
     {//off mode
       delay(500);
       if(blipCnt==25)
       {
         Serial.println(".");
         blipCnt=0;
       }
       else
       {
         Serial.print(".");       
         blipCnt++;
       }
     }
}

void turnOffFans(){
  analogWrite(fan1, 0);
  analogWrite(fan2, 0);
  analogWrite(fan3, 0);
  analogWrite(fan4, 0);
}



void mainProgram()
{
//your actual main program here
//make all static variables global
  
}

void setFan(int fan, int val){
  int dFan;
  switch(fan){
    case 1:
      fan1speed = val;
      dFan = fan1;
      break;
    case 2:
      fan2speed = val; 
      dFan= fan2;
      break;
    case 3:
      fan3speed = val; 
      dFan = fan3;
      break;
    case 4:
      fan4speed = val; 
      dFan = fan4;
      break;  
  }
  Serial.print("\nFan ");
  Serial.print(fan,DEC);
  Serial.print(" set to speed: ");
  Serial.println(val, DEC);
  if(run==HIGH){
    analogWrite(dFan, val);  
  }
}

void readMsg(void)
{
          char ch = Serial.read();
          int cnt = 0;
          int fan = -1;
          int value=0;
          switch(ch)
          {
               case 'l':
               case 'L':
               case '?':
                    Serial.println("List of available commands: (case insensitive unless mentioned otherwise)");
                    Serial.println("<ch> : <command>");
                    Serial.println("<x><yyy> : x: 1 to 4, yyy: 000 to 255 and has to be 3 digit, sets fan x to speed yyy");
                    Serial.println("g : Run the main loop program.");       
                    Serial.println("q : Quit, stop all.");                    
                    Serial.println("v : Lists the program version");     
                    Serial.println("l,? : Relays list of available commands");                    
                    Serial.println("---End of Command list---");
                    break;
               case '1':
               case '2':
               case '3':
               case '4':
                   fan = ch-'0';
                   Serial.print(ch);
                   while(cnt<3){
                     if(Serial.available()){
                       ch = Serial.read();
                       Serial.print(ch);
                       cnt++;
                       if(ch<'0'||'9'<ch){
                         Serial.println("Error!");
                         break;
                       }
                       else{
                         value = value*10+(ch-'0');
                       }
                     }
                   }
                   if(value<256){
                     setFan(fan,value);
                   }
                   else{
                     Serial.println("Error2!");
                   }
                   break;
               case 's':
               case 'S':
                    if(pinstate==LOW) pinstate=HIGH;
                    else pinstate=LOW;
                    break;
               case 'q':
               case 'Q':
                    run = LOW;
                    //add your run-abort code here
                    Serial.println("Your run-abort code here.");
                    turnOffFans();
                    //end run-abort code
                    blipCnt=0;
                    Serial.println("All OFF!");
                    Serial.println(startMsg);
                    break;
               case 'g':
               case 'G':
                   if(run!=HIGH)
                   {
                     Serial.println("");
                    //add your reinit code here
                    initFans();
                    //end reinit code
                   }
                    run = HIGH;
                    Serial.println("All ON!");
                    break;             
              case 'v':
                   Serial.println(progVer);
                   break;              
              default:
                   Serial.print("Unknown cmd: ");
                   Serial.println(ch);
          }
}


