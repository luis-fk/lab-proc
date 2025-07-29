#include "sched.h"
#include "./libmem/mem.h"

extern void enable_irq(int);

/*
 * Ponto de entrada do sistema.
 */
void system_main(void) {
  mmu_stop();
  mmu_flat();
  mmu_start();
  sched_init();
  asm volatile("b processes_entrypoint"); // transfere o controle ao primeiro thread
}

int a = 23;

/*
 * Ponto de entrada do primeiro task.
 */
void user1_main(void) {
  int i;
  a = 100;
  for (;;) {
    for (i = 0; i < a; i++) {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}

/*
 * Ponto de entrada do segundo task.
 */
void user2_main(void) {
  int i;
  a = 200;
  for (;;) {
    for (i = 0; i < a; i++) {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}

void user3_main(void) {
  int i;
  a = 300;
  for (;;) {
    for (i = 0; i < a; i++) {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}
