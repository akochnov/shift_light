#ifndef LedStrip_h
#define LedStrip_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class LedStrip : public Adafruit_NeoPixel
{
  public:
    LedStrip(uint16_t n, uint8_t p, neoPixelType t);
    void piu();
    int test();
    void pixels(int n, uint32_t c);
    void setMode(int m);
    void showPercent(int p);
  private:
    int mode;
    int rMin;
    int rPerf;
    int rShift;

};

#endif

