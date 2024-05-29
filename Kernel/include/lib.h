#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int intToString(uint64_t n, char* s);
void my_strcpy(char *dest, const char *src);
int my_strlen(char * s);

#endif