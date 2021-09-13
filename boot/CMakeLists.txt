add_efi_executable(boot boot.cc efi.cc)
set_target_properties(boot PROPERTIES
  OUTPUT_NAME "BOOTX64.EFI")
install(TARGETS boot DESTINATION EFI/BOOT)
target_link_libraries(boot PRIVATE nstd)
set_target_properties(boot PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
