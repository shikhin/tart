 #
 # Tart build system.
 #
 # Copyright (c) 2013, Shikhin Sethi
 # 
 # This software is provided 'as-is', without any express or implied
 # warranty. In no event will the authors be held liable for any damages
 # arising from the use of this software.
 #
 # Permission is granted to anyone to use this software for any purpose,
 # including commercial applications, and to alter it and redistribute it
 # freely, subject to the following restrictions:
 #
 #     1. The origin of this software must not be misrepresented; you must not
 #        claim that you wrote the original software. If you use this software
 #        in a product, an acknowledgment in the product documentation would be
 #        appreciated but is not required.
 #
 #     2. Altered source versions must be plainly marked as such, and must not be
 #        misrepresented as being the original software.
 #
 #     3. This notice may not be removed or altered from any source
 #        distribution.
 #

# The prefix & for tools.
PREFIX ?= ./Tools/bin
ARMGNU ?= $(PREFIX)/arm-none-eabi

# Some color codes.
Blue ?= \033[34m
Green ?= \033[92m
End ?= \033[0m

# Ensure colors are only for terminal output.
ifeq ($(-t 1),0)
    Blue=''
    End=''
    Green=''
endif

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
