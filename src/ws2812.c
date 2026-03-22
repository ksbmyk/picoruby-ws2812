/*
 * WS2812 mruby/c bindings
 */

#include <mrubyc.h>
#include "../include/ws2812.h"

#define GETIV(str) mrbc_instance_getiv(&v[0], mrbc_str_to_symid(#str))

/*
 * WS2812._convert
 * Apply brightness scaling and color order conversion using instance variables
 * @pixel_packed == 0: returns byte Array [c1, c2, c3, ...] (for RMT)
 * @pixel_packed != 0: returns 32-bit packed Array [0xC1C2C300, ...] (for PIO)
 */
static void
c__convert(mrbc_vm *vm, mrbc_value *v, int argc)
{
    mrbc_value data = GETIV(buffer);
    int brightness = mrbc_integer(GETIV(brightness));
    mrbc_sym rgb_sym = mrbc_str_to_symid("rgb");
    int color_order = (GETIV(order).sym_id == rgb_sym) ? WS2812_ORDER_RGB : WS2812_ORDER_GRB;
    int pack32 = mrbc_integer(GETIV(pixel_packed));

    if (mrbc_type(data) != MRBC_TT_ARRAY) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "data must be an Array");
        return;
    }

    int num_leds = mrbc_integer(GETIV(num));

    mrbc_value result = mrbc_array_new(vm, pack32 ? num_leds : num_leds * 3);

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
