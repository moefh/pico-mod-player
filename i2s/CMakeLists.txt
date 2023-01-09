
add_executable(i2s_mod_player
  main.c
  ../lib/sound_i2s.c
  ../lib/mod_play.c
)

pico_generate_pio_header(i2s_mod_player ${CMAKE_CURRENT_LIST_DIR}/../lib/sound_i2s_8bits.pio)
pico_generate_pio_header(i2s_mod_player ${CMAKE_CURRENT_LIST_DIR}/../lib/sound_i2s_16bits.pio)

target_include_directories(i2s_mod_player
  PUBLIC
  ../lib
)

target_link_libraries(i2s_mod_player
  pico_stdlib
  hardware_dma
  hardware_pio
  hardware_irq
)

pico_enable_stdio_usb(i2s_mod_player 1)
pico_add_extra_outputs(i2s_mod_player)
