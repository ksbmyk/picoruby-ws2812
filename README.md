# picoruby-ws2812

A WS2812 LED driver gem for PicoRuby. Supports ESP32 (using RMT peripheral) and RP2040/RP2350 (using PIO peripheral).

## Features

- Control LEDs using RGB values, hexadecimal color codes, or HSB values
- Control multiple LEDs simultaneously
- Platform-specific optimized drivers:
  - ESP32: RMT (Remote Control) peripheral
  - RP2040/RP2350: PIO (Programmable I/O) peripheral

## Installation

### ESP32

Add the following line to your `picoruby/build_config/xtensa-esp.rb`:

```ruby
conf.gem github: 'ksbmyk/picoruby-ws2812', branch: 'main'
```

### RP2040/RP2350

Add the following line to your PicoRuby build configuration:

```ruby
conf.gem github: 'ksbmyk/picoruby-ws2812', branch: 'main'
```

Then clean and rebuild:

```
rake clean build
```

## Usage

### ESP32 (RMTDriver)

```ruby
require 'ws2812'

# Initialize with RMT driver (specify GPIO pin)
driver = RMTDriver.new(27)
led = WS2812.new(driver)

# Control with RGB values
led.show_rgb([255, 0, 0], [0, 255, 0], [0, 0, 255])  # Red, Green, Blue

# Control with hexadecimal color codes
led.show_hex(0xFF0000, 0x00FF00, 0x0000FF)  # Red, Green, Blue

# Control with HSB values (H: 0-360, S: 0-100, B: 0-100)
led.show_hsb([0, 100, 100], [120, 100, 100], [240, 100, 100])  # Red, Green, Blue
```

### RP2040/RP2350 (PIODriver)

```ruby
require 'ws2812'

# Initialize with PIO driver (specify GPIO pin)
driver = PIODriver.new(25)
led = WS2812.new(driver)

# Control with RGB values
led.show_rgb([255, 0, 0], [0, 255, 0], [0, 0, 255])  # Red, Green, Blue

# Control with hexadecimal color codes
led.show_hex(0xFF0000, 0x00FF00, 0x0000FF)  # Red, Green, Blue

# Control with HSB values
led.show_hsb([0, 100, 100], [120, 100, 100], [240, 100, 100])  # Red, Green, Blue

# Clean up when done
driver.close
```

## Examples

Check out the `examples` directory for more detailed examples, including:
- Basic LED control
- 5x5 matrix control for Atom Matrix

## Platform Support

### Supported Platforms

- **ESP32** (using RMT peripheral)
  - Optimized for AtomMatrix LED matrix
  - Uses hardware-accelerated timing control
  - `picoruby-rmt` dependency is automatically included

- **RP2040/RP2350** (using PIO peripheral)
  - Hardware-accelerated bit-banging via PIO
  - Tested on Raspberry Pi Pico and RP2350-based boards
  - 800kHz WS2812 timing

## Dependencies

- ESP32: `picoruby-rmt` (automatically added when `ESP32_PLATFORM` is defined)
- RP2040/RP2350: No additional dependencies (PIO driver is built-in)

## Development Boards

- M5Stack Atom Matrix (ESP32)
  - 5x5 WS2812 LED matrix
  - GPIO 27 (default)
- Raspberry Pi Pico / RP2040-based boards
  - Any GPIO pin with WS2812 LEDs
- RP2350-based boards
  - Any GPIO pin with WS2812 LEDs

## License

MIT License
