SECTIONS {
  .boot 0x8000: {
     *(.boot)
   }

   . = ALIGN(8);
  .text : {
    *(.text)
  }

  . = 0x100000;
  
   . = ALIGN(8);
  .rodata : { *(.rodata) }

  . = ALIGN(8);
  data_begin = .;
  .data : { *(.data) }
  data_end = .;

  . = ALIGN(8);
  bss_begin = .;
  .bss : { *(.bss*) }
  bss_end = .;

  .heapstack : {
     . = ALIGN(8);
     heap_addr = .;
     . = . + 32K;
     . = . + 4K;
     stack_user = .;
  }

  . = 0x200000;

  . = 0x500000;
  .tcb : { *(.tcb) }
  . = . + 16K;
  stack_sys = .;
  . = . + 16K;
  stack_irq = .;

  . = 0x700000;
}
