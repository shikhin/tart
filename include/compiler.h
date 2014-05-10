#ifndef _COMPILER_H
#define _COMPILER_H

#if __GNUC__
    #define __PACKED    __attribute__((packed))
#else
    #error Compiler not supported.
#endif

#endif /* _COMPILER_H */
