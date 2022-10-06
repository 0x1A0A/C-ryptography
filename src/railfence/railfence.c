#include "railfence.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void railfence_encrypt(char *str, int key)
{
	int fence[key];
	char **rail = NULL;
	size_t str_size = 0;
	rail = (char**)malloc(sizeof(char*) * key);

	memset(fence, 0, sizeof(int)*key);
	size_t i=0;
	int row=0;
	int down = -1;

	while( str[i] ) {
		if (isalpha(str[i])) {
			++fence[row];
			if (row == 0) down=1;
			if (row == key-1) down=-1;
			row += down;	
		}
		++i;
	}

	i=0;
	while (i<key) {
		rail[i] = (char*)malloc(sizeof(char) * fence[i]);
		fence[i] = 0;
		++i;
	}

	i=row=0;
	down=-1;
	while (str[i]) {
		if (isalpha(str[i])) {
			rail[row][fence[row]++] = str[i];
			if (row==0) down = 1;
			if (row==key-1) down = -1;
			row+=down;
		}
		++i;
	}

	i=0;
	while (i<key) {
		memmove(str + str_size, rail[i], fence[i]);
		str_size += fence[i];
		++i;
	}
	str[str_size]=0;
	i=0;
	while (i++<key) free(rail[i-1]);
	free(rail);
}

void railfence_decrypt(char *src, int key)
{

}

int main(void)
{
	char text[] = "test key with ssssss";
	railfence_encrypt(text, 3);
	puts(text);
	return 0;
}
