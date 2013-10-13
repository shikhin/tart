ENTRY(_Start)

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
    FileEnd = .;

    /* Align to 16 byte boundary. */
    . = ALIGN(16);

    .bss :
    {
        BSSStart = .;
        *(.bss)
    }

    /* Align to 16 byte boundary. */
    . = ALIGN(16);

    /* Kernel ends here. */
    KernelEnd = .;
}