
#include <stdint.h>
#include "mem.h"

#define _(X) X "\n\t"

#define TTB_L1_ADDR 0x3EFFC000;

void mmu_stop(void) {
   asm volatile(
      _("mrc p15, 0, r2, c1, c0, 0")
      _("bic r2, #0x1000")
      _("bic r2, #0x0004")
      _("bic r2, #0x0001")
      _("mcr p15, 0, r2, c1, c0, 0")
   );
}

void tlb_invalida(void) {
   asm volatile(
      _("mov r2, #0")
      _("mcr p15, 0, r2, c8, c7, 0")
      _("mcr p15, 0, r2, c7, c10, 4")
   ::: "r2");
}

void mmu_start() {
   ttb_l1_t *ttb_l1 = (ttb_l1_t *)TTB_L1_ADDR
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
   :: "r"(ttb_l1) : "r2");
}

void map_section(uint32_t virt, uint32_t fis, uint32_t flags) {
   ttb_l1_t *ttb_l1 = (ttb_l1_t *)TTB_L1_ADDR; // Endereço da tabela de páginas de nível 1
   int indice = (virt & 0xfff00000) >> 20;              // índice do descritor, entre 0 e 4095 (20 bits do endereço virtual)
   int descr = (fis & 0xfff00000) | 0xc002 | flags;     // valor do descritor: 12 bits do endereço físico + flags.
   ttb_l1->descriptor[indice] = descr;
}

void map_invalid(uint32_t virt) {
   ttb_l1_t *ttb_l1 = (ttb_l1_t *)TTB_L1_ADDR; // Endereço da tabela de páginas de nível 1
   int indice = (virt & 0xfff00000) >> 20;     //0xf     // índice do descritor, entre 0 e 4095 (12 bits do endereço virtual)
   ttb_l1->descriptor[indice] = 0;
}

void mmu_flat() {
   int i;
   for(i=0; i<4096; i++) { // Iterar 4K vezes
      map_section(i << 20, i << 20, 0x0000);
   }
}
