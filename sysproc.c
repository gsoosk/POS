#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "ticketlock.h"

//initialization of testing locks :
struct sleeplock lock;
struct ticketlock ticketLock;
int sharedCounter = 0;


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  
  return kill(pid);
}

int
sys_getpid(void)
{
  struct proc *curproc = myproc();
  curproc->count++;

  addNewTrace(curproc -> pid, SYS_getpid, "");
  return curproc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *curproc = myproc();
  curproc->count++;
  
  if(argint(0, &n) < 0)
    return -1;

  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, n), "int");
  addNewTrace(curproc -> pid, SYS_sbrk, argsForTrace);

  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  struct proc *curproc = myproc();
  curproc->count++;
 
  if(argint(0, &n) < 0)
    return -1;
  
  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, n), "int");
  addNewTrace(curproc -> pid, SYS_sleep, argsForTrace);

  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  struct proc *curproc = myproc();
  curproc->count++;
  addNewTrace(curproc -> pid, SYS_uptime, "");
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int 
sys_inc_num(void)
{
  struct proc *curproc = myproc();
  curproc->count++;
  cprintf("increased number is : %d \n", curproc->tf->edx + 1);

  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, curproc->tf->edx), "int");
  addNewTrace(curproc -> pid, SYS_inc_num, argsForTrace);
  return 1;
}

int 
sys_invoked_syscalls(void)
{
  struct proc *curproc = myproc();
  curproc->count++;
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  
  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, pid), "int");
  addNewTrace(curproc -> pid, SYS_invoked_syscalls, argsForTrace);

  showPidTraces(pid);
  return 1;
}

void
sys_log_syscalls(void)
{
  struct proc *curproc = myproc();
  curproc->count++;
  addNewTrace(curproc -> pid, SYS_log_syscalls, "");
  showLogOfProcesses();
}

int
sys_sort_syscalls(void){
  struct proc *curproc = myproc();
  curproc->count++;
  
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  
  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, pid), "int");
  addNewTrace(curproc -> pid, SYS_sort_syscalls, argsForTrace);

  sort_syscalls_trap(pid);
  return 1;
}

int
sys_get_count(void)
{
  struct proc *curproc = myproc();
  
  curproc->count++;
  int pid, sysNum;
  if(argint(0, &pid) < 0)
    return -1;
  if(argint(1, &sysNum) < 0)
    return -1;
  
  char argsForTrace[256]; strncpy(argsForTrace, " ", 2);
  char temp[128]; 
  addNewArgTrace(argsForTrace, itoa_simple(temp, pid), "int");
  addNewArgTrace(argsForTrace, itoa_simple(temp, sysNum), "int");
  addNewTrace(curproc -> pid, SYS_get_count, argsForTrace);

  int syscallsCount = getSyscallCount(pid, sysNum);
  if(syscallsCount != 0)
    cprintf("~System call count in this process is %d\n", syscallsCount);
  else
    cprintf("~There is no systemcall for this process\n");
  return syscallsCount;
}

void sys_acquiresleep_syscalls(void)
{
  lock.lk.locked = 0;
  lock.locked = 0;
  lock.pid = myproc()->pid;
  acquiresleep(&lock);
  cprintf("lock is now acquire by parent, pid : %d\n", myproc()->pid);
}

void sys_releasesleep_syscalls(void)
{
  newreleasesleep(&lock);
}

void sys_ticketlockinit(void)
{
  initticketlock(&ticketLock, "TestTicketLock");
}

void sys_ticketlocktest(void)
{
  acquireticket(&ticketLock);
  sharedCounter ++;
  cprintf("counter increamented to %d in pid = %d \n", sharedCounter, myproc()->pid);
  releaseticket(&ticketLock);
}