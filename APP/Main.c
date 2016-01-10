#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "KmaAPI.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

static void TraceCallback(IN void *addr, IN unsigned int size) {
	void * value = NULL;
	int ret = 0;

	printf("TraceCallback : %p, %d\n", addr, size);

	value = (void *)malloc(size);
	if(!value) return;

	ret = ReadValue(addr, size, value);
	if(ret) return;

	printf("Value : ");
	PrintByteAsHEX(size, value);

	return;
}

/*
 * @brief ConvertHexStringToHex
 * @param isReverse If this value is 1, hexBuf is saved in contrast to hexStr. input
 * @param hexStr hex string. ex) "C0102030". input
 * @param hexBuf byte array buffer that hexStr is converted. output
*/
static void ConvertHexStringToHex(IN int isReverse, IN const char *hexStr, OUT void *hexBuf) {
	int len = strlen(hexStr);
	int i, j;
	unsigned int mul = 1;
	unsigned char data = 0x0;
	unsigned char *ptr = (unsigned char*)hexBuf;

	if(isReverse) j = len/2 - 1;
	else j = 0;

	for(i=0; i<len; i++) {
		if(i%2 == 0) mul = 16;
		else mul = 1;

		if(hexStr[i] >= '0' && hexStr[i] <= '9') {
			data += ( (hexStr[i] - 48) * mul );
		}else if(hexStr[i] >= 'a' && hexStr[i] <= 'f') {
			data += ( (hexStr[i] - 87) * mul );
		}else if(hexStr[i] >= 'A' && hexStr[i] <= 'F') {
			data += ( (hexStr[i] - 55) * mul );
		}else {
			printf("ConvertHexStringToHex fail\n");
			exit(-1);
		}

		if(i%2 != 0) {
			ptr[j] = data;
			data = 0x0;

			if(isReverse) j--;
			else j++;
		}
	}
}

/*
 * @brief ConvertCmdStrToCmd
 * @param cmdStr kma command string from user. input
 * @return -1 If operation is error
 * @return integer kma command
*/
static int ConvertCmdStrToCmd(IN const char *cmdStr) {
	if( strcmp(cmdStr, KMA_CMD_STR_READ_VALUE) == 0 ) 			return KMA_CMD_READ_VALUE;
	else if( strcmp(cmdStr, KMA_CMD_STR_READ_METADATA) == 0 ) 	return KMA_CMD_READ_METADATA;
	else if( strcmp(cmdStr, KMA_CMD_STR_READ_ALL) == 0 ) 		return KMA_CMD_READ_ALL;
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_VALUE) == 0 ) 	return KMA_CMD_WRITE_VALUE;
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_METADATA) == 0 )	return KMA_CMD_WRITE_METADATA;
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_ALL) == 0 ) 		return KMA_CMD_WRITE_ALL;
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_VALUE) == 0 ) 	return KMA_CMD_TRACE_VALUE;
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_METADATA) == 0 )	return KMA_CMD_TRACE_METADATA;
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_ALL) == 0 ) 		return KMA_CMD_TRACE_ALL;

	return -1;
}

