# TLC5958
C++/Arduino driver for TI TLC5958x multiplexed LED driver

## TODO
- Optimize 48-bit LED color storage to be more memory-efficient (currently, colors are stored in 64 bits, wasting 16 bits per LED)
- Improve upon startup, potentially adding a `begin()` method that handles all init in a single step
- Add a `refresh()` method which sends grayscale data and a vsync command in one go
