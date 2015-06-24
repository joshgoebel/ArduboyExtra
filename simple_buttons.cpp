#include "simple_buttons.h"

// store current and previous buttons state for frame based button events
// you should be using nextFrame() in almost all cases, not calling this
// directly
SimpleButtons::SimpleButtons(Arduboy &a)
{
  arduboy = &a;
}

void SimpleButtons::poll()
{
  previousButtonState = currentButtonState;
  currentButtonState = arduboy->getInput();
}

// returns true if the button mask passed in is pressed
//
//   if (pressed(LEFT_BUTTON + A_BUTTON))
boolean SimpleButtons::pressed(uint8_t buttons)
{
 return (currentButtonState & buttons) == buttons;
}

// returns true if a button that has been held down was just released
// this function only reliably works with a single button. You should not
// pass it multiple buttons as you can with some of the other button
// functions.
//
// This can be used for confirmations or other times when you want to take
// an action AFTER the user finishes the pressing rather than immediately
// when the button goes down.  Not that there is any good way for someone
// to change their mind, but the experience can feel very different.
boolean SimpleButtons::justReleased(uint8_t button)
{
 return ((previousButtonState & button) && !(currentButtonState & button));
}

// returns true if a button has just been pressed
// if the button has been held down for multiple frames this will return
// false.  You should only use this to poll a single button.
boolean SimpleButtons::justPressed(uint8_t button)
{
 return (!(previousButtonState & button) && (currentButtonState & button));
}

// returns true if the button mask passed in not pressed
//
//   if (not_pressed(LEFT_BUTTON))
boolean SimpleButtons::notPressed(uint8_t buttons)
{
  return (currentButtonState & buttons) == 0;
}

// returns true if up was just pressed
boolean SimpleButtons::up() {
	return justPressed(UP_BUTTON);
}

boolean SimpleButtons::down() {
	return justPressed(DOWN_BUTTON);
}

boolean SimpleButtons::right() {
	return justPressed(RIGHT_BUTTON);
}

boolean SimpleButtons::left() {
	return justPressed(LEFT_BUTTON);
}

boolean SimpleButtons::a() {
	return justPressed(A_BUTTON);
}

boolean SimpleButtons::b() {
	return justPressed(B_BUTTON);
}
