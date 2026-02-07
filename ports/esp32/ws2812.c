/*
 * WS2812 stub for ESP32
 * ESP32 uses pure Ruby implementation with picoruby-rmt
 *
 * These stub implementations satisfy the linker when src/ws2812.c is compiled.
 * The actual LED control is done via pure Ruby using the RMT peripheral.
 */

#include <stdint.h>

int WS2812_init(uint8_t pin)
{
    (void)pin;
    return 0;
}

void WS2812_write(const uint8_t *data, int len)
{
    (void)data;
    (void)len;
}

void WS2812_deinit(void)
{
}
