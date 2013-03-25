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

# The prefix & for tools.
PREFIX ?= ./Tools/bin
ARMGNU ?= $(PREFIX)/arm-none-eabi

# Some color codes.
# If stdout isn't a terminal for you, just delete the part in front of ?= for the following.
Blue ?= \033[34m
Green ?= \033[92m
End ?= \033[0m

# Get list of warnings and cflags.
CFLAGS := -std=c99 -Wall -Wextra -nostdlib -ffreestanding -lgcc -O2

# Get a list of source files and header files.
KERNELC := $(shell find Source/Kernel -type f -name "*.c")
KERNELASM := $(shell find Source/Kernel -type f -name "*.S")
KERNELHDR := $(shell find Source/Kernel/Include -type f -name "*.h")

# Get the object files.
KERNELOBJ := $(patsubst %.S,%.o,$(KERNELASM)) $(patsubst %.c,%.o,$(KERNELC))

# Get the dependancies.
KERNELDEP := $(patsubst %.c,%.d,$(KERNELC))

# List phony targets.
.PHONY: all clean todo

# The default target.
all: Tart.kern

# Tart.kern target (in reality, this is the Kernel).
Tart.kern: $(KERNELOBJ) Source/Kernel/Link.ld
	@echo -e "  $(Blue)[LD]$(End)    Tart.elf"
	@$(ARMGNU)-ld $(KERNELOBJ) -TSource/Kernel/Link.ld -o Tart.elf

	@echo -e "  $(Blue)[OBJ]$(End)   Tart.kern"
	@$(ARMGNU)-objcopy Tart.elf -O binary Tart.kern

# Include $(KERNELDEP).
-include $(KERNELDEP)

# Clean.
clean: 
	@$(foreach File,$(KERNELOBJ),echo "Removed" $(File);)
	@$(foreach File,$(KERNELDEP),echo "Removed" $(File);)

	@echo "Removed Tart.elf"
	@echo "Removed Tart.kern"

	-@$(RM) $(wildcard $(KERNELOBJ) $(KERNELDEP) Tart.elf Tart.kern)

# CC.
%.o: %.c Makefile
	@echo -e "  $(Blue)[CC]$(End)   " $<
	@$(ARMGNU)-gcc $(CFLAGS) -ISource/Kernel/Include -ISource/Include -MMD -MP -c $< -o $@

# AS.
%.o: %.S Makefile
	@echo -e "  $(Blue)[AS]$(End)   " $<
	@$(ARMGNU)-as -c $< -o $@