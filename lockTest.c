#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen
#define NCHILD 10
void ownerBase(void);
void ticketLockTest(void);
void delay(int);
int main(int argc, char *argv[]) 
{
    printf(1, "Which lock do you want to test ? \n");
    printf(1, "1.owner base release sleep lock\n");
    printf(1, "2.ticket lock shared counter test\n");

    char buf[1024];
    
    while(read(1, buf, 1024))
    {
        if(atoi(buf) == 1)
        {
            ownerBase();
            break;
        }
        else if(atoi(buf) == 2)
        {
            ticketLockTest();
        }
        else
            printf(1, "enter a valid number please.\n");
    }
   

    exit();
}

void ownerBase()
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
void ticketLockTest()
{
    int pid;
    ticketlockinit();
    delay(100);
    pid = fork();
    int i;
    for(i = 1; i < NCHILD; i++)
        if(pid > 0)
            pid = fork();
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        delay(100);
        printf(1, "child adding to shared counter\n");
        ticketlocktest();
    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
        printf(1, "user program finished\n");
    }
}
void delay(int numberOfClocks)
{
    int firstClock = uptime();
    int incClock = uptime();
    while(incClock >= (firstClock + numberOfClocks) )
    {
        incClock = uptime();
    }
}