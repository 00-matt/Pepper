add_efi_executable(boot boot.cc)
set_target_properties(boot PROPERTIES
  OUTPUT_NAME "BOOTX64.EFI")
install(TARGETS boot DESTINATION EFI/BOOT)
