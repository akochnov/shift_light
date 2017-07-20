#include "ModeSwitch.h"

ModeSwitch::ModeSwitch(uint8_t m = 0) 
{
	mode = m;
}

uint8_t ModeSwitch::getMode()
{
	return mode;
}

void nextMode()
{
	mode++;
	if (3 > mode) {mode = 0;}
}
