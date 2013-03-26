# Configuration file for Tart build.

# The prefix for the cross-compiler and binutils.
# DEFAULT: Tools created by crosstools.sh.
PREFIX := ./tools/bin

# The target.
# Allowed targets: bcm2835.
# DEFAULT: bcm2835.
TARGET := bcm2835

# CC flags.
# User can adopt these to his own, and uncomment the line.
#CCFLAGS := -O2