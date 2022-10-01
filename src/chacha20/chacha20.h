#ifndef __DF_CHACHA20_H__
#define __DF_CHACHA20_H__

#include "../util.h"

struct chacha_block {
	word cc_const[4], key[8], counter, nonce[3];
};

struct chacha20  {
	word stream[16];
	struct chacha_block block;
	uint32_t index;
};

typedef chacha20_block chacha_block_t;
typedef chacha20 chacha20_t, *chacha20_ptr;

chacha20_ptr chacha20_create();
void chacha20_destroy(chacha20_ptr ctx);
void chacha20_block_init(chacha20_ptr ctx, byte key[32], byte nonce[12]);
void chacha20_key_stream_gen(chacha20_ptr ctx);

#endif
