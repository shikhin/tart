#ifndef _ASM_H
#define _ASM_H

#define FUNCTION(x) .type x, STT_FUNC; x:
#define DATA(x) .type x, STT_OBJECT; x:
#define GLOBAL(x) .global x;

#endif /* _ASM_H */