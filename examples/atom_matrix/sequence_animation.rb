require 'ws2812'

rmt = RMTDriver.new(27)
led = WS2812.new(rmt)

class AtomMatrix
  def initialize(led)
    @led = led
    # Initialize 5x5 matrix (each element is [0,0,0])
    @matrix = []
    5.times do
      row = []
      5.times do
        row << [0, 0, 0]
      end
      @matrix << row
    end
  end

  def set_pixel(x, y, color)
    return if x < 0 || x >= 5 || y < 0 || y >= 5
    @matrix[y][x] = color.dup  # Duplicate color array
  end

  def show
    pixels = []
    5.times do |y|
      5.times do |x|
        pixels << @matrix[y][x]
      end
    end
    @led.show_rgb(*pixels)
  end

  def clear
    5.times do |y|
      5.times do |x|
        @matrix[y][x] = [0, 0, 0]
      end
    end
    show
  end

  # Sequential lighting animation
  def sequence_animation(delay_ms: 100)
    loop do
      5.times do |y|
        5.times do |x|
          clear
          set_pixel(x, y, [255, 0, 0])
          show
          sleep_ms(delay_ms)
        end
      end
    end
  end
end

matrix = AtomMatrix.new(led)
matrix.sequence_animation(delay_ms: 200)
