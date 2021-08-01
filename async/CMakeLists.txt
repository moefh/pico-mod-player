
add_executable(async_mod_player
  main.c
  core1_main.c
  async_msg.c
  ../lib/audio.c
  ../lib/mod_play.c
)

target_include_directories(async_mod_player
  PUBLIC
  ../lib
)

target_link_libraries(async_mod_player
  pico_stdlib
  pico_multicore
  hardware_dma
  hardware_pwm
)

pico_enable_stdio_usb(async_mod_player 1)
pico_add_extra_outputs(async_mod_player)
