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

// example of using it in your actual sketch
void loop()
{
    buttons.poll();
    if (buttons.pressed(LEFT_BUTTON)) {
        // do cool stuff
    }
}
```
