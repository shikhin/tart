# Rules for Tart.

# Get the arch.
# TODO: error if not allowed.
ifeq ($(TARGET), bcm2835)
    ARCH := arm
    SUBARCH := armv6k
    CPU := arm1176jzf-s

    # Set CFLAGS.
    CFLAGS := -mfpu=vfp -mfloat-abi=hard -march=$(SUBARCH) -mcpu=$(CPU)
endif

# Get the tool prefix.
# TODO: error if not allowed.
ifeq ($(ARCH), arm)
    TOOLPREFIX := $(PREFIX)/arm-none-eabi

    HOSTCC := $(TOOLPREFIX)-gcc
    HOSTAS := $(TOOLPREFIX)-as
    HOSTLD := $(TOOLPREFIX)-ld
    HOSTOBJCOPY := $(TOOLPREFIX)-objcopy
endif