# Try to load RMT for ESP32 (ignore if not available on RP2040/RP2350)
begin
  require 'rmt'
rescue LoadError
end

class WS2812
  attr_reader :brightness

  def initialize(pin:, num:)
    @num_leds = num
    @brightness = 100
    @buffer = Array.new(num * 3, 0)

    # ESP32: use RMT directly, RP2040/RP2350: use C bindings
    begin
      @rmt = RMT.new(pin,
        t0h_ns: 350,
        t0l_ns: 800,
        t1h_ns: 700,
        t1l_ns: 600,
        reset_ns: 60000
      )
    rescue NameError
      # RMT not available, use C bindings (RP2040/RP2350)
      _init(pin)
    end
  end

  def set_rgb(index, r, g, b)
    return if index < 0 || index >= @num_leds
    @buffer[index * 3]     = r & 0xFF
    @buffer[index * 3 + 1] = g & 0xFF
    @buffer[index * 3 + 2] = b & 0xFF
  end

  def set_hsb(index, h, s, b)
    r, g, b_rgb = hsb_to_rgb(h, s, b)
    set_rgb(index, r, g, b_rgb)
  end

  def set_hex(index, hex)
    r = (hex >> 16) & 0xFF
    g = (hex >> 8) & 0xFF
    b = hex & 0xFF
    set_rgb(index, r, g, b)
  end

  def fill(r, g, b)
    @num_leds.times { |i| set_rgb(i, r, g, b) }
  end

  def brightness=(val)
    @brightness = [[val, 0].max, 100].min
  end

  def show
    bytes = []
    scale = @brightness / 100.0
    @num_leds.times do |i|
      r = (@buffer[i * 3] * scale).to_i
      g = (@buffer[i * 3 + 1] * scale).to_i
      b = (@buffer[i * 3 + 2] * scale).to_i
      bytes << g << r << b  # GRB order
    end

    if @rmt
      @rmt.write(bytes)
    else
      _write(bytes)
    end
  end

  def clear
    @buffer.size.times { |i| @buffer[i] = 0 }
    show
  end

  def close
    if @rmt
      @rmt.close if @rmt.respond_to?(:close)
      @rmt = nil
    else
      _deinit
    end
  end

  private

  def hsb_to_rgb(h, s, b)
    h = h % 360
    s = s / 100.0
    b = b / 100.0

    if s == 0
      gray = (b * 255).to_i
      return [gray, gray, gray]
    end

    h_sector = h / 60.0
    sector = h_sector.to_i
    fraction = h_sector - sector

    tint1 = b * (1 - s)
    tint2 = b * (1 - s * fraction)
    tint3 = b * (1 - s * (1 - fraction))

    r, g, b_rgb = case sector
    when 0
      [b, tint3, tint1]
    when 1
      [tint2, b, tint1]
    when 2
      [tint1, b, tint3]
    when 3
      [tint1, tint2, b]
    when 4
      [tint3, tint1, b]
    else
      [b, tint1, tint2]
    end

    [(r * 255).to_i, (g * 255).to_i, (b_rgb * 255).to_i]
  end
end
