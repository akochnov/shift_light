#ifndef EngineSimulator_h
#define EngineSimulator_h

#include <Arduino.h>

class EngineSimulator
{
  public:
    EngineSimulator(uint16_t lowerBound, uint16_t upperBound);
    uint16_t simulateEngine();
    
  private:
    uint16_t _upperBound, _lowerBound;
    uint16_t _rpm;
    uint16_t _rpmIncrement;
    uint32_t _rpmLastChangeTime;
};

#endif

