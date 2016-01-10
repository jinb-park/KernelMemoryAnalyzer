#include "ARM32Analyzer.h"
#include "CommonAnalyzer.h"
#include "Util.h"
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>

int ARM32ReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	/*
	 * [ToDo]
	 * 1. Check Read Permission
	 * 2. Read
	*/
	return CommonReadValue(addr, size, value);
}

int ARM32ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM32ReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM32WriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	/*
	 * [ToDo] 
	 * 1. Check Write Permission
	 * 2. Write
	 * 3. Flush Cache
	*/
	return CommonWriteValue(addr, size, value);
}

int ARM32WriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return SUCCESS;
}

int ARM32WriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM32TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return CommonTraceValue(addr, size, pid, traceID, callback);
}

int ARM32TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int ARM32TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int ARM32Init(void) {
	return SUCCESS;
}

int ARM32Close(void) {
	int ret = SUCCESS;

	ret = CommonTraceClose();
	if(ret) {
		LOGE("CommonTraceClose error : %d", ret);
		return ret;
	}

	return ret;
}