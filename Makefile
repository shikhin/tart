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
CFLAGS := -std=c99 -Wall -Wextra -pedantic -O2 -Wshadow -Wcast-align \
          -Wwrite-strings -Wredundant-decls -Wnested-externs \
          -Winline -Wno-attributes -Wno-deprecated-declarations \
          -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal -Wformat=2 \
          -Wno-format-extra-args -Winit-self -Winvalid-pch \
          -Wmissing-format-attribute -Wmissing-include-dirs \
          -Wno-multichar \
          -Wno-pointer-to-int-cast -Wredundant-decls \
          -Wshadow -Wno-sign-compare \
          -Wswitch -Wsystem-headers -Wundef \
          -Wno-pragmas -Wno-unused-but-set-parameter -Wno-unused-but-set-variable \
          -Wno-unused-result -Wwrite-strings -Wdisabled-optimization \
          -Werror -pedantic-errors -Wpointer-arith -nostdlib \
          -nostartfiles -ffreestanding \
          -nodefaultlibs -fno-builtin -fomit-frame-pointer -O2

# Get a list of source files and header files.
LOADERC := $(shell find Source/Loader -type f -name "*.c")
LOADERASM := $(shell find Source/Loader -type f -name "*.S")
LOADERHDR := $(shell find Source/Loader/Include -type f -name "*.h")

# Get the object files.
LOADEROBJ := $(patsubst %.S,%.o,$(LOADERASM)) $(patsubst %.c,%.o,$(LOADERC))

# Get the dependancies.
LOADERDEP := $(patsubst %.c,%.d,$(LOADERC))

# List phony targets.
.PHONY: all clean todo

# The default target.
all: kernel.img

# Kernel.img target (in reality, this is the loader).
kernel.img: $(LOADEROBJ) Source/Loader/Link.ld
	@echo -e "  $(Blue)[LD]$(End)    kernel.elf"
	@$(ARMGNU)-ld $(LOADEROBJ) -TSource/Loader/Link.ld -o kernel.elf

	@echo -e "  $(Blue)[OBJ]$(End)   kernel.img"
	@$(ARMGNU)-objcopy kernel.elf -O binary kernel.img

# Include $(LOADERDEP).
-include $(LOADERDEP)

# Clean.
clean: 
	@$(foreach File,$(LOADEROBJ),echo "Removed" $(File);)
	@$(foreach File,$(LOADERDEP),echo "Removed" $(File);)

	@echo "Removed kernel.elf"
	@echo "Removed kernel.img"

	-@$(RM) $(wildcard $(LOADEROBJ) $(LOADERDEP) kernel.elf kernel.img)

# CC.
%.o: %.c Makefile
	@echo -e "  $(Blue)[CC]$(End)   " $<
	@$(ARMGNU)-gcc $(CFLAGS) -ISource/Loader/Include -ISource/Include -MMD -MP -c $< -o $@

# AS.
%.o: %.S Makefile
	@echo -e "  $(Blue)[AS]$(End)   " $<
	@$(ARMGNU)-as -c $< -o $@
