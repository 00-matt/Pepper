#include "efi.hh"

efi::status efi_main(efi::handle *image, efi::system_table *st) {
    st->con_out->reset(st->con_out, true);
    st->con_out->output_string(st->con_out, L"Hello, World!\r\n");

    efi::loaded_image_protocol *loaded_image;
    if (st->boot_services->handle_protocol(image, &efi::loaded_image_protocol::guid, reinterpret_cast<void **>(&loaded_image)) != efi::status::success) {
        st->con_out->output_string(st->con_out, L"epic fail\r\n");
        while (true) {
            asm volatile("cli");
            asm volatile("hlt");
        }
    }

    while (true) ;
}
