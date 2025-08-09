
FONTES = boot.s switch.s sched.c main.c stubs.c
RPICPU = bcm2836
LDSCRIPT = linker.ld
PROJETO = tasks

# Interface serial a utilizar
TTY = /dev/ttyUSB0

# Configuração JTAG
OCD_CFG = rpi2-jlink.cfg

#
# Arquivos de saída 
#
EXEC = ${PROJETO}.elf
MAP = ${PROJETO}.map
LIST = ${PROJETO}.list
IMAGE = ${PROJETO}.img
HEXFL = ${PROJETO}.hex

PREFIXO = arm-none-eabi-
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJCPY = ${PREFIXO}objcopy
OBJDMP = ${PREFIXO}objdump
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

PATH_GCC = /usr/lib/gcc/arm-none-eabi/13.2.1/
PATH_NEWLIB = /usr/lib/arm-none-eabi/lib/
PATH_LIBMEM = ./libmem/

QEMU := env -u LD_LIBRARY_PATH /usr/bin/qemu-system-arm

ifeq (${RPICPU}, bcm2836)
	# Raspberry Pi v.2 ou v.3
	ASMOPTIONS = -g --defsym RPICPU=2
	COPTIONS = -march=armv7-a -mtune=cortex-a7 -g -D RPICPU=2
	LDOPTS = -lgcc -L${PATH_GCC} -L${PATH_NEWLIB} -L${PATH_LIBMEM} -lc_nano -lmem
else
	ifeq (${RPICPU}, bcm2835)
  		# Raspberry Pi v.0 ou v.1
   	ASMOPTIONS = -march=armv6zk -g --defsym RPICPU=0
   	COPTIONS = -march=armv6zk -mtune=arm1176jzf-s -g -D RPICPU=0
		LDOPTS = -lgcc -L${PATH_GCC} -L${PATH_NEWLIB} -L${PATH_LIBMEM} -lc_nano -lmem
	endif
endif

all: ${EXEC} ${LIST} ${IMAGE} ${HEXFL}

rebuild: clean all

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} ${OBJETOS} -o $@ ${LDOPTS}

#
# Gerar listagem
#
${LIST}: ${EXEC}
	${OBJDMP} -std ${EXEC} > ${LIST}

#
# Gerar imagem binária
#
${IMAGE}: ${EXEC}
	${OBJCPY} -O binary ${EXEC} ${IMAGE}

#
# Gerar arquivo HEX
#
${HEXFL}: ${EXEC}
	${OBJCPY} -O ihex ${EXEC} ${HEXFL}

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${COPTIONS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} ${ASMOPTIONS} -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${EXEC} ${MAP} ${LIST} ${IMAGE}

#
# Iniciar qemu
#
qemu: ${EXEC}
	@# if there's already a qemu-system-arm running, skip
	@if pgrep -x qemu-system-arm >/dev/null; then \
	  echo "qemu já está executando"; \
	else \
	  echo "iniciando qemu…"; \
	  $(QEMU) -s -M raspi2b & \
	fi

#
# Executar openocd
#
ocd:
	@if pgrep openocd >/dev/null ; then \
		echo "openocd já está executando" ; \
	else openocd -f ${OCD_CFG} & \
	fi


define BREAKS
-ex "b system_main" \
-ex "b switch.s:94" \
-ex "b main.c:36" \
-ex "b main.c:43" \
-ex "b main.c:56" \
-ex "b main.c:63" \
-ex "b main.c:73" \
-ex "b main.c:80" \
-ex "b main.c:20" \
-ex "b sched.c:100"
endef

#
# Gdb via serial
#
gdb: ${EXEC}
	@if pgrep openocd >/dev/null; then \
		gdb-multiarch ${EXEC} \
			-ex "target extended-remote :3333" \
			-ex "load" \
			${BREAKS} \
			-ex "load"; \
	else \
		gdb-multiarch -b 115200 ${EXEC} \
			-ex "target remote ${TTY}" \
			-ex "load" \
			${BREAKS} \
			-ex "load"; \
	fi

#
# Gdb via qemu
#
gdbqemu: ${EXEC}
	gdb-multiarch -ex "target extended-remote :1234" \
					  -ex "set architecture arm" \
					  -ex "load" \
					  ${BREAKS} \
					  -ex "load" \
					  ${EXEC}

#
# Elimina os processos
#
kill:
	-killall openocd
	-killall gdb-multiarch
	-killall -9 qemu-system-arm