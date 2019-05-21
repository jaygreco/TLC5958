# TLC5958
C++/Arduino driver for [TI TLC5958x multiplexed LED driver](https://www.ti.com/product/TLC5958)
Designed to be Arduino-compatible, but is written to be platform agnostic. All required hardware I/O and timing functions are impmlemented generically, or are mapped with function pointers.
Includes support for index mapping, so hardware that uses channels out-of-order can be remapped in software with very low overhead at the library level.
Supports TLC5958, TLC59581, and TLC59582 out of the box. Tested with a Teensy 3.6 running at 240MHz overclock, driving 64 LEDS in a 16x4 mulitplexed arrangement.

## How to use
Include the library in your Arduino (or generic C sketch) and instantiate a TLC5958 object
```c++
#include "TLC5958.h"
TLC5958 leds = TLC5958(NUM_ROWS, NUM_COLS);
```

Call the required setup functions, map to GPIO I/O, and send configuration data (if using a non-default config, such as multiplexed rows)
```c++
leds.set_pins(SCLK, SIN, GCLK, LAT);
leds.map_gpio_handler(digitalWriteFast);
leds.map_row_handler(set_row);
leds.set_index_map(led_index_map);
leds.configure_FC1();
leds.configure_FC2();
leds.send_gs_data();
leds.vsync();
```
Periodically (in the main loop, a timer-based interrupt, or otherwise) send the GSCLK signal. This is required for the chip to update the LED PWM/brightness values. The more frequently this is sent, the faster the refresh rate of the LEDs.
```c++
leds.send_gs_clk();
```
LEDs can be updated individually using `set_led(row, col, r, g, b)` or all at once using `fill(r, g, b)`. All color values are 16 bit unsigned integers.

After updating the buffer with new colors or brightnesses, the data must be sent to the chip. This is done by sending the GS data, followed by a vsync pulse.
```c++
leds.send_gs_data();
leds.vsync();
```

## TODO
- Error and bounds checks, especially on in-memory structures (like the LED buffer)
- Optimize 48-bit LED color storage to be more memory-efficient (currently, colors are stored in 64 bits, wasting 16 bits per LED)
- Improved startup, potentially adding a `begin()` method that handles all init in a single step
- Add a `refresh()` method which sends grayscale data and a vsync command in one go
- More (read: cooler) demo LED modes in example sketch
