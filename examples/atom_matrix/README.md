# Atom Matrix Examples

This directory contains examples of WS2812 LED control for M5Stack Atom Matrix.
ATOM Matrix Product reference: https://www.switch-science.com/products/6260

## Prerequisites

- M5Stack Atom Matrix
  - 5x5 WS2812 LED matrix (25 LEDs)
  - GPIO 27 for LED control
- [R2P2-ESP32](https://github.com/picoruby/R2P2-ESP32) must be running

## LED Layout

The 25 LEDs are arranged in a 5x5 matrix with the following physical layout:
```
 0  1  2  3  4
 5  6  7  8  9
10 11 12 13 14
15 16 17 18 19
20 21 22 23 24
```

## File Description

- `basic.rb`: Basic LED control example
- `jumping_ruby.rb`: Ruby logo animation moving up and down
- `sequence_animation.rb`: Demonstrates how to control LEDs using coordinates, with a sequential lighting animation from top-left to bottom-right

## How Coordinates Work in sequence_animation.rb

The LEDs are wired in row major order, so a 2D coordinate maps to an LED number with a single multiplication. That is the whole conversion `sequence_animation.rb` does:

```ruby
index = y * grid_size + x
```

- First row (y=0): (0,0)->0, (1,0)->1, (2,0)->2, (3,0)->3, (4,0)->4
- Second row (y=1): (0,1)->5, (1,1)->6, (2,1)->7, (3,1)->8, (4,1)->9
- And so on, up to (4,4)->24

No lookup table or zigzag correction is needed.

## Usage

1. Copy files to `R2P2-ESP32/storage/home/`
2. Flash to Atom Matrix:
   ```
   rake flash
   ```
3. Start PicoRuby Shell:
   ```
   rake monitor
   ```
4. Run:
   ```ruby
   ./basic.rb
   ```
