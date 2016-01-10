
default:
	$(CROSS_COMPILE)-gcc -shared -fPIC -D_APP_BUILD -g -o libKernelMemoryAnalyzer.so KernelMemoryAnalyzer.c -lpthread
	$(CROSS_COMPILE)-gcc -D_APP_BUILD -g -o KernelMemoryAnalyzer Main.c -L. -lKernelMemoryAnalyzer

clean:
	rm -f KernelMemoryAnalyzer
	rm -f *.so
