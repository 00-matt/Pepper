ENTRY(kmain)

SECTIONS {
         . = 1M;

         .text : ALIGN(4K) {
                 *(.text*)
         }

         .rodata : ALIGN(4K) {
                 *(.init_array)
                 *(.rodata*)
         }

         .data : ALIGN(4K) {
                 *(.data*)
         }

         .bss : ALIGN(4K) {
                *(COMMON)
                *(.bss*)
         }

         /DISCARD/ : {
                *(.comment)
                *(.interp)
         }
}
