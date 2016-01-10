#include "CommonAnalyzer.h"
#include "Util.h"
#include "Command.h"
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/siginfo.h>
#include <asm/cacheflush.h>
#include <linux/pid_namespace.h>
#include <linux/pid.h>

int CommonReadValue(IN void *addr, IN unsigned int size, OUT void *value) {
	if(addr == NULL || size == 0 || value == NULL) {
		return ERROR_NULL_PARAMETER;
	}

	memcpy(value, addr, size);
	return SUCCESS;
}

int CommonReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int CommonReadAll(IN void *addr, OUT AddressInfo *addrInfo) {
	return SUCCESS;
}

int CommonWriteValue(IN void *addr, IN unsigned int size, IN void *value) {
	if(addr == NULL || size == 0 || value == NULL) {
		return ERROR_NULL_PARAMETER;
	}

	memcpy(addr, value, size);
	return SUCCESS;
}

int CommonWriteMetadata(IN void *addr, IN AddressMetadata *metaData) {
	return SUCCESS;
}

int CommonWriteAll(IN void *addr, IN AddressInfo *addrInfo) {
	return SUCCESS;
}

/*
 * Variables for TraceWork
*/
static void CommonTraceWorkHandler(struct work_struct *w);

static struct workqueue_struct *commonTraceWorkQueue = NULL;
static DECLARE_DELAYED_WORK(commonTraceWork, CommonTraceWorkHandler);
static LIST_HEAD(commonTraceList);
static DEFINE_MUTEX(commonTraceListMutex);
static const unsigned long commonTraceWorkPeriod = 3000;

static void SendSignalToProcess(IN int sigNum, IN int pid, IN int traceID, IN void *arg) {
	struct siginfo info;
	struct task_struct *task = NULL;
	int ret = 0;

	if(pid < 0) {
		LOGE("It's from LKM. Do not send signal");
		return;
	}

	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = sigNum;
	info.si_errno = 0;
	info.si_code = SI_QUEUE;
	info.si_int = traceID;

	rcu_read_lock();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	rcu_read_unlock();

	if(!task) {
		return;
	}

	ret = send_sig_info(sigNum, &info, task);
	if(ret) {
		LOGE("send_sig_info error");
		return;
	}

	//LOGD("SendSignalToProcess success");
	return;
}

static void CommonTraceWorkHandler(struct work_struct *w) {
	TraceWork *work = NULL;

	mutex_lock(&commonTraceListMutex);
	list_for_each_entry(work, &commonTraceList, list) {
		if( GetCRC32(work->addr, work->size) == work->checksum ) {
			//LOGD("addr : %p, size : %d, pid : %d, Not Modified", work->addr, work->size, work->pid);
		}else {
			LOGE("addr : %p, size : %d, pid : %d, Modified", work->addr, work->size, work->pid);
			work->checksum = GetCRC32(work->addr, work->size);
			work->callback(work->addr, work->size);

			SendSignalToProcess(KMA_RTS_NUM, work->pid, work->traceID, work->addr);
		}
	}
	mutex_unlock(&commonTraceListMutex);

	queue_delayed_work(commonTraceWorkQueue, &commonTraceWork, msecs_to_jiffies(commonTraceWorkPeriod));
	return;
}

static int CommonTraceIsInitialized(void) {
	if(commonTraceWorkQueue) {
		return 1;
	}
	return 0;
}

static void CommonTraceListInit(void) {
	mutex_init(&commonTraceListMutex);
	INIT_LIST_HEAD(&commonTraceList);
	return;
}

static void CommonTraceListAdd(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	TraceWork *work = NULL;

	mutex_lock(&commonTraceListMutex);

	work = (TraceWork*)kzalloc(sizeof(TraceWork), GFP_KERNEL);
	work->pid = pid;
	work->traceID = traceID;
	work->addr = addr;
	work->size = size;
	work->checksum = GetCRC32(addr, size);
	work->callback = callback;
	list_add(&work->list, &commonTraceList);
	LOGD("CommonTraceListAdd - addr : %p, size : %d, pid : %d, traceID : %d, callback : %p", addr, size, pid, traceID, callback);

	mutex_unlock(&commonTraceListMutex);
	return;
}

/*
 * [ToDo]
 * CommonTraceListRemove - This function must be implemented in future.
*/

static void CommonTraceListClose(void) {
	TraceWork *work = NULL;
	TraceWork *deletedWork = NULL;

	mutex_lock(&commonTraceListMutex);
	list_for_each_entry_safe(work, deletedWork, &commonTraceList, list) {
		if(deletedWork) {
			//kfree(deletedWork);
		}
	}
	mutex_unlock(&commonTraceListMutex);

	mutex_destroy(&commonTraceListMutex);
	return;
}

int CommonTraceInit(void) {
	if( CommonTraceIsInitialized() ) {
		return SUCCESS;
	}

	commonTraceWorkQueue = create_singlethread_workqueue("commonTraceWorkQueue");
	if(!commonTraceWorkQueue) {
		LOGE("create_singlethread_workqueue error");
		return ERROR;
	}

	CommonTraceListInit();
	
	queue_delayed_work(commonTraceWorkQueue, &commonTraceWork, msecs_to_jiffies(commonTraceWorkPeriod));

	return SUCCESS;
}

int CommonTraceClose(void) {
	if( !CommonTraceIsInitialized() ) {
		LOGE("commonTraceWorkQueue is not initialized");
		return SUCCESS;
	}

	cancel_delayed_work_sync(&commonTraceWork);
	destroy_workqueue(commonTraceWorkQueue);
	commonTraceWorkQueue = NULL;
	CommonTraceListClose();

	return SUCCESS;
}

int CommonTraceValue(IN void *addr, IN unsigned int size, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	int ret = SUCCESS;

	if(!addr || !size || !callback) {
		return ERROR_NULL_PARAMETER;
	}

	if( !CommonTraceIsInitialized() ) {
		ret = CommonTraceInit();
		if(ret) {
			LOGE("CommonTraceInit error : %d", ret);
			return ret;
		}
	}

	CommonTraceListAdd(addr, size, pid, traceID, callback);
	return ret;
}

int CommonTraceMetadata(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}

int CommonTraceAll(IN void *addr, IN int pid, IN int traceID, IN TraceCallbackFunc callback) {
	return SUCCESS;
}