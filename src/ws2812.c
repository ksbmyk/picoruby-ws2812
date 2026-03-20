/*
 * WS2812 mruby/c bindings
 */

#include <mrubyc.h>
#include "../include/ws2812.h"

/*
 * WS2812._convert(buffer, brightness, color_order, pack32)
 * Apply brightness scaling and color order conversion
 * pack32 == 0: returns byte Array [c1, c2, c3, ...] (for RMT)
 * pack32 != 0: returns 32-bit packed Array [0xC1C2C300, ...] (for PIO)
 */
static void
c__convert(mrbc_vm *vm, mrbc_value *v, int argc)
{
    mrbc_value data = v[1];
    int brightness = GET_INT_ARG(2);
    int color_order = GET_INT_ARG(3);
    int pack32 = (argc >= 4) ? GET_INT_ARG(4) : 0;

    if (mrbc_type(data) != MRBC_TT_ARRAY) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "data must be an Array");
        return;
    }

    int len = mrbc_array_size(&data);
    int num_leds = len / 3;

    mrbc_value result = mrbc_array_new(vm, pack32 ? num_leds : len);

    for (int i = 0; i < num_leds; i++) {
        uint8_t r = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3));
        uint8_t g = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3 + 1));
        uint8_t b = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3 + 2));

        r = (r * brightness) / 100;
        g = (g * brightness) / 100;
        b = (b * brightness) / 100;

        uint8_t c1, c2, c3;
        if (color_order == WS2812_ORDER_RGB) {
            c1 = r; c2 = g; c3 = b;
        } else {  /* GRB (default) */
            c1 = g; c2 = r; c3 = b;
        }

        if (pack32) {
            uint32_t pixel = ((uint32_t)c1 << 24) | ((uint32_t)c2 << 16) | ((uint32_t)c3 << 8);
            mrbc_array_push(&result, &mrbc_integer_value(pixel));
        } else {
            mrbc_array_push(&result, &mrbc_integer_value(c1));
            mrbc_array_push(&result, &mrbc_integer_value(c2));
            mrbc_array_push(&result, &mrbc_integer_value(c3));
        }
    }

    SET_RETURN(result);
}

/*
 * Initialize WS2812 class
 */
void
mrbc_ws2812_init(mrbc_vm *vm)
{
    mrbc_class *mrbc_class_WS2812 = mrbc_define_class(vm, "WS2812", mrbc_class_object);

    mrbc_define_method(vm, mrbc_class_WS2812, "_convert", c__convert);
}
