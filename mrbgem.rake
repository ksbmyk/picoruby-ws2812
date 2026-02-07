MRuby::Gem::Specification.new('picoruby-ws2812') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Miyuki Koshiba'
  spec.summary = 'WS2812 LED driver for PicoRuby'

  if build.name.downcase.include?('esp')
    spec.add_dependency 'picoruby-rmt'
  end
end
