#ifndef __DF_SHA256_H__
#define __DF_SHA256_H__

#include "../util.h"


struct df_sha256context {
	byte data_chunk[64];
	word state[8];
	uint64_t total_bit;
	uint16_t current_index; // current index of 
};

typedef struct df_sha256context sha256_t, *sha256_ptr;

// manual
//		create sha256contex
sha256_ptr sha256context_create();
//		add input to sha256contex
uint8_t sha256context_add(sha256_ptr ctx, byte *in, size_t length);
//		produce final output
uint8_t sha256context_end(sha256_ptr ctx, byte out[32]);
// 		destroy sha256 contex
void sha256context_destroy(sha256_ptr ctx);


#endif