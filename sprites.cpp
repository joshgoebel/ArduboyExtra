#include "sprites.h"

Sprites::Sprites(Arduboy &a)
{
  arduboy = &a;
  sBuffer = arduboy->getBuffer();
}

SimpleSprite::SimpleSprite(int x, int y, const uint8_t *bitmap) :
x(x), y(y), bitmap(bitmap)
{
}

Sprite::Sprite(int x, int y, const uint8_t *bitmap) :
  SimpleSprite(x,y, bitmap) {
}

Sprite::Sprite(int x, int y, const uint8_t *bitmap, const uint8_t *mask) :
  SimpleSprite(x,y, bitmap), mask(mask) {
}


void Sprites::draw(Sprite sprite)
{
  draw(sprite.x, sprite.y,
    sprite.bitmap, sprite.frame,
    sprite.mask, sprite.maskFrame);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap)
{
  draw(x, y, bitmap, 0, NULL, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask)
{
  draw(x, y, bitmap, 0, mask, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
  draw(x, y, bitmap, frame, NULL, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame, const uint8_t *mask, uint8_t sprite_frame)
{
  uint8_t width = pgm_read_byte(bitmap);
  uint8_t height = pgm_read_byte(++bitmap);
  unsigned int frame_offset;
  bitmap++;
  if (frame > 0 || sprite_frame > 0) {
    frame_offset = (width * ( height/8 + ( height%8 == 0 ? 0 : 1)));
    bitmap += frame * frame_offset;
    if (mask!=0)
      mask += sprite_frame * frame_offset;
  }

  drawBitmap(x, y, bitmap, mask, width, height, mask==0 ? SPRITE_UNMASKED : SPRITE_MASKED);
}

// should we still even support these modes?
// void Sprites::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {
  // drawComplexBitmap(x, y, bitmap, NULL, w, h, color ? SPRITE_IS_MASK : SPRITE_IS_MASK_ERASE);
// }

void Sprites::drawBitmap(int16_t x, int16_t y,
  const uint8_t *bitmap, const uint8_t *mask,
  int8_t w, int8_t h, uint8_t draw_mode) {
  // no need to draw at all of we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  // xOffset technically doesn't need to be 16 bit but the math operations
  // are measurably faster if it is
  int xOffset, ofs;
  int8_t yOffset = abs(y) % 8;
  int8_t sRow = y / 8;
  uint8_t loop_h, start_h, rendered_width;

  if (y < 0 && yOffset > 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  // if the right side of the render is offscreen skip those loops
  if (x+w > WIDTH-1) {
    rendered_width = ((WIDTH-x) - xOffset);
  } else {
    rendered_width = (w - xOffset);
  }

  // if the left side of the render is offscreen skip those loops
  if (x<0) {
    xOffset = abs(x);
  } else {
    xOffset = 0;
  }

  // if the top side of the render is offscreen skip those loops
  if (sRow < -1) {
    start_h = abs(sRow)-1;
  } else {
    start_h = 0;
  }

  loop_h = h/8 + (h%8 > 0 ? 1 : 0); // divide, then round up

  // if (sRow + loop_h - 1 > (HEIGHT/8)-1)
  if (sRow + loop_h > (HEIGHT/8)) {
    loop_h = (HEIGHT/8) - sRow;
  }

  // prepare variables for loops later so we can compare with 0
  // instead of comparing two variables
  loop_h -= start_h;

  sRow += start_h;
  ofs = (sRow*WIDTH) + x + xOffset;
  uint8_t *bofs = (uint8_t *)bitmap+(start_h*w) + xOffset;
  uint8_t *mask_ofs;
  if (mask!=0)
    mask_ofs = (uint8_t *)mask+(start_h*w) + xOffset;
  uint8_t data;

  uint8_t mul_amt = 1 << yOffset;
  uint16_t mask_data;
  uint16_t bitmap_data;

  switch (draw_mode) {
    case SPRITE_UNMASKED:
    // we only want to mask the 8 bits of our own sprite, so we can
    // calculate the mask before the start of the loop
    mask_data = ~(0xFF * mul_amt);
    // really if yOffset = 0 you have a faster case here that could be
    // optimized
      for (uint8_t a = 0; a < loop_h; a++) {
        for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
          bitmap_data = pgm_read_byte(bofs) * mul_amt;

          if (sRow >= 0) {
            data = sBuffer[ofs];
            data &= (uint8_t)(mask_data);
            data |= (uint8_t)(bitmap_data);
            sBuffer[ofs] = data;
          }
          if (yOffset != 0 && sRow < 7) {
            data = sBuffer[ofs+WIDTH];
            data &= (*((unsigned char *) (&mask_data) + 1));
            data |= (*((unsigned char *) (&bitmap_data) + 1));
            sBuffer[ofs+WIDTH] = data;
          }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;

    case SPRITE_IS_MASK:
      for (uint8_t a = 0; a < loop_h; a++) {
        for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
          bitmap_data = pgm_read_byte(bofs) * mul_amt;
            if (sRow >= 0) {
                sBuffer[ofs] |= (uint8_t)(bitmap_data);
            }
            if (yOffset != 0 && sRow<7) {
                sBuffer[ofs+WIDTH] |= (*((unsigned char *) (&bitmap_data) + 1));
            }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;

    case SPRITE_IS_MASK_ERASE:
      for (uint8_t a = 0; a < loop_h; a++) {
        for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
          bitmap_data = pgm_read_byte(bofs) * mul_amt;
            if (sRow >= 0) {
              sBuffer[ofs]  &= ~(uint8_t)(bitmap_data);
            }
            if (yOffset != 0 && sRow<7) {
              sBuffer[ofs+WIDTH] &= ~(*((unsigned char *) (&bitmap_data) + 1));
            }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;

    case SPRITE_MASKED:
    // asm volatile("nop\n":: "y" (sBuffer) :);
    uint8_t xi = loop_h; // used for x loop below
    uint8_t yi = rendered_width; // used for x loop below
    asm volatile(
      "push r28\n" // save Y
      "push r29\n"
      "mov r28, %A[buffer_page2_ofs]\n" // Y = buffer page 2 offset
      "mov r29, %B[buffer_page2_ofs]\n"
      "loop_y:\n"

      "loop_x:\n"
        // load bitmap and mask data
        "lpm %A[bitmap_data], Z+\n"

        // self mask for now
        // "lpm %A[mask_data], Z+\n"
        "mov %A[mask_data], %A[bitmap_data]\n"

        // shift mask and buffer data
        "tst %[yOffset]\n"
        "breq skip_shifting\n"
          "mul %A[bitmap_data], %[mul_amt]\n"
          "mov %A[bitmap_data], r0\n"
          "mov %B[bitmap_data], r1\n"
          "mul %A[mask_data], %[mul_amt]\n"
          "mov %A[mask_data], r0\n"
          "mov %B[mask_data], r1\n"
          "clr __zero_reg__\n"
          "com %B[mask_data]\n" // invert high byte of mask
        "skip_shifting:\n"
          "com %A[mask_data]\n"


        // FIRST PAGE
        // if sRow >= 0
        "tst %[sRow]\n"
        "brmi end_first_page\n"
          // then
          "ld %[data], %a[buffer_ofs]\n"
          "and %[data], %A[mask_data]\n"
          "or %[data], %A[bitmap_data]\n"
          // update buffer, increment
          "st %a[buffer_ofs], %[data]\n"

        // increment X counter
        "end_first_page:\n"
          "adiw r26, 1\n"

        // SECOND PAGE
        // if yOffset != 0 && sRow < 7
        "tst %[yOffset]\n"
        "breq end_second_page\n"
        "cpi %[sRow], 7\n"
        "brge end_second_page\n"
        // then
          "ld %[data], Y\n"
          "and %[data], %B[mask_data]\n"
          "or %[data], %B[bitmap_data]\n"
          // update buffer, increment
          "st Y, %[data]\n"

        // increment Y counter
        "end_second_page:\n"
          "adiw r28, 1\n"

        // "x_loop_next:\n"
        "dec %[xi]\n"
        "breq next_loop_y\n"
        "rjmp loop_x\n"

      // increment y
      "next_loop_y:\n"
      "dec %[yi]\n"
      "breq finished\n"
      "mov %[xi], %[x_count]\n" // reset x counter
      // sRow++;
      "inc %[sRow]\n"
      // sprite_ofs += w - rendered_width;
      "add %A[sprite_ofs], %[w]\n"
      "adc %B[sprite_ofs], __zero_reg__\n"
      "sub %A[sprite_ofs], %[y_count]\n"
      "sbc %B[sprite_ofs], __zero_reg__\n"
      // buffer_ofs += WIDTH - rendered_width;
      "ldi r16, %[width]\n"
      "add %A[buffer_ofs], r16\n"
      "adc %B[buffer_ofs], __zero_reg__\n"
      "sub %A[buffer_ofs], %[y_count]\n"
      "sbc %B[buffer_ofs], __zero_reg__\n"
      // buffer_ofs_page_2 += WIDTH - rendered_width;
      "add r28, r16\n"
      "adc r29, __zero_reg__\n"
      "sub r28, %[y_count]\n"
      "sbc r29, __zero_reg__\n"


      "rjmp loop_y\n"
      "finished:\n"
      // put the Y register back in place
      "pop r29\n"
      "pop r28\n"
      "clr __zero_reg__\n" // just in case
      :
      : [data] "r" (data),
        [xi] "r" (xi),
        [yi] "r" (yi),
        [x_count] "r" (loop_h),
        [y_count] "r" (rendered_width),
        [mask_data] "r" (mask_data),
        [bitmap_data] "r" (bitmap_data),
        [sprite_ofs] "z" (bofs),
        [buffer_ofs] "x" (sBuffer+ofs),
        [buffer_page2_ofs] "r" (sBuffer+ofs+WIDTH), // Y pointer
        [yOffset] "r" (yOffset),
        [mul_amt] "r" (mul_amt),
        [sRow] "a" (sRow), // CPI requires an upper register
        [w] "r" (w),
        [width] "M" (WIDTH)

      : "r16"
    );
    return;

    for (uint8_t a = 0; a < loop_h; a++) {
      for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
        // NOTE: you might think in the yOffset==0 case that this result
        // in more effort, but in all my testing the compiler was forcing
        // 16-bit math to happen here anyways, so this isn't actually
        // compiling to more code than it otherwise would. If the offset
        // is 0 the high part of the word will just never be used.

        // load data and bit shift
        // mask needs to be bit flipped
        mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);
        bitmap_data = pgm_read_byte(bofs) * mul_amt;

        if (sRow >= 0) {
          data = sBuffer[ofs];
          data &= (uint8_t)(mask_data);
          data |= (uint8_t)(bitmap_data);
          sBuffer[ofs] = data;
        }
        if (yOffset != 0 && sRow < 7) {
          data = sBuffer[ofs+WIDTH];
          data &= (*((unsigned char *) (&mask_data) + 1));
          data |= (*((unsigned char *) (&bitmap_data) + 1));
          sBuffer[ofs+WIDTH] = data;
        }
        ofs++;
        mask_ofs++;
        bofs++;
      }
      sRow++;
      bofs += w - rendered_width;
      mask_ofs += w - rendered_width;
      ofs += WIDTH - rendered_width;
    }
    break;


  }
}
