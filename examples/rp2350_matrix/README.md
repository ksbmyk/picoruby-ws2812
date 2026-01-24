# RP2350-Matrix Examples

Examples for Waveshare RP2350-Matrix with 8x8 WS2812 LED matrix.
Product reference: https://www.switch-science.com/products/10726

## Prerequisites

- Waveshare RP2350-Matrix
  - 8x8 WS2812 LED matrix (64 LEDs)
  - GPIO 25 for LED control
- [R2P2](https://github.com/picoruby/R2P2) with picoruby-ws2812 gem installed

## LED Layout

The 64 LEDs are arranged in an 8x8 matrix with sequential layout:
```
 0  1  2  3  4  5  6  7
 8  9 10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
```

## Examples

- `basic.rb` - Basic color display (red, green, blue, white)
- `jumping_ruby.rb` - Animated Ruby logo bouncing up and down
- `sequence_animation.rb` - Sequential pixel lighting across the matrix

## Usage

1. Copy files to R2P2 storage:
   ```
   cp *.rb /Volumes/R2P2/home/
   ```
2. Connect via serial:
   ```
   picocom -b 115200 --imap lfcrlf /dev/tty.usbmodem*
   ```
3. Run an example:
   ```ruby
   ./basic.rb
   ```
