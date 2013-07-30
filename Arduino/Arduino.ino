int incomingByte = 0;
int led = 13;
void setup() {                
  Serial.begin(9600);
  pinMode(led, OUTPUT); 
}

void loop() {
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000); 
  
  }
}
