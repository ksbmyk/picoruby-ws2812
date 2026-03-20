#include <stdlib.h>
#include <stdbool.h>

#include "rmt.h"
#include "../../include/ws2812.h"

static bool initialized = false;

int
WS2812_init(uint8_t pin)
{
    if (initialized) {
        return 0;  /* Already initialized */
    }

    RMT_symbol_dulation_t timing = {
        .t0h_ns   = 350,
        .t0l_ns   = 800,
        .t1h_ns   = 700,
        .t1l_ns   = 600,
        .reset_ns = 60000
    };

    int ret = RMT_init((uint32_t)pin, &timing);
    if (ret == 0) {
        initialized = true;
    }
    return ret;
}

void
WS2812_show(const uint8_t *rgb_data, int num_leds, uint8_t brightness, uint8_t color_order)
{
    if (!initialized) return;

    int nbytes = num_leds * 3;
    uint8_t *buf = (uint8_t *)malloc(nbytes);
    if (!buf) return;

    for (int i = 0; i < num_leds; i++) {
        /* Apply brightness scaling with integer math */
        uint8_t r = (rgb_data[i * 3]     * brightness) / 100;
        uint8_t g = (rgb_data[i * 3 + 1] * brightness) / 100;
        uint8_t b = (rgb_data[i * 3 + 2] * brightness) / 100;

        if (color_order == WS2812_ORDER_RGB) {
            buf[i * 3]     = r;
            buf[i * 3 + 1] = g;
            buf[i * 3 + 2] = b;
        } else {  /* GRB (default) */
            buf[i * 3]     = g;
            buf[i * 3 + 1] = r;
            buf[i * 3 + 2] = b;
        }
    }

    RMT_write(buf, (uint32_t)nbytes);

    free(buf);
}

void
WS2812_deinit(void)
{
    initialized = false;
}
