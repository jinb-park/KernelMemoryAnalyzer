#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "ARM32Analyzer.h"
#include "ARM64Analyzer.h"
#include "Intel32Analyzer.h"
#include "Intel64Analyzer.h"
#include "Util.h"
#include "Command.h"
#include "KmaAPI.h"

static KernelMemoryAnalyzer analyzer;

static unsigned int testVar1 = 0x10;
static unsigned int testVar2 = 0x8080;

#if 0
static void testTraceCallback(IN void *addr, IN unsigned int size) {
	LOGD("testTraceCallback : %p, %d", addr, size);
}
#endif

static void testFunc(void) {
	unsigned int value1 = 0;
	unsigned int value2 = 0;

	ReadValue(&testVar1, sizeof(value1), &value1);
	ReadValue(&testVar2, sizeof(value2), &value2);

	LOGD("addr1 : %p, value1 : %08x", &testVar1, value1);
	LOGD("addr2 : %p, value2 : %08x", &testVar2, value2);

	value1 = 0x2020;
	value2 = 0x9090;

	WriteValue(&testVar1, sizeof(value1), &value1);
	WriteValue(&testVar2, sizeof(value2), &value2);

	LOGD("addr1 : %p, value1 : %08x", &testVar1, testVar1);
	LOGD("addr2 : %p, value2 : %08x", &testVar2, testVar2);

	#if 0
	TraceValue(&testVar2, sizeof(testVar2), testTraceCallback);
	msleep(5 * 1000);
	testVar2 = 0x7070;
	#endif
}

static void TraceCallback(IN void *addr, IN unsigned int size) {
	return;
}

/*
 * Export Functions for Users and LKMs
 * You can refer to KmaAPI.h to get details about these functions.
*/
int KmaInit(void) {
	return SUCCESS;
}
void KmaClose(void) {
	return;
}
int ReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	return analyzer.ReadValue(addr, size, value);
}
int ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return analyzer.ReadMetadata(addr, metaData, addrInfo);
}
int ReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return analyzer.ReadAll(addr, addrInfo);
}
int WriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	return analyzer.WriteValue(addr, size, value);
}
int WriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return analyzer.WriteMetadata(addr, metaData);
}
int WriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return analyzer.WriteAll(addr, addrInfo);
}
int TraceValue(IN void *addr, IN unsigned int size, IN TraceCallbackFunc callback) {
	return analyzer.TraceValue(addr, size, KMA_LKM_PID, KMA_LKM_TRACE_ID, callback);
}
int TraceMetadata(IN void *addr, IN TraceCallbackFunc callback) {
	return analyzer.TraceMetadata(addr, KMA_LKM_PID, KMA_LKM_TRACE_ID, callback);
}
int TraceAll(IN void *addr, IN TraceCallbackFunc callback) {
	return analyzer.TraceAll(addr, KMA_LKM_PID, KMA_LKM_TRACE_ID, callback);
}
void TraceCancel(IN void *addr, IN TraceCallbackFunc callback) {
	/*
	 * [ToDo]
	*/
	//analyzer.TraceCancel(callback);
}

EXPORT_SYMBOL(ReadValue);
EXPORT_SYMBOL(ReadMetadata);
EXPORT_SYMBOL(ReadAll);
EXPORT_SYMBOL(WriteValue);
EXPORT_SYMBOL(WriteMetadata);
EXPORT_SYMBOL(WriteAll);
EXPORT_SYMBOL(TraceValue);
EXPORT_SYMBOL(TraceMetadata);
EXPORT_SYMBOL(TraceAll);
EXPORT_SYMBOL(TraceCancel);

