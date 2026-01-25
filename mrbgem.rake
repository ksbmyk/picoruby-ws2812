MRuby::Gem::Specification.new('picoruby-ws2812') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Miyuki Koshiba'
  spec.summary = 'WS2812 LED driver for PicoRuby'

  # picoruby-rmt is only available on ESP32 platforms
  if cc.defines.include?('ESP32_PLATFORM')
    spec.add_dependency 'picoruby-rmt'
  end
end
