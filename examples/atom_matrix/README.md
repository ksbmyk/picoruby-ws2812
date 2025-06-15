# Atom Matrix Examples

This directory contains examples of WS2812 LED control for M5Stack Atom Matrix.
ATOM Matrix Product reference: https://www.switch-science.com/products/6260

## Prerequisites

- M5Stack Atom Matrix
  - 5x5 WS2812 LED matrix (25 LEDs)
  - GPIO 27 for LED control
- [R2P2-ESP32](https://github.com/picoruby/R2P2-ESP32) must be running

## File Description

- `basic.rb`: Basic LED control example
- `jumping_ruby.rb`: Ruby logo animation moving up and down

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
