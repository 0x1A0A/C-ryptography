#include "railfence.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static void calculateFence(char *str, int *fence, int key)
{
	memset(fence, 0, sizeof(int)*key);
	size_t i=0;
	int row=0, down=-1;

	while( str[i++] ) {
		if (isalpha(str[i-1])) {
			++fence[row];
			if (row == 0) down=1;
			if (row == key-1) down=-1;
			row += down;	
		}
	}

}

static char** allocateRail(int key, int *fence)
{
	size_t i=0;
	char **rail = (char**)malloc(sizeof(char*) * key);
	
	while (i++<key) 
		rail[i-1] = (char*)malloc(sizeof(char) * fence[i-1]);

	return rail;
}

void railfence_encrypt(char *str, int key)
{
	int fence[key];
	char **rail = NULL;
	size_t str_size = 0;

	size_t i=0;
	int row=0;
	int down = -1;

	calculateFence(str, fence, key);
	rail = allocateRail(key, fence);
	memset(fence, 0, sizeof(int)*key);

	while (str[i++]) {
		if (isalpha(str[i-1])) {
			rail[row][fence[row]++] = str[i-1];
			if (row==0) down = 1;
			if (row==key-1) down = -1;
			row+=down;
		}
	}

	i=0;
	while (i++<key) {
		memmove(str + str_size, rail[i-1], fence[i-1]);
		str_size += fence[i-1];
	}

	str[str_size]='\0';

	i=0;
	while (i++<key) free(rail[i-1]);
	free(rail);
}

void railfence_decrypt(char *str, int key)
{
	int fence[key];
	char **rail = NULL;
	size_t str_size = 0;

	size_t i = 0;
	int row = 0, down = -1;

	calculateFence(str, fence, key);
	rail = allocateRail(key, fence);

	while (row<key) {
		size_t j=0;
		while ( j<fence[row] ) {
			rail[row][j] = str[i];
			++i, ++j;
		}
		++row;
	}
	
	str_size = i;
	row = i = 0;
	memset(fence, 0, sizeof(int)*key);

	while (str[i++]) {
		if (isalpha(str[i-1])) {
			str[i-1] = rail[row][fence[row]++];
			if (row==0) down = 1;
			if (row==key-1) down = -1;
			row+=down;
		}
	}
			
}
