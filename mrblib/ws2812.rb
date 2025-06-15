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
end
