 #
 # Tart build system (for Crust)
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

# Some color codes.
# If stdout isn't a terminal for you, just delete the part in front of ?= for the following.
Blue ?= \033[34m
Green ?= \033[92m
End ?= \033[0m

# Get list of warnings and cflags.
CFLAGS := -std=c99 -Wall -Wextra -pedantic -O2 -Wno-unused-parameter

# Get a list of source files and header files.
CRUSTC := $(shell find ./ -type f -name "*.c")
CRUSTHDR := $(shell find ./Include -type f -name "*.h")

# Get the object files.
CRUSTOBJ := $(patsubst %.c,%.o,$(CRUSTC))

# Get the dependancies.
CRUSTDEP := $(patsubst %.c,%.d,$(CRUSTC))

# List phony targets.
.PHONY: all clean todo

# The default target.
all: crust

# Crust target.
crust: $(CRUSTOBJ)
	@echo -e "  $(Blue)[LD]$(End)    Crust"
	@$(CC) $(CRUSTOBJ) -o Crust

# Include $(CRUSTDEP).
-include $(CRUSTDEP)

# Clean.
clean: 
	@$(foreach File,$(CRUSTOBJ),echo "Removed" $(File);)
	@$(foreach File,$(CRUSTDEP),echo "Removed" $(File);)

	@echo "Removed Crust"

	-@$(RM) $(wildcard $(CRUSTOBJ) $(CRUSTDEP) Crust)

# CC.
%.o: %.c Makefile
	@echo -e "  $(Blue)[CC]$(End)   " $<
	@$(CC) $(CFLAGS) -IInclude -MMD -MP -c $< -o $@