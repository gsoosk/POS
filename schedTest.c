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
    printf(1, "Which sched do you want to test ? \n");
    printf(1, "1. Priority \n");
    printf(1 ,"2. FCFS \n");
    printf(1 ,"3. lottery \n");
    printf(1, "4. multi-level queue\n");
    printf(1, "5. show processes scheduling state\n");
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
            break;
        }
        else if(atoi(buf) == 4){
            multilevelQueue();
            break;
        }
        else if(atoi(buf) == 5){
            showProcessScheduling();
            break;
        }
        else
            printf(1, "enter a valid number please.\n");
    }
   

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
        printf(1, "Main user program finished pid %d\n", getpid());
    }
}

void multilevelQueue() {
    int pid = getpid();
    set_sched_queue(LOTTERY, getpid());
    set_lottery_ticket(100000, getpid());
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
        
        int ownPid;
        ownPid = getpid();
        int i;
        for(i = 0 ; i < 10000 ; i++)
        {
            delay(200000000);
        }
        
        printf(1, "%d\n", ownPid);

    }
    else if(pid == 0 && queue == PRIORITY)
    {
        
        int ownPid;
        ownPid = getpid();
        printf(1, "%d\n", ownPid);
        printf(1, "THIS IS A IO FOR TESTING IO BOUND PROCESS \n");
    }
    else
    {
        int i;
        showProcessScheduling();
        for(i = 0; i < NCHILD * 3 ; i++)
            wait();
        printf(1, "Main user program finished pid %d\n", getpid());
    }
}