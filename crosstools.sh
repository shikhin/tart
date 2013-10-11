#!/bin/sh

# The download a file function.
download()
{
    local url=$1
    
    # Download the file.
    wget  --progress=dot -c -P tools $url 2>&1 | grep "%" | sed -u -e "s,\.,,g" -e "s,\,,,g" | awk '{printf("\b\b\b\b%4s", $2)}'

    # Delete everything and show done.
    echo -ne "\b\b\b\b"
    echo " [DONE]"
}

# Export some common stuff - the defaults, if they aren't in the arguments.
test ! -n "$PREFIX" && PREFIX=$(readlink -f ./tools) #Bourne Shell requires there to be no spaces before '='

# TARGET allowed values.
# For everything ARM (currently: RPi [BCM2835]): bcm2835
test ! -n "$TARGET" && TARGET=bcm2835 # See above

# Parse command line options for prefix & target.
while getopts "p:t:" optname
    do
      case "$optname" in
        "p")
          PREFIX=$OPTARG
          ;;
        "t")
          TARGET=$OPTARG 
          ;;
      esac
    done

if test z"$TARGET" = z"bcm2835"
then
    TARGET=arm-none-eabi
fi

# Make the build directory.
mkdir -p tools

# Get the tools.

# Binutils.
echo -n "  [WGET]  tools/binutils-2.23.1.tar.bz2,    "
download "http://ftp.gnu.org/gnu/binutils/binutils-2.23.2.tar.bz2"

# GCC.
echo -n "  [WGET]  tools/gcc-4.7.2.tar.bz2,    "
download "http://ftp.gnu.org/gnu/gcc/gcc-4.8.1/gcc-4.8.1.tar.bz2"

# Untar them.

# Binutils.
echo "  [UNTAR] tools/binutils-2.23.1.tar.bz2"
tar -xf tools/binutils-2.23.1.tar.bz2 -C tools >/dev/null
rm tools/binutils-2.23.1.tar.bz2

# GCC.
echo "  [UNTAR] tools/gcc-4.7.2.tar.bz2"
tar -xf tools/gcc-4.7.2.tar.bz2 -C tools >/dev/null
rm tools/gcc-4.7.2.tar.bz2

# Build the tools.

# Binutils.
mkdir -p tools/build-binutils

# Configure.
echo "  [BINUT] Configuring"
cd tools/build-binutils && ../binutils-2.23.1/configure --target=$TARGET --prefix=$PREFIX --disable-nls
cd ../../

# Compile.
echo "  [BINUT] Compiling"
make -C tools/build-binutils all

# Install.
echo "  [BINUT] Installing"
make -C tools/build-binutils install

# Clean.
echo "  [BINUT] Cleaning"
rm -rf tools/build-binutils tools/binutils-2.23.1

# GCC.
mkdir -p tools/build-gcc

# Configure.
echo "  [GCC]   Configuring"
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
echo "  [GCC]   Compiling"
make -C tools/build-gcc all-gcc

# Install.
echo "  [GCC]   Installing"
make -C tools/build-gcc install-gcc

# Compile libgcc.
echo "  [LIBGCC] Compiling"
make -C tools/build-gcc all-target-libgcc CFLAGS_FOR_TARGET="-g -O2"

# Install
echo "  [LIBGCC] Installing"
make -C tools/build-gcc install-target-libgcc

# Clean.
echo "  [GCC]   Cleaning"
rm -rf tools/build-gcc tools/gcc-4.7.2