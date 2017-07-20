#include "LedStrip.h"

LedStrip::LedStrip(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800) : Adafruit_NeoPixel(n, p, t) 
{
  mode = 0;
  rMin = 0;
  rPerf = 4500;
  rShift = 7000;
}

void LedStrip::setMode(int m)
{
  if (3 > mode) mode = m;
}



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

void LedStrip::showPercent(int p)
{
  int pix = 100 / Adafruit_NeoPixel::numPixels();
  int c = p / pix;
  LedStrip::pixels(c , Adafruit_NeoPixel::Color(150, 150, 150));

}

