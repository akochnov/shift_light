#include "Indicator.h"

Indicator::Indicator(uint16_t qtyPixels, uint8_t pin)
{
  _pixels = Adafruit_NeoPixel(qtyPixels, pin, NEO_GRB + NEO_KHZ800);
  _pixels.begin();

  //Default value
  _smoothFilter = 10;

  _blinkLastTime = 0;
  _lightOn = false;

}



void Indicator::setSmoothingFilter(uint16_t filterValue)
{
  _smoothFilter = filterValue;
}




void Indicator::setColors(uint32_t colorIdle, uint32_t colorPerf, uint32_t colorShift)
{
  _colorIdle = colorIdle;
  _colorPerf = colorPerf;
  _colorShift = colorShift;
}




void Indicator::setResolution(uint16_t rpmMin, uint16_t rpmPerf, uint16_t rpmShift)
{
  _rpmMin = rpmMin;
  _rpmPerf = rpmPerf;
  _rpmShift = rpmShift;
  
  Serial.print(_rpmMin);
  Serial.println(" - _rpmMin");
  Serial.print(_rpmPerf);
  Serial.println(" - _rpmPerf");
  Serial.print(_rpmShift);
  Serial.println(" - _rpmShift");
}




void Indicator::piu()
{
  for (uint16_t i = 0; i <= _pixels.numPixels(); i++)
  {
      _pixels.setPixelColor(i, Adafruit_NeoPixel::Color(150, 150, 150));
      _pixels.setPixelColor(i-1, Adafruit_NeoPixel::Color(0, 0, 0));
      delay(10);
      _pixels.show();
  }
}




void Indicator::showRpm(uint16_t rpm, uint16_t style)
{
  if (rpm > _rpmShift)
  {
    shiftLight();
    return;
  }

  //Which color to use
  uint32_t colorToUse = (rpm > _rpmPerf) ? _colorPerf : _colorIdle;
  uint32_t c;              
  uint16_t pixelsToFireQTY;
  
  if (style == 0)
  {
    pixelsToFireQTY = rpmToPixelsQty(rpm, _pixels.numPixels());

    if (pixelsToFireQTY != _oldPixelsCount)
    {
      for (uint16_t i = 0; i < _pixels.numPixels(); i++)                             
      {
        c = (i < pixelsToFireQTY) ? colorToUse : Adafruit_NeoPixel::Color(0, 0, 0);
        _pixels.setPixelColor(i, c);
      }
      
      _pixels.show();                                              
      _oldPixelsCount = pixelsToFireQTY;
    }
  }

  if (style == 1)
  {
    uint16_t activePixelsQty = (uint16_t)(_pixels.numPixels() / 2);
    if (_pixels.numPixels() % 2 > 0) activePixelsQty++;                                  //in case odd qty of pixels
      
    pixelsToFireQTY = rpmToPixelsQty(rpm, activePixelsQty);
    
    if (pixelsToFireQTY != _oldPixelsCount)
    {
      for (uint16_t i = 0; i < activePixelsQty; i++)
      {
        c = (i < pixelsToFireQTY) ? colorToUse : Adafruit_NeoPixel::Color(0, 0, 0);
        _pixels.setPixelColor(i, c);
        _pixels.setPixelColor(_pixels.numPixels() - i - 1, c);
      }
    
      _pixels.show();
      _oldPixelsCount = pixelsToFireQTY;
    }
  }
  
  //Reset shift-light state
  _lightOn = false;                                                         
}




uint16_t Indicator::rpmToPixelsQty(uint16_t rpm, uint16_t totalPixels)
{
  //How many leds to activate
  uint16_t countPixels = 0;
  uint16_t segment;
  
  if (rpm > _rpmMin)
  {
    segment = (uint16_t)(_rpmShift - _rpmMin) / totalPixels;
    countPixels = (uint16_t)((rpm - _rpmMin) / segment) + 1;
/*
    Serial.print(segment);
    Serial.print(" - segment, ");
    Serial.print(countPixels);
    Serial.print(" - countPixels, ");
    Serial.print(countPixels * segment + _rpmMin);
    Serial.print(" - threshold, ");
*/
    //Avoid led blinking at boundary values
    //
    if ((countPixels - _oldPixelsCount == 1) && ((countPixels - 1) * segment + _rpmMin + _smoothFilter > rpm))
    {
//      Serial.print("--");
      countPixels--;
    }

    if ((_oldPixelsCount - countPixels == 1) && ((countPixels) * segment + _rpmMin - _smoothFilter < rpm))
    {
      countPixels++;
//      Serial.print("++");
    }
//    Serial.println(countPixels);
  }
  
  return countPixels;

}



void Indicator::pixels(uint16_t n, uint32_t c)
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




void Indicator::shiftLight()                         
{
    if ((millis() - _blinkLastTime) > 100)     //Shift light blinks every 40 milliseconds
    {
      int countPixels = (!_lightOn) ? _pixels.numPixels() : 0;
      
      pixels(countPixels, _colorShift);
      _blinkLastTime = millis();
      _lightOn = !_lightOn;
    }
}



