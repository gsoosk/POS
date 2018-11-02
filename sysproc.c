#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"

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
  addNewTrace(curproc -> pid, SYS_getpid);
  return curproc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *curproc = myproc();
  addNewTrace(curproc -> pid, SYS_sbrk);
  if(argint(0, &n) < 0)
    return -1;
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
  addNewTrace(curproc -> pid, SYS_sleep);
  if(argint(0, &n) < 0)
    return -1;
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
  addNewTrace(curproc -> pid, SYS_uptime);
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int 
sys_inc_num(void)
{
  int n;
  struct proc *curproc = myproc();
  addNewTrace(curproc -> pid, SYS_inc_num);
  argint(0, &n);
  cprintf("Hello World! %d \n", n);
  return 1;
}

int 
sys_invoked_syscalls(void)
{
  struct proc *curproc = myproc();
  addNewTrace(curproc -> pid, SYS_invoked_syscalls);
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  showPidTraces(pid);
  return 1;
}