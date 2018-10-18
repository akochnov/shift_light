#include "EngineSimulator.h"

EngineSimulator::EngineSimulator()
{
  _rpm = 3000;
  _rpmLastChangeTime = 0;
  _rpmIncrement = 20;
}


int EngineSimulator::simulateEngine()
{
  if ((millis() - _rpmLastChangeTime) > 15)
  {
    _rpm = _rpm + _rpmIncrement;
    if (_rpm > 7700 || _rpm < 3000) _rpmIncrement = -_rpmIncrement;
    _rpmLastChangeTime = millis();
  }
  return _rpm;
}

