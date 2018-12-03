#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "ticketlock.h"

int readCount = 0;
int value = 0;

struct spinlock lk;



void performReadLock(struct ticketlock *wrt)
{
    if(readCount > 0)
    {
        acquire(&lk);
        readCount++;
        release(&lk);

        cprintf("reading is performed value is %d\n", value);

        acquire(&lk);
        readCount--;
        release(&lk);

        if(readCount == 0)
            releaseticket(wrt);
    }
    else
    {
        acquireticket(wrt);

        acquire(&lk);
        readCount++;
        release(&lk);

        cprintf("reading is performed value is %d\n", value);

        acquire(&lk);
        readCount--;
        release(&lk);

        if(readCount == 0)
            releaseticket(wrt);
    }
}

void performWriteLock(struct ticketlock *wrt)
{
    acquireticket(wrt);

    //writing is performed
    value++;
    cprintf("writing is performed value is %d\n", value);

    releaseticket(wrt);
}