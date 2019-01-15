#include<netinet/in.h>    
#include<stdio.h>    
#include<stdlib.h>    
#include<sys/socket.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<unistd.h>    
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <string.h>

int main() {    
    int create_socket, new_socket;    
    socklen_t addrlen;    
    int bufsize = 4096;    
    char *buffer = malloc(bufsize);    
    char *buffer2 = malloc(bufsize);
    struct sockaddr_in address;    
    char host[64];
    struct sysinfo info;
    struct utsname buf;
    
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0){    
        printf("The socket was created\n");
    }
    
    address.sin_family = AF_INET;    
    address.sin_addr.s_addr = INADDR_ANY;    
    address.sin_port = htons(8001);    
    
    if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){    
        printf("Binding Socket\n");
    }
    
    while (1) {    
        buffer = calloc(bufsize, sizeof(char));
        
        int rvalsysinfo = sysinfo(&info);
        int rvalutsname = uname(&buf);
        int rval = gethostname(host,64);
        
        if(rval == -1 || rvalsysinfo == -1 || rvalutsname == -1)
            return 1;
        
        if (listen(create_socket, 10) < 0) {    
            perror("server: listen");    
            exit(1);    
        }    
        
        if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {    
            perror("server: accept");    
            exit(1);    
        }    
        
        if (new_socket > 0){    
            printf("The Client is connected...\n");
        }
        
        recv(new_socket, buffer2, bufsize, 0);    
        printf("%s\n", buffer2);    
        
        sprintf(buffer + strlen(buffer), "---- gethostname Information ----\n");
        sprintf(buffer + strlen(buffer),"Hostname: %s\n", host);
        sprintf(buffer + strlen(buffer),"\n");
        
        sprintf(buffer + strlen(buffer),"---- sysinfo Information ----\n");
        sprintf(buffer + strlen(buffer),"Uptime: %ld\n", info.uptime);
        sprintf(buffer + strlen(buffer),"Process count: %u\n",info.procs);
        sprintf(buffer + strlen(buffer),"Total RAM: %lu\n",info.totalram);
        sprintf(buffer + strlen(buffer),"Free RAM: %lu\n",info.freeram);
        sprintf(buffer + strlen(buffer),"Page size: %u\n",info.mem_unit);
        sprintf(buffer + strlen(buffer),"\n");
        
        sprintf(buffer + strlen(buffer),"---- utsname Information ----\n");
        sprintf(buffer + strlen(buffer),"system name: %s\n", buf.sysname);
        sprintf(buffer + strlen(buffer),"node name: %s\n", buf.nodename);
        sprintf(buffer + strlen(buffer),"release: %s\n", buf.release);
        sprintf(buffer + strlen(buffer),"version: %s\n", buf.version);
        sprintf(buffer + strlen(buffer),"machine: %s\n", buf.machine);
        
        write(new_socket, buffer, strlen(buffer));    
        close(new_socket);    
    }    
    close(create_socket);    
    return 0;    
}
