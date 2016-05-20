#define rpmPin 2 // датчик Холла

const int tunePin = A0;

long microsPrev = 0;
double rpmCur = 0;
double rpmPrev = 0;
int spikesCounter = 0;

bool blinkState = false;
long blinkLastTime = 0;


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
  Serial.println(rpmCur);
  showRpm(rpmCur, shiftRpm);
  //showRpm(9000, 6000);

  
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
    double curRpm = rpm;

    if (rpm > shiftRpm )
    {
      Serial.println(blinkState);
      if (blinkState && millis() - blinkLastTime > 50)
      {
        for (int i = 3; i < 9; i++)
        {
          digitalWrite(i, HIGH);
        }
        blinkLastTime = millis();
        blinkState = !blinkState;
      }
      else if (!blinkState && millis() - blinkLastTime > 50)
      {
        for (int i = 3; i < 9; i++)
        {
          digitalWrite(i, LOW);
        }
        blinkLastTime = millis();
        blinkState = !blinkState;
      }


    }
    else
    {    
      for (int i = 3; i < 9; i++)
      {
        if (curRpm > shiftRpm / 7)
        {
          digitalWrite(i, HIGH);
        }
        else
        {
          digitalWrite(i, LOW);
        }
  
        curRpm = curRpm - shiftRpm / 7;
      }
    }
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
