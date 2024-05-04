include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_MODULES=$(wildcard shell/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_COMMANDS=$(wildcard shell/commands/*.c)

OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM) $(SOURCES_MODULES) $(SOURCES_COMMANDS)
	$(GCC) $(GCCFLAGS) -I./include -I./shell/include  -T sampleCodeModule.ld _loader.c $(SOURCES) $(SOURCES_MODULES) $(OBJECTS_ASM) $(SOURCES_COMMANDS) -o ../$(MODULE) -fno-pic

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o *.o

.PHONY: all clean print
