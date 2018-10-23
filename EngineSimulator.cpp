#include "EngineSimulator.h"



EngineSimulator::EngineSimulator(uint16_t lowerBound = 3000, uint16_t upperBound = 5600)
{
  _rpm = 3000;
  _rpmLastChangeTime = 0;
  _rpmIncrement = 15;

  _lowerBound = lowerBound;
  _upperBound = upperBound;
}



uint16_t EngineSimulator::simulateEngine()
{
  if ((millis() - _rpmLastChangeTime) > 20)
  {
    _rpm = _rpm + _rpmIncrement;
    if (_rpm > _upperBound || _rpm < _lowerBound) _rpmIncrement = -_rpmIncrement;
    _rpmLastChangeTime = millis();
  }
  return _rpm;
}