/*
 * misc driver interface & functions
*/
long kmaIOCTL(struct file *file, unsigned int cmd, unsigned long arg) {
	int ret = SUCCESS;
	KmaIoctlData data;
	void *value = NULL;

	switch(cmd) {
		case KMA_CMD_READ_VALUE:
			if( copy_from_user(&data, (KmaIoctlData*)arg, sizeof(KmaIoctlData)) ) {
				LOGE("KMA_CMD_READ_VALUE : copy_from_user KmaIoctlData error");
				return -1;
			}
			LOGE("KMA_CMD_READ_VALUE : %p, %d", data.addr, data.size);

			value = (void*)kzalloc(data.size, GFP_KERNEL);
			ret = analyzer.ReadValue(data.addr, data.size, value);
			if(ret) {
				LOGE("KMA_CMD_READ_VALUE : ReadValue error");
				return ret;
			}

			if( copy_to_user( ((KmaIoctlData*)arg)->value, value, data.size) ) {
				LOGE("KMA_CMD_READ_VALUE : copy_to_user error");
				return -1;
			}

			LOGD("KMA_CMD_READ_VALUE - success");
			break;
		case KMA_CMD_WRITE_VALUE:
			if( copy_from_user(&data, (KmaIoctlData*)arg, sizeof(KmaIoctlData)) ) {
				LOGE("KMA_CMD_WRITE_VALUE : copy_from_user KmaIoctlData error");
				return -1;
			}
			LOGE("KMA_CMD_WRITE_VALUE : %p, %d", data.addr, data.size);

			value = (void*)kzalloc(data.size, GFP_KERNEL);
			if( copy_from_user(value, ((KmaIoctlData*)arg)->value, data.size) ) {
				LOGE("KMA_CMD_WRITE_VALUE : copy_from_user value error");
				return -1;
			}
			PrintByteAsHEX("data.value", data.size, value);

			ret = analyzer.WriteValue(data.addr, data.size, value);
			if(ret) {
				LOGE("KMA_CMD_WRITE_VALUE : WriteValue error");
				return ret;
			}

			LOGD("KMA_CMD_WRITE_VALUE - success");
			break;
		case KMA_CMD_TRACE_VALUE:
			if( copy_from_user(&data, (KmaIoctlData*)arg, sizeof(KmaIoctlData)) ) {
				LOGE("KMA_CMD_TRACE_VALUE : copy_from_user KmaIoctlData error");
				return -1;
			}
			LOGE("KMA_CMD_TRACE_VALUE : %p, %d, %d, %d", data.addr, data.size, data.pid, data.traceID);

			ret = analyzer.TraceValue(data.addr, data.size, data.pid, data.traceID, TraceCallback);
			if(ret) {
				LOGE("KMA_CMD_TRACE_VALUE : TraceValue error");
				return ret;
			}

			LOGD("KMA_CMD_TRACE_VALUE - success");
			break;
		default:
			LOGE("Unsupport Ioctl Command");
			break;
	}

	if(value) {
		kfree(value);
	}
	return 0;
}

static const struct file_operations kmaFops = {
    .owner			= THIS_MODULE,
    .unlocked_ioctl  = kmaIOCTL,
};

static struct miscdevice kmaDevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = KMA_DEVICE_NAME,
    .fops = &kmaFops,
};

static int RegisterMiscDriver(void) {
	int ret = SUCCESS;

	ret = misc_register(&kmaDevice);
	if(ret) {
		LOGE("misc_register error : %d", ret);
		return ret;
	}

	return ret;
}

static void UnRegisterMiscDriver(void) {
	misc_deregister(&kmaDevice);
	return;
}

