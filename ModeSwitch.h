#ifndef ModeSwitch_h
#define ModeSwitch_h

#include <Arduino.h>

class ModeSwitch
{
  public:
    ModeSwitch(uint8_t m);
    uint8_t getMode();
    void nextMode();
  private:
    uint8_t mode;
};

#endif
