#include<stdlib.h>
#include <sys/stat.h>
#include <assert.h>

int main() {
           struct stat sb;
	char* path = "/dev/SimpleChardev";

           if (stat(path, &sb) == -1) {
               perror("stat");
               exit(EXIT_FAILURE);
           }    

printf("ID of containing device:  [%lx,%lx]\n",
                (long) major(sb.st_dev), (long) minor(sb.st_dev));

           printf("File type:                ");

           switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }	   

	return 0;
}
