#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen
#define NCHILD 10
void priorityTest();
void FCFSTest();
void lotteryTest();
int main(int argc, char const *argv[])
{
    printf(1, "Which sched do you want to test ? \n");
    printf(1, "1. Priority \n");
    printf(1 , "2. FCFS \n");
    char buf[1024];
    
    while(read(1, buf, 1024))
    {
        if(atoi(buf) == 1){
            priorityTest();
            break;

        }else if(atoi(buf) == 2){
            FCFSTest();
            break;
        }else if(atoi(buf) == 3){
            lotteryTest();
        }
        else
            printf(1, "enter a valid number please.\n");
    }
   

    exit();
}
void priorityTest()
{
  set_priority(0, getpid());
  int pid = getpid();
    
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();
            if(pid > 0)
            {
            set_priority(10-i, pid);
            }
            if(pid < 0 )
                break;
            
        }
            
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        
        int ownPid;
        ownPid = getpid();
        int i;
        for(i = 0 ; i < 20000 ; i++)
        {
            delay(200000000);
        }
        
        printf(1, "%d\n", ownPid);

    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
        printf(1, "Main user program finished fucking pid %d\n", getpid());
    }
}

void FCFSTest(){

  int pid = getpid();
    
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();

            if(pid < 0 )
                break;
            
        }
            
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        
        int ownPid;
        ownPid = getpid();
        int i;
        for(i = 0 ; i < 20000 ; i++)
        {
            delay(200000000);
        }
        
        printf(1, "%d\n", ownPid);

    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
        printf(1, "Main user program finished fucking pid %d\n", getpid());
    }
}

void lotteryTest(){
    set_lottery_ticket(100000, getpid());
  int pid = getpid();
    
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();
            if(pid > 0)
            {
            set_lottery_ticket(10-i, pid);
            }
            if(pid < 0 )
                break;
            
        }
            
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        
        int ownPid;
        ownPid = getpid();
        int i;
        for(i = 0 ; i < 20000 ; i++)
        {
            delay(200000000);
        }
        
        printf(1, "%d\n", ownPid);

    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
        printf(1, "Main user program finished fucking pid %d\n", getpid());
    }
}
