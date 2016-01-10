obj-m := KernelMemoryAnalyzer.o 

KernelMemoryAnalyzer-y += Main.o
KernelMemoryAnalyzer-y += Util.o
KernelMemoryAnalyzer-y += CommonAnalyzer.o
KernelMemoryAnalyzer-y += ARM32Analyzer.o
KernelMemoryAnalyzer-y += ARM64Analyzer.o
KernelMemoryAnalyzer-y += Intel32Analyzer.o
KernelMemoryAnalyzer-y += Intel64Analyzer.o

ccflags-y += -Wall
ccflags-y += -Werror
ccflags-y += -D_LKM_BUILD

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod *.symvers *.order *.mod.c .*

