#ifndef _INTEL32ANALYZER_H
#define _INTEL32ANALYZER_H

#include "Structure.h"

#define Intel32ANALYZER_NAME "Intel32"

int Intel32Init(void);
int Intel32Close(void);

int Intel32ReadValue(IN void *addr, IN unsigned int size, OUT void *value);
int Intel32ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
int Intel32ReadAll(IN void *addr, OUT AddressInfo *addrInfo);

int Intel32WriteValue(IN void *addr, IN unsigned int size, IN void *value);
int Intel32WriteMetadata(IN void *addr, IN AddressMetadata *metaData);
int Intel32WriteAll(IN void *addr, IN AddressInfo *addrInfo);

int Intel32TraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int Intel32TraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);
int Intel32TraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback);

#endif