#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
 int main() {
 
 char host[64];
 int rval = gethostname(host,64);
 struct sysinfo info;
 
 int rvalsysinfo = sysinfo(&info);
 
 if(rval == -1 || rvalsysinfo == -1)
	return 1;
 
 printf("Hostname: %s\n", host);
 printf("Uptime: %ld\n", info.uptime);
 printf("Process count: %u\n",info.procs);
 printf("Total RAM: %lu\n",info.totalram);
 printf("Free RAM: %lu\n",info.freeram);
 printf("Page size: %u\n",info.mem_unit);
 
 return 0;
 }
