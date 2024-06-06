#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

int my_strcmp(char* s1, char *s2);
char* my_strcpy(char* destination, const char* source);
char *cpuVendor(char *result);
int intToString(uint64_t n, char* s);

#endif