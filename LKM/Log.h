#ifndef _LOG_H
#define _LOG_H

#include <linux/kernel.h>

#define LOG_TAG "KMA"

#define LOGD(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGI(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGN(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGW(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGE(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGC(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)

#define LOGA(...) do{\
	printk( "[%s] ", LOG_TAG);\
	printk( __VA_ARGS__);\
	printk( "\n");\
}while(0)


#endif