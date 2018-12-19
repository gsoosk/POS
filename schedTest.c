#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen
#define NCHILD 10
void priorityTest();
int main(int argc, char const *argv[])
{
    printf(1, "Which sched do you want to test ? \n");

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
    ticketlockinit();
    pid = fork();
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
            pid = fork();
        if(i % 3 == 0 && pid == 0)
            delay(2000);
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        printf(1, "child adding to shared counter\n");
        
    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
        printf(1, "user program finished\n");
    }
}
