# The default output format.
# Allowed: $(OUTBIN), $(OUTELF).
OUTFORMAT := $(OUTBIN)

# Define specific variables for bcm2835.
ARCH := arm
SUBARCH := armv6k
CPU := arm1176jzf-s

# Set CFLAGS.
CFLAGS := -mfpu=vfp -mfloat-abi=hard -march=$(SUBARCH) -mcpu=$(CPU) -DARMV=6
