require 'ws2812'

# RP2350-Matrix: 8x8 LED matrix on GPIO 25
led = WS2812.new(pin: 25, num: 64)

# basic color display (single LED)
led.set_rgb(0, 255, 0, 0)  # red
led.show
sleep 1

led.set_rgb(0, 0, 255, 0)  # green
led.show
sleep 1

led.set_rgb(0, 0, 0, 255)  # blue
led.show
sleep 1

led.fill(255, 255, 255)    # white (all LEDs)
led.show
sleep 1

led.clear
led.close
