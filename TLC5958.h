#include "TLC5958_defs.h"
#include "Arduino.h"

#ifndef TLC5958_H
#define TLC5958_H

class TLC5958 {
public:
    TLC5958(uint8_t rows, uint8_t cols);
    ~TLC5958(void);

    void
    vsync(void),
    send_gs_data(void),
    configure_FC1(void),
    configure_FC2(void),
    send_gs_clk(void),

    fill(uint16_t r, uint16_t g, uint16_t b),
    set_led(uint8_t row, uint8_t col, uint16_t r, uint16_t g, uint16_t b),

    map_row_handler(void (*p)(uint8_t)),
    map_gpio_handler(void (*p)(uint8_t, uint8_t)),
    set_index_map(uint8_t *index_map),
    set_pins(uint8_t sclk, uint8_t sin, uint8_t gclk, uint8_t latch);

    uint64_t
    *get_buffer(void);

private:
    void
    _fc_write_en(void),
    _send_single_gsclk(void),
    _send(uint64_t data, uint8_t bits, uint8_t latch_clks),
    _print64hex(uint64_t to_print),
    (*_set_row)(uint8_t row) = NULL,
    (*_set_gpio)(uint8_t pin, uint8_t row) = NULL;

    inline void
    _delay_high(void),
    _delay_low(void),
    _gs_delay_high(void),
    _gs_delay_low(void);

    uint8_t
    _lat,
    _sclk,
    _sin,
    _gclk,
    _rows,
    _cols,
    *_index_map;

    uint64_t
    _build_gs_data(uint16_t r, uint16_t g, uint16_t b),
    *_buffer;

    bool _index_mapping = false;
};

#endif