#include "LedStrip.h"

#define DELIMITER       2         //
#define NUMPIXELS       10        //qty of LEDs
#define REV_MIN         3000      //Tachometer minimal rpm
#define REV_PERF        3300      //Color indication change threshold
#define REV_SHIFT       4700      //Shift-light RPM
#define RPM_PIN         2         //Tachometer signal pin
#define DIM_PIN         8         //Dimmer +12v
#define LEDSTRIP_PIN    10        //Digital output to led strip
#define TACHO_STYLE     0         //  0 = linear; 1 = Side-to-center

#define MAX_SPIKES      5
#define BENCHMODE       false         //Run without external RPM source


LedStrip pixels = LedStrip(NUMPIXELS, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

//Coloring globals                t         (R)      (G)     (B)
uint32_t colorIdle =          pixels.Color(150,     50,    0);          //Amber
uint32_t colorPerformance =   pixels.Color(0,       150,    0);          //Green
uint32_t colorShift =         pixels.Color(150,     150,    150);        //White

//Speed globals
long prevMicros = 0;          //micros() When last time speed was measured
double curSpeed = 0;          
double prevSpeed = 0;

//Speed spikes catcher globals
int spikesCounter = 0;

//Shift Light blinker globals
bool lightOn = false;
long blinkLastTime = 0;

//Variables for engine rpm simulation
unsigned int rpm = 3000;
unsigned long lastRpmChangedMillis = 0;
int rpmIncrement = 20;

int dim = 0;

void setup() { 
  Serial.begin(9600);

  pinMode(DIM_PIN, INPUT);
  pinMode(13, OUTPUT);
  
  digitalWrite(RPM_PIN, 1);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), getRpm, RISING);

  pixels.begin();
  pixels.piu();
  
  delay(500);
}


//
//  Main loop here
//
void loop() 
{
  Serial.println(curSpeed);
  
  dim = digitalRead(DIM_PIN);     // read the input pin
  if (dim == 1) pixels.setBrightness(50);
  else pixels.setBrightness(255);
  //Serial.println(dim);
  digitalWrite(13, dim);    // sets the LED to the button's value
  
  //Engine simulation
  if (BENCHMODE) simulateEngine();

  if (!engineRunning()) indicateRpm(0);
  
  if (engineRunning() && curSpeed < REV_SHIFT) 
  {
    if (TACHO_STYLE == 0) indicateRpm(curSpeed);
    if (TACHO_STYLE == 1) indicateRpm2(curSpeed);
  }
  if (engineRunning() && curSpeed >= REV_SHIFT) shiftLight();
}


//
//  For BENCHMODE = 1 simulation of engine running for
//
void simulateEngine()
{
    if ((millis() - lastRpmChangedMillis) > 15)
    {
      rpm = rpm + rpmIncrement;
      if (rpm > 7700 || rpm < 3000) rpmIncrement = -rpmIncrement;
      lastRpmChangedMillis = millis();
      curSpeed = rpm;
    }
}


//
//  Function determins if engine is currently running or not
//
bool engineRunning()
{
  //Serial.println((micros() - prevMicros));
  if (BENCHMODE) return true;
  return ((micros() - prevMicros) < 500000);                       //Returns true if engine is running
}



//
//  Linear indication of rpm
//
void indicateRpm(double rpm)            
{
  uint32_t color = (rpm > REV_PERF) ? colorPerformance : colorIdle;               //Which color to use
  int countPixels = (rpm < REV_MIN) ? 0 : ((int)rpm - REV_MIN) / ((REV_SHIFT - REV_MIN) / pixels.numPixels());        //How many leds to activate
  pixels.pixels(countPixels, color);                                              //Light LEDs
  lightOn = false;                                                                //Next time to start shift blinking with leds ON
}



//
//  RPM Indication from side to center
//
void indicateRpm2(unsigned int rpm)
{
  uint32_t color = (rpm > REV_PERF) ? colorPerformance : colorIdle;               //Which color to use
  byte n;
  if (pixels.numPixels() % 2 > 0)          //in case odd qty of pixels
    n = (pixels.numPixels() / 2) + 1;
  else
    n = pixels.numPixels() / 2;          //case even qty of pixels
  
  pixels.pixels2(pixels.rpmToPixelsQty(n, rpm, REV_MIN, REV_SHIFT - 300), color);
}


//
//  Shift light blinker
//
void shiftLight()                           
{
    if ((millis() - blinkLastTime) > 40)    //Shift light blinks every 40 milliseconds
    {
      int countPixels = (!lightOn) ? pixels.numPixels() : 0;

//Timofey non-blinking shift-light
      countPixels = pixels.numPixels();
//End

      pixels.pixels(countPixels, colorShift);
      blinkLastTime = millis();
      lightOn = !lightOn;
    }
}

//
//  Function determines if measured speed is spike or not
//
bool isSpike(double rpm1, double rpm2)          //Helper function to define spike (noise) between two rpm measurements
{
  const int spikeThreshold = 200;              //200 rev/minute is defined as a spike
  return abs(rpm1 - rpm2) > spikeThreshold;
}

//
//  Function is assigned to interruption on tacho PIN (rising). Calculates engine rpm.
//
void getRpm()
{
  curSpeed = (1000000.0/(micros() - prevMicros))*60 / DELIMITER;
  
  //Catch spikes
  if (isSpike(curSpeed, prevSpeed)) 
  {
    if (spikesCounter < MAX_SPIKES)                    //Several spikes in a row considered as good value
    {
      spikesCounter++;
      curSpeed = prevSpeed;                           //Take previous rpm value in case spike is detected
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

