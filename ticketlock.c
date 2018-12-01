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
    lock->proc = myproc();
    lock->ticket = 0;
    lock->turn = 0;
}

void acquireticket(struct ticketlock *lock)
{
    int ticket;
    ticket = atomic_increament(&lock->ticket);

    while(lock->turn )
}