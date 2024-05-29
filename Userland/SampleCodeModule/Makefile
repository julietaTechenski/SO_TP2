include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_MODULES=$(wildcard shell/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_COMMANDS=$(wildcard shell/commands/*.c)
SOURCES_TESTING=$(wildcard shell/testing/*.c)

OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM) $(SOURCES_MODULES) $(SOURCES_COMMANDS) $(SOURCES_TESTING)
	$(GCC) $(GCCFLAGS) -I./include -I./shell/include  -T sampleCodeModule.ld _loader.c $(SOURCES) $(SOURCES_MODULES) $(OBJECTS_ASM) $(SOURCES_COMMANDS) $(SOURCES_TESTING) -o ../$(MODULE) -fno-pic -fPIE
	$(GCC) $(GCCFLAGS) -I./include -I./shell/include  -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(SOURCES_MODULES) $(OBJECTS_ASM) $(SOURCES_COMMANDS) $(SOURCES_TESTING) -o ../0000-sampleCodeModule.elf -fno-pic -fPIE

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o *.o */*.elf

.PHONY: all clean print
