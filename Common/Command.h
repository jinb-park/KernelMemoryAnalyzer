#ifndef _COMMAND_H
#define _COMMAND_H

#define KMA_DEVICE_DIR "/dev"
#define KMA_DEVICE_NAME "KMA"
#define KMA_VERSION "0.5"

enum {
	KMA_TYPE_READ =  	1,
	KMA_TYPE_WRITE = 	2,
	KMA_TYPE_TRACE =		4,
	KMA_TYPE_VALUE = 	8,
	KMA_TYPE_METADATA = 	16,
};

enum {
	KMA_CMD_READ_VALUE 		= KMA_TYPE_READ | KMA_TYPE_VALUE,
	KMA_CMD_READ_METADATA 	= KMA_TYPE_READ | KMA_TYPE_METADATA,
	KMA_CMD_READ_ALL 		= KMA_TYPE_READ | KMA_TYPE_VALUE | KMA_TYPE_METADATA,
	KMA_CMD_WRITE_VALUE 		= KMA_TYPE_WRITE | KMA_TYPE_VALUE,
	KMA_CMD_WRITE_METADATA 	= KMA_TYPE_WRITE | KMA_TYPE_METADATA,
	KMA_CMD_WRITE_ALL 		= KMA_TYPE_WRITE | KMA_TYPE_VALUE | KMA_TYPE_METADATA,
	KMA_CMD_TRACE_VALUE 		= KMA_TYPE_TRACE | KMA_TYPE_VALUE,
	KMA_CMD_TRACE_METADATA 	= KMA_TYPE_TRACE | KMA_TYPE_METADATA,
	KMA_CMD_TRACE_ALL 		= KMA_TYPE_TRACE | KMA_TYPE_VALUE | KMA_TYPE_METADATA,
};

#define IS_KMA_TYPE_READ(cmd) ( (cmd & KMA_TYPE_READ) == KMA_TYPE_READ )
#define IS_KMA_TYPE_WRITE(cmd) ( (cmd & KMA_TYPE_WRITE) == KMA_TYPE_WRITE )
#define IS_KMA_TYPE_TRACE(cmd) ( (cmd & KMA_TYPE_TRACE) == KMA_TYPE_TRACE )
#define IS_KMA_TYPE_VALUE(cmd) ( (cmd & KMA_TYPE_VALUE) == KMA_TYPE_VALUE )
#define IS_KMA_TYPE_METADATA(cmd) ( (cmd & KMA_TYPE_METADATA) == KMA_TYPE_METADATA )

#define KMA_CMD_STR_READ_VALUE		"ReadValue"
#define KMA_CMD_STR_READ_METADATA	"ReadMetadata"
#define KMA_CMD_STR_READ_ALL			"ReadAll"
#define KMA_CMD_STR_WRITE_VALUE		"WriteValue"
#define KMA_CMD_STR_WRITE_METADATA	"WriteMetadata"
#define KMA_CMD_STR_WRITE_ALL		"WriteAll"
#define KMA_CMD_STR_TRACE_VALUE		"TraceValue"
#define KMA_CMD_STR_TRACE_METADATA	"TraceMetadata"
#define KMA_CMD_STR_TRACE_ALL		"TraceAll"

/*
 * PID & TraceID for 3rd LKM
*/
#define KMA_LKM_PID -1
#define KMA_LKM_TRACE_ID -1

#define KMA_RTS_NUM 36

/*
 * @struct KmaIoctlData
 * @brief structure used for communication between APP and LKM
 * @var int cmd kma command. It's KMA_CMD_XXX_XXX. It defines what LKM do.
 * @var int pid application process id. Since RTS(Real-Time Signal) is used for notification from LKM to App,
 *			pid is needed in LKM for trace function. 
 * @var int traceID It's to support application to register "TRACE_XXX" more than one.
 *					In other words, Identifier of each "TraceXXX" is combination of pid and traceID.
 * @var void* addr address
 * @var unsigned int size size
 * @var void* value If cmd is KMA_TYPE_WRITE, It is input buffer that will be written to addr.
 *					If cmd is KMA_TYPE_READ, It is output buffer that value will be saved in.
 *					If cmd is KMA_TYPE_TRACE, It's not used.
 * @var unsigned char *printData Not used variable yet. It's for the future.
*/
typedef struct _KmaIoctlData{
	int cmd;
	int pid;
	int traceID;
	void *addr;
	unsigned int size;
	void *value;
	unsigned char *printData;
}KmaIoctlData;

#endif