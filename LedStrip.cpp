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


//
//  Linear
//
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


//
//  From side to center
//
void LedStrip::pixels2(int n, uint32_t c)
{
  int q;
  if (Adafruit_NeoPixel::numPixels() % 2 > 0)
    q = (Adafruit_NeoPixel::numPixels() / 2) + 1;
  else
    q = Adafruit_NeoPixel::numPixels() / 2;

  
  for (int i = 0; i < q; i++)
  {
    if (i < n)
    {
      Adafruit_NeoPixel::setPixelColor(i, c);
      Adafruit_NeoPixel::setPixelColor(Adafruit_NeoPixel::numPixels() - 1 - i, c);
    }
    else
    {
      Adafruit_NeoPixel::setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 0));
      Adafruit_NeoPixel::setPixelColor(Adafruit_NeoPixel::numPixels() -1 - i, Adafruit_NeoPixel::Color(0, 0, 0));
    }
  }

  Adafruit_NeoPixel::show();
}


//
//  Calculation of pixels qty
//
uint16_t LedStrip::rpmToPixelsQty(uint16_t qtyPixels, unsigned int rpm, unsigned int minRpm, unsigned int maxRpm)
{
  int segment = (maxRpm - minRpm) / (qtyPixels - 1);      //Determine the segment between two pixels (resolution)
  int count;

  if (rpm > minRpm)
    if ((rpm - minRpm) < segment)
      count = 1;
    else
      if (rpm > maxRpm) 
        count = qtyPixels;
      else 
        count = ((rpm - minRpm) / segment) + 1;
  else
    count = 0;

  return (uint16_t) count;
}

