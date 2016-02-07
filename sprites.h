#ifndef SPRITES_H
#define SPRITES_H

#include "Arduboy.h"

#define SPRITE_MASKED 1
#define SPRITE_UNMASKED 2
#define SPRITE_OVERWRITE 2
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

	// drawExternalMask() uses a separate mask to mask image (MASKED)
	//
	// image  mask   before  after
	//  
	// .....  .OOO.  .....   ..... 
	// ..O..  OOOOO  .....   ..O..
	// OO.OO  OO.OO  .....   OO.OO
	// ..O..  OOOOO  .....   ..O..
	// .....  .OOO.  .....   .....
	//  
	// image  mask   before  after
	//   
	// .....  .OOO.  OOOOO   O...O
	// ..O..  OOOOO  OOOOO   ..O..
	// OO.OO  OOOOO  OOOOO   OO.OO
	// ..O..  OOOOO  OOOOO   ..O..
	// .....  .OOO.  OOOOO   O...O
	//
	void drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap, 
	  const uint8_t *mask, uint8_t frame, uint8_t mask_frame); 

	// drawPlusMask has the same behavior as drawExternalMask except the
	// data is arranged in byte tuples interposing the mask right along
	// with the image data (SPRITE_PLUS_MASK)
	//
	// typical image data (8 bytes): 
	// [I][I][I][I][I][I][I][I]
	//
	// interposed image/mask data (8 byes):
	// [I][M][I][M][I][M][I][M]
	//
	// The byte order does not change, just for every image byte you mix
	// in it's matching mask byte.  Softare tools make easy work of this.
	// 
	// See: https://github.com/yyyc514/img2ard
	void drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

	// drawOverwrite() replaces the existing content completely (UNMASKED)
	//
	// image  before  after
	//
	// .....  .....   ..... 
	// ..O..  .....   ..O..
	// OO.OO  .....   OO.OO
	// ..O..  .....   ..O..
	// .....  .....   .....
	//
	// image  before  after
	//
	// .....  OOOOO   ..... 
	// ..O..  OOOOO   ..O..
	// OO.OO  OOOOO   OO.OO
	// ..O..  OOOOO   ..O..
	// .....  OOOOO   .....
	//
	void drawOverwrite(int16_t x, int16_t y, 
		const uint8_t *bitmap, uint8_t frame); 

	// drawErase() removes the lit pixels in the image from the display
	// (SPRITE_IS_MASK_ERASE)
	//
	// image  before  after
	//
	// .....  .....   ..... 
	// ..O..  .....   .....
	// OO.OO  .....   .....
	// ..O..  .....   .....
	// .....  .....   .....
	//
	// image  before  after
	//
	// .....  OOOOO   OOOOO 
	// ..O..  OOOOO   OO.OO
	// OO.OO  OOOOO   ..O..
	// ..O..  OOOOO   OO.OO
	// .....  OOOOO   OOOOO
	//

	void drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame); 

	// drawSelfMasked() only draws lit pixels, black pixels in
	// your image are treated as "transparent" (SPRITE_IS_MASK)
	//
	// image  before  after
	//
	// .....  .....   ..... 
	// ..O..  .....   ..O..
	// OO.OO  .....   OO.OO
	// ..O..  .....   ..O..
	// .....  .....   .....
	//
	// image  before  after
	//
	// .....  OOOOO   OOOOO  (no change because all pixels were
	// ..O..  OOOOO   OOOOO  already white)
	// OO.OO  OOOOO   OOOOO
	// ..O..  OOOOO   OOOOO
	// .....  OOOOO   OOOOO
	//
	void drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame); 

	// master function, needs to be abstracted into sep function for 
	// every render type
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
