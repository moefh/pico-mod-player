
add_executable(simple_mod_player
  main.c
  ../lib/audio.c
  ../lib/mod_play.c
)

target_include_directories(simple_mod_player
  PUBLIC
  ../lib
)

target_link_libraries(simple_mod_player
  pico_stdlib
  hardware_dma
  hardware_pwm
)

pico_enable_stdio_usb(simple_mod_player 1)
pico_add_extra_outputs(simple_mod_player)
