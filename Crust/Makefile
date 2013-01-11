 #
 # Tart build system (for Crust and loader).
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
PREFIX ?= ./../Tools/bin
ARMGNU ?= $(PREFIX)/arm-none-eabi

# Some color codes.
# If stdout isn't a terminal for you, just delete the part in front of ?= for the following.
Blue ?= \033[34m
Green ?= \033[92m
End ?= \033[0m

# Get list of warnings and cflags.
TARTCFLAGS := -std=c99 -Wall -Wextra -pedantic -O2 -Wshadow -Wcast-align \
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

CRUSTCFLAGS := -std=c99 -Wall -Wextra -pedantic -O2 -Wno-unused-parameter

# Get a list of source files and header files.
LOADERC := $(shell find Loader -type f -name "*.c")
LOADERASM := $(shell find Loader -type f -name "*.S")
LOADERHDR := $(shell find Loader/Include -type f -name "*.h")

CRUSTC := Main.c

# Get the object files.
LOADEROBJ := $(patsubst %.S,%.o,$(LOADERASM)) $(patsubst %.c,%.o,$(LOADERC))

# Get the dependancies.
LOADERDEP := $(patsubst %.c,%.d,$(LOADERC))
CRUSTDEP := Crust.d

# List phony targets.
.PHONY: all clean todo

# The default target.
all: Crust Loader.img

# Crust target.
Crust: $(CRUSTC)
	@echo -e "  $(Blue)[CC]$(End)    Crust"
	@$(CC) -IInclude $(CRUSTCFLAGS) -MMD -MP $(CRUSTC) -o Crust

# Include $(CRUSTDEP).
-include $(CRUSTDEP)

# Loader target.
Loader.img: $(LOADEROBJ) Loader/Link.ld
	@echo -e "  $(Blue)[LD]$(End)    Loader.elf"
	@$(ARMGNU)-ld $(LOADEROBJ) -TLoader/Link.ld -o Loader.elf

	@echo -e "  $(Blue)[OBJ]$(End)   Loader.img"
	@$(ARMGNU)-objcopy Loader.elf -O binary Loader.img

# Include $(LOADERDEP).
-include $(LOADERDEP)

# CC.
%.o: %.c Makefile
	@echo -e "  $(Blue)[CC]$(End)   " $<
	@$(ARMGNU)-gcc $(CFLAGS) -ILoader/Include -IInclude -MMD -MP -c $< -o $@

# AS.
%.o: %.S Makefile
	@echo -e "  $(Blue)[AS]$(End)   " $<
	@$(ARMGNU)-as -c $< -o $@

# Clean.
clean: 
	@$(foreach File,$(CRUSTDEP),echo "Removed" $(File);)
	@echo "Removed Crust"

	-@$(RM) $(wildcard $(CRUSTDEP) Crust)

	@$(foreach File,$(LOADEROBJ),echo "Removed" $(File);)
	@$(foreach File,$(LOADERDEP),echo "Removed" $(File);)

	@echo "Removed Loader.elf"
	@echo "Removed Loader.img"

	-@$(RM) $(wildcard $(LOADEROBJ) $(LOADERDEP) Loader.elf Loader.img)
