# Rules for Tart.

# Get the arch.
# TODO: error if not allowed.
ifeq ($(TARGET), BCM2835)
    ARCH = ARM
    SUBARCH = armv6k
    CPU = arm1176jzf-s
endif

# Get the tool prefix.
# TODO: error if not allowed.
ifeq ($(ARCH), ARM)
    TOOL_PREFIX = $(PREFIX)/arm-none-eabi
endif

# If terminal output, get colors.
ifeq ($(TERM_OUTPUT), true)
    BLUE = \033[34m
    GREEN = \033[92m
    END = \033[0m
endif