PREFIX = '/usr/local'
DESTDIR = ''

#CFLAGS = -O2 -Wall -ansi -pedantic -static --std=c18
#CFLAGS := -O2 -Wall -ansi -pedantic
CFLAGS := -O2 -Wall -pedantic -std=c11
LDLIBS := -lSDL2 -lm
ELF_FLAGS := $(CFLAGS) $(LDLIBS)

BINARIES = $(notdir $(basename $(SOURCES)))

#ELFS = $(addsuffix .elf,$(addprefix source/,$(BINARIES)))
ELFS = $(addsuffix .elf,$(BINARIES))

elf: $(ELFS)
%.elf: source/%.c
	$(CC) $^ -o $@ $(ELF_FLAGS)

clean_elf:
	rm -f $(ELFS)
