
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
     stack_irq = .;
     . = . + 4K;
     stack_user1 = .;
     . = . + 4K;
     stack_user2 = .;
     . = . + 4K;
     stack_user3 = .;
  }

  . = 0x200000;

  . = 0x500000;
  .tcb : { *(.tcb) }
  . = . + 16K;
  stack_sys = .;
}
