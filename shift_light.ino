#include "LedStrip.h"
#include "Tachometer.h"
#include "EngineSimulator.h"

#define DELIMITER       2         //
#define NUMPIXELS       12        //qty of LEDs
#define REV_MIN         3000      //Tachometer minimal rpm
#define REV_PERF        3300      //Color indication change threshold
#define REV_SHIFT       4700      //Shift-light RPM
#define RPM_PIN         2         //Tachometer signal pin
#define DIM_PIN         8         //Dimmer +12v
#define LEDSTRIP_PIN    10        //Digital output to led strip
#define TACHO_STYLE     0         //  0 = linear; 1 = Side-to-center

#define MAX_SPIKES      5
#define BENCHMODE       false         //Run without external RPM source

//Coloring                               t            (R)      (G)     (B)
uint32_t const COLOR_IDLE = LedStrip::Color          (150,     50,     0);          //Amber
uint32_t const COLOR_PERFORMANCE = LedStrip::Color   (0,       150,    0);          //Green
uint32_t const COLOR_SHIFT = LedStrip::Color         (150,     150,    150);        //White


//Shift Light blinker globals
bool lightOn = false;
long blinkLastTime = 0;

Tachometer tacho = Tachometer(MAX_SPIKES, DELIMITER);
EngineSimulator simulator = EngineSimulator();
LedStrip pixels = LedStrip(NUMPIXELS, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() { 
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  //Assign interruption on pin
  digitalWrite(RPM_PIN, 1);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), getRpm, RISING);


  pixels.begin();
  pixels.piu();
  
  delay(500);
}

//
//  Main loop
//
void loop() 
{
  long rpm;

  if (!BENCHMODE)
  {
    rpm = tacho.getEngineSpeed();
  }
  else
  {
    rpm = simulator.simulateEngine();
  }
    
  Serial.println(rpm);

  if (rpm < REV_SHIFT) 
  {
    if (TACHO_STYLE == 0) indicateRpm(rpm);
    if (TACHO_STYLE == 1) indicateRpm2(rpm);
  }
  if (rpm >= REV_SHIFT) shiftLight();
}


//
//  Linear indication of rpm
//
void indicateRpm(double rpm)            
{
  uint32_t color = (rpm > REV_PERF) ? COLOR_PERFORMANCE : COLOR_IDLE;               //Which color to use
  int countPixels = (rpm < REV_MIN) ? 0 : ((int)rpm - REV_MIN) / ((REV_SHIFT - REV_MIN) / pixels.numPixels());        //How many leds to activate
  pixels.pixels(countPixels, color);                                              //Light LEDs
  lightOn = false;                                                                //Next time to start shift blinking with leds ON
}



//
//  RPM Indication from side to center
//
void indicateRpm2(unsigned int rpm)
{
  uint32_t color = (rpm > REV_PERF) ? COLOR_PERFORMANCE : COLOR_IDLE;               //Which color to use
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

      pixels.pixels(countPixels, COLOR_SHIFT);
      blinkLastTime = millis();
      lightOn = !lightOn;
    }
}

//
//  Function is assigned to interruption on tacho PIN (rising)
//

void getRpm()
{
  tacho.processInterrupt();
}

