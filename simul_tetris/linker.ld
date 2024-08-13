
SECTIONS {
  .text 0x8000 : {
    boot.o(.text)
    *(.text)
  }

  . = ALIGN(0x4);
  data_begin = .;
  .data : { *(.data) }

  . = ALIGN(0x4);
  bss_begin = .;
  .bss : { *(.bss*) }
  bss_end = .;

  . = ALIGN(8);
  heap_addr = .;
  . = . + 32K;
  stack_addr = .;
}
