#include "sched.h"
#include <stdlib.h>
#include "./libmem/mem.h"

#define ARRAY_SIZE 1024 // Ocupa até metade de seção de data

extern void enable_irq(int);

/*
 * Ponto de entrada do sistema.
 */
void system_main(void) {
  mmu_flat((ttb_l1_t*)0x3EFFC000); // Inicializa a MMU com uma tabela plana
  sched_init();
  asm volatile("b task_switch"); // transfere o controle ao primeiro thread
}

int32_t a[ARRAY_SIZE];
int32_t *b;

/*
 * Ponto de entrada do primeiro task.
 */
void user1_main(void) {
  int i;
  int n = 5;
  b = malloc(n*sizeof(int32_t));
  for (;;) {
    for (i = 0; i < n; i++) {
      b[i] = 0xA1A1A1A1;
    }
    for (i = 0; i < ARRAY_SIZE; i++) {
      a[i] = 0xAAAAAAAA;
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
  int n = 10;
  b = malloc(n*sizeof(int32_t));
  for (;;) {
    for (i = 0; i < n; i++) {
      b[i] = 0xB2B2B2B2;
    }
    for (i = 0; i < ARRAY_SIZE; i++) {
      a[i] = 0xBBBBBBBB;
    }
    asm volatile("nop");
    yield();
  }
}

void user3_main(void) {
  int i;
  int n = 100;
  b = malloc(n*sizeof(int32_t));
  for (;;) {
    for (i = 0; i < n; i++) {
      b[i] = 0xC3C3C3C3;
    }
    for (i = 0; i < ARRAY_SIZE; i++) {
      a[i] = 0xCCCCCCCC;
    }
    asm volatile("nop");
    yield();
  }
}
