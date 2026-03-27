begin; require 'rmt'; rescue LoadError; end
begin; require 'pio'; rescue LoadError; end

class WS2812
  attr_reader :brightness

  def initialize(pin:, num:, order: :grb)
    @num = num
    @brightness = 5
    @buffer = Array.new(num * 3, 0)
    @order = order
    @rmt = nil
    @sm = nil

    begin
      init_rmt(pin)
    rescue NameError
      init_pio(pin)
    end
  end

  def set_rgb(index, r, g, b)
    return if index < 0 || index >= @num
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
    i = @num
    while 0 < i
      i -= 1
      set_rgb(i, r, g, b)
    end
  end

  def brightness=(val)
    @brightness = [[val, 0].max, 100].min
  end

  def show
    if @rmt
      @rmt.write(_convert)
    else
      @sm.put_buffer(_convert)
    end
    nil
  end

  def clear
    fill(0, 0, 0)
    show
  end

  def close
    @rmt = nil
    @sm = nil
  end

  private

  def init_rmt(pin)
    @rmt = RMT.new(pin,
      t0h_ns: 350,
      t0l_ns: 800,
      t1h_ns: 700,
      t1l_ns: 600,
      reset_ns: 60000
    )
  end

  def init_pio(pin)
    program = self.class.pio_program
    @sm = PIO::StateMachine.new(
      pio: PIO::PIO0,
      sm: 0,
      program: program,
      freq: 800_000 * 10,
      sideset_pins: pin,
      out_shift_right: false,
      out_shift_autopull: true,
      out_shift_threshold: 24,
      fifo_join: PIO::FIFO_JOIN_TX
    )
    @sm.start
  end

  def self.pio_program
    PIO.asm(side_set: 1) do
      wrap_target
      label :bitloop
      out :x, 1,                    side: 0, delay: 2
      jmp :do_zero, cond: :not_x,  side: 1, delay: 1
      label :do_one
      jmp :bitloop,                 side: 1, delay: 4
      label :do_zero
      nop                           side: 0, delay: 4
      wrap
    end
  end

  # Apply brightness scaling and color order conversion.
  # C-accelerated version available in picoruby-ws2812-plus.
  def _convert
    result = []
    buf = @buffer
    br = @brightness
    rgb = @order == :rgb
    pack = @rmt == nil
    j = 0
    i = @num
    while 0 < i
      r = (buf[j]     * br) / 100
      g = (buf[j + 1] * br) / 100
      b = (buf[j + 2] * br) / 100

      if rgb
        c1, c2, c3 = r, g, b
      else
        c1, c2, c3 = g, r, b
      end

      if pack
        result << ((c1 << 24) | (c2 << 16) | (c3 << 8))
      else
        result << c1 << c2 << c3
      end
      j += 3
      i -= 1
    end
    result
  end

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
