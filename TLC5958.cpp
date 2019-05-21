#include "TLC5958.h"

// Public
TLC5958::TLC5958(uint8_t rows, uint8_t cols) {
    _rows = rows;
    _cols = cols;
    uint16_t bytes = _rows * _cols * 8; //4 bytes (64 bits) per LED.
    if((_buffer = (uint64_t *)malloc(bytes))) {
        memset(_buffer, 0, bytes);
    }
}

TLC5958::~TLC5958(void) {
    if(_buffer) free(_buffer);
}

void TLC5958::vsync(void) {
    //send 3 SCLK while LAT is high
    _send(DATA_NONE, VSYNC_BITS, VSYNC_LAT_CLKS);
}

void TLC5958::send_gs_data(void) {
    for (int i=0; i<_rows*_cols; i++) {
        if(_index_mapping) {
            _send(_buffer[_index_map[i]], GS_DATA_BITS, GS_LAT_CLKS);
        } else {
            _send(_buffer[i], GS_DATA_BITS, GS_LAT_CLKS);
        }
    }
}

void TLC5958::configure_FC1(void) {
    _fc_write_en();
    uint64_t FC1Config = 0;

    // Build FC1 config using bitmasking
    // Enable these as needed to override defaults.
    FC1Config |= FCx_ADDR(ADDR_FC1);
    FC1Config |= FC1_LOD_THRESH(LOD_THRESH_0V4);
    FC1Config |= FC1_TD0_SEL(TD0_SEL_25NS);
    FC1Config |= FC1_LOD_REMOVE(LOD_REMOVE_EN);
    FC1Config |= FC1_PRE_CHARGE(PRE_CHARGE_DIS);
    FC1Config |= FC1_PRE_CHG_MODE(PRE_CHG_MODE_DEFAULT);
    FC1Config |= FC1_UNIFORMITY(UNIF_ENCHANCE_EN);
    FC1Config |= FC1_CCB(180LL);
    FC1Config |= FC1_CCG(215LL);
    FC1Config |= FC1_CCR(511LL);
    FC1Config |= FC1_BC(0x2LL);

    _send(FC1Config, FCx_DATA_BITS, FCx_LAT_CLKS);
    //_print64hex(FC1Config);
}

void TLC5958::configure_FC2(void) {
    _fc_write_en();
    uint64_t FC2Config = 0x80000;

    // Build FC2 config using bitmasking.
    // Enable these as needed to override defaults.
    FC2Config |= FCx_ADDR(ADDR_FC2);
    FC2Config |= FC2_MAX_LINE(_rows);
    FC2Config |= FC2_PSAVE(PSAVE_DIS);
    #if GS_DUAL_EDGE
    FC2Config |= FC2_GCLK_EDGE(DUAL_EDGE);
    #else
    FC2Config |= FC2_GCLK_EDGE(RISING_EDGE);
    #endif
    FC2Config |= FC2_PRECHARGE(PRE_CHG_MODE_DEFAULT);
    FC2Config |= FC2_EMI_MODE_B(EMI_REDUCED);
    FC2Config |= FC2_EMI_MODE_G(EMI_REDUCED);
    FC2Config |= FC2_EMI_MODE_R(EMI_REDUCED);
    FC2Config |= FC2_SEL_PWM(PWM_MODE_8_8);
    FC2Config |= FC2_LINE1(ENCHANCE_LOW);
    FC2Config |= FC2_LGSE1_B(0x0000LL);
    FC2Config |= FC2_LGSE1_G(0x0000LL);
    FC2Config |= FC2_LGSE1_R(0x0000LL);
    FC2Config |= FC2_INF_B(ENCHANCE_NONE);
    FC2Config |= FC2_REV_V_B(REV_VOLT_HIGH);
    FC2Config |= FC2_INF_G(ENCHANCE_NONE);
    FC2Config |= FC2_REV_V_G(REV_VOLT_HIGH);
    FC2Config |= FC2_INF_R(ENCHANCE_NONE);
    FC2Config |= FC2_REV_V_R(REV_VOLT_HIGH);

    _send(FC2Config, FCx_DATA_BITS, FCx_LAT_CLKS);
    //_print64hex(FC2Config);
}

