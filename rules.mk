# Rules for Tart.

# Get the arch.
ifeq ($(TARGET), bcm2835)
    ARCH := arm
    SUBARCH := armv6k
    CPU := arm1176jzf-s

    # Set CFLAGS.
    CFLAGS := -mfpu=vfp -mfloat-abi=hard -march=$(SUBARCH) -mcpu=$(CPU) -DARMV=6
else
    $(error TARGET set to $(TARGET), which is not a recognized target)
endif

# Get the tool prefix.
ifeq ($(ARCH), arm)
    TOOLPREFIX := $(PREFIX)/arm-none-eabi

    HOSTCC := $(TOOLPREFIX)-gcc
    HOSTAS := $(TOOLPREFIX)-as
    HOSTLD := $(TOOLPREFIX)-ld
    HOSTOBJCOPY := $(TOOLPREFIX)-objcopy
else
    $(error ARCH set to $(ARCH), which is not a recognized architecture)
endif