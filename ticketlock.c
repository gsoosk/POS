// Sleeping locks

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


void initticketlock(struct ticketlock *lock, char* name)
{
    lock->name = name;
    lock->ticket = 0;
    lock->turn = 1;
}


void acquireticket(struct ticketlock *lock)
{
    int ticket;
    ticket = atomic_increment(&lock->ticket);

    while(lock->turn != ticket ){
        ticketSleep(lock);
    }
}

void releaseticket(struct ticketlock *lock)
{
    atomic_increment(&lock->turn); 
    wakeup(lock);
}


