#include "Intel32Analyzer.h"
#include "CommonAnalyzer.h"
#include "Util.h"
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>

int Intel32ReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	/*
	 * [ToDo]
	 * 1. Check Read Permission
	 * 2. Read
	*/
	return CommonReadValue(addr, size, value);
}

int Intel32ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int Intel32ReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int Intel32WriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	/*
	 * [ToDo] 
	 * 1. Check Write Permission
	 * 2. Write
	 * 3. Flush Cache
	*/
	return CommonWriteValue(addr, size, value);
}

int Intel32WriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return SUCCESS;
}

int Intel32WriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return SUCCESS;
}

int Intel32TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return CommonTraceValue(addr, size, pid, traceID, callback);
}

int Intel32TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int Intel32TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int Intel32Init(void) {
	return SUCCESS;
}

int Intel32Close(void) {
	int ret = SUCCESS;

	ret = CommonTraceClose();
	if(ret) {
		LOGE("CommonTraceClose error : %d", ret);
		return ret;
	}

	return ret;
}