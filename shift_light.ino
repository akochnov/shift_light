#define rpmPin 2 // датчик Холла

const int tunePin = A0;
const int potentiometerPin = A0;
const int ledCount = 6;

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

int getShiftThreshold()                                         //Function checks the potentiometer input and converts it to rev/minute (3000 - 9000)
{
  int sensorValue = analogRead(potentiometerPin);
  sensorValue = constrain(sensorValue, 0, 1023);
  return map(sensorValue, 0, 1023, 3000, 9000);
}

void leds(int count)                                              //Function activates/deactivates XX number of leds in the strip
{
  for (int i = 1; i <= ledCount; i++)                             //Hardcoded 6 LEDS on PINS D3-D8
  {
    if (i > count) 
    {
      digitalWrite(i+2, LOW);
    }
    else
    {
      digitalWrite(i+2, HIGH);
    }
  }
}

void showRpm(double rpm, int shiftRpm)                          //Activates indicator based on current rpm and shift limit
{
    if (rpm >= shiftRpm && (millis() - blinkLastTime) > 40)     //Shift light blinks every 40 milliseconds
    {
      if (!blinkState)
      {
        leds(ledCount);                                         //Start with all leds ON
      }
      else 
      {
        leds(0);
      }
      blinkLastTime = millis();
      blinkState = !blinkState;
    }
    else if (rpm < shiftRpm)                          //Indicate current engine speed with leds strip
    {    
      int count = (int)rpm / (shiftRpm / ledCount);   //Count how many leds to activate
      leds(count);
      blinkState = false;                             //Next time to start shift blinking with leds ON
    }
}

bool checkForSpike(double rpm1, double rpm2)          //Helper function to define spike (noise) between two rpm measurements
{
  const int spikeThreshold = 200;                     //200 rev/minute is defined as a spike
  
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
