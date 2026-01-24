/*
 * WS2812 mruby/c bindings for RP2040/RP2350 (PIO driver)
 */

#include <mrubyc.h>
#include "../include/ws2812.h"

/*
 * WS2812._init(pin)
 * Initialize WS2812 PIO driver on specified GPIO pin
 */
static void
c__init(mrbc_vm *vm, mrbc_value *v, int argc)
{
    int pin = GET_INT_ARG(1);
    int result = WS2812_init((uint8_t)pin);

    if (result < 0) {
        mrbc_raise(vm, MRBC_CLASS(RuntimeError), "Failed to initialize WS2812");
        return;
    }

    SET_NIL_RETURN();
}

/*
 * WS2812._write(data)
 * Send multiple pixels from array (GRB order)
 */
static void
c__write(mrbc_vm *vm, mrbc_value *v, int argc)
{
    mrbc_value data = v[1];
    if (mrbc_type(data) != MRBC_TT_ARRAY) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "data must be an Array");
        return;
    }

    int len = mrbc_array_size(&data);
    uint8_t *buf = mrbc_alloc(vm, len);
    if (!buf) {
        mrbc_raise(vm, MRBC_CLASS(RuntimeError), "memory allocation failed");
        return;
    }

    for (int i = 0; i < len; i++) {
        buf[i] = (uint8_t)mrbc_integer(mrbc_array_get(&data, i));
    }

    WS2812_write(buf, len);

    mrbc_free(vm, buf);

    SET_NIL_RETURN();
}

/*
 * WS2812._deinit
 * Deinitialize WS2812 driver
 */
static void
c__deinit(mrbc_vm *vm, mrbc_value *v, int argc)
{
    WS2812_deinit();
    SET_NIL_RETURN();
}

/*
 * Initialize WS2812 class
 */
void
mrbc_ws2812_init(mrbc_vm *vm)
{
    mrbc_class *mrbc_class_WS2812 = mrbc_define_class(vm, "WS2812", mrbc_class_object);

    mrbc_define_method(vm, mrbc_class_WS2812, "init", c__init);
    mrbc_define_method(vm, mrbc_class_WS2812, "write", c__write);
    mrbc_define_method(vm, mrbc_class_WS2812, "deinit", c__deinit);
}
