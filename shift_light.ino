#include <Adafruit_NeoPixel.h>

#define RPM_PIN        2    //Tachometer signal
#define LEDSTRIP_PIN   9    //Digital output to led strip
#define NUMPIXELS     10    //qty of leds in strip
#define TUNE_PIN      A0    //Tuning potentiometer PIN

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

long microsPrev = 0;
double rpmCur = 0;
double rpmPrev = 0;
int spikesCounter = 0;

bool blinkState = false;
long blinkLastTime = 0;


void setup() { 
  Serial.begin(9600);

  digitalWrite(RPM_PIN, 1);
  attachInterrupt(0, getRpm, RISING);

  pixels.begin();
}


void loop() 
{
  int shiftRpm = getShiftThreshold();                           //Read potentiometer to define current shift threshhold
  Serial.println(rpmCur);
  showRpm(rpmCur, shiftRpm);
}

int getShiftThreshold()                                         //Function checks the potentiometer input and converts it to rev/minute (3000 - 9000)
{
  int sensorValue = analogRead(TUNE_PIN);
  sensorValue = constrain(sensorValue, 0, 1023);
  return map(sensorValue, 0, 1023, 3000, 9000);
}

void leds(int count)                                              //Function activates/deactivates XX number of leds in the strip
{
  int red = 150;
  int green = 150;
  int blue = 0;

  if (count > NUMPIXELS * 2 / 3) {                                 //Performance range mode
    red = 0;
    blue = 0;
    green = 150;
  }

  if (count == NUMPIXELS) {                                         //Shift-light mode
    red = 0;
    blue = 150;
    green = 0;
  }
  
  for (int i = 0; i < NUMPIXELS; i++)                             
  {
    if (i > count) 
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
  }
  pixels.show();
}

void showRpm(double rpm, int shiftRpm)                          //Activates indicator based on current rpm and shift limit
{
    if (rpm >= shiftRpm && (millis() - blinkLastTime) > 40)     //Shift light blinks every 40 milliseconds
    {
      if (!blinkState)
      {
        leds(NUMPIXELS);                                         //Start with all leds ON
      }
      else 
      {
        leds(0);
      }
      blinkLastTime = millis();
      blinkState = !blinkState;
    }
    else if (rpm < shiftRpm)                          //Indicate current engine speed with leds strip
    {    
      int count = (int)rpm / (shiftRpm / NUMPIXELS);   //Count how many leds to activate
      leds(count);
      blinkState = false;                             //Next time to start shift blinking with leds ON
    }
}

bool checkForSpike(double rpm1, double rpm2)          //Helper function to define spike (noise) between two rpm measurements
{
  const int spikeThreshold = 200;                     //200 rev/minute is defined as a spike
  
  return abs(rpm1 - rpm2) > spikeThreshold;
}

void getRpm()
{
  const int maxSpikes = 5;                   
  
  rpmCur = (1000000.0/(micros() - microsPrev))*60 / 2;
  
  //Catch spikes
  bool isSpike = checkForSpike(rpmCur, rpmPrev);
  if (isSpike) 
  {
    if (spikesCounter < maxSpikes)           //More than (qty = maxSpikes) in a row considered as good value
    {
      spikesCounter++;
      rpmCur = rpmPrev;                              //Take previous rpm value in case spike is detected
    }
    else
    {
      spikesCounter = 0;
    }
  }
  else
  {
    spikesCounter = 0;
  }
  
  microsPrev = micros();
  rpmPrev = rpmCur;
  //Serial.println(rpm);
  
}
