#ifndef Tachometer_h
#define Tachometer_h

#include <Arduino.h>


class Tachometer
{
  public:
    Tachometer(uint8_t spikesAllowed, uint8_t pulsesPerRevolution);
    uint16_t getEngineSpeed();
    void processInterrupt();
    
  private:
    //Settings
    uint8_t _delimeter;
    
    //RPM calculation related
    uint32_t _lastInterruptTime;                      //Last interruption time
    uint16_t _rpm;                                    //Current RPM value
    uint16_t _prevRpm;                                //Previous RPM value
    
    //Speed spikes catcher globals
    uint8_t _maxSpikes;
    uint8_t _spikesCounter;
    
    //Helper methods
    bool isSpike(uint16_t rpm1, uint16_t rpm2);
    bool isRunning();

};

#endif

