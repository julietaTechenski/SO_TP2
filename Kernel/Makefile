include Makefile.inc

KERNEL=kernel.bin

SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_INT=$(wildcard interruptions/*.c)
SOURCES_MEM=$(wildcard memory/*.c)
SOURCES_PROC=$(wildcard processes/*.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_INTERRUPTS=$(SOURCES_INT:.c=.o)
OBJECTS_MEM=$(SOURCES_MEM:.c=.o)
OBJECTS_PROC=$(SOURCES_PROC:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

MEMORY_MANAGER ?= LIST
GCCFLAGS += -D$(MEMORY_MANAGER)

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEM) $(OBJECTS_PROC)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEM) $(OBJECTS_PROC)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64  -o kernel.elf $(KERNELELF) $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEM) $(OBJECTS_PROC)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ -fno-pic

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o interruptions/*.o */*.elf memory/*.o processes/*.o

.PHONY: all clean
