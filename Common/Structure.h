#ifndef _STRUCTURE_H
#define _STRUCTURE_H

#ifdef _LKM_BUILD
	#include "Common.h"
	#include <linux/list.h>
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT 
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

/*
 * Structures & Types for Address
*/

typedef enum _AreaType {
	KERNEL_TEXT = 1,
	KERNEL_RO_DATA = 2,
	KERNEL_RW_DATA = 3,
	KERNEL_MODULES = 4,
	HIGH_MEM = 5,
}AreaType;

/*
 * @brief This function pointer will be into each object in KMA
 * 		  It's responsible for printing object.
 *		  ex) PrintObjFunc in AddressInfo is responsible for printing AddressInfo
*/
typedef void (*PrintObjFunc)(IN void* obj);

typedef struct _AddressMetadata {
	AreaType areaType;
	void *pgd;
	void *pud;
	void *pmd;
	void *pte;
	PrintObjFunc PrintObj;
}AddressMetadata;

typedef struct _AddressInfo {
	void *addr;
	void *value;
	AddressMetadata metaData;
	PrintObjFunc PrintObj;
}AddressInfo;


/*
 * Structrures & Types for Analyzer
*/
#define ANALYZER_NAME_LEN 128

typedef int (*InitFunc)(void);
typedef int (*CloseFunc)(void);

typedef int (*ReadValueFunc)(IN void *addr, IN unsigned int size, OUT void *value);
typedef int (*ReadMetadataFunc)(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);
typedef int (*ReadAllFunc)(IN void *addr, OUT AddressInfo *addrInfo);

typedef int (*WriteValueFunc)(IN void *addr, IN unsigned int size, IN void *value);
typedef int (*WriteMetadataFunc)(IN void *addr, IN AddressMetadata *metaData);
typedef int (*WriteAllFunc)(IN void *addr, IN AddressInfo *addrInfo);

typedef void (*TraceCallbackFunc)(IN void *addr, IN unsigned int size);

typedef int (*TraceValueFunc)(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callbackFunc);
typedef int (*TraceMetadataFunc)(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callbackFunc);
typedef int (*TraceAllFunc)(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callbackFunc);

#ifdef _LKM_BUILD

	/*
	 * @struct TraceWork
	 * @brief Defines a TraceWork. TraceWork means a work that user register for TRACE_XXX.
	 * @var int pid application process id. 
	 				If this TraceWork is registered by 3rd LKM, Is must be KMA_LKM_PID (-1)
	 * @var int traceID It's to support application to register "TRACE_XXX" more than one.
	 *					If this TraceWork is registered by 3rd LKM, It must be KMA_LKM_TRACE_ID (-1)
	 * @var void* addr address
	 * @var unsigned int size size
	 * @var unsigned int checksum It's crc32 checksum for value from [addr ~ addr+size]
	 *							 It's used for checking whether value is modified or not.
	 *							 If checksum is changed, KMA LKM will notify changes to App or 3rd LKM.
	 * @var TraceCallbackFunc callback callback function that will be called when checksum is changed.
	 * @var list_head list linked list for TraceWork
	*/
	typedef struct _TraceWork {
		int pid;
		int traceID;
		void *addr;
		unsigned int size;
		unsigned int checksum;
		TraceCallbackFunc callback;
		struct list_head list;
	}TraceWork;

	/*
	 * @struct KernelMemoryAnalyzer
	 * @brief Defines a KernelMemoryAnalyzer. It will be filled in according to each arch.
	 * @var char name[ANALYZER_NAME_LEN]  name of analyzer
	 * @var InitFunc Init function pointer to initialize analyzer
	 * @var CloseFunc Close function pointer to close analyzer
	 * @var ReadValueFunc ReadValue function pointer for ReadValue
	 * @var ReadMetadataFunc ReadMetadata function pointer for ReadMetadata
	 * @var ReadAllFunc ReadAll function pointer for ReadAll
	 * @var WriteValueFunc WriteValue function pointer for WriteValue
	 * @var WriteMetadataFunc WriteMetadata function pointer for WriteMetadata
	 * @var WriteAllFunc WriteAll function pointer for WriteAll
	 * @var TraceValueFunc TraceValue function pointer for TraceValue
	 * @var TraceMetadataFunc TraceMetadata function pointer for TraceMetadata
	 * @var TraceAllFunc TraceAll function pointer for TraceAll
	*/
	typedef struct _KernelMemoryAnalyzer {
		char name[ANALYZER_NAME_LEN];

		InitFunc Init;
		CloseFunc Close;

		ReadValueFunc ReadValue;
		ReadMetadataFunc ReadMetadata;
		ReadAllFunc ReadAll;

		WriteValueFunc WriteValue;
		WriteMetadataFunc WriteMetadata;
		WriteAllFunc WriteAll;

		TraceValueFunc TraceValue;
		TraceMetadataFunc TraceMetadata;
		TraceAllFunc TraceAll;
	}KernelMemoryAnalyzer;

#endif

#endif