#include "LedStrip2.h"

LedStrip2::LedStrip2(uint16_t qtyPixels, uint8_t pin)
{
  _pixels = Adafruit_NeoPixel(qtyPixels, pin, NEO_GRB + NEO_KHZ800);
  _pixels.begin();

  _blinkLastTime = 0;
  _lightOn = false;
}



void LedStrip2::showRpm(uint16_t rpm)
{
  if (rpm > _rpmShift)
  {
    shiftLight();
    return;
  }

  //Which color to use
  uint32_t color = (rpm > _rpmPerf) ? _colorPerf : _colorIdle;               

  //How many leds to activate
  uint16_t countPixels = 0;
  if (rpm > _rpmMin)
  {
    countPixels = (rpm - _rpmMin) / ((_rpmShift - _rpmMin) / _pixels.numPixels());        //How many leds to activate
  }

  //Fire indicator
  pixels(countPixels, color);                                              
  
  //Reset shift-light state
  _lightOn = false;                                                         
}


void LedStrip2::pixels(uint16_t n, uint32_t c)
{
  uint16_t qtyPixels = _pixels.numPixels();
  uint32_t color;

  for (uint16_t i = 0; i < qtyPixels; i++)                             
  {
    color = (i < n) ? c : Adafruit_NeoPixel::Color(0, 0, 0);
    _pixels.setPixelColor(i, color);
  }
  _pixels.show();
}


void LedStrip2::shiftLight()                         
{
    if ((millis() - _blinkLastTime) > 40)     //Shift light blinks every 40 milliseconds
    {
      int countPixels = (!_lightOn) ? _pixels.numPixels() : 0;
      
      pixels(countPixels, _colorShift);
      _blinkLastTime = millis();
      _lightOn = !_lightOn;
    }
}


void LedStrip2::setColors(uint32_t colorIdle, uint32_t colorPerf, uint32_t colorShift)
{
  _colorIdle = colorIdle;
  _colorPerf = colorPerf;
  _colorShift = colorShift;
}


void LedStrip2::setResolution(uint16_t rpmMin, uint16_t rpmPerf, uint16_t rpmShift)
{
  _rpmMin = rpmMin;
  Serial.print(_rpmMin);
  Serial.println(" - _rpmMin");
  _rpmPerf = rpmPerf;
  Serial.print(_rpmPerf);
  Serial.println(" - _rpmPerf");
  _rpmShift = rpmShift;
  Serial.print(_rpmShift);
  Serial.println(" - _rpmShift");
}


void LedStrip2::piu()
{
  for (uint16_t i = 0; i <= _pixels.numPixels(); i++)
  {
      _pixels.setPixelColor(i, Adafruit_NeoPixel::Color(150, 150, 150));
      _pixels.setPixelColor(i-1, Adafruit_NeoPixel::Color(0, 0, 0));
      delay(10);
      _pixels.show();    
  }
}
