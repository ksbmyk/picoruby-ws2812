/*
 * WS2812 driver for ESP32 (wraps picoruby-rmt)
 */

#include "rmt.h"
#include "../include/ws2812.h"

static RMT_symbol_dulation_t ws2812_timing = {
    .t0h_ns = 350,
    .t0l_ns = 800,
    .t1h_ns = 700,
    .t1l_ns = 600,
    .reset_ns = 60000
};

int WS2812_init(uint8_t pin) {
    return RMT_init((uint32_t)pin, &ws2812_timing);
}

void WS2812_write(const uint8_t *data, int len) {
    RMT_write((uint8_t *)data, (uint32_t)len);
}

void WS2812_deinit(void) {
    /* picoruby-rmt does not have deinit function yet */
}
