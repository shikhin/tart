#!/bin/sh

# The download a file function.
function download()
{
    local url=$1
    
    # Download the file.
    wget  --progress=dot -c -P tools $url 2>&1 | grep --line-buffered "%" | sed -u -e "s,\.,,g" -e "s,\,,,g" | awk '{printf("\b\b\b\b%4s", $2)}'

    # Delete everything and show done.
    echo -ne "\b\b\b\b"
    echo -e " $Green[DONE]$End"
}

# Some color codes.
Blue='\033[34m'
Green='\033[92m'
End='\033[0m'

# Ensure colors are only there for terminal output.
if [[ ! -t 1 ]]; then
    Blue=''
    End=''
    Green=''
fi

# Export some common stuff - the defaults, if they aren't in the arguments.
PREFIX ?= $(readlink -f ./tools)

# TARGET allowed values.
# For everything ARM (currently: RPi [BCM2835]): bcm2835
TARGET ?= bcm2835

# Parse command line options for prefix & target.
while getopts "p:t:" optname
    do
      case "$optname" in
        "p")
          PREFIX := $OPTARG
          ;;
        "t")
          TARGET := $OPTARG 
          ;;
      esac
    done

ifeq ($(TARGET),bcm2835)
    TARGET := arm-none-eabi
endif

# Make the build directory.
mkdir -p tools

# Get the tools.

# Binutils.
echo -ne "  $Blue[WGET]$End  tools/binutils-2.23.1.tar.bz2,    "
download "http://ftp.gnu.org/gnu/binutils/binutils-2.23.1.tar.bz2"

# GCC.
echo -ne "  $Blue[WGET]$End  tools/gcc-4.7.2.tar.bz2,    "
download "http://ftp.gnu.org/gnu/gcc/gcc-4.7.2/gcc-4.7.2.tar.bz2"

# Untar them.

# Binutils.
echo -e "  $Blue[UNTAR]$End tools/binutils-2.23.1.tar.bz2"
tar -xf tools/binutils-2.23.1.tar.bz2 -C tools >/dev/null
rm tools/binutils-2.23.1.tar.bz2

# GCC.
echo -e "  $Blue[UNTAR]$End tools/gcc-4.7.2.tar.bz2"
tar -xf tools/gcc-4.7.2.tar.bz2 -C tools >/dev/null
rm tools/gcc-4.7.2.tar.bz2

# Build the tools.

# Binutils.
mkdir -p tools/build-binutils

# Configure.
echo -e "  $Blue[BINUT]$End Configuring"
cd tools/build-binutils && ../binutils-2.23.1/configure --target=$TARGET --prefix=$PREFIX --disable-nls
cd ../../

# Compile.
echo -e "  $Blue[BINUT]$End Compiling"
make -C tools/build-binutils all

# Install.
echo -e "  $Blue[BINUT]$End Installing"
make -C tools/build-binutils install

# Clean.
echo -e "  $Blue[BINUT]$End Cleaning"
rm -rf tools/build-binutils tools/binutils-2.23.1

# GCC.
mkdir -p tools/build-gcc

# Configure.
echo -e "  $Blue[GCC]$End   Configuring"
export PATH=$PATH:$PREFIX/bin
cd tools/build-gcc && ../gcc-4.7.2/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers --with-gnu-as --with-gnu-ld
cd ../../

export LD_FOR_TARGET=$PREFIX/bin/$TARGET-elf-ld
export OBJDUMP_FOR_TARGET=$PREFIX/bin/$TARGET-elf-objdump
export NM_FOR_TARGET=$PREFIX/bin/$TARGET-elf-nm
export RANLIB_FOR_TARGET=$PREFIX/bin/$TARGET-elf-ranlib
export READELF_FOR_TARGET=$PREFIX/bin/$TARGET-elf-readelf
export STRIP_FOR_TARGET=$PREFIX/bin/$TARGET-elf-strip
export AS_FOR_TARGET=$PREFIX/bin/$TARGET-elf-as

# Compile.
echo -e "  $Blue[GCC]$End   Compiling"
make -C tools/build-gcc all-gcc

# Install.
echo -e "  $Blue[GCC]$End   Installing"
make -C tools/build-gcc install-gcc

# Compile libgcc.
echo -e "  $Blue[LIBGCC]$End Compiling"
make -C tools/build-gcc all-target-libgcc CFLAGS_FOR_TARGET="-g -O2"

# Install
echo -e "  $Blue[LIBGCC]$End Installing"
make -C tools/build-gcc install-target-libgcc

# Clean.
echo -e "  $Blue[GCC]$End   Cleaning"
rm -rf tools/build-gcc tools/gcc-4.7.2