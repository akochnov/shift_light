#include "Tachometer.h"

Tachometer::Tachometer(uint8_t delimeter = 2)
{
  //TODO variable delimeters
}

double Tachometer::getEngineSpeed()
{
  return curSpeed;
}

bool Tachometer::isSpike(double rpm1, double rpm2)          //Spike of more than 200 rev/min defined as noise signal
{
  return abs(rpm1 - rpm2) > 200;                 
}

void Tachometer::processInterrupt()
{
  curSpeed = (1000000.0/(micros() - prevMicros))*60 / DELIMITER;
  
  //Catch spikes
  if (isSpike(curSpeed, prevSpeed)) 
  {
    if (spikesCounter < MAX_SPIKES)                    //Several spikes in a row considered as good value
    {
      spikesCounter++;
      curSpeed = prevSpeed;                           //Take previous rpm value in case spike is detected
    }
    else
    {
      spikesCounter = 0;
    }
  }
  else
  {
    spikesCounter = 0;
  }
  
  prevMicros = micros();
  prevSpeed = curSpeed;
}

