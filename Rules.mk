# Rules for Tart.

# Get the arch.
# TODO: error if not allowed.
ifeq ($(TARGET), BCM2835)
    ARCH = ARM
    SUBARCH = armv6k
    CPU = arm1176jzf-s

    # Set CFLAGS.
    CFLAGS = -mfpu=vfp -mfloat-abi=hard -march=$(SUBARCH) -mcpu=$(CPU)
endif

# Get the tool prefix.
# TODO: error if not allowed.
ifeq ($(ARCH), ARM)
    TOOLPREFIX = $(PREFIX)/arm-none-eabi
endif