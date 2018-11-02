#include "param.h"
#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "traps.h"
#include "mmu.h"
#include "x86.h"
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
    cprintf ( "number of traps : %d pid is : %d trap is : %d  number : %d time : %d\n",
                 i,
                 pid,
                 traces[pid].trap[i],
                 traces[pid].trapNum[i],
                 traces[pid].times[i].year);
}