#ifndef LedStrip_h
#define LedStrip_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class LedStrip : public Adafruit_NeoPixel
{
  public:
    LedStrip(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800) : Adafruit_NeoPixel(n, p, t) {};
    void piu();
    int test();
    void pixels(int n, uint32_t c);

};

#endif

