#define RPMpin 2 // датчик Холла
long microsold = 0;
int rpm = 0;
int lastRpm = 0;
int spikesCounter = 0;
bool DEBUG = true;

void setup() { 
  Serial.begin(57600);
  digitalWrite(RPMpin, 1);
  attachInterrupt(0, RPM, FALLING);

  for (int i = 5; i < 9; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void debugPrint(){
    Serial.print("rpm = ");
    Serial.println(rpm, DEC); // об/мин
    
    Serial.print("lastRpm = ");
    Serial.println(lastRpm);
    int diff = abs(lastRpm - rpm);
       
    Serial.print("diff = ");
    Serial.println(diff);

    Serial.print("SPIKE COUNTER = ");
    Serial.println(spikesCounter);  
}

void loop() {
    int diff = abs(lastRpm - rpm);
       
    if (diff > 300 && spikesCounter < 3){
      //debugPrint();
      spikesCounter++;
      rpm = lastRpm;
    } 
    else 
    {
      if (spikesCounter > 0) 
      {
        spikesCounter--;  
      }
      else
      {
        spikesCounter = 0;
      }
    }


    Serial.println(rpm, DEC); // об/мин
   
    
    if (rpm > 2000) {digitalWrite(5, HIGH);} else {digitalWrite(5, LOW);}
    if (rpm > 3000) {digitalWrite(6, HIGH);} else {digitalWrite(6, LOW);}
    if (rpm > 4000) {digitalWrite(7, HIGH);} else {digitalWrite(7, LOW);}
    if (rpm > 5000) {digitalWrite(8, HIGH);} else {digitalWrite(8, LOW);}
    
    
    lastRpm = rpm;
    //delay(10);

}
void RPM (){
  rpm = (1000000.0/(micros() - microsold))*60;
  microsold = micros();
}
