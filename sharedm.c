#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ticketlock.h"
#define SHARED_MEMS_SIZE 32
#define ONLY_OWNER_WRITE  0x001
#define ONLY_CHILD_CAN_ATTACH 0x002
#define MAX_PAGES 10
struct {
  struct ticketlock lock;
  struct shm_page {
    uint firstSize;
    uint id;
    int flags;
    void *frame[10];
    void *pointer[10];
    int refcnt;
    int owner;
    int size;
  } shm_pages[SHARED_MEMS_SIZE];
} shm_table;

void sys_shm_init() {
  int i, j;
  initticketlock(&(shm_table.lock), "SHM lock");
  acquireticket(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    shm_table.shm_pages[i].id =0;
    for(j = 0 ; j < MAX_PAGES ; j++){
      shm_table.shm_pages[i].frame[j] =0;
      shm_table.shm_pages[i].pointer[j] =0;
    }
      
    shm_table.shm_pages[i].refcnt =0;
    shm_table.shm_pages[i].flags = 0;
    shm_table.shm_pages[i].owner = 0;
  }
  releaseticket(&(shm_table.lock));
}



void* attach(int id, struct proc* process) {
  
  void* pointer = 0;

  int i, j;
  acquireticket(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    if(shm_table.shm_pages[i].id == id) {
     
      int enter = 0;
      if(process->pid == shm_table.shm_pages[i].owner )
      {
        // releaseticket(&(shm_table.lock));
        // return shm_table.shm_pages[i].pointer[0];
        enter = 1;
      }
      else if((shm_table.shm_pages[i].flags & ONLY_CHILD_CAN_ATTACH) == 0)
        enter = 1;
      else if(process->parent->pid == shm_table.shm_pages[i].owner)
        enter = 1;
      if(enter)
      { 
        
        shm_table.shm_pages[i].refcnt++;
        int flag;
        if(process->pid == shm_table.shm_pages[i].owner)
          flag = PTE_W | PTE_U;
        else if(shm_table.shm_pages[i].flags & ONLY_OWNER_WRITE)
          flag = PTE_U;
        else
          flag = PTE_W | PTE_U;

        if(process -> pid == myproc() -> pid)
        {
          process->id[process->filledId] = id;
          process->filledId++;
          
        }
        
          for(j = 0 ; j < shm_table.shm_pages[i].size ; j++)
          {
          mappages(process->pgdir, (void*) PGROUNDUP(process->sz), PGSIZE, V2P(shm_table.shm_pages[i].frame[j]), flag);
          shm_table.shm_pages[i].pointer[j] =(void *) PGROUNDUP(process->sz);
          process->pointer[process->filledPointer] = process->sz; 
          process->physical[process->filledPointer] = shm_table.shm_pages[i].frame[j];
          process->filledPointer ++;
          process->sz += PGSIZE;
          } 
          cprintf("\n\n");
        
        
        releaseticket(&(shm_table.lock));
        return shm_table.shm_pages[i].pointer[0];
      }
      else
      {
        cprintf("shm_attach err : you can not attach to this shared memory\n");
        releaseticket(&(shm_table.lock));
        return pointer;
      } 
      
    }
  }
   releaseticket(&(shm_table.lock));
   cprintf("shm_attach err : id not found\n");
  return pointer;
}
void* sys_shm_attach()
{
  int id;
  if(argint(0, &id) < 0)
    return "";
  return attach(id, myproc());
}
void increfcnt(int id)
{
  shm_table.shm_pages[id].refcnt ++;
}

int sys_shm_open() {
  int i, j;
  int id, page_count, flag;
  if(argint(0, &id) < 0)
    return -1;
  if(argint(1, &page_count) < 0)
    return -1;
  if(argint(2, &flag) < 0)
    return -1;
 
  if(page_count < 1)
  {
    cprintf("shm_error : page count should be posetive\n");
    return -1;
  }
  acquireticket(&(shm_table.lock));
  // Shared memory DNE
  for (i = 0; i < SHARED_MEMS_SIZE; ++i) {
    if(shm_table.shm_pages[i].id == 0) {
      shm_table.shm_pages[i].id = id;
      shm_table.shm_pages[i].refcnt = 1;
      shm_table.shm_pages[i].owner = myproc()->pid;
      shm_table.shm_pages[i].flags = flag;
      shm_table.shm_pages[i].size = page_count;
      for(j = 0 ; j < shm_table.shm_pages[i].size ; j++)
      {
        shm_table.shm_pages[i].frame[j] = kalloc();
        memset(shm_table.shm_pages[i].frame[j], 0, PGSIZE);

        // mappages(myproc()->pgdir, (void*) PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.shm_pages[i].frame[j]), PTE_W|PTE_U);
        // shm_table.shm_pages[i].pointer[j] =(void *) PGROUNDUP(myproc()->sz);
        // myproc()->sz += PGSIZE;
      }
      
      releaseticket(&(shm_table.lock));
      return 1;
    }
  }
  cprintf("shm_error : shm table is full\n");
  releaseticket(&(shm_table.lock));
  return -1;
}


int sys_shm_close() {
  int id;
  if(argint(0, &id) < 0)
    return -1;
  int i, j;
  initticketlock(&(shm_table.lock), "SHM lock");
  acquireticket(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    if(shm_table.shm_pages[i].id == id) {
      shm_table.shm_pages[i].refcnt--;

      if(myproc()->pid == shm_table.shm_pages[i].owner)
        shm_table.shm_pages[i].flags = 0;
      
      if(shm_table.shm_pages[i].refcnt > 0) break;
      shm_table.shm_pages[i].id = 0;
      for(j = 0 ; j < MAX_PAGES ; j++){
        shm_table.shm_pages[i].frame[j] =0;
        shm_table.shm_pages[i].pointer[j] =0;
      }
      shm_table.shm_pages[i].refcnt = 0;
      break;
    }
  }
  if(shm_table.shm_pages[i].id != id) { // shared memory item not found
    releaseticket(&(shm_table.lock));
    return 1;
  }
  releaseticket(&(shm_table.lock));
  return 0;
}
