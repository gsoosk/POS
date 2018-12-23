#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

#define NCHILD 5

void priorityTest();
void FCFSTest();
void lotteryTest();
void multilevelQueue();
void showProcessScheduling();

int main(int argc, char const *argv[])
{
    set_sched_queue(LOTTERY, getpid());
    set_lottery_ticket(500, getpid());
    multilevelQueue();
    exit();
}

void showProcessScheduling()
{
    show_processes_scheduling();
}

void priorityTest()
{
  set_sched_queue(PRIORITY, getpid());
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
            set_sched_queue(PRIORITY, pid);
            set_priority(10-i, pid);
            }
            if(pid == 0 )
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
        for(i = 0 ; i < 200000 ; i++)
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
        printf(1, "Main user program finished pid %d\n", getpid());
    }
}

void FCFSTest(){

  int pid = getpid();
   set_sched_queue(FCFS, getpid());
    
    int i;
    for(i = 1; i < NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();
            if(pid > 0)
            {
                set_sched_queue(FCFS, pid);
            }
            if(pid == 0 )
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
        for(i = 0; i < NCHILD + 1; i++)
            wait();
        printf(1, "Main user program finished pid %d\n", getpid());
    }
}

void lotteryTest(){
    set_sched_queue(LOTTERY, getpid());
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
                set_sched_queue(LOTTERY, pid);
                set_lottery_ticket(10-i, pid);
            }
            if(pid == 0 )
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
        for(i = 0 ; i < 20000000 ; i++)
        {
            delay(2000000000);
        }
        
        printf(1, "%d\n", ownPid);

    }
    else
    {
        int i;
        for(i = 0; i < NCHILD; i++)
            wait();
    }
}

void multilevelQueue() {
    int pid = getpid();
    int queue = LOTTERY;
    int i;
    for(i = 0; i < 3 * NCHILD; i++)
    {
        if(pid > 0)
        {
            pid = fork();
            if(pid > 0)
            { 
                if(i < NCHILD)
                {
                    set_sched_queue(PRIORITY, pid);
                    set_priority(NCHILD * 3 - i, pid);
                }
                else if( i < NCHILD * 2)
                {
                    set_sched_queue(FCFS, pid);
                }
                else if( i < NCHILD * 3)
                {
                    set_sched_queue(LOTTERY, pid);
                    set_lottery_ticket(NCHILD * 3 - i, pid);
                }
                
            }
            if(pid == 0 )
            {
                queue = i < NCHILD ? PRIORITY : i < NCHILD * 2 ? FCFS : LOTTERY;
                break;
            }
        }
            
    }
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0 && (queue == LOTTERY || queue == FCFS))
    {
        int i;
        for(i = 0 ; i < 200000 ; i++)
            delay(2000000000);
    }
    else if(pid == 0 && queue == PRIORITY)
    {
        int i;
        for(i = 0 ; i < 200000 ; i++)
            delay(2000000000);
    }
    else
    {
        int i;
        for(i = 0; i < NCHILD * 3 ; i++)
            wait();
    }
}