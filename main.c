#include "sched.h"
#include "./libmem/mem.h"

extern void enable_irq(int);

/*
 * Ponto de entrada do sistema.
 */
void system_main(void) {
  mmu_flat((ttb_l1_t*)0x3EFFC000); // Inicializa a MMU com uma tabela plana
  sched_init();
  asm volatile("b task_switch"); // transfere o controle ao primeiro thread
}

int a = 23;
int b = 42;

/*
 * Ponto de entrada do primeiro task.
 */
void user1_main(void) {
  int i;
  a = 2;
  b = 20;
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
  a = 3;
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
  a = 4;
  for (;;) {
    for (i = 0; i < a; i++) {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}
