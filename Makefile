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

# Include the config and rules.
include Config.mk
include Rules.mk
include Target/$(TARGET)/Rules.mk

OUTELF = Tart
OUTBIN = $(OUTELF).bin

# List of CFLAGS.
CFLAGS += -std=c99 -Wall -Wextra -nostdlib -ffreestanding -lgcc -O2

# Get a list of source files.
CSRC = $(shell find Arch/$(ARCH) -type f -name "*.c") $(shell find Kernel -type f -name "*.c")  \
       $(shell find Target/$(TARGET) -type f -name "*.c")

ASMSRC = $(shell find Arch/$(ARCH) -type f -name "*.S") $(shell find Kernel -type f -name "*.S")  \
       $(shell find Target/$(TARGET) -type f -name "*.S")

# Get the object files.
OBJ = $(patsubst %.S,%.o,$(ASMSRC)) $(patsubst %.c,%.o,$(CSRC))

# Get the dependancies.
DEP = $(patsubst %.c,%.d,$(CSRC))

# Linker script.
LINK = Arch/$(ARCH)/Link.ld

# Make related files.
MAKEDEPS = Makefile Rules.mk Config.mk Target/$(TARGET)/Rules.mk

# List phony targets.
.PHONY: all clean

# The default target.
all: $(OUTFORMAT)

# ELF output.
$(OUTELF): $(OBJ) $(LINK)
	$(TOOLPREFIX)-ld $(OBJ) -T$(LINK) -o $@

# Binary output.
$(OUTBIN): $(OUTELF)
	$(TOOLPREFIX)-objcopy $(OUTELF) -O binary $@

# Include dependancy files.
-include $(DEP)

# Clean.
clean: 
	-$(RM) $(wildcard $(OBJ) $(DEP) $(OUTELF) $(OUTFORMAT))

# CC.
%.o: %.c $(MAKEDEPS)
	$(TOOLPREFIX)-gcc $(CFLAGS) -IInclude -IKernel/Include -IArch/$(ARCH)/Include -ITarget/$(TARGET)/Include -MMD -MP -c $< -o $@

# AS.
%.o: %.S $(MAKEDEPS)
	$(TOOLPREFIX)-as -c $< -o $@