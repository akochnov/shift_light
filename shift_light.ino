#include "Indicator.h"
#include "Tachometer.h"
#include "EngineSimulator.h"
#include <Adafruit_NeoPixel.h>

#define DELIMITER       2         //Pulses per revolution
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

//Coloring                               t                     (R)      (G)     (B)
uint32_t const COLOR_IDLE = Adafruit_NeoPixel::Color          (150,     50,     0);          //Amber
uint32_t const COLOR_PERFORMANCE = Adafruit_NeoPixel::Color   (0,       150,    0);          //Green
uint32_t const COLOR_SHIFT = Adafruit_NeoPixel::Color         (150,     150,    150);        //White

Tachometer tacho = Tachometer(MAX_SPIKES, DELIMITER);
EngineSimulator simulator = EngineSimulator();

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
  {
    rpm = tacho.getEngineSpeed();
  }
  else
  {
    rpm = simulator.simulateEngine();
  }
    
  Serial.println(rpm);

  indicator.showRpm(rpm);
}


//
//  Function is assigned to interruption on tacho PIN (rising)
//
void getRpm()
{
  tacho.processInterrupt();
}

