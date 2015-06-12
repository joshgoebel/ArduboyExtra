/*
  Simple Animation example of a sprite
  Written By Joeri http://www.JO3RI.be , 12 June 2015
*/

#include <SPI.h>
#include <EEPROM.h>
#include <Arduboy.h>
#include <ArduboyExtra.h>
#include <sprites.h>
#include "bitmaps.h"

#define PAUSE_BETWEEN_FRAMES 180

Arduboy arduboy;
Sprites sprites (arduboy);
Sprite wizard (51, 19, wizard_bitmaps);

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clearDisplay();
  // animate every 180ms or so
  if (arduboy.everyXFrames(11)) {
    wizard.frame++;
    if (wizard.frame > 3)
      wizard.frame = 0;
  }
  drawWizard();
  arduboy.display();
}

void drawWizard()
{
  sprites.draw(wizard);
}
