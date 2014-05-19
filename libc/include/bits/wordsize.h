#ifndef _WORDSIZE_H
#define _WORDSIZE_H

#ifdef __arm__
    #define __WORDSIZE      32
#else
    #error "Architecture not recognized."
#endif

#endif /* _WORDSIZE_H */
