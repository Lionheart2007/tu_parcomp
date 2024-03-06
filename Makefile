CC = gcc
CFLAGS = -Wall -fopenmp -Wextra -Iutil

# Directories
SRCDIRS := $(shell find code_examples -type d)

# Source files
UTILSRC := util/util.c
STENCILSRC := $(wildcard code_examples/*/*.c)

# Object files
UTILOBJECTS := $(UTILSRC:.c=.o)
STENCILOBJECTS := $(STENCILSRC:.c=.o)

# Executables
STENCILBINARIES := $(STENCILOBJECTS:.o=)

all: $(STENCILBINARIES)

$(STENCILBINARIES): $(STENCILSRC) $(UTILOBJECTS)
	$(CC) $(CFLAGS) $@.c $(UTILOBJECTS) -o $@.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(UTILOBJECTS) $(STENCILOBJECTS) $(STENCILBINARIES)
