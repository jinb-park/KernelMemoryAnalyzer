#ifndef _ARM32ANALYZER_H
#define _ARM32ANALYZER_H

#include "Structure.h"

#define ARM32ANALYZER_NAME "ARM32"

int ARM32Init(void);
int ARM32Close(void);

int ARM32ReadValue(IN void *addr, IN unsigned int size, OUT void *value);
int ARM32ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
int ARM32ReadAll(IN void *addr, OUT AddressInfo *addrInfo);

int ARM32WriteValue(IN void *addr, IN unsigned int size, IN void *value);
int ARM32WriteMetadata(IN void *addr, IN AddressMetadata *metaData);
int ARM32WriteAll(IN void *addr, IN AddressInfo *addrInfo);

int ARM32TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int ARM32TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int ARM32TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);

#endif