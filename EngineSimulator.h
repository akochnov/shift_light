#ifndef EngineSimulator_h
#define EngineSimulator_h

#include <Arduino.h>

class EngineSimulator
{
  public:
    EngineSimulator();
    int simulateEngine();
    
  private:
    unsigned int _rpm;
    unsigned long _rpmLastChangeTime;
    int _rpmIncrement;
};

#endif

