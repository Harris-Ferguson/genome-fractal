# makefile for cgr
# Author: Harris Ferguson

# C Compiler Flags
CFLAGS = -Wall -Wextra -lnetpbm -g 
# Linker Flags
LDFLAGS = 

.PHONY: all cgr clean

CGR_OBJ = cgr.o cgr_aux.o cgr_plot.o

all: cgr

cgr: $(CGR_OBJ)
	gcc $(CGR_OBJ) -o cgr

cgr.o: cgr.c cgr.h
cgr_aux.o: cgr_aux.c cgr_aux.h
cgr_plot.o: cgr_plot.c cgr_plot.h

clean:
	rm -f cgr.o cgr_aux.o cgr_plot.o
