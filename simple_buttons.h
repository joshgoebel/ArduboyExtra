#ifndef BUTTON_API_H
#define BUTTON_API_H

#include <Arduino.h>
#include "Arduboy.h"

class SimpleButtons
{
public:
	SimpleButtons(Arduboy &arduboy);

	void poll();
	boolean pressed(uint8_t buttons);
	boolean notPressed(uint8_t buttons);
	boolean justReleased(uint8_t button);
	boolean justPressed(uint8_t button);


private:
	uint8_t currentButtonState = 0;
	uint8_t previousButtonState = 0;

	Arduboy *arduboy;
};


#endif
