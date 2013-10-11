 #
 # Tart build system.
 #
 # Copyright (c) 2013, Shikhin Sethi
 #
 # Permission is hereby granted, free of charge, to any person obtaining a
 # copy of this software and associated documentation files (the "Software"),
 # to deal in the Software without restriction, including without limitation 
 # the rights to use, copy, modify, merge, publish, distribute, sublicense,
 # and/or sell copies of the Software, and to permit persons to whom the Software
 # is furnished to do so, subject to the following conditions:
 #
 #     * The above copyright notice and this permission notice shall be included in
 #       all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 # WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 # OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 #

# OUTELF and OUTBIN.
OUTELF := tart
OUTBIN := $(OUTELF).bin

CCFLAGS ?= -O2

# Include the config and rules.
include config.mk
include target/$(TARGET)/rules.mk
include rules.mk

# List of CFLAGS.
CFLAGS += -std=c99 -Wall -Wextra -nostdlib -ffreestanding -lgcc

# Includes.
CPPFLAGS := -Iinclude -Ikernel/include -Iarch/$(ARCH)/include -Itarget/$(TARGET)/include

# Get a list of source files.
CSRC := $(shell find arch/$(ARCH) -type f -name "*.c") $(shell find kernel -type f -name "*.c")  \
       $(shell find target/$(TARGET) -type f -name "*.c")

ASMSRC := $(shell find arch/$(ARCH) -type f -name "*.S") $(shell find kernel -type f -name "*.S")  \
       $(shell find target/$(TARGET) -type f -name "*.S")

# Get the object files.
OBJ := $(patsubst %.S,%.o,$(ASMSRC)) $(patsubst %.c,%.o,$(CSRC))

# Get the dependancies.
DEP := $(patsubst %.S,%.d,$(ASMSRC)) $(patsubst %.c,%.d,$(CSRC))

# Linker script.
LINK := arch/$(ARCH)/link.ld

# Make related files.
MAKEDEPS := Makefile rules.mk config.mk target/$(TARGET)/rules.mk

# The default target.
all: $(OUTFORMAT)

# List phony targets.
.PHONY: all clean

# ELF output.
$(OUTELF): $(OBJ) $(LINK)
	$(HOSTLD) $(OBJ) -T$(LINK) -o $@

# Binary output.
$(OUTBIN): $(OUTELF)
	$(HOSTOBJCOPY) $(OUTELF) -O binary $@

# Include dependancy files.
-include $(DEP)

# Clean.
clean: 
	-$(RM) $(wildcard $(OBJ) $(DEP) $(OUTELF) $(OUTFORMAT))

# Dog.
dog:
	$(warning Experimental dog generator. Don't try it out; the default size isn't set, so-)

# CC.
%.o: %.c $(MAKEDEPS)
	$(HOSTCC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

# AS.
%.o: %.S $(MAKEDEPS)
	$(HOSTCC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@
