#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen
#define NCHILD 4
void priorityTest();
int main(int argc, char const *argv[])
{
    printf(1, "Which sched do you want to test ? \n");
    printf(1, "1. Priority \n");
    char buf[1024];
    
    while(read(1, buf, 1024))
    {
        if(atoi(buf) == 1)
        {
            priorityTest();
            break;
        }
        else
            printf(1, "enter a valid number please.\n");
    }
   

    exit();
}
void priorityTest()
{
  int pid;
    pid = fork();
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();
            printf(1, "What priority use for %d", pid);
            char buf[1024];
            read(1, buf, 1024);
            set_priority(atoi(buf), pid);
        }
            
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        int i ;
        int ownPid;
        ownPid = getpid();
        printf(1, "Process %d is started \n", ownPid);
        for(i = 0 ; i < 2000; i++)
        {
            // 1234.0323 * 3214.124;
        }
        printf(1, "Process %d is ended \n", ownPid);

    }
    else
    {
        printf(1, "Main user program finished\n");
    }
}
