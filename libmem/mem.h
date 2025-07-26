
#include <stdint.h>

#define _(X) X "\n\t"

/**
 * Tabela de tradução de endereços (L1).
 */
typedef struct {
   uint32_t descriptor[4096];
} ttb_l1_t;

/**
 * Desabilita o gerenciador de memória.
 */
void mmu_stop(void);

/**
 * Invalida o TLB atual.
 */
void tlb_invalida(void);

/**
 * Ativa o gerenciador de memória.
 * @param ttb Endereço para o primeiro nível da tabela de tradução de páginas.
 */
void mmu_start(ttb_l1_t *ttb);

/**
 * Cria uma entrada de seção na tabela de nível 1.
 * @param virt Endereço virtual a definir.
 * @param fis Endereço físico correspondente.
 * @param flags Flags adicionais.
 */
void map_section(ttb_l1_t* ttb_l1, uint32_t virt, uint32_t fis, uint32_t flags);

/**
 * Cria uma entrada de seção inválida (não mapeada) na tabela de nível 1.
 * @param virt Endereço virtual da seção.
 */
void map_invalid(ttb_l1_t* ttb_l1, uint32_t virt);

/**
 * Cria uma tabela de tradução "plana" (endereços físicos e virtuais são iguais).
 */
void mmu_flat(ttb_l1_t* ttb_l1);
