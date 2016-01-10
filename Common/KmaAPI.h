#ifndef _KMAAPI_H
#define _KMAAPI_H

#include "Command.h"
#include "Structure.h"

/*
 * APIs
*/

/*
 * @fn int KmaInit(void)
 * @brief Intialize KernelMemoryAnalyzer
 * @return 
 *	0 If operation is successful		
 *	other values If operations has an error
*/
int KmaInit(void);

/*
 * @fn void KmaClose(void)
 * @brief Close KernelMemoryAnalyzer
*/
void KmaClose(void);

/*
 * @fn ReadValue
 * @brief ReadValue
 * @param addr address that will be read. input
 * @param size size that will be read. input
 * @param value buffer that value will be saved. output
 * @return 0 If operation is successful
 * @return other values If operations has an error
*/
int ReadValue(IN void *addr, IN unsigned int size, OUT void *value);

/*
 * @brief Not implemented yet
*/
int ReadMetadata(IN void *addr, OUT AddressMetadata *metaData, OUT AddressInfo *addrInfo);

/*
 * @brief Not implemented yet
*/
int ReadAll(IN void *addr, OUT AddressInfo *addrInfo);

/*
 * @fn WriteValue
 * @brief WriteValue
 * @param addr address that will be written. input
 * @param size size that will be written. input
 * @param value input value that will be written. input
 * @return 0 If operation is successful
 * @return other values If operations has an error
*/
int WriteValue(IN void *addr, IN unsigned int size, IN void *value);

/*
 * @brief Not implemented yet
*/
int WriteMetadata(IN void *addr, IN AddressMetadata *metaData);

/*
 * @brief Not implemented yet
*/
int WriteAll(IN void *addr, IN AddressInfo *addrInfo);

/*
 * @fn TraceValue
 * @brief TraceValue
 * @param addr address that will be traced. input
 * @param size size that will be traced. input
 * @param callback callback function pointer that will be called when value of the addr is changed. input
 * @return 0 If operation is successful
 * @return other values If operations has an error
*/
int TraceValue(IN void *addr, IN unsigned int size, IN TraceCallbackFunc callback);

/*
 * @brief Not implemented yet
*/
int TraceMetadata(IN void *addr, IN TraceCallbackFunc callback);

/*
 * @brief Not implemented yet
*/
int TraceAll(IN void *addr, IN TraceCallbackFunc callback);

/*
 * @brief Not implemented yet
*/
void TraceCancel(IN void *addr, IN TraceCallbackFunc callback);

#ifdef _APP_BUILD
/*
 * Utils
*/

 /*
 * @brief PrintByteAsHEX
 * @param size size that will be printed. input
 * @param data data buffer that will be printed as HEX. input
*/
void PrintByteAsHEX(unsigned int size, void *data);
#endif

#endif