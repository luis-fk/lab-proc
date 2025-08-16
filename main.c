#include "sched.h"
#include <stdlib.h>
#include "./libmem/mem.h"

#define ARRAY_SIZE 1024 // Ocupa até metade de seção de data
extern volatile int tid;

/*
 * Ponto de entrada do sistema.
 */
void system_main(void) {
  // mmu_flat((ttb_l1_t*)0x3EFFC000)
 
  /*###########################################################################################################################*/
  /*
   * Versão mais restritiva do mmu_flat mostrado no vídeo, bloqueando as áreas inválidas de memória e deixando válidas apenas:
   Seção 0: .text
   Seção 5: scheduler
   Seção 1: será mapeada pelo processo
  */
  for (int i = 0; i < 4096; i++)
    if (i != 0 && i != 5)
      // Bloqueia o acesso às seções
      map_invalid((ttb_l1_t*)0x3EFFC000, i << 20);
    else
      // .text and seções do scheduler devem ser válidas
      map_section((ttb_l1_t*)0x3EFFC000, i << 20, i << 20, 0x0);
  /*###########################################################################################################################*/
  
  sched_init();

  /*###########################################################################################################################*/
  // Adicionado depois do vídeo: precisa ser feito explicitamente por casa da remoção do mmu_flat()
  map_section((ttb_l1_t*)0x3EFFC000, 0x100000, 0x100000 * (tid + 1), 0x0); 
  /*###########################################################################################################################*/

  asm volatile("b task_switch"); // transfere o controle ao primeiro thread
}

/* Função utilizatária de soma para ilustrar a pilha */
int soma(int a, int b) {
  yield();         // Desescalonando processo para ver funcionamento da pilha
  return a + b;
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
    int c = soma(1, 1);
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
    int c = soma(1, 2);
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
    int c = soma(1, 3);
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
