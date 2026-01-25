require 'ws2812'

# RP2350-Matrix's LED is connected to GPIO 25
driver = PIODriver.new(25)
led = WS2812.new(driver)

# basic color display
led.show_rgb([255, 0, 0])     # red
sleep 1
led.show_rgb([0, 255, 0])     # green
sleep 1
led.show_rgb([0, 0, 255])     # blue
sleep 1
led.show_rgb([255, 255, 255]) # white

driver.close
