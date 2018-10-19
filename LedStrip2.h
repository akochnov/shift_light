#ifndef LedStrip2_h
#define LedStrip2_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class LedStrip2
{
  public:
    LedStrip2(uint16_t qtyPixels, uint8_t pin);
    void setColors(uint32_t colorIdle, uint32_t colorPerf, uint32_t colorShift);
    void setResolution(uint16_t rpmMin, uint16_t rpmPerf, uint16_t rpmShift);
    
    void piu();
    void showRpm(uint16_t rpm);

  private:
    Adafruit_NeoPixel _pixels;
    uint32_t _colorIdle, _colorPerf, _colorShift;
    uint16_t _rpmMin, _rpmPerf, _rpmShift;
    uint32_t _blinkLastTime;
    bool _lightOn;

    void pixels(uint16_t n, uint32_t c);
    void shiftLight();
};

#endif

