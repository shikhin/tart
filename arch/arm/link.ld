/*
 * Linker file for ARM architecture.
 */

ENTRY(_start)

SECTIONS
{
    /* Starts at 0x8000. */
    .text 0x8000 :
    {
        KEEP(*(.text.boot))
        *(.text)
        *(.rodata)
    }

    .data ALIGN(4) :
    {
        *(.data)
    }

    . = ALIGN(4);

    /* The physical file ends here. */
    file_end = .;

    /* Align to 16 byte boundary. */
    . = ALIGN(16);

    .bss :
    {
        bss_start = .;
        *(.bss)
    }

    /* Align to 16 byte boundary. */
    . = ALIGN(16);

    bss_end = .;
}