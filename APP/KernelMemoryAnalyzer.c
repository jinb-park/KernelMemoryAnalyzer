#include "KmaAPI.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#define TRACE_FUNC_ARRAY_SIZE 256

typedef struct _TraceFunc{
	void *addr;
	unsigned int size;
	TraceCallbackFunc callback;
}TraceFunc;

static int fdKMA = -1;
static TraceFunc traceFuncArray[TRACE_FUNC_ARRAY_SIZE];
static pthread_spinlock_t traceFuncArrayLock;

static int AddTraceFunc(void *addr, unsigned int size, TraceCallbackFunc callback) {
	int i;

	pthread_spin_lock(&traceFuncArrayLock);
	for(i=0; i<TRACE_FUNC_ARRAY_SIZE; i++) {
		if(traceFuncArray[i].addr == NULL && traceFuncArray[i].callback == NULL) {
			traceFuncArray[i].addr = addr;
			traceFuncArray[i].size = size;
			traceFuncArray[i].callback = callback;
			pthread_spin_unlock(&traceFuncArrayLock);
			return i;
		}
	}
	pthread_spin_unlock(&traceFuncArrayLock);

	return -1;
}

static int CancelTraceFunc(void *addr, TraceCallbackFunc callback) {
	int i;

	pthread_spin_lock(&traceFuncArrayLock);
	for(i=0; i<TRACE_FUNC_ARRAY_SIZE; i++) {
		if(traceFuncArray[i].addr == addr && traceFuncArray[i].callback == callback) {
			traceFuncArray[i].addr = NULL;
			traceFuncArray[i].size = 0;
			traceFuncArray[i].callback = NULL;
			pthread_spin_unlock(&traceFuncArrayLock);
			return i;
		}
	}
	pthread_spin_unlock(&traceFuncArrayLock);

	return -1;
}

static void CancelAllTraceFunc(void) {
	pthread_spin_lock(&traceFuncArrayLock);
	memset(traceFuncArray, 0, sizeof(traceFuncArray));
	pthread_spin_unlock(&traceFuncArrayLock);
}

static void SignalHandler(int signum, siginfo_t *info, void *context) {
	int traceID = -1;
	TraceCallbackFunc callback = NULL;
	void *addr = NULL;
	unsigned int size = 0;

	if(info->si_signo != KMA_RTS_NUM) {
		return;
	}

	traceID = info->si_value.sival_int;
	//printf("traceID : %d\n", traceID);

	if(traceID >= 0 && traceID < TRACE_FUNC_ARRAY_SIZE) {
		pthread_spin_lock(&traceFuncArrayLock);
		callback = traceFuncArray[traceID].callback;
		addr = traceFuncArray[traceID].addr;
		size = traceFuncArray[traceID].size;
		pthread_spin_unlock(&traceFuncArrayLock);

		if(callback && addr && size) {
			callback( addr, size );
		}
	}

	return;
}

static int RegisterSignal(void) {
	struct sigaction act;
	int ret = 0;

	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = SignalHandler;

	ret = sigaction(KMA_RTS_NUM, &act, NULL);
	if(ret) {
		printf("sigaction error : %d", errno);
		return ret;
	}

	return 0;
}

int KmaInit(void) {
	int ret = 0;
	char kmaDeviceName[1024] = {0,};

	memset(traceFuncArray, 0, sizeof(traceFuncArray));

	snprintf(kmaDeviceName, 1024, "%s/%s", KMA_DEVICE_DIR, KMA_DEVICE_NAME);

	fdKMA = open(kmaDeviceName, O_RDWR);
	if(fdKMA < 0) {
		printf("file [%s] open error : %d\n", kmaDeviceName, errno);
		return -1;
	}

	ret = pthread_spin_init(&traceFuncArrayLock, PTHREAD_PROCESS_PRIVATE);
	if(ret) {
		printf("pthread_spin_init error");

		close(fdKMA);
		return ret;
	}

	ret = RegisterSignal();
	if(ret) {
		printf("RegisterSignal error\n");

		close(fdKMA);
		pthread_spin_destroy(&traceFuncArrayLock);
		return ret;
	}

	return 0;
}

void KmaClose(void) {
	if(fdKMA < 0) {
		printf("fdKMA is not initialized. please call Init() first\n");
		return;
	}

	CancelAllTraceFunc();

	pthread_spin_destroy(&traceFuncArrayLock);

	close(fdKMA);
	return;
}

#define CHECK_INPUT(fdKMA, addr, size) do{\
	if(fdKMA < 0) {\
		printf("fdKMA is not initialized. please call Init() first\n");\
		return -1;\
	}\
	if(!addr || !size) {\
		printf("Input NULL\n");\
		return -1;\
	}\
}while(0)

#define SET_KMA_IOCTL_DATA(d, c, a, s) do{\
	d.cmd = c;\
	d.pid = (int)getpid();\
	d.addr = a;\
	d.size = s;\
}while(0)

int ReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	int ret = 0;
	KmaIoctlData data;

	CHECK_INPUT(fdKMA, addr, size);

	SET_KMA_IOCTL_DATA(data, KMA_CMD_READ_VALUE, addr, size);
	data.value = value;
	memset(data.value, 0, data.size);

	ret = ioctl(fdKMA, data.cmd, &data);
	if(ret < 0) {
		printf("ioctl error : %d\n", ret);
		return ret;
	}

	return ret;
}

int ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return 0;
}

int ReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return 0;
}

int WriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	int ret = 0;
	KmaIoctlData data;

	CHECK_INPUT(fdKMA, addr, size);

	SET_KMA_IOCTL_DATA(data, KMA_CMD_WRITE_VALUE, addr, size);
	data.value = value;

	ret = ioctl(fdKMA, data.cmd, &data);
	if(ret < 0) {
		printf("ioctl error : %d\n", ret);
		return ret;
	}

	return ret;
}

int WriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return 0;
}

int WriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return 0;
}

int TraceValue(IN void *addr, IN unsigned int size, IN TraceCallbackFunc callback) {
	int ret = 0;
	int traceID = -1;
	KmaIoctlData data;

	CHECK_INPUT(fdKMA, addr, size);

	SET_KMA_IOCTL_DATA(data, KMA_CMD_TRACE_VALUE, addr, size);

	traceID = AddTraceFunc(addr, size, callback);
	if(traceID < 0) {
		printf("AddTraceFunc error\n");
		return -1;
	}
	data.traceID = traceID;

	ret = ioctl(fdKMA, data.cmd, &data);
	if(ret < 0) {
		printf("ioctl error : %d\n", ret);
		return ret;
	}

	//printf("PID : %d, TraceID : %d, Addr : %p, Size : %d\n", data.pid, data.traceID, data.addr, data.size);
	return ret;
}

int TraceMetadata(IN void *addr, IN TraceCallbackFunc callback) {
	return 0;
}

int TraceAll(IN void *addr, IN TraceCallbackFunc callback) {
	return 0;
}

/*
 * [ToDo]
 * Function to request TraceCancel to LKM  must be implemented in the future.
*/
void TraceCancel(IN void *addr, IN TraceCallbackFunc callback) {
	CancelTraceFunc(addr, callback);
}

#ifdef _APP_BUILD
void PrintByteAsHEX(unsigned int size, void *data) {
	unsigned char *d = (unsigned char *)data;
	int i = 0;

	if(size == 0 || !data) {
		return;
	}

	for(i=0; i<size; i++) {
		if(i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", d[i]);
	}
	printf("\n");

	return;
}
#endif
