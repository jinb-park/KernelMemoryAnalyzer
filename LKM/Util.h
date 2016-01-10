#ifndef _UTIL_H
#define _UTIL_H

void PrintByteAsHEX(const char *msg, unsigned int size, void *data);
unsigned int GetCRC32(void *data, unsigned int size);

#endif