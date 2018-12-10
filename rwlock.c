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
int writeCount = 0;
int value = 0;
int writeValue = 0;
int queueWrite = 0;

struct spinlock lk;
struct spinlock wlk;



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

void performWriteFirstWritingLock(struct ticketlock *wrt, struct ticketlock *writeLock)
{

    if(writeCount == 0) {
        acquire(&wlk);
        queueWrite++;
        cprintf("write come in queue size : %d\n", queueWrite);
        release(&wlk);

        acquireticket(wrt);

        acquire(&wlk);
        queueWrite--;
        cprintf("write goes out queue size %d\n", queueWrite);
        release(&wlk);
    }

    acquire(&wlk);
    writeCount++;
    release(&wlk);

    acquireticket(writeLock);
    writeValue++;
    releaseticket(writeLock);
        
    cprintf("writing is performed, value is %d\n", writeValue);

    acquire(&wlk);
    writeCount--;
    release(&wlk);

    if(writeCount == 0)
        releaseticket(wrt);
}

void performWriteFirstReadingLock(struct ticketlock *wrt)
{
    acquireticket(wrt);

    while(queueWrite > 0)
    {
        cprintf("queue is greater than zero : %d\n", queueWrite);
        releaseticket(wrt);
        acquireticket(wrt);
    }
    //reading
    cprintf("reading is performed value, is %d\n", writeValue);

    releaseticket(wrt);
}