#define rpmPin 2 // датчик Холла

const int tunePin = A0;

long microsPrev = 0;
double rpmCur = 0;
double rpmPrev = 0;
int spikesCounter = 0;


void setup() { 
  Serial.begin(9600);
  digitalWrite(rpmPin, 1);
  attachInterrupt(0, getRpm, RISING);

  for (int i = 3; i < 9; i++)
  {
    pinMode(i, OUTPUT);
  }
}


void loop() 
{
 
  int shiftRpm = getShiftThreshold();
  Serial.println(shiftRpm);
  showRpm(rpmCur, shiftRpm);

  
}

int getShiftThreshold()
{
  const int potentiometerPin = A0;

  int sensorValue = analogRead(potentiometerPin);
  sensorValue = constrain(sensorValue, 0, 1023);
  return map(sensorValue, 0, 1023, 3000, 9000);
}



void showRpm(double rpm, int shiftRpm)
{
    const double rpmMin = 800;
    const double ledMax = 1536;             //6 leds x 256
    int digitalValue;
    digitalValue = (int)constrain(ledMax / shiftRpm * rpm, 0, ledMax);

    for (int i = 3; i < 9; i++)
    {
      digitalWrite(i, constrain(digitalValue, 0, 255));
      digitalValue = digitalValue - 256;
    }

    
    //Serial.println(digitalValue, DEC);
    /*
    if (rpm > 2000) {digitalWrite(3, HIGH);} else {digitalWrite(3, LOW);}
    if (rpm > 3000) {digitalWrite(4, HIGH);} else {digitalWrite(4, LOW);}
    if (rpm > 4000) {digitalWrite(5, HIGH);} else {digitalWrite(5, LOW);}
    if (rpm > 5000) {digitalWrite(6, HIGH);} else {digitalWrite(6, LOW);}
    if (rpm > 6000) {digitalWrite(7, HIGH);} else {digitalWrite(7, LOW);}
    if (rpm > 7000) {digitalWrite(8, HIGH);} else {digitalWrite(8, LOW);}
    */
    //Serial.println(rpm);
}

bool checkForSpike(double rpm1, double rpm2)
{
  const int spikeThreshold = 200;
  
  return abs(rpm1 - rpm2) > spikeThreshold;
}

void getRpm()
{
  const int maxSpikes = 5;                   
  
  rpmCur = (1000000.0/(micros() - microsPrev))*60;
  
  //Catch spikes
  bool isSpike = checkForSpike(rpmCur, rpmPrev);
  if (isSpike) 
  {
    if (spikesCounter < maxSpikes)           //More than XX spikes in a row considered as good value
    {
      spikesCounter++;
      rpmCur = rpmPrev;                              //Take previous rpm value in case spike is detected
    }
    else
    {
      spikesCounter = 0;
    }
  }
  else
  {
    spikesCounter = 0;
  }
  
  microsPrev = micros();
  rpmPrev = rpmCur;
  //Serial.println(rpm);
  
}
