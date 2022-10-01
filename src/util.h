#ifndef __DFALSE_CRYPTO_UITL_H__
#define __DFALSE_CRYPTO_UITL_H__

#include <stdint.h>
#include <stddef.h>

#define ROT_L(a,n) ( a<<n ) | ( a>>(32-n) )
#define ROT_R(a,n) ( a>>n ) | ( a<<(32-n) )

typedef uint8_t byte;
typedef uint32_t word;

#endif