#ifndef _ARM64ANALYZER_H
#define _ARM64ANALYZER_H

#include "Structure.h"

#define ARM64ANALYZER_NAME "ARM64"

int ARM64Init(void);
int ARM64Close(void);

int ARM64ReadValue(IN void *addr, IN unsigned int size, OUT void *value);
int ARM64ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
int ARM64ReadAll(IN void *addr, OUT AddressInfo *addrInfo);

int ARM64WriteValue(IN void *addr, IN unsigned int size, IN void *value);
int ARM64WriteMetadata(IN void *addr, IN AddressMetadata *metaData);
int ARM64WriteAll(IN void *addr, IN AddressInfo *addrInfo);

int ARM64TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int ARM64TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int ARM64TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);

#endif