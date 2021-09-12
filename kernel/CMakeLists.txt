add_executable(kernel kmain.cc)
set_target_properties(kernel PROPERTIES
  OUTPUT_NAME "kernel.elf")
install(TARGETS kernel DESTINATION .)
target_compile_options(kernel PRIVATE
  -ffreestanding
  -fno-exceptions
  -fno-rtti
  -fno-pic
  -mno-red-zone
  -mno-sse)
target_link_options(kernel PRIVATE
  -nostdlib
  -Wl,-T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
