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
byte frameCount = 0;
byte wizard_X = 51;
byte wizard_Y = 19;

unsigned long next_frame = 0;

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);
  arduboy.clearDisplay();
  arduboy.display();
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;
  arduboy.clearDisplay();
  drawWizard();
  arduboy.display();
}

void drawWizard()
{
  sprites.draw(wizard_X, wizard_Y, wizard_bitmaps, frameCount);
  if (millis() >= next_frame)
  {
    frameCount++;
    if (frameCount > 3) frameCount = 0;
     next_frame = millis() + PAUSE_BETWEEN_FRAMES;
  }
}
