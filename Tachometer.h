#ifndef Tachometer_h
#define Tachometer_h

#include <Arduino.h>


class Tachometer
{
  public:
    Tachometer(uint8_t delimeter = 2);
    double getEngineSpeed();
    void processInterrupt();
    
  private:
    //Speed globals
    long prevMicros = 0;          //micros() When last time speed was measured
    double curSpeed = 0;          
    double prevSpeed = 0;
    
    //Speed spikes catcher globals
    int spikesCounter = 0;

    //Variables for engine rpm simulation
    unsigned int rpm = 3000;
    unsigned long lastRpmChangedMillis = 0;
    int rpmIncrement = 20;
    
    //Helper methods
    bool isSpike(double rpm1, double rpm2);

};

#endif

