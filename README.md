ArduboyExtra
============



## Usage

```c++
#include <ArduboyExtra.h>
// include the extra modules you intent to use
#include "simple_buttons.h"

// create your arduboy object from the core lib
Arduboy arduboy;
// create a variable to hold buttons API
SimpleButtons buttons (arduboy);

// define our own nextFrame so we can inject other libraries that may
// need to poll or do things per frame
boolean nextFrame()
{
    if (arduboy.nextFrame()) {
        buttons.poll();
        return true;
    }
    return false;
}

// example of using it in your actual sketch
void loop()
{
    if (!nextFrame())
        return;
        
    if (buttons.pressed(LEFT_BUTTON)) {
        // do cool stuff
    }
}
```
