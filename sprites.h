#ifndef SPRITES_H
#define SPRITES_H

#include "Arduboy.h"

#define SPRITE_MASKED 1
#define SPRITE_UNMASKED 2
// PLUS_MASK means that the mask data is included in the bitmap itself
// 1st byte image, 2nd byte mask, 3rd byte image, 4th byte mask, etc.
// The 2nd byte ask provides the masking data for the 1st byte of
// image data.
#define SPRITE_PLUS_MASK 3
#define SPRITE_IS_MASK 250
#define SPRITE_IS_MASK_ERASE 251
// will select SPRITE_MASKED or SPRITE_UNMASKED depending on the presence
// of mask (if one was passed to the draw function)
#define SPRITE_AUTO_MODE 255

/// base struct other Sprites inherit from
struct SimpleSprite
{
	SimpleSprite(int x, int y, const uint8_t *bitmap);
	int x, y;
	const uint8_t *bitmap;
	uint8_t frame = 0;
	uint8_t drawMode = SPRITE_AUTO_MODE;
};

struct Sprite : public SimpleSprite
{
	Sprite(int x, int y, const uint8_t *bitmap);
	Sprite(int x, int y, const uint8_t *bitmap, const uint8_t *mask);
	const uint8_t *mask;
	uint8_t maskFrame = 0;
};

class Sprites
{
public:
	Sprites(Arduboy &arduboy);

	void draw(Sprite sprite);
	void draw(SimpleSprite sprite);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);
	void draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame,
		const uint8_t *mask, uint8_t sprite_frame, uint8_t drawMode);

	void drawBitmap(int16_t x, int16_t y,
	  const uint8_t *bitmap, const uint8_t *mask,
	  int8_t w, int8_t h, uint8_t draw_mode);

private:

	Arduboy *arduboy;
	unsigned char *sBuffer;
};


#endif
