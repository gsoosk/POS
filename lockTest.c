#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

int main(int argc, char *argv[]) 
{
    printf(1, "Which lock do you want to test ? \n");
    printf(1, "1.owner base release sleep lock\n");

    char buf[1024];
    read(1, buf, 1024);
    if(atoi(buf) == 1)
    {
        acquiresleep_syscalls();
        int pid = fork();
        if(pid == 0)
        {
            releasesleep_syscalls();
            exit();
        }
        wait();
        printf(1, "child terminated\n");
        releasesleep_syscalls();
    }
    exit();
}