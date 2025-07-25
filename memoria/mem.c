
#include <stdint.h>

#define _(X) X "\n\t"

/**
 * Tabela de tradução de endereços (L1).
 */
typedef struct {
   uint32_t descriptor[4096];
} ttb_l1_t;

/**
 * Tabela de tradução de endereços (L2).
 */
typedef struct {
   uint32_t descriptor[256];
} ttb_l2_t;

/**
 * TTB no endereço 0x4000 (fixo e alinhado em 16 kiB).
 */
ttb_l1_t *ttb_l1 = (ttb_l1_t*)0x4000; /*Como esse endereço é decidido?*/

/**
 * Desabilita o gerenciador de memória.
 */
void mmu_stop(void) {
   asm volatile(
      _("mrc p15, 0, r2, c1, c0, 0")
      _("bic r2, #0x1000")
      _("bic r2, #0x0004")
      _("bic r2, #0x0001")
      _("mcr p15, 0, r2, c1, c0, 0")
   );
}

/**
 * Invalida o TLB atual.
 */
void tlb_invalida(void) {
   asm volatile(
      _("mov r2, #0")
      _("mcr p15, 0, r2, c8, c7, 0")
      _("mcr p15, 0, r2, c7, c10, 4")
   ::: "r2");
}

/**
 * Ativa o gerenciador de memória.
 * @param ttb Endereço para o primeiro nível da tabela de tradução de páginas.
 */
void mmu_start(ttb_l1_t *ttb) {
   asm volatile(
      _("mvn r2, #0")
      _("bic r2, #0xC")
      _("mcr p15, 0, r2, c3, c0, 0")
      _("mcr p15, 0, %0, c2, c0, 0")
      _("mcr p15, 0, %0, c2, c0, 1")
      _("mrc p15, 0, r2, c1, c0, 0")
      _("orr r2, r2, #0x05")
      _("orr r2, r2, #0x1000")
      _("mcr p15, 0, r2, c1, c0, 0")
   :: "r"(ttb) : "r2");
}

/**
 * Cria uma entrada de seção na tabela de nível 1.
 * @param virt Endereço virtual a definir.
 * @param fis Endereço físico correspondente.
 * @param flags Flags adicionais.
 */
void map_section(uint32_t virt, uint32_t fis, uint32_t flags) {
   int indice = (virt & 0xfff00000) >> 20;              // índice do descritor, entre 0 e 4095 (20 bits do endereço virtual)
   int descr = (fis & 0xfff00000) | 0xc002 | flags;     // valor do descritor: 12 bits do endereço físico + flags.
   ttb_l1->descriptor[indice] = descr;
}

/**
 * Cria uma entrada de seção inválida (não mapeada) na tabela de nível 1.
 * @param virt Endereço virtual da seção.
 */
void map_invalid(uint32_t virt) {
   int indice = (virt & 0xfff00000) >> 20;              // índice do descritor, entre 0 e 4095 (12 bits do endereço virtual)
   ttb_l1->descriptor[indice] = 0;
}

/**
 * Cria uma tabela de tradução "plana" (endereços físicos e virtuais são iguais).
 */
void mmu_flat(void) {
   int i;
   for(i=0; i<4096; i++) { // Iterar 4K vezes
      map_section(i << 20, i << 20, 0x0000);
   }
}

int main(void) {
   uint32_t *ptr = 0x0020fff0;
   mmu_flat();
   map_section(0x00100000, 0x00000000, 0x0000);    // mapeia o primeiro MiB da memória física no segundo MiB da memória virtual.
   map_invalid(0x00200000);                        // "desmapeia" o terceiro MiB da memória virtual
   tlb_invalida();
   mmu_start(ttb_l1);                              // liga o bicho
   *ptr = 0x0de0c0c0;
   for(;;);
}

