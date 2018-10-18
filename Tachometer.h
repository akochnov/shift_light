#ifndef Tachometer_h
#define Tachometer_h

#include <Arduino.h>


class Tachometer
{
  public:
    Tachometer(uint8_t spikesAllowed, uint8_t pulsesPerRevolution);
    double getEngineSpeed();
    void processInterrupt();
    
  private:
    //Settings
    uint8_t _delimeter;
    
    //RPM calculation related
    long _lastInterruptTime;                     //Last interruption time
    double _rpm;                     //Current RPM value
    double _prevRpm;                    //Previous RPM value
    
    //Speed spikes catcher globals
    uint8_t _maxSpikes;
    uint8_t _spikesCounter;
    
    //Helper methods
    bool isSpike(double rpm1, double rpm2);
    bool isRunning();

};

#endif

