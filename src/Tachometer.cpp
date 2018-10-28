#include "Tachometer.h"

Tachometer::Tachometer(uint8_t spikesAllowed = 5, uint8_t pulsesPerRevolution = 2)
{
  _delimeter = pulsesPerRevolution;
  _lastInterruptTime = 0;
  _rpm = 0;          
  _prevRpm = 0;

  _maxSpikes = spikesAllowed;
  _spikesCounter = 0;
}



bool Tachometer::isRunning()
{
  return ((micros() - _lastInterruptTime) < 500000);                       //Returns true if engine is running
}




uint16_t Tachometer::getEngineSpeed()
{
  if (!isRunning()) _rpm = 0;
  
  return _rpm;
}




bool Tachometer::isSpike(uint16_t rpm1, uint16_t rpm2)          //Spike of more than 200 rev/min defined as noise signal
{
  return abs(rpm1 - rpm2) > 200;                 
}




void Tachometer::processInterrupt()
{
  _rpm = (uint16_t)(1000000/(micros() - _lastInterruptTime))*60 / _delimeter;  
  
  //Catch noise values
  if (isSpike(_rpm, _prevRpm) && _spikesCounter < 5) 
  {
    _spikesCounter++;
    _rpm = _prevRpm;                           //Take previous rpm value in case spike is detected
  }
  else
  {
    _spikesCounter = 0;
  }

  
  _lastInterruptTime = micros();
  _prevRpm = _rpm;
}