/*
 * @brief CallCmdFunc
 * @param cmdStr kma command string from user. input
 * @param addr address string from user. input
 * @param size size string from user. input
 * @param value value string from user. input
 * @return 0 If operation is successful
 * @return -1 If operation is error
*/
static int CallCmdFunc(IN const char *cmdStr, IN const char *addr, IN const char *size, IN const char *value) {
	void *inAddr = NULL;
	unsigned int inSize = 0;
	void *inValue = NULL;
	int ret = 0;

	if(!cmdStr || !addr || !size) {
		return -1;
	}

	ConvertHexStringToHex(1, addr, &inAddr);
	inSize = (unsigned int)atoi(size);

	if(value && strlen(value) > 0 && strlen(value) % 2 == 0) {
		inValue = (void*)malloc(strlen(value) / 2);
		memset(inValue, 0, strlen(value) / 2);
		ConvertHexStringToHex(0, value, inValue);
	}else {
		inValue = (void*)malloc(inSize);
		memset(inValue, 0, inSize);
	}

	if( strcmp(cmdStr, KMA_CMD_STR_READ_VALUE) == 0 ) 			ret = ReadValue(inAddr, inSize, inValue);
	else if( strcmp(cmdStr, KMA_CMD_STR_READ_METADATA) == 0 ) 	ret = ReadMetadata(inAddr, NULL, NULL);
	else if( strcmp(cmdStr, KMA_CMD_STR_READ_ALL) == 0 ) 		ret = ReadAll(inAddr, NULL);
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_VALUE) == 0 ) 	ret = WriteValue(inAddr, inSize, inValue);
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_METADATA) == 0 )	ret = WriteMetadata(inAddr, NULL);
	else if( strcmp(cmdStr, KMA_CMD_STR_WRITE_ALL) == 0 ) 		ret = WriteAll(inAddr, NULL);
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_VALUE) == 0 ) 	ret = TraceValue(inAddr, inSize, TraceCallback);
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_METADATA) == 0 )	ret = TraceMetadata(inAddr, NULL);
	else if( strcmp(cmdStr, KMA_CMD_STR_TRACE_ALL) == 0 ) 		ret = TraceAll(inAddr, NULL);
	else {
		ret = -1;
	}

	if(!ret) {
		printf("Command : %s\n", cmdStr);
		printf("Addr : %p\n", inAddr);

		if( strcmp(cmdStr, KMA_CMD_STR_TRACE_VALUE) != 0 ) {
			printf("Value : ");
			PrintByteAsHEX(inSize, inValue);
		}
	}

	if(inValue) {
		free(inValue);
	}

	return ret;
}

static void PrintUsage(void) {
	printf("KMA(KernelMemoryAnalyzer) APP version : %s\n\n", KMA_VERSION);
	printf("CommandString : \n");
	printf("		%s\n", KMA_CMD_STR_READ_VALUE);
	printf("		%s\n", KMA_CMD_STR_READ_METADATA);
	printf("		%s\n", KMA_CMD_STR_READ_ALL);
	printf("		%s\n", KMA_CMD_STR_WRITE_VALUE);
	printf("		%s\n", KMA_CMD_STR_WRITE_METADATA);
	printf("		%s\n", KMA_CMD_STR_WRITE_ALL);
	printf("		%s\n", KMA_CMD_STR_TRACE_VALUE);
	printf("		%s\n", KMA_CMD_STR_TRACE_METADATA);
	printf("		%s\n", KMA_CMD_STR_TRACE_ALL);
	printf("Addr : \n");
	printf("		Address that you want to do something. ex)C0000010\n");
	printf("Size : \n");
	printf("		Size that you want to do something. ex) 4\n");
	printf("Usage : \n");
	printf("		sudo ./KernelMemoryAnalyzer <CommandString> <Addr> <Size>\n");
	printf("		sudo ./KernelMemoryAnalyzer <CommandString> <Addr> <Size> <Value>\n");
	printf("Example : \n");
	printf("		sudo ./KernelMemoryAnalyzer ReadValue C0000010 4\n");
	printf("		sudo ./KernelMemoryAnalyzer WriteValue C0000010 4 00000010\n");
	printf("		sudo ./KernelMemoryAnalyzer TraceValue C0000010 4\n");

	return;
}

static void ParseArgs(int argc, char **argv) {
	int cmd = -1;

	if(argc < 4) {
		printf("Wrong Input\n");
		PrintUsage();
		return;
	}

	cmd = ConvertCmdStrToCmd(argv[1]);
	if(cmd < 0) {
		printf("Wrong Input\n");
		PrintUsage();
		return;
	}

	if( IS_KMA_TYPE_WRITE(cmd) ) {
		if(argc != 5) {
			printf("Wrong Input\n");
			PrintUsage();
			return;
		}

		CallCmdFunc(argv[1], argv[2], argv[3], argv[4]);
	}else {
		CallCmdFunc(argv[1], argv[2], argv[3], NULL);
	}

	if( IS_KMA_TYPE_TRACE(cmd) ) {
		/*
		 * asleep for tracing
		 * If you want to develop your own application, 
		 * you have to use wait function for trace function.
		*/
		while(1) {
			sleep(1);
		}
	}

	return;
}

int main(int argc, char **argv) {
	int ret = 0;

	ret = KmaInit();
	if(ret) {
		return ret;
	}

	ParseArgs(argc, argv);
	KmaClose();

	return 0;
}
