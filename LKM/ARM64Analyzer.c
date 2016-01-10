#include "ARM64Analyzer.h"
#include "CommonAnalyzer.h"
#include "Util.h"
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>

int ARM64ReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	/*
	 * [ToDo]
	 * 1. Check Read Permission
	 * 2. Read
	*/
	return CommonReadValue(addr, size, value);
}

int ARM64ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM64ReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM64WriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	/*
	 * [ToDo] 
	 * 1. Check Write Permission
	 * 2. Write
	 * 3. Flush Cache
	*/
	return CommonWriteValue(addr, size, value);
}

int ARM64WriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return SUCCESS;
}

int ARM64WriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return SUCCESS;
}

int ARM64TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return CommonTraceValue(addr, size, pid, traceID, callback);
}

int ARM64TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int ARM64TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int ARM64Init(void) {
	return SUCCESS;
}

int ARM64Close(void) {
	int ret = SUCCESS;

	ret = CommonTraceClose();
	if(ret) {
		LOGE("CommonTraceClose error : %d", ret);
		return ret;
	}

	return ret;
}