#include "LedStrip.h"

#define RPM_PIN       2         //Tachometer signal
#define LEDSTRIP_PIN  9         //Digital output to led strip
#define NUMPIXELS     9         //qty of leds in strip
#define TUNE_PIN      A0        //Tuning potentiometer PIN
#define P_RANGE       3500      //Threshold to switch color of indication

LedStrip pixels = LedStrip(NUMPIXELS, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

long microsPrev = 0;
double rpmCur = 0;
double rpmPrev = 0;
int spikesCounter = 0;

bool blinkState = false;
long blinkLastTime = 0;

uint32_t colorIdle = pixels.Color(150, 0, 0);             //Red
uint32_t colorPerformance = pixels.Color(0, 150, 0);      //Green
uint32_t colorShift = pixels.Color(150, 150, 150);        //White

void setup() { 
  Serial.begin(9600);

  digitalWrite(RPM_PIN, 1);
  attachInterrupt(0, getRpm, RISING);

  pixels.begin();
  pixels.piu();
}


void loop() 
{
  int shiftRpm = getShiftThreshold();                           //Read potentiometer to define current shift threshhold
  Serial.println(rpmCur);
  if (idle())
  {
    showRpm(rpmCur, shiftRpm);
  }
  else
  {
    pixels.piu();
  }
}

bool idle()
{
  Serial.println((micros() - microsPrev));
  return ((micros() - microsPrev) < 100000);                       //Returns true if engine is running
}

int getShiftThreshold()                                         //Function checks the potentiometer input and converts it to rev/minute (3000 - 9000)
{
  int sensorValue = analogRead(TUNE_PIN);
  sensorValue = constrain(sensorValue, 0, 1023);
  return map(sensorValue, 0, 1023, 3000, 9000);
}

void showRpm(double rpm, int shiftRpm)                          //Activates indicator based on current rpm and shift limit
{
    uint32_t color = (rpm > P_RANGE) ? colorPerformance : colorIdle;
    
    if (rpm >= shiftRpm && (millis() - blinkLastTime) > 40)     //Shift light blinks every 40 milliseconds
    {
      if (!blinkState)
      {
        pixels.pixels(NUMPIXELS, colorShift);                                         //Start with all leds ON
      }
      else 
      {
        pixels.pixels(0, colorShift);
      }
      blinkLastTime = millis();
      blinkState = !blinkState;
    }
    else if (rpm < shiftRpm)                          //Indicate current engine speed with leds strip
    {    
      int count = (int)rpm / (shiftRpm / NUMPIXELS);   //Count how many leds to activate
      pixels.pixels(count, color);
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
  
  rpmCur = (1000000.0/(micros() - microsPrev))*60 / 2;
  
  //Catch spikes
  bool isSpike = checkForSpike(rpmCur, rpmPrev);
  if (isSpike) 
  {
    if (spikesCounter < maxSpikes)           //More than (qty = maxSpikes) in a row considered as good value
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

