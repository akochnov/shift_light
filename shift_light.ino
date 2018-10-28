#define DEBUG
#include "src/DebugMacros.h"

#include "src/Tachometer.h"
#include "src/EngineSimulator.h"
#include "src/Indicator.h"
#include <Adafruit_NeoPixel.h>

#define REV_MIN             1000      //Tachometer minimal rpm
#define REV_PERF            2000      //Color indication change threshold
#define REV_SHIFT           3000      //Shift-light RPM

//Coloring                               t                     (R)      (G)     (B)
uint32_t const COLOR_IDLE = Adafruit_NeoPixel::Color          (150,     50,     0);          //Amber
uint32_t const COLOR_PERFORMANCE = Adafruit_NeoPixel::Color   (0,       150,    0);          //Green
uint32_t const COLOR_SHIFT = Adafruit_NeoPixel::Color         (150,     150,    150);        //White


#define TACHO_STYLE         1         //  0 = linear; 1 = Side-to-center
#define PPR                 2         //Pulses per revolution
#define MAX_SPIKES          10
#define SMOOTHING_FILTER    10

#define NUMPIXELS           12        //qty of LEDs
#define LEDSTRIP_PIN        10        //Digital output to led strip
#define RPM_PIN             2         //Tachometer signal pin
#define DIM_PIN             8         // not in use

#define BENCHMODE           false     //Activates simulator


Tachometer tacho = Tachometer(MAX_SPIKES, PPR);

EngineSimulator simulator = EngineSimulator(2000, 6200);

Indicator indicator = Indicator(NUMPIXELS, LEDSTRIP_PIN);

//
//  Setup
//

void setup() { 
  Serial.begin(9600);
    
  digitalWrite(RPM_PIN, 1);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), getRpm, RISING);

  indicator.setColors(COLOR_IDLE, COLOR_PERFORMANCE, COLOR_SHIFT);
  indicator.setResolution(REV_MIN, REV_PERF, REV_SHIFT);
  indicator.setSmoothingFilter(SMOOTHING_FILTER);
  indicator.piu();
  
  delay(500);
}




//
//  Main loop
//
void loop() 
{

  long rpm;

  if (!BENCHMODE)
    rpm = tacho.getEngineSpeed();
  else
    rpm = simulator.simulateEngine();
    
  DEBUG_PRINT(rpm);

  indicator.showRpm(rpm, TACHO_STYLE);

}





//
//  Function assigned to interruption (rising)
//
void getRpm()
{
  tacho.processInterrupt();
}

