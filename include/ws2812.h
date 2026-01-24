#ifndef WS2812_DEFINED_H_
#define WS2812_DEFINED_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize WS2812 driver on specified pin */
int WS2812_init(uint8_t pin);

/* Send a single pixel (GRB order) */
void WS2812_put_pixel(uint8_t g, uint8_t r, uint8_t b);

/* Send multiple pixels from array */
void WS2812_write(const uint8_t *data, int len);

/* Deinitialize WS2812 driver */
void WS2812_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* WS2812_DEFINED_H_ */
