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

// later on in your code
[snip]
if (buttons.pressed(LEFT_BUTTON)) {
    // do cool stuff
}
[snip]
```
