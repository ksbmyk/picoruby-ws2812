/*
 * WS2812 stub for ESP32
 * ESP32 uses pure Ruby implementation with picoruby-rmt
 */

#include <mrubyc.h>

/*
 * Empty init function for ESP32
 * The WS2812 class is implemented in pure Ruby using RMT
 */
void
mrbc_ws2812_init(mrbc_vm *vm)
{
    /* No C bindings needed on ESP32 */
    (void)vm;
}
