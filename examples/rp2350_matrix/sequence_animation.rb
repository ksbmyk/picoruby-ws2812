require 'ws2812'

driver = PIODriver.new(25)
led = WS2812.new(driver)

class LEDMatrix
  GRID_SIZE = 8

  def initialize(led)
    @led = led
    @matrix = []
    GRID_SIZE.times do
      row = []
      GRID_SIZE.times do
        row << [0, 0, 0]
      end
      @matrix << row
    end
  end

  def set_pixel(x, y, color)
    return if x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE
    @matrix[y][x] = color.dup
  end

  def show
    pixels = []
    GRID_SIZE.times do |y|
      GRID_SIZE.times do |x|
        pixels << @matrix[y][x]
      end
    end
    @led.show_rgb(*pixels)
  end

  def clear
    GRID_SIZE.times do |y|
      GRID_SIZE.times do |x|
        @matrix[y][x] = [0, 0, 0]
      end
    end
    show
  end

  # Sequential lighting animation
  def sequence_animation(delay_ms: 100)
    loop do
      GRID_SIZE.times do |y|
        GRID_SIZE.times do |x|
          clear
          set_pixel(x, y, [255, 0, 0])
          show
          sleep_ms(delay_ms)
        end
      end
    end
  end
end

matrix = LEDMatrix.new(led)
matrix.sequence_animation(delay_ms: 50)
