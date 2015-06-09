#ifndef SPRITES_H
#define SPRITES_H

#include "Arduboy.h"

#define SPRITE_MASKED 0
#define SPRITE_UNMASKED 1
#define SPRITE_IS_MASK 250
#define SPRITE_IS_MASK_ERASE 251

/// base struct other Sprites inherit from
struct SimpleSprite
{
	int x, y;
	uint8_t w, h;
	const uint8_t *bitmap;
	uint8_t frame = 0;
};

struct Sprite : public SimpleSprite
{
	const uint8_t *mask;
	uint8_t maskFrame = 0;
};

class Sprites
{
public:
	Sprites(Arduboy &arduboy);

	void draw(Sprite sprite);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame, const uint8_t *mask, uint8_t sprite_frame);

	void drawBitmap(int16_t x, int16_t y,
	  const uint8_t *bitmap, const uint8_t *mask,
	  int16_t w, int16_t h, uint8_t draw_mode);

private:

	Arduboy *arduboy;
	unsigned char *sBuffer;
};


#endif
