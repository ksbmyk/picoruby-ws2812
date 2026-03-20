/*
 * WS2812 mruby/c bindings
 */

#include <mrubyc.h>
#include "../include/ws2812.h"

/*
 * WS2812._convert(buffer, brightness, color_order)
 * Apply brightness scaling and color order conversion
 * Returns a new Array ready for transmission
 */
static void
c__convert(mrbc_vm *vm, mrbc_value *v, int argc)
{
    mrbc_value data = v[1];
    int brightness = GET_INT_ARG(2);
    int color_order = GET_INT_ARG(3);

    if (mrbc_type(data) != MRBC_TT_ARRAY) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "data must be an Array");
        return;
    }

    int len = mrbc_array_size(&data);
    int num_leds = len / 3;

    mrbc_value result = mrbc_array_new(vm, len);

    for (int i = 0; i < num_leds; i++) {
        uint8_t r = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3));
        uint8_t g = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3 + 1));
        uint8_t b = (uint8_t)mrbc_integer(mrbc_array_get(&data, i * 3 + 2));

        r = (r * brightness) / 100;
        g = (g * brightness) / 100;
        b = (b * brightness) / 100;

        if (color_order == WS2812_ORDER_RGB) {
            mrbc_array_push(&result, &mrbc_integer_value(r));
            mrbc_array_push(&result, &mrbc_integer_value(g));
            mrbc_array_push(&result, &mrbc_integer_value(b));
        } else {  /* GRB (default) */
            mrbc_array_push(&result, &mrbc_integer_value(g));
            mrbc_array_push(&result, &mrbc_integer_value(r));
            mrbc_array_push(&result, &mrbc_integer_value(b));
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