/*
 * Analyzer Functions
*/ 
static int InitAnalyzer(void) {
	int ret = SUCCESS;

	/*
	 * set functions for each arch
	*/
#ifdef CONFIG_ARM
	#ifdef CONFIG_ARCH_PHYS_ADDR_T_64BIT
	 	#pragma message("[KMA] Arch - ARM64")
	 	analyzer.Init = ARM64Init;
		analyzer.Close = ARM64Close;
		analyzer.ReadValue = ARM64ReadValue;
		analyzer.ReadMetadata = ARM64ReadMetadata;
		analyzer.ReadAll = ARM64ReadAll;
		analyzer.WriteValue = ARM64WriteValue;
		analyzer.WriteMetadata = ARM64WriteMetadata;
		analyzer.WriteAll = ARM64WriteAll;
		analyzer.TraceValue = ARM64TraceValue;
		analyzer.TraceMetadata = ARM64TraceMetadata;
		analyzer.TraceAll = ARM64TraceAll;
	#else
		#pragma message("[KMA] Arch - ARM32")
		analyzer.Init = ARM32Init;
		analyzer.Close = ARM32Close;
		analyzer.ReadValue = ARM32ReadValue;
		analyzer.ReadMetadata = ARM32ReadMetadata;
		analyzer.ReadAll = ARM32ReadAll;
		analyzer.WriteValue = ARM32WriteValue;
		analyzer.WriteMetadata = ARM32WriteMetadata;
		analyzer.WriteAll = ARM32WriteAll;
		analyzer.TraceValue = ARM32TraceValue;
		analyzer.TraceMetadata = ARM32TraceMetadata;
		analyzer.TraceAll = ARM32TraceAll;
	#endif	
#else
	#ifdef CONFIG_ARCH_PHYS_ADDR_T_64BIT
		#pragma message("[KMA] Arch - Intel64")
		analyzer.Init = Intel64Init;
		analyzer.Close = Intel64Close;
		analyzer.ReadValue = Intel64ReadValue;
		analyzer.ReadMetadata = Intel64ReadMetadata;
		analyzer.ReadAll = Intel64ReadAll;
		analyzer.WriteValue = Intel64WriteValue;
		analyzer.WriteMetadata = Intel64WriteMetadata;
		analyzer.WriteAll = Intel64WriteAll;
		analyzer.TraceValue = Intel64TraceValue;
		analyzer.TraceMetadata = Intel64TraceMetadata;
		analyzer.TraceAll = Intel64TraceAll;
	#else
		#pragma message("[KMA] Arch - Intel32")
		analyzer.Init = Intel32Init;
		analyzer.Close = Intel32Close;
		analyzer.ReadValue = Intel32ReadValue;
		analyzer.ReadMetadata = Intel32ReadMetadata;
		analyzer.ReadAll = Intel32ReadAll;
		analyzer.WriteValue = Intel32WriteValue;
		analyzer.WriteMetadata = Intel32WriteMetadata;
		analyzer.WriteAll = Intel32WriteAll;
		analyzer.TraceValue = Intel32TraceValue;
		analyzer.TraceMetadata = Intel32TraceMetadata;
		analyzer.TraceAll = Intel32TraceAll;
	#endif
#endif

	ret = analyzer.Init();
	if(ret) {
		LOGE("analyzer.Init error : %d", ret);
		return ret;
	}

	return ret;
}

static void CloseAnalyzer(void) {
	if(analyzer.Close) {
		analyzer.Close();
		msleep(5 * 1000);
	}
}

static int __init KernelMemoryAnalyzerInit(void) {
	int ret = SUCCESS;

	memset(&analyzer, 0, sizeof(analyzer));

	ret = InitAnalyzer();
	if(ret) {
		LOGE("InitAnalyzer error : %d", ret);
		return ret;
	}

	ret = RegisterMiscDriver();
	if(ret) {
		LOGE("RegisterMiscDriver error : %d", ret);
		return ret;
	}

	/*
	 * test function
	*/
	testFunc();

	return ret;
}

static void __exit KernelMemoryAnalyzerExit(void) {
	UnRegisterMiscDriver();
	CloseAnalyzer();
	return;
}

module_init( KernelMemoryAnalyzerInit );
module_exit( KernelMemoryAnalyzerExit );

MODULE_AUTHOR( "Jinbum Park" );
MODULE_LICENSE( "GPL v2" );
MODULE_DESCRIPTION( "KernelMemoryAnalyzer" );
MODULE_VERSION( "0.5" );
