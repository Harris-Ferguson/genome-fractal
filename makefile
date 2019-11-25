# makefile for cgr
# Author: Harris Ferguson

# C Compiler Flags
CFLAGS = -Wall -Wextra -g 
# Linker Flags
LDFLAGS = -lnetpbm -o cgr

.PHONY: all clean

CGR_OBJ = cgr.o cgr_aux.o cgr_plot.o

all: cgr

cgr: $(CGR_OBJ)

cgr.o: cgr.c cgr.h
cgr_aux.o: cgr_aux.c cgr_aux.h
cgr_plot.o: cgr_plot.c cgr_plot.h

clean:
	rm -f cgr.o cgr_aux.o cgr_plot.o
