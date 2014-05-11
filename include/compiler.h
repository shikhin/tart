#ifndef _COMPILER_H
#define _COMPILER_H

#ifdef __GNUC__
    #define __PACKED        __attribute__ ((packed))
    #define __UNUSED        __attribute__ ((unused))
    #define __ALIGNED(x)    __attribute__ ((aligned (x)))
#else
    #error Compiler not supported.
#endif

#endif /* _COMPILER_H */
