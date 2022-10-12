#ifndef __DFALSE_MD5_H__
#define __DFALSE_MD5_H__

#include "../util.h"

struct df_md5context {
	byte data_chunk[64];
	word state[4];
	size_t total_bits;
	size_t current_index;
};

typedef struct df_md5context md5_t, *md5_ptr;

md5_ptr md5context_create();
void md5context_destroy(md5_ptr ctx);

void md5context_add(md5_ptr ctx, byte *in, size_t length);
void md5context_end(md5_ptr ctx, byte out[16]);

#endif
