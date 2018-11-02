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
    {
        i ++ ;
    }
        
    traces[pid].trap[i] = 1;
    traces[pid].trapNum[i] = trapNum;
    cmostime( &traces[pid].times[i] );
    // cprintf ( "number of traps : %d pid is : %d trap is : %d  number : %d time : %d\n",
    //              i,
    //              pid,
    //              traces[pid].trap[i],
    //              traces[pid].trapNum[i],
    //              traces[pid].times[i].year);
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
    }
    return "";
}