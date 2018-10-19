#ifndef EngineSimulator_h
#define EngineSimulator_h

#include <Arduino.h>

class EngineSimulator
{
  public:
    EngineSimulator();
    uint16_t simulateEngine();
    
  private:
    uint16_t _rpm;
    uint32_t _rpmLastChangeTime;
    uint16_t _rpmIncrement;
};

#endif

