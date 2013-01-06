/* 
 * This contains standard integer types (intx_t, uintx_t) definitions.
 *
 * Copyright (c) 2013, Shikhin Sethi
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *        claim that you wrote the original software. If you use this software
 *        in a product, an acknowledgment in the product documentation would be
 *        appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *        misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *        distribution.
 */

#ifndef _STDINT_H
#define _STDINT_H

/* intx_t */
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long long int int64_t;

/* uintx_t */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

/* xptr_t */
typedef int intptr_t;

typedef unsigned int uintptr_t;
typedef uint32_t size_t;

#endif /* _STDINT_H */
