cmake_minimum_required(VERSION 3.21)
project(pepper ASM_NASM CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AddEfiExecutable)

set(CMAKE_INSTALL_PREFIX sysroot)
# string(APPEND CMAKE_CXX_FLAGS " -Weverything -Wno-missing-prototypes")
string(APPEND CMAKE_CXX_FLAGS " \
-Werror \
-Weverything \
-Wno-c++20-compat \
-Wno-c++98-compat \
-Wno-c++98-compat-pedantic \
-Wno-missing-prototypes")

add_subdirectory(boot)
add_subdirectory(nstd)
add_subdirectory(kernel)

find_program(MCOPY_EXECUTABLE mcopy)
find_program(MFORMAT_EXECUTABLE mformat)
find_program(MMD_EXECUTABLE mmd)
if(MCOPY_EXECUTABLE)
  add_custom_target(image
    BYPRODUCTS pepper.img
    COMMAND truncate --size 1440000 pepper.img
    COMMAND "${MFORMAT_EXECUTABLE}" -i pepper.img -f 1440 ::
    COMMAND "${MMD_EXECUTABLE}" -i pepper.img ::/EFI
    COMMAND "${MMD_EXECUTABLE}" -i pepper.img ::/EFI/BOOT
    COMMAND "${MCOPY_EXECUTABLE}" -s -i pepper.img "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/*" ::
    DEPENDS install)
else()
  message(WARNING "Failed to find mtools")
endif()

find_program(QEMU_EXECUTABLE qemu-system-x86_64)
if(QEMU_EXECUTABLE)
  if(NOT DEFINED KVM_OK)
    if(EXISTS /dev/kvm)
      set(QEMU_KVM_ENABLED ON CACHE BOOL "")
    else()
      message(WARNING "KVM not enabled")
      set(QEMU_KVM_ENABLED OFF CACHE BOOL "")
    endif()
  endif()
  
  add_custom_target(run
    COMMAND
    "${QEMU_EXECUTABLE}"
    -bios /home/matt/Repos/umbongo/OVMF.fd
    $<$<BOOL:${QEMU_KVM_ENABLED}>:-enable-kvm>
    -cpu max
    -machine q35
    -nodefaults
    -no-reboot
    -display sdl
    -device virtio-vga,xres=800,yres=600
    -device qemu-xhci
    -device usb-storage,drive=esp
    -drive "file=fat:rw:${CMAKE_CURRENT_BINARY_DIR}/sysroot,format=raw,id=esp,if=none"
    DEPENDS install
    USES_TERMINAL)
else()
  message(WARNING "Failed to find qemu-system-x86_64")
endif()
