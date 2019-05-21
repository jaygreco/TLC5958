#include <stdlib.h>
#include "TLC5958.h"

#include "config.h"

TLC5958 leds = TLC5958(NUM_ROWS, NUM_COLS);

// Optional LED index map, for hardware that populates LED channels out of order.
uint8_t led_index_map[NUM_ROWS*NUM_COLS] = {
    5,  4,  3,  2,  1,  0,  15, 14, 13, 12, 11, 10, 9,  8,  7,  6,
    21, 20, 19, 18, 17, 16, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,
    42, 34, 41, 33, 40, 32, 47, 39, 46, 38, 45, 37, 44, 36, 43, 35,
    58, 50, 57, 49, 56, 48, 63, 55, 62, 54, 61, 53, 60, 52, 59, 51
};

// GPIO pin setup
void gpio_init(void) {
    pinMode(ROWDRV_A, OUTPUT);
    pinMode(ROWDRV_B, OUTPUT);
    pinMode(ROWDRV_C, OUTPUT);
    pinMode(ROWDRV_EN, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(GCLK, OUTPUT);
    pinMode(LAT, OUTPUT);
    pinMode(SIN, OUTPUT);

    digitalWriteFast(SCLK, LOW);
    digitalWriteFast(GCLK, LOW);
    digitalWriteFast(LAT, LOW);
    digitalWriteFast(SIN, LOW);
    digitalWriteFast(ROWDRV_A, LOW);
    digitalWriteFast(ROWDRV_B, LOW);
    digitalWriteFast(ROWDRV_C, LOW);
    digitalWriteFast(ROWDRV_EN, LOW);
}

// Example row scanning function.
// Takes a single integer argument which sets the currently active row.
const uint8_t row_index[] = {ROWDRV_A, ROWDRV_B, ROWDRV_C};
void set_row(uint8_t row) {
    digitalWriteFast(ROWDRV_EN, LOW);

    for (int i=0; i<3; i++) {
        digitalWriteFast(row_index[i], (row >> i) & 0x01);
    }
    delayMicroseconds(1);
    digitalWriteFast(ROWDRV_EN, HIGH);
}

// Simple demo LED animation modes
void demo(uint8_t mode) {
    uint16_t colors[] = {0xFFFF, 0x7FFF, 0x0};
    uint16_t r = colors[rand() % 3];
    uint16_t g = colors[rand() % 3];
    uint16_t b = colors[rand() % 3];
    if(mode == 1) {
        // Turn on a random LED to a random color
        uint8_t row = rand() % NUM_ROWS;
        uint8_t col = rand() % NUM_COLS;

        leds.set_led(row, col, r, g, b);
    }
    else if(mode == 2) {
        // Turn on all LEDs to a random color
        leds.fill(r, g, b);
    }
    leds.send_gs_data();
    leds.vsync();
}

void setup() {
    Serial.begin(9600);
    delay(2000);
    gpio_init();
    delay(1);

    // Set GPIO pins 
    leds.set_pins(SCLK, SIN, GCLK, LAT);
    // Set the GPIO handler
    leds.map_gpio_handler(digitalWriteFast);
    // Set the row handler
    leds.map_row_handler(set_row);
    // Optional: set an index map if channels are used out-of-order
    leds.set_index_map(led_index_map);
    // Optional: set configuration, if the default config is not being used
    leds.configure_FC1();
    leds.configure_FC2();
    // Send the initial LED color data and store in the GSbuffer
    leds.send_gs_data();
    leds.vsync();
}

unsigned long last = 0;
void loop() {
    if(millis() - last > 500) {
        demo(1);
        last = millis();
    }
    // Send the required GS clock pulses so the driver can update the LED PWMs
    leds.send_gs_clk();
}