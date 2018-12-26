CC=aarch64-linux-gnu-gcc
CFLAGS=-static

.PHONY: sysinfo

sysinfo: sysinfo.o
	$(CC) $(CFLAGS) $< -o sysinfo

sysinfomake: sysinfo.c
	$(CC) -c $<

clean:
	rm -f sysinfo.o sysinfo
