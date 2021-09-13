#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace efi {

using handle = void *;

using guid = std::array<std::uint8_t, 16>;

consteval std::size_t status_error(unsigned exp) {
    return static_cast<std::size_t>(1) << exp;
}

enum class status : std::size_t {
    success = 0,
    load_error = status_error(1),
    invalid_parameter = status_error(2),
    unsupported = status_error(3),
    bad_buffer_size = status_error(4),
    buffer_too_small = status_error(5),
    not_ready = status_error(6),
    device_error = status_error(7),
};

struct table {
    std::uint64_t signature;
    std::uint32_t revision;
    std::uint32_t header_size;
    std::uint32_t checksum;
    std::uint32_t reserved;
};

struct simple_text_output_protocol {
    status (*reset)(simple_text_output_protocol *, bool);
    status (*output_string)(simple_text_output_protocol *, const wchar_t *);
    std::array<void *, 4> unimplemented_;
    status (*clear_screen)(simple_text_output_protocol *);
    std::array<void *, 3> unimplemented2_;
};

struct configuration_table {
    guid vendor_guid;
    void *vendor_data;
};

enum class allocate_type : std::size_t {
    allocate_any_pages,
    allocate_max_address,
    allocate_address,
    max_allocate_type,
};

enum class memory_type : std::size_t {
    efi_reserved_memory_type,
    efi_loader_code,
    efi_loader_data,
    efi_boot_services_code,
    efi_boot_services_data,
    efi_runtime_services_code,
    efi_runtime_services_data,
    efi_conventional_memory,
    efi_unusable_memory,
    efi_acpi_reclaim_memory,
    efi_acpi_memory_nvs,
    efi_memory_mapped_io,
    efi_memory_mapped_io_port_space,
    efi_pal_code,
    efi_persistent_memory,
    efi_unaccepted_memory_type,
    efi_max_memory_type,
};

struct boot_services : public table {
    std::array<void *, 2> unimplemented_;
    status (*allocate_pages)(allocate_type, memory_type, std::size_t, void *);
    status (*free_pages)(void *, std::size_t);
    void *get_memory_map;
    status (*allocate_pool)(memory_type, std::size_t, void **);
    status (*free_pool)(void *);
    std::array<void *, 9> unimplemented2_;
    status (*handle_protocol)(handle, const guid *, void **);
};

struct system_table : public table {
    wchar_t *firmware_vendor;
    std::uint32_t firmware_revision;
    handle console_in_handle;
    void *con_in;
    handle console_out_handle;
    simple_text_output_protocol *con_out;
    handle standard_error_handle;
    void *std_err;
    void *runtime_services;
    boot_services *boot_services;
    std::size_t config_table_count;
    configuration_table *config_table;
};

struct loaded_image_protocol {
    static constexpr guid guid{0xa1,0x31,0x1b,0x5b,0x62,0x95,0xd2,0x11,
        0x8e,0x3f,0x00,0xa0,0xc9,0x69,0x72,0x3b};

    std::uint32_t revision;
    handle parent_handle;
    system_table *system_table;
    handle device_handle;
    // ...
};

struct file_protocol {
    std::uint64_t revision;
};

struct simple_file_system_protocol {
    static constexpr guid guid{0x22,0x5b,0x43,0x64,0x09,0x59,0x64,0xd2,0x11,
        0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72};

    std::uint64_t revision;
    status (*open_volume)(simple_file_system_protocol *, void **);
};

} // namespace efi
