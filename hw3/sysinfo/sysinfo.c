#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
 int main() {
 
 char host[64];
 int rval = gethostname(host,64);
 
 struct sysinfo info;
 int rvalsysinfo = sysinfo(&info);
 
 struct utsname buf;
 int rvalutsname = uname(&buf);

 if(rval == -1 || rvalsysinfo == -1 || rvalutsname == -1)
	return 1;
 
 printf("---- gethostname Information ----\n");
 printf("Hostname: %s\n", host);
 printf("\n");

 printf("---- sysinfo Information ----\n");
 printf("Uptime: %ld\n", info.uptime);
 printf("Process count: %u\n",info.procs);
 printf("Total RAM: %lu\n",info.totalram);
 printf("Free RAM: %lu\n",info.freeram);
 printf("Page size: %u\n",info.mem_unit);
 printf("\n");

 printf("---- utsname Information ----\n");
 printf("system name: %s\n", buf.sysname);
 printf("node name: %s\n", buf.nodename);
 printf("release: %s\n", buf.release);
 printf("version: %s\n", buf.version);
 printf("machine: %s\n", buf.machine);
 
 return 0;
 }
