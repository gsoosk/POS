#include "param.h"
#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "traps.h"
#include "mmu.h"
#include "x86.h"
#include "syscall.h"
struct sysCallTraces traces[MAX_PID_NUMS];
void initTraces()
{
    int i;
    int j;
    for( j = 0 ; j < MAX_PID_NUMS ; j++)
        for( i = 0 ; i < MAX_SYS_CALLS ; i++)
        {
            traces[j].exists[i] = 0;
            strncpy(traces[j].syscallArgs[i], "", 0);
        }  
}

void addNewTrace(int pid, int syscallNumber)
{
    int i = 0 ;
    while (traces[pid].exists[i] != 0)
        i++;  
    traces[pid].exists[i] = 1;
    traces[pid].syscallNumber[i] = syscallNumber;
    cmostime( &traces[pid].times[i] );
}

void showPidTraces(int pid)
{
    int i;
    for(i = 0 ; i < MAX_SYS_CALLS ; i++)
    {
        if(traces[pid].exists[i] != 0)
        {
            cprintf("~ SystemCall %s  with trap number %d called at %d/%d/%d  %d:%d:%d\n",
                    syscallName(traces[pid].syscallNumber[i]),
                    traces[pid].syscallNumber[i],
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
        case ( 25 ) : return "sort_syscalls";
    }
    return "";
}

void sort_syscalls_trap(int pid)
{
    int i , j;
    for (i = 0; i < MAX_SYS_CALLS; i++)
	{
		for (j = 0; j < MAX_SYS_CALLS; j++)
		{
			if (traces[pid].exists[i] != 0 &&  traces[pid].exists[j] != 0 && traces[pid].syscallNumber[j] > traces[pid].syscallNumber[i])             
			{
				int tmp = traces[pid].exists[i];
				traces[pid].exists[i] = traces[pid].exists[j];
				traces[pid].exists[j] = tmp;
                
                tmp = traces[pid].syscallNumber[i];
				traces[pid].syscallNumber[i] = traces[pid].syscallNumber[j];
				traces[pid].syscallNumber[j] = tmp;
                
                struct rtcdate temp;
                temp = traces[pid].times[i];
				traces[pid].times[i] = traces[pid].times[j];
				traces[pid].times[j] = temp;
			}  
		}
	}
}

int getSyscallCount(int pid, int sysNum)
{
    int i;
    int num = 0;
    for(i = 0 ; i < MAX_SYS_CALLS ; i++)
    {
        if(traces[pid].exists[i] != 0 && traces[pid].syscallNumber[i] == sysNum)
            num++;
    }
    return num;
}