#ifndef _COMMONANALYZER_H
#define _COMMONANALYZER_H

#include "Structure.h"

int CommonReadValue(IN void *addr, IN unsigned int size, OUT void *value);
int CommonReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
int CommonReadAll(IN void *addr, OUT AddressInfo *addrInfo);

int CommonWriteValue(IN void *addr, IN unsigned int size, IN void *value);
int CommonWriteMetadata(IN void *addr, IN AddressMetadata *metaData);
int CommonWriteAll(IN void *addr, IN AddressInfo *addrInfo);

int CommonTraceInit(void);
int CommonTraceClose(void);

int CommonTraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int CommonTraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int CommonTraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);

#endif