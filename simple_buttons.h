#ifndef BUTTON_API_H
#define BUTTON_API_H

#include <Arduino.h>
#include "Arduboy.h"

#define SOFT_RESET

class SimpleButtons
{
public:
	SimpleButtons(Arduboy &arduboy);

	/// Poll the hardware buttons and tracks state over time
	/**
	This must be called before any of the other button member functions.  It should be called either in your main `loop()` or as part of the frame system (called pre-frame).
	*/
	void poll();
	boolean pressed(uint8_t buttons);
	boolean notPressed(uint8_t buttons);
	boolean justReleased(uint8_t button);
	boolean justPressed(uint8_t button);

	boolean up();
	boolean down();
	boolean right();
	boolean left();
	boolean a();
	boolean b();


private:
	uint8_t currentButtonState = 0;
	uint8_t previousButtonState = 0;

	Arduboy *arduboy;
};


#endif
