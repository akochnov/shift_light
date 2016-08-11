#include "LedStrip.h"

void LedStrip::piu()
{
  for (int i = 0; i <= Adafruit_NeoPixel::numPixels(); i++)                             
  {
      Adafruit_NeoPixel::setPixelColor(i, Adafruit_NeoPixel::Color(150, 150, 150));
      Adafruit_NeoPixel::setPixelColor(i-1, Adafruit_NeoPixel::Color(0, 0, 0));
      delay(10);
      Adafruit_NeoPixel::show();    
  }
}

void LedStrip::pixels(int n, uint32_t c)
{
  if (n > Adafruit_NeoPixel::numPixels())
  {
    n = Adafruit_NeoPixel::numPixels();
  }

  for (int i = 0; i < Adafruit_NeoPixel::numPixels(); i++)                             
  {
    if (i < n) 
    {
      Adafruit_NeoPixel::setPixelColor(i, c);
    }
    else
    {
      Adafruit_NeoPixel::setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 0));
    }
  }
  Adafruit_NeoPixel::show();
}



