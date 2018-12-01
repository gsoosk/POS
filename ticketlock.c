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
    lock->pid = myproc()->pid;
    lock->ticket = 0;
    lock->turn = 0;
    initlock(&lock->lk, "ticket lock");
}

void acquireticket(struct ticketlock *lock)
{
    
    if(holdingticket(lock))
        panic("acquire");

    int ticket;
    ticket = atomic_increament(&lock->ticket);

    acquire(&lock->lk);
    while(lock->turn != ticket ){
        sleep(lock, &lock->lk);
    }
    
    lock->pid = myproc()->pid;
}
void releaseticket(struct ticketlock *lock)
{
    if(!holdingticket(lock))
        panic("release");

    lock->pid = 0;
    
    atomic_increament(&lock->turn);
    wakeup(lock);
}
int holdingticket(struct ticketlock *lock)
{
  return (lock->ticket != lock->turn) && (lock->pid == myproc()->pid);
}
