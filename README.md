# picoruby-ws2812

A WS2812 LED driver gem for PicoRuby. Supports ESP32 (using RMT peripheral) and RP2040/RP2350 (using PIO peripheral).

## Features

- Control LEDs using RGB values, hexadecimal color codes, or HSB values
- Buffered LED control with explicit `show` command
- Global brightness control (0-100%)
- Platform-specific optimized drivers:
  - ESP32: RMT (Remote Control) peripheral
  - RP2040/RP2350: PIO (Programmable I/O) peripheral

## Installation

### ESP32

Add the following line to your `picoruby/build_config/xtensa-esp.rb`:

```ruby
conf.gem github: 'ksbmyk/picoruby-ws2812', branch: 'main'
```

### RP2040/RP2350 (R2P2)

1. Add the following line to your build configuration (e.g., `build_config/r2p2-picoruby-pico2.rb`):

   ```ruby
   conf.gem github: 'ksbmyk/picoruby-ws2812', branch: 'main'
   ```

2. Add the following line to `CMakeLists.txt` to include platform-specific drivers from external gems:

   ```diff
    file(GLOB SOURCE_FILES CONFIGURE_DEPENDS
      src/*.c
      ${CMAKE_SOURCE_DIR}/lib/picoruby/mrbgems/*/ports/rp2040/*.c
      ${CMAKE_SOURCE_DIR}/lib/picoruby/mrbgems/*/ports/common/*.c
   +  ${CMAKE_SOURCE_DIR}/lib/picoruby/build/repos/*/*/ports/rp2040/*.c
    )
   ```

3. Clean and rebuild:

   ```
   rake picoruby:pico2:clean
   rake picoruby:pico2:prod
   ```

## Usage

```ruby
require 'ws2812'

# Initialize (specify GPIO pin and number of LEDs)
led = WS2812.new(pin: 25, num: 64)

# Set individual LED colors (writes to internal buffer)
led.set_rgb(0, 255, 0, 0)      # LED 0: Red (R, G, B: 0-255)
led.set_hex(1, 0x00FF00)       # LED 1: Green (24-bit hex)
led.set_hsb(2, 240, 100, 100)  # LED 2: Blue (H: 0-360, S: 0-100, B: 0-100)

# Fill all LEDs with the same color
led.fill(255, 255, 255)        # All white

# Adjust brightness (0-100%)
led.brightness = 50

# Send buffer to LEDs
led.show

# Turn off all LEDs
led.clear

# Clean up when done
led.close
```

## API Reference

### Constructor

```ruby
WS2812.new(pin:, num:)
```

- `pin`: GPIO pin number (Integer)
- `num`: Number of LEDs (Integer)

### Methods

| Method | Description |
|--------|-------------|
| `set_rgb(index, r, g, b)` | Set LED color using RGB values (0-255) |
| `set_hex(index, hex)` | Set LED color using 24-bit hex (0xRRGGBB) |
| `set_hsb(index, h, s, b)` | Set LED color using HSB (H: 0-360, S: 0-100, B: 0-100) |
| `fill(r, g, b)` | Fill all LEDs with the same RGB color |
| `brightness` | Get current brightness (0-100) |
| `brightness=` | Set brightness (0-100) |
| `show` | Send buffer to LEDs |
| `clear` | Turn off all LEDs (fills buffer with 0 and calls show) |
| `close` | Release driver resources |

### Notes

- Color values are written to an internal buffer
- Call `show` to send the buffer to the LEDs
- Out-of-range index values are silently ignored
- RGB values are masked to 0-255
- Brightness is applied at `show` time (buffer values are not modified)

## Examples

Check out the `examples` directory for more detailed examples, including:
- `atom_matrix/` - 5x5 matrix control for M5Stack Atom Matrix (ESP32)
- `rp2350_matrix/` - Examples for Waveshare RP2350-Matrix (RP2350)

## Platform Support

### Supported Platforms

- **ESP32** (using RMT peripheral)
  - Optimized for AtomMatrix LED matrix
  - Uses hardware-accelerated timing control
  - `picoruby-rmt` dependency is automatically included

- **RP2040/RP2350** (using PIO peripheral)
  - Hardware-accelerated bit-banging via PIO
  - Tested on [Waveshare RP2350-Matrix](https://www.waveshare.com/wiki/RP2350-Matrix)
  - 800kHz WS2812 timing

## Dependencies

- ESP32: `picoruby-rmt` (automatically added when `ESP32_PLATFORM` is defined)
- RP2040/RP2350: No additional dependencies (PIO driver is built-in)

## Development Boards

- M5Stack Atom Matrix (ESP32)
  - 5x5 WS2812 LED matrix
  - GPIO 27 (default)
- [Waveshare RP2350-Matrix](https://www.waveshare.com/wiki/RP2350-Matrix)
  - 8x8 WS2812 LED matrix
  - GPIO 25

## License

MIT License
