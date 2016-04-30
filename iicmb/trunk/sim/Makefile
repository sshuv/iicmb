
# Makefile for IICMB project

# VHDL compiler name:
VCOM               = vcom

# Verilog compiler name:
VLOG               = vlog

# path to the sources:
IICMB_DIR          = ..

# paths to compiled libraries:
LIB_IICMB          = iicmb
LIB_IICMB_TB       = work


include ./include.mk


.PHONY : all
all : $(ALL_TARGETS)


.PHONY : clean
clean : $(CLEAN_TARGETS)
	rm -rf transcript vsim.wlf;

