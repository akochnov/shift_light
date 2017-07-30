#include "LedStrip.h"

#define DELIMITER       2         //BMW = 2
#define NUMPIXELS       10        //qty of leds in strip
#define REV_MIN         3000         //Minimal speed to consider
#define REV_PERF        4800      //Threshold to switch color of indication
#define REV_SHIFT       7300      //Threshold to shift-light
#define RPM_PIN         2         //Tachometer signal
#define LEDSTRIP_PIN    9         //Digital output to led strip

LedStrip pixels = LedStrip(NUMPIXELS, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

//Coloring globals                         (R)  (G)  (B)
uint32_t colorIdle =          pixels.Color(150, 100,   0);          //Red
uint32_t colorPerformance =   pixels.Color(0,   150, 0);          //Green
uint32_t colorShift =         pixels.Color(150, 150, 150);        //White

//Speed globals
long prevMicros = 0;          //micros() When last time speed was measured
double curSpeed = 0;          
double prevSpeed = 0;

//Speed spikes catcher globals
int spikesCounter = 0;

//Shift Light blinker globals
bool lightOn = false;
long blinkLastTime = 0;


void setup() { 
  Serial.begin(9600);

  digitalWrite(RPM_PIN, 1);
  attachInterrupt(0, getRpm, RISING);

  pixels.begin();
  pixels.piu();
}

void loop() 
{
  Serial.println(curSpeed);
  
  if (engineRunning())
  {
    if (curSpeed >= REV_SHIFT){
      shiftLight();
    } else { 
      indicateRpm(curSpeed);
    }
  }
  else
  {
    //pixels.piu();
    //delay(1000);
  }
}

bool engineRunning()
{
  //Serial.println((micros() - prevMicros));
  return ((micros() - prevMicros) < 500000);                       //Returns true if engine is running
}

void indicateRpm(double rpm)
{
  uint32_t color = (rpm > REV_PERF) ? colorPerformance : colorIdle;               //Which color to use
  int countPixels = (rpm < REV_MIN) ? 0 : ((int)rpm - REV_MIN) / ((REV_SHIFT - REV_MIN) / pixels.numPixels());        //How many leds to activate
  pixels.pixels(countPixels, color);                                              //Light LEDs
  lightOn = false;                                                                //Next time to start shift blinking with leds ON
}

void shiftLight()                           
{
    if ((millis() - blinkLastTime) > 40)    //Shift light blinks every 40 milliseconds
    {
      int countPixels = (!lightOn) ? pixels.numPixels() : 0;
      pixels.pixels(countPixels, colorShift);
      blinkLastTime = millis();
      lightOn = !lightOn;
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
  
  curSpeed = (1000000.0/(micros() - prevMicros))*60 / DELIMITER;
  
  //Catch spikes
  if (checkForSpike(curSpeed, prevSpeed)) 
  {
    if (spikesCounter < maxSpikes)                    //More than (qty = maxSpikes) in a row considered as good value
    {
      spikesCounter++;
      curSpeed = prevSpeed;                              //Take previous rpm value in case spike is detected
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
  
  prevMicros = micros();
  prevSpeed = curSpeed;
}

