#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "../../include/ws2812.h"
#include "ws2812.pio.h"

#define WS2812_FREQ 800000

typedef struct {
    PIO pio;
    uint sm;
    uint offset;
    uint pin;
    bool initialized;
} ws2812_config_t;

static ws2812_config_t ws2812_config = { .initialized = false };

int
WS2812_init(uint8_t pin)
{
    if (ws2812_config.initialized) {
        return 0;  /* Already initialized */
    }

    /* Try pio0 first, then pio1 */
    ws2812_config.pio = pio0;
    if (!pio_can_add_program(ws2812_config.pio, &ws2812_program)) {
        ws2812_config.pio = pio1;
        if (!pio_can_add_program(ws2812_config.pio, &ws2812_program)) {
            return -1;  /* No PIO space available */
        }
    }

    ws2812_config.offset = pio_add_program(ws2812_config.pio, &ws2812_program);
    ws2812_config.sm = pio_claim_unused_sm(ws2812_config.pio, true);
    ws2812_config.pin = pin;

    ws2812_program_init(ws2812_config.pio, ws2812_config.sm,
                        ws2812_config.offset, pin, WS2812_FREQ, false);

    ws2812_config.initialized = true;
    return 0;
}

void
WS2812_put_pixel(uint8_t g, uint8_t r, uint8_t b)
{
    if (!ws2812_config.initialized) return;

    /* Send as GRB order (WS2812 expects GRB) */
    uint32_t pixel = ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
    ws2812_put_pixel(ws2812_config.pio, ws2812_config.sm, pixel);
}

void
WS2812_write(const uint8_t *data, int len)
{
    if (!ws2812_config.initialized) return;

    /* Data comes in GRB order, send as GRB (WS2812 expects GRB) */
    for (int i = 0; i + 2 < len; i += 3) {
        uint32_t pixel = ((uint32_t)data[i] << 16) |      /* G */
                         ((uint32_t)data[i + 1] << 8) |   /* R */
                         (uint32_t)data[i + 2];           /* B */
        ws2812_put_pixel(ws2812_config.pio, ws2812_config.sm, pixel);
    }

    /* Wait for FIFO to drain and add reset time (>50us) */
    while (!pio_sm_is_tx_fifo_empty(ws2812_config.pio, ws2812_config.sm)) {
        tight_loop_contents();
    }
    sleep_us(60);
}

void
WS2812_deinit(void)
{
    if (!ws2812_config.initialized) return;

    pio_sm_set_enabled(ws2812_config.pio, ws2812_config.sm, false);
    pio_remove_program(ws2812_config.pio, &ws2812_program, ws2812_config.offset);
    pio_sm_unclaim(ws2812_config.pio, ws2812_config.sm);

    ws2812_config.initialized = false;
}
