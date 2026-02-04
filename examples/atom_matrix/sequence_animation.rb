require 'ws2812'

# Atom Matrix: 5x5 LED matrix on GPIO 27
GRID_SIZE = 5
led = WS2812.new(pin: 27, num_leds: GRID_SIZE * GRID_SIZE)

# Sequential lighting animation
def sequence_animation(led, grid_size, delay_ms: 100)
  loop do
    grid_size.times do |y|
      grid_size.times do |x|
        led.clear
        index = y * grid_size + x
        led.set_rgb(index, 255, 0, 0)
        led.show
        sleep_ms(delay_ms)
      end
    end
  end
end

sequence_animation(led, GRID_SIZE, delay_ms: 200)
