# OUTELF and OUTBIN.
OUTELF := tart
OUTBIN := $(OUTELF).bin

CCFLAGS ?= -O2

# Include the config and rules.
include config.mk
include target/$(TARGET)/rules.mk
include platform/$(PLATFORM)/rules.mk
include rules.mk

# List of CFLAGS.
CFLAGS += -std=c99 -Wall -Wextra -nostdlib -ffreestanding -lgcc $(CCFLAGS)

# Includes.
CPPFLAGS := -Iinclude -Ikernel/include -Iarch/$(ARCH)/include -Itarget/$(TARGET)/include -Iplatform/$(PLATFORM)/include

# Get a list of source files.
CSRC := $(shell find arch/$(ARCH) -type f -name "*.c") $(shell find kernel -type f -name "*.c")  \
       $(shell find target/$(TARGET) -type f -name "*.c") $(shell find platform/$(PLATFORM) -type f -name "*.c")

ASMSRC := $(shell find arch/$(ARCH) -type f -name "*.S") $(shell find kernel -type f -name "*.S")  \
       $(shell find target/$(TARGET) -type f -name "*.S") $(shell find platform/$(PLATFORM) -type f -name "*.S")

# Get the object files.
OBJ := $(patsubst %.S,%.o,$(ASMSRC)) $(patsubst %.c,%.o,$(CSRC))

# Get the dependancies.
DEP := $(patsubst %.S,%.d,$(ASMSRC)) $(patsubst %.c,%.d,$(CSRC))

# Linker script.
LINK := arch/$(ARCH)/link.ld

# Make related files.
MAKEDEPS := Makefile rules.mk config.mk target/$(TARGET)/rules.mk platform/$(PLATFORM)/rules.mk

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
