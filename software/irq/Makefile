
# /*******************************************************************************
# **                                                                             *
# **    Project: IIC Multiple Bus Controller (IICMB)                             *
# **                                                                             *
# **    File:    Makefile IRQ based driver for IICMB                             *
# **    Version:                                                                 *
# **             1.0,     Feb 24, 2023                                           *
# **                                                                             *
# **    Author:  Andreas Kaeberlein, (andreas.kaeberlein@siemens.com)            *
# **                                                                             *
# ********************************************************************************
# ********************************************************************************
# ** Copyright (c) 2023, Sergey Shuvalkin                                        *
# ** All rights reserved.                                                        *
# **                                                                             *
# ** Redistribution and use in source and binary forms, with or without          *
# ** modification, are permitted provided that the following conditions are met: *
# **                                                                             *
# ** 1. Redistributions of source code must retain the above copyright notice,   *
# **    this list of conditions and the following disclaimer.                    *
# ** 2. Redistributions in binary form must reproduce the above copyright        *
# **    notice, this list of conditions and the following disclaimer in the      *
# **    documentation and/or other materials provided with the distribution.     *
# **                                                                             *
# ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
# ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
# ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  *
# ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE    *
# ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         *
# ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        *
# ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
# ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     *
# ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     *
# ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  *
# ** POSSIBILITY OF SUCH DAMAGE.                                                 *
# *******************************************************************************/



# select compiler
CC = gcc

# set linker
LINKER = gcc

# set compiler flags
ifeq ($(origin CFLAGS), undefined)
  CFLAGS = -c -O -Wall -Wextra -Wconversion -I . -I ../
endif

# linking flags here
ifeq ($(origin LFLAGS), undefined)
  LFLAGS = -Wall -Wextra -I. -lm
endif


all: iicmb_test


iicmb_test: iicmb_test.o iicmb.o
	$(LINKER) ./obj/iicmb_test.o ./obj/iicmb.o $(LFLAGS) -o ./test/iicmb_test

iicmb.o: ./iicmb.c
	$(CC) $(CFLAGS) -DIICMB_PRINTF_EN ./iicmb.c -o ./obj/iicmb.o

iicmb_test.o: ./test/iicmb_test.c
	$(CC) $(CFLAGS) ./test/iicmb_test.c -o ./obj/iicmb_test.o

ci: ./iicmb.c
	$(CC) $(CFLAGS) -Werror ./iicmb.c -o ./obj/iicmb.o

clean:
	rm -f ./obj/*.o ./test/iicmb_test