void TLC5958::send_gs_clk(void) { 
    for (int i=0; i<256; i++) {
        for (int j=0; j<_rows; j++) {
            _set_row(j);
            _send_single_gsclk();
        }
    }
}

void TLC5958::fill(uint16_t r, uint16_t g, uint16_t b) {
    uint64_t color = _build_gs_data(r, g, b);
    for(uint16_t i=0; i<_rows*_cols; i++) {
        if(_buffer) {
            _buffer[i] = color;
        }
    }
}

void TLC5958::set_led(uint8_t row, uint8_t col, uint16_t r, uint16_t g, uint16_t b) {
    uint64_t color = _build_gs_data(r, g, b);
    if(_buffer) {
        _buffer[(row * _cols) + col] = color;
    }
}

void TLC5958::map_row_handler(void (*p)(uint8_t)) {
    _set_row = p;
}

void TLC5958::map_gpio_handler(void (*p)(uint8_t, uint8_t)) {
    _set_gpio = p;
}

void TLC5958::set_index_map(uint8_t *index_map) {
    _index_map = index_map;
    _index_mapping = true;
} 

void TLC5958::set_pins(uint8_t sclk, uint8_t sin, uint8_t gclk, uint8_t latch) {
    _lat = latch;
    _sclk = sclk;
    _sin = sin;
    _gclk = gclk;
}

uint64_t* TLC5958::get_buffer(void) {
    return _buffer;
}

// Private

void TLC5958::_fc_write_en(void) {
    //send 15 SCLK while LAT is high
    _send(DATA_NONE, WRTEN_BITS, WRTEN_LAT_CLKS);
}

void TLC5958::_send_single_gsclk(void) {
    #if GS_DUAL_EDGE
    for (int i=0; i<129; i++) {
        _set_gpio(_gclk, LOW);
        _gs_delay_low();
        _set_gpio(_gclk, HIGH);
        _gs_delay_high();
    }
    _set_gpio(_gclk, LOW);
    #else
    for (int i=0; i<257; i++) {
        _set_gpio(_gclk, LOW);
        _gs_delay_low();
        _set_gpio(_gclk, HIGH);
        _gs_delay_high();
    }
    _set_gpio(_gclk, LOW);
    #endif
}

//Internal send function.
void TLC5958::_send(uint64_t data, uint8_t bits, uint8_t latch_clks) {
    _set_gpio(_lat, LOW);
    for(int i=(bits-1); i>=0; i--) {
        _set_gpio(_sclk, LOW);
        if (i < latch_clks) _set_gpio(_lat, HIGH);
        _set_gpio(_sin, (data >> i) & 0x01);
        _delay_low();
        _set_gpio(_sclk, HIGH);
        _delay_high();
    }
    _set_gpio(_sclk, LOW);
    _set_gpio(_sin, LOW);
    _set_gpio(_lat, LOW);
}

void TLC5958::_print64hex(uint64_t to_print) {
    uint32_t tmp1 = (to_print >> 32) & 0xFFFF;
    uint32_t tmp2 = (to_print);
    Serial.printf("0x%04X%08X\r\n", tmp1, tmp2);
}

// Tunable delays to allow for GPIO rise time
inline void TLC5958::_delay_high(void) {
    asm volatile(
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    );
}

inline void TLC5958::_delay_low(void) {
    asm volatile(
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    );
}

// Tunable delays to allow for GPIO rise time
inline void TLC5958::_gs_delay_high(void) {
    delayMicroseconds(1);
    asm volatile(
    "nop \n"
    );
}

inline void TLC5958::_gs_delay_low(void) {
    delayMicroseconds(1);
    asm volatile(
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    );
}

// For each RGB color: 0xFFFF is full on, 0x0000 is full off.
uint64_t TLC5958::_build_gs_data(uint16_t r, uint16_t g, uint16_t b) {
    uint64_t tmp = 0;
    tmp |= (uint64_t)r;
    tmp |= ((uint64_t)g << 16);
    tmp |= ((uint64_t)b << 32);
    return tmp;
}