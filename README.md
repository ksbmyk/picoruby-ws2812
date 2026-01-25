# picoruby-ws2812

A WS2812 LED driver gem for PicoRuby. Currently optimized for ESP32 (particularly AtomMatrix) using RMT (Remote Control) peripheral, with plans to support other platforms in the future.

## Features

- Control LEDs using RGB values or hexadecimal color codes
- Control multiple LEDs simultaneously
- Optimized timing parameters for ESP32 RMT

## Installation

Currently, this gem is only supported on ESP32 (Xtensa architecture). Support for other platforms is planned for future releases.

1. Add the following line to your `picoruby/build_config/xtensa-esp.rb`:
   ```ruby
   conf.gem github: 'ksbmyk/picoruby-ws2812', branch: 'main'
   ```

2. Clean and rebuild:
   ```
   rake clean build
   ```

## Usage

```ruby
require 'ws2812'

# Initialize LED (specify GPIO pin number)
rmt = RMTDriver.new(27)
led = WS2812.new(rmt)

# Control with RGB values
led.show_rgb([255, 0, 0], [0, 255, 0], [0, 0, 255])  # Red, Green, Blue

# Control with hexadecimal color codes
led.show_hex(0xFF0000, 0x00FF00, 0x0000FF)  # Red, Green, Blue
```

## Examples

Check out the `examples` directory for more detailed examples, including:
- Basic LED control
- 5x5 matrix control for Atom Matrix


## Platform Support

### Currently Supported
- ESP32 (using RMT peripheral)
  - Optimized for AtomMatrix LED matrix
  - Uses hardware-accelerated timing control

### Planned Support
- Other ESP32 variants
- Raspberry Pi Pico
- Other platforms
- Other drivers (SPI, PIO)

## Dependencies

- picoruby-rmt (for ESP32 RMT support)

### Development Boards
- M5Stack Atom Matrix
  - 5x5 WS2812 LED matrix
  - GPIO 27 (default)
  - Tested with various animations and color patterns

## License

MIT License

