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
 9  8  7  6  5
10 11 12 13 14
19 18 17 16 15
20 21 22 23 24
```

## File Description

- `basic.rb`: Basic LED control example
- `jumping_ruby.rb`: Ruby logo animation moving up and down
- `sequence_animation.rb`: Demonstrates how to control LEDs using coordinates, with a sequential lighting animation from top-left to bottom-right

## How Coordinates Work in sequence_animation.rb

By default, the LEDs are controlled in their physical connection order (0 to 24). The `AtomMatrix` class in `sequence_animation.rb` converts this physical order into a coordinate system for easier programming.

The `show` method in `AtomMatrix` class converts the 2D coordinates (x,y) to the physical LED numbers:

```ruby
def show
  pixels = []
  5.times do |y|
    5.times do |x|
      # Convert 2D coordinates to physical LED number
      # For example: (0,0) -> 0, (1,0) -> 1, (0,1) -> 9, etc.
      pixels << @matrix[y][x]
    end
  end
  @led.show_rgb(*pixels)
end
```

This conversion maps the coordinates to the physical LED numbers in the following way:
- First row (y=0): (0,0)->0, (1,0)->1, (2,0)->2, (3,0)->3, (4,0)->4
- Second row (y=1): (0,1)->9, (1,1)->8, (2,1)->7, (3,1)->6, (4,1)->5
- And so on...

You can modify the `show` method to change how the physical LED numbers map to coordinates.

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
