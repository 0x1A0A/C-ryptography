#include "rot13.h"
#include <ctype.h>

// char | ' ' == lower case, char & '_' == uppercase
void rot13(char *str)
{
	size_t i=0;
	while (str[i]) {
		if (isalpha(str[i])) {
			int isLow = islower(str[i]);
			int next = (str[i] | ' ') - 'a';
			next = (next+13)%26;
			str[i] = isLow ? 'a'+next: 'A'+next;
		}
		++i;
	}
}
