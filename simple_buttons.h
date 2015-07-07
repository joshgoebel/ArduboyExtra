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

	// returns true if button is current depressed
	boolean up();
	boolean down();
	boolean right();
	boolean left();
	boolean a();
	boolean b();

	// returns true if button is was just pressed (previously unpressed)
	boolean pressedUp();
	boolean pressedDown();
	boolean pressedRight();
	boolean pressedLeft();
	boolean pressedA();
	boolean pressedB();


private:
	uint8_t currentButtonState = 0;
	uint8_t previousButtonState = 0;

	Arduboy *arduboy;
};


#endif
