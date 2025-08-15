#include <stdint.h>

#pragma once

typedef struct tcb_s tcb_t;
typedef struct tcb_ll tcb_ll_t;

extern volatile int tid;
extern volatile tcb_t *tcb;

/**
 * Estrutura do
 * Task control block (TCB).
 */
typedef struct tcb_s {
  uint32_t regs[17]; // Contexto (r0-r15, cpsr)
  char *heap;
} tcb_t;

/**
 *
 * Estrutura dados lista ligada de tasks
 *
 */
typedef struct tcb_ll {
  tcb_t tcb;
  int tid;
  volatile struct tcb_ll *next;
} tcb_ll_t;

void yield(void);
int getpid(void);
unsigned getticks(void);

void insert_tcb(volatile tcb_ll_t *, tcb_t);
void schedule(void);
void trata_swi(unsigned);
void trata_irq(void);
void sched_init(void);
