#ifndef Indicator_h
#define Indicator_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Indicator
{
  public:
    Indicator(uint16_t qtyPixels, uint8_t pin);
    void setColors(uint32_t colorIdle, uint32_t colorPerf, uint32_t colorShift);
    void setResolution(uint16_t rpmMin, uint16_t rpmPerf, uint16_t rpmShift);
    void setSmoothingFilter(uint16_t filterValue);
    
    void piu();
    void showRpm(uint16_t rpm, uint16_t style);


  private:
    Adafruit_NeoPixel _pixels;
    
    uint32_t _colorIdle, _colorPerf, _colorShift;
    uint16_t _rpmMin, _rpmPerf, _rpmShift;
    
    uint16_t _oldPixelsCount;
    uint16_t _smoothFilter;
    
    uint32_t _blinkLastTime;
    bool _lightOn;
    
    
    uint16_t rpmToPixelsQty(uint16_t rpm, uint16_t totalPixels);
    void pixels(uint16_t n, uint32_t c);
    void shiftLight();
};

#endif

