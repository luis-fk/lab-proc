
FONTES = mem.c
PROJETO = libmem

#
# Arquivos de saída 
#
LIB = ${PROJETO}.a

PREFIXO = arm-none-eabi-
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJETOS = $(FONTES:.c=.o)

LDOPTS = -lgcc -L/usr/lib/gcc/arm-none-eabi/13.2.1/ -i -M=${PROJETO}.map
COPTIONS = -march=armv7-a -mtune=cortex-a7 -g -D RPICPU=2 -c

all: ${LIB}

rebuild: clean all

#
# Gerar a blblioteica
#
${LIB}: ${OBJETOS}
	${LD} ${OBJETOS} -o $@ ${LDOPTS} 

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${COPTIONS} -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${LIB} ${PROJETO}.map

