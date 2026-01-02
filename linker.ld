ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000, LENGTH = 256M
}

SECTIONS
{
    .text 0x80000 : {
        *(.text.boot)
        *(.text)
        *(.text.*)
    } > RAM

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > RAM

    .data : {
        *(.data)
        *(.data.*)
    } > RAM

    bss_start = .;
    .bss : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } > RAM
    bss_end = .;
    
    /DISCARD/ : {
        *(.note.GNU-stack)
        *(.gnu_debuglink)
        *(.gnu.lto_*)
    }
}
