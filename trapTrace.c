#include "param.h"
#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "traps.h"
#include "mmu.h"
#include "x86.h"
#include "syscall.h"

struct sysCallTraces traces[100];
void initTraces()
{
    int i;
    int j;
    for( j = 0 ; j < 100 ; j++)
        for( i = 0 ; i < 200 ; i++)
            traces[j].trap[i] = 0;
   
}
void addNewTrace(int pid, int trapNum)
{
    int i = 0 ;
    while (traces[pid].trap[i] != 0)
        i++;  
    traces[pid].trap[i] = 1;
    traces[pid].trapNum[i] = trapNum;
    cmostime( &traces[pid].times[i] );
}

void showPidTraces(int pid)
{
    int i;
    for(i = 0 ; i < 200 ; i++)
    {
        if(traces[pid].trap[i] != 0)
        {
            cprintf("~ SystemCall %s  with trap number %d called at %d/%d/%d  %d:%d:%d\n",
                    syscallName(traces[pid].trapNum[i]),
                    traces[pid].trapNum[i],
                    traces[pid].times[i].year,
                    traces[pid].times[i].month,
                    traces[pid].times[i].day,
                    traces[pid].times[i].hour,
                    traces[pid].times[i].minute,
                    traces[pid].times[i].second);
        }
        else if( i == 0)
        {
            cprintf("~This pid can not found\n");
            return ;
        }
    }
}

int timeToNumber(struct rtcdate date)
{
    int minute = ((date.minute % 10) * 100) + (((date.minute / 10) % 10) * 1000);
    int hour = ((date.hour % 10) * 10000) + (((date.hour / 10) % 10) * 100000);
    int day = ((date.day % 1000000) * 100) + (((date.day / 10) % 10) * 10000000);
    int month = ((date.month % 100000000) * 100) + (((date.month / 10) % 10) * 1000000000);
    return date.second + minute + hour + day + month;
}

void showLogOfProcesses()
{
    int numberOfTraps = 0;
    struct syscallLog logTrace[20000];
    int i=0, j=0;
    for(i=0; i<100; i++) {
        for(j=0; j<200; j++) {
            if(!traces[i].trap[j])
                break;
            logTrace[numberOfTraps].date = traces[i].times[j];            
            logTrace[numberOfTraps].dateTime = timeToNumber(traces[i].times[j]);
            logTrace[numberOfTraps].pid = i;
            logTrace[numberOfTraps].syscallNumber = traces[i].trapNum[j];
            numberOfTraps++;
        }
    }

    for (i = 0; i < numberOfTraps; i++)
    {
        for (j = 0; j < numberOfTraps; j++)
        {
            if (logTrace[j].dateTime > logTrace[i].dateTime)             
            {
                int tmp = logTrace[i].dateTime;
                logTrace[i].dateTime = logTrace[j].dateTime;
                logTrace[j].dateTime = tmp;
                            
                tmp = logTrace[i].pid;
                logTrace[i].pid = logTrace[j].pid;
                logTrace[j].pid = tmp;
                            
                struct rtcdate temp;
                temp = logTrace[i].date;
                logTrace[i].date = logTrace[j].date;
                logTrace[j].date = temp;
            }  
        }
    }
    
    for(i = 0; i < numberOfTraps; i++)
    {
        cprintf("~ SystemCall %s %d called by process %d at %d/%d/%d  %d:%d:%d\n",
                    syscallName(logTrace[i].syscallNumber),
                    logTrace[i].syscallNumber,
                    logTrace[i].pid,
                    logTrace[i].date.year,
                    logTrace[i].date.month,
                    logTrace[i].date.day,
                    logTrace[i].date.hour,
                    logTrace[i].date.minute,
                    logTrace[i].date.second);
    }
}

char* syscallName(int syscallNum)
{
    switch(syscallNum)
    {
        case ( 1 ) : return "fork";
        case ( 2 ) : return "exit";
        case ( 3 ) : return "wait";
        case ( 4 ) : return "pipe";
        case ( 5 ) : return "read";
        case ( 6 ) : return "kill";
        case ( 7 ) : return "exec";
        case ( 8 ) : return "fstat";
        case ( 9 ) : return "chdir";
        case ( 10 ) : return "dup";
        case ( 11 ) : return "getpid";
        case ( 12 ) : return "sbrk";
        case ( 13 ) : return "sleep";
        case ( 14 ) : return "uptime";
        case ( 15 ) : return "open";
        case ( 16 ) : return "write";
        case ( 17 ) : return "mknod";
        case ( 18 ) : return "unlink";
        case ( 19 ) : return "link";
        case ( 20 ) : return "mkdir";
        case ( 21 ) : return "close";
        case ( 22 ) : return "inc_num";
        case ( 23 ) : return "invoked_syscalls";
        case ( 24 ) : return "get_count";
        case ( 26 ) : return "log_syscalls";
    }
    return "";
}

int getSyscallCount(int pid, int sysNum)
{
    int i;
    int num = 0;
    for(i = 0 ; i < 200 ; i++)
    {
        if(traces[pid].trap[i] != 0 && traces[pid].trapNum[i] == sysNum)
            num++;
    }
    return num;
}