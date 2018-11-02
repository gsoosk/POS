#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

int main(int argc, char *argv[]) 
{
    printf(1, "What system call do you like to test ? \n");
    printf(1, "1.invoked_syscalls\n");
    printf(1, "2.get_count\n");

    char buf[1024];
    read(1, buf, 1024);
    if(atoi(buf) == 1)
    {
        printf(1, "enter pid : \n");
        read(1, buf, 1024);
        int pid = atoi(buf);
        invoked_syscalls(pid);
    }
    else if(atoi(buf) == 2)
    {
        printf(1, "enter pid : \n");
        read(1, buf, 1024);
        int pid = atoi(buf);
        printf(1, "enter syscall number : \n");
        read(1, buf, 1024);
        int sysNum = atoi(buf);
        get_count(pid, sysNum);
    }
    exit();
}