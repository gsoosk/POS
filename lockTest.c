#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

#define NCHILD 10
void ownerBase(void);
void ticketLockTest(void);
void readerWriterLock(void);
void writerReaderLock(void);
void delay(int);

int main(int argc, char *argv[]) 
{
    printf(1, "Which lock do you want to test ? \n");
    printf(1, "1.owner base release sleep lock\n");
    printf(1, "2.ticket lock shared counter test\n");
    printf(1, "3.test reader writer lock with readers priority\n");
    printf(1, "4.test reader writer lock with writers priority\n");

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
            break;
        }
        else if(atoi(buf) == 3)
        {
            readerWriterLock();
            break;
        }
        else if(atoi(buf) == 4)
        {
            writerReaderLock();
            break;
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

int findNumberDigits(uint pattern)
{
  int num = 0;
  while(1)
  {
    pattern = pattern / 10;
    if(pattern == 0)
      break;
    num++;
  }
  return num+1;
}

void readerWriterLock()
{
    char buf[1024];
    memset(buf, 0, 1024);
    printf(1, "enter pattern : \n");
    read(1, buf, 1024);

    rwinit();
    char p[1];
    int i;
    for(i = 1; i < strlen(buf) - 1; i++)
    {
        if (fork() == 0)
        {
            p[0] = buf[i];
            rwtest(atoi(p));
            exit();
        }
        else
            continue;
    }
    for(i = 1; i < strlen(buf) - 1; i++)
        wait();
}

void writerReaderLock()
{
    char buf[1024];
    memset(buf, 0, 1024);
    printf(1, "enter pattern : \n");
    read(1, buf, 1024);

    wrinit();
    char p[1];
    int i;
    for(i = 1; i < strlen(buf) - 1; i++)
    {
        if (fork() == 0)
        {
            p[0] = buf[i];
            wrtest(atoi(p));
            exit();
        }
        else
            continue;
    }
    for(i = 1; i < strlen(buf) - 1; i++)
        wait();
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