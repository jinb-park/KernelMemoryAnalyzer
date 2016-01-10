#ifndef _INTEL64ANALYZER_H
#define _INTEL64ANALYZER_H

#include "Structure.h"

#define INTEL64ANALYZER_NAME "Intel64"

int Intel64Init(void);
int Intel64Close(void);

int Intel64ReadValue(IN void *addr, IN unsigned int size, OUT void *value);
int Intel64ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
int Intel64ReadAll(IN void *addr, OUT AddressInfo *addrInfo);

int Intel64WriteValue(IN void *addr, IN unsigned int size, IN void *value);
int Intel64WriteMetadata(IN void *addr, IN AddressMetadata *metaData);
int Intel64WriteAll(IN void *addr, IN AddressInfo *addrInfo);

int Intel64TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int Intel64TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int Intel64TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);

#endif