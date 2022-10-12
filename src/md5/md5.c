#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const word s[] = { 
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

static const word k[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static const word MD5_INIT_STATE[] = {
	0x67452301,
	0xefcdab89,
	0x98badcfe,
	0x10325476,
};

static void md5_init(md5_ptr ctx) 
{
	memset(ctx->data_chunk,0,64);
	ctx->current_index = ctx->total_bits = 0;
	for (int i=0; i<4; ++i) ctx->state[i] = MD5_INIT_STATE[i];
}

md5_ptr md5context_create()
{
	md5_ptr o = (md5_ptr) malloc(sizeof(md5_t));
	md5_init(o);

	return o;
}

void md5context_destroy(md5_ptr ctx)
{
	md5_init(ctx);
	free(ctx);
	ctx = NULL;
}

static void md5_process(md5_ptr ctx)
{
	word *M;
	M = (word*)ctx->data_chunk;

	word A = ctx->state[0];
	word B = ctx->state[1];
	word C = ctx->state[2];
	word D = ctx->state[3];

	for (size_t i=0; i<64; ++i) {
		word F, g;
		if 		( i<=15 )	{ F = (B&C) | ((~B)&D); g = i; }
		else if ( i<=31 )	{ F = (D&B) | ((~D)&C); g = (5*i +1) % 16; }
		else if ( i<=47 )	{ F = B ^ C ^ D; g = (3*i+5)%16; }
		else 				{ F = C ^ ( B|(~D)); g = (7*i)%16; }

		F = F + A + k[i] + M[g];
		A = D;
		D = C;
		C = B;
		B = B + ROT_L(F, s[i]);
	}

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
}

void md5context_add(md5_ptr ctx, byte *in, size_t length)
{
	if (in == NULL) return;		
	// create 512-bit chunk
	size_t i=0;
	while (i < length) {
		ctx->data_chunk[ctx->current_index++] = in[i++];
		++ctx->total_bits;
		if (ctx->current_index == 64) {
			md5_process(ctx);
			ctx->current_index = 0;
		}
	}
}

void md5context_end(md5_ptr ctx, byte out[16])
{
	size_t i = ctx->current_index;
	if ( i < 56 ) {
		ctx->data_chunk[i++] = 0x80;
		while ( i < 56 ) ctx->data_chunk[i++] = 0x00;
	} else {
		ctx->data_chunk[i++] = 0x80;
		while (i<64)
			ctx->data_chunk[i++] = 0x00;
		md5_process(ctx);
		memset(ctx->data_chunk, 0, 56);
	}

	ctx->total_bits <<= 3; // total bit of input data

	for (i=0; i<8; ++i) ctx->data_chunk[56+i] = ((uint8_t*)&(ctx->total_bits))[i];

	md5_process(ctx);

	for ( i=0; i<4; ++i )
		for ( size_t j=0; j<4; ++j )
			out[j+i*4] = ((byte *)&(ctx->state[i]))[j];
}
