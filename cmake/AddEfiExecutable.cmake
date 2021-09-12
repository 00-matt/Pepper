function(add_efi_executable name)
  add_executable(${name} ${ARGN})
  target_compile_options(${name} PRIVATE
    -fno-exceptions
    -fno-pic
    -fno-rtti
    -fshort-wchar
    -mno-red-zone
    -mno-sse
    -target x86_64-unknown-windows)
  target_link_options(${name} PRIVATE
    -nostdlib
    -Wl,-entry:efi_main
    -Wl,-subsystem:efi_application
    -target x86_64-unknown-windows)
endfunction()
