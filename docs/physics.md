# Physics

Functions

- bool collide(Point point, Rect rect);
- bool collide(Rect rect, Rect rect2);

Constants

- LEFT_EDGE
- RIGHT_EDGE
- TOP_EDGE
- BOTTOM_EDGE

Rect constants are defined for all 4 screen edges to allow you to do easy collisions checks with the sides of the screen (ala, Breakout).  The edges are all offscreen, so an object won't collide unless it moves past the edge of the screen.


## Objects

### Rectangle

Usage

```
Rect rect = { .x = 0, .y = 0, .width = 10, .height = 10 };
```

### Point

Usage

```
Point point = { .x = 0, .y = 0 };
```


## Functions

### bool collide(Point point, Rect rect);

Detects collisions between two rectangles.  If any of the rectangles are touching then collide returns true.

Usage

```
Point point = { .x = 16, .y = 16 }
Rect rect = { .x = 15, .y = .15, .width = 10, .height = 10 }
if (physics.collide(point, rect)) {
    // code to handle collision
}
```

### bool collide(Rect rect, Rect rect2);

Detects collision between a point and a rectangle.  If the point is anywhere inside the rectangle this function returns true.

Usage

```
Point ball = { .x = 16, .y = 16, .width = 2, .height = 2 }
Rect paddle = { .x = 32, .y = .62, .width = 15, .height = 12 }
if (physics.collide(ball, paddle)) {
    // bounce the ball off the paddle
}

```
