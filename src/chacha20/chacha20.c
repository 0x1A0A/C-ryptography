#include "util.h"
#include "chacha20.h"

#include <string.h>
#include <stdlib.h>

#define QR(a, b, c, d) (			\
	a += b,  d ^= a,  d = ROT_L(d,16),	\
	c += d,  b ^= c,  b = ROT_L(b,12),	\
	a += b,  d ^= a,  d = ROT_L(d, 8),	\
	c += d,  b ^= c,  b = ROT_L(b, 7))

#define ROUNDS 20

chacha20_ptr chacha20_create()
{
	chacha20_ptr neww = (chacha20_ptr)malloc(sizeof(chacha20_t));
	neww->index = 0;
	return neww;
}

void chacha20_destroy(chacha20_ptr ctx)
{
	free(ctx);
}

void chacha20_block_init(chacha20_ptr ctx, byte key[32], byte nonce[12])
{
	strncpy( (byte*)(ctx->block.cc_const), "expand 32-byte k", 16 );
	strncpy( (byte*)(ctx->block.key), key, 32 );
	ctx->block.counter = 0;
	strncpy( (byte*)(ctx->block.nonce), nonce,12 );
}

void chacha20_key_stream_gen(chacha20_ptr ctx)
{
	memcpy(ctx->stream, &(ctx->block), 64);
	word *cell = ctx->stream;
	for (size_t i=0; i<ROUNDS; i+=2) {
		QR(cell[0], cell[4], cell[ 8], cell[12]); // column 0
		QR(cell[1], cell[5], cell[ 9], cell[13]); // column 1
		QR(cell[2], cell[6], cell[10], cell[14]); // column 2
		QR(cell[3], cell[7], cell[11], cell[15]); // column 3
		// Even round
		QR(cell[0], cell[5], cell[10], cell[15]); // diagonal 1
		QR(cell[1], cell[6], cell[11], cell[12]); // diagonal 2
		QR(cell[2], cell[7], cell[ 8], cell[13]); // diagonal 3
		QR(cell[3], cell[4], cell[ 9], cell[14]); // diagonal 4
	}

	// mix it with original block (ADD)
	for (size_t i=0; i<16; ++i) {
		cell[i] += ((word*)&(ctx->block))[i];
	}
}
