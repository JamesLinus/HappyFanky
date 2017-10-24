ENTRY (multiboot_entry)
SECTIONS{
    . = 0x00501000;	
    .text :{
        *(.text)
    }
    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }
  
    .data ALIGN (0x1000) : {
        *(.data)
    }
    .bss : {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
}
