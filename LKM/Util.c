#include "Util.h"
#include "Log.h"

void PrintByteAsHEX(const char *msg, unsigned int size, void *data) {
	unsigned char *d = (unsigned char *)data;
	int i = 0;

	if(!msg || size == 0 || !data) {
		return;
	}

	printk("[%s] =", msg);
	for(i=0; i<size; i++) {
		if(i % 16 == 0) {
			printk("\n");
		}
		printk("0x%02x, ", d[i]);
	}
	printk("\n");

	return;
}

unsigned int GetCRC32(void *data, unsigned int size) {
	int i, j;
	unsigned int byte, crc, mask;
	unsigned char *msg = (unsigned char *)data;

	i = 0;
	crc = 0xFFFFFFFF;
	while (size != 0) {
	  byte = msg[i];            
	  crc = crc ^ byte;
	  for (j = 7; j >= 0; j--) {   
	     mask = -(crc & 1);
	     crc = (crc >> 1) ^ (0xEDB88320 & mask);
	  }
	  i = i + 1;
	  size--;
	}
	return ~crc;
}