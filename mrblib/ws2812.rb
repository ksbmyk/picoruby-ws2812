require 'rmt'

class RMTDriver
  def initialize(pin, t0h_ns: 350, t0l_ns: 800, t1h_ns: 700, t1l_ns: 600, reset_ns: 60000)
    @rmt = RMT.new(
      pin,
      t0h_ns: t0h_ns,
      t0l_ns: t0l_ns,
      t1h_ns: t1h_ns,
      t1l_ns: t1l_ns,
      reset_ns: reset_ns
    )
  end

  def write(bytes)
    @rmt.write(bytes)
  end
end

class WS2812
  def initialize(driver)
    @driver = driver
  end

  # ex. show([255, 0, 0], [0, 255, 0], [0, 0, 255]) # Array of RGB values
  def show_rgb(*colors)
    bytes = []
    colors.each do |color|
      r, g, b = color
      bytes << g << r << b
    end

    @driver.write(bytes)
  end

  # ex. show(0xff0000, 0x00ff00, 0x0000ff)  # Hexadecimal RGB values
  def show_hex(*colors)
    bytes = []
    colors.each do |color|
      r, g, b = [(color>>16)&0xFF, (color>>8)&0xFF, color&0xFF]
      bytes << g << r << b
    end

    @driver.write(bytes)
  end

  # ex. show_hsb([0, 100, 100], [120, 100, 100], [240, 100, 100])
  # H: 0-360, S: 0-100, B: 0-100
  def show_hsb(*colors)
    bytes = []
    colors.each do |color|
      h, s, b = color
      r, g, b_rgb = hsb_to_rgb(h, s, b)
      bytes << g << r << b_rgb
    end

    @driver.write(bytes)
  end

  private

  def hsb_to_rgb(h, s, b)
    # Normalize values
    h = h % 360
    s = s / 100.0
    b = b / 100.0

    c = b * s
    x = c * (1 - ((h / 60.0) % 2 - 1).abs)
    m = b - c

    r_prime, g_prime, b_prime = case h
    when 0...60
      [c, x, 0]
    when 60...120
      [x, c, 0]
    when 120...180
      [0, c, x]
    when 180...240
      [0, x, c]
    when 240...300
      [x, 0, c]
    else
      [c, 0, x]
    end

    r = ((r_prime + m) * 255).round
    g = ((g_prime + m) * 255).round
    b = ((b_prime + m) * 255).round

    [r, g, b]
  end
end
