#include "physics.h"

bool Physics::collide(Point point, Rect rect)
{
  // does point fall within the bounds of rect
  return ((point.x >= rect.x) && (point.x < rect.x + rect.width) &&
      (point.y >= rect.y) && (point.y < rect.y + rect.height));
}

bool Physics::collide(Rect rect1, Rect rect2)
{
  return !( rect2.x                 >=  rect1.x+rect1.width    ||
            rect2.x + rect2.width   <=  rect1.x                ||
            rect2.y                 >=  rect1.y + rect1.height ||
            rect2.y + rect2.height  <=  rect1.y);
}

// boolean Gamebuino::collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h){
//     if((x1>=x2)&&(x1<x2+w))
//     if((y1>=y2)&&(y1<y2+h))
//     return true;
//     return false;
// }

// boolean Gamebuino::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2){
//   return !( x2     >=  x1+w1  ||
//             x2+w2  <=  x1     ||
//             y2     >=  y1+h1  ||
//             y2+h2  <=  y1     );
// }
