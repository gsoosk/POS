#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#define SHARED_MEMS_SIZE 32
#define ONLY_OWNER_WRITE  0x001
#define ONLY_CHILD_CAN_ATTACH 0x002
struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    int flags;
    void *frame;
    void *pointer;
    int refcnt;
    int owner;
  } shm_pages[SHARED_MEMS_SIZE];
} shm_table;

void sys_shm_init() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
    shm_table.shm_pages[i].flags = 0;
    shm_table.shm_pages[i].owner = 0;
  }
  release(&(shm_table.lock));
}

void* sys_shm_attach() {
  int id;
  void* pointer = 0;

  if(argint(0, &id) < 0)
    return "";

  int i;
  acquire(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    if(shm_table.shm_pages[i].id == id) {
     
      int enter = 0;
      if(myproc()->pid == shm_table.shm_pages[i].owner )
      {
        release(&(shm_table.lock));
        return shm_table.shm_pages[i].pointer;
      }
      else if((shm_table.shm_pages[i].flags & ONLY_CHILD_CAN_ATTACH) == 0)
        enter = 1;
      else if(myproc()->parent->pid == shm_table.shm_pages[i].owner)
        enter = 1;
      if(enter)
      { 
        int flag;
        if(myproc()->pid == shm_table.shm_pages[i].owner)
          flag = PTE_W | PTE_U;
        else if(shm_table.shm_pages[i].flags & ONLY_OWNER_WRITE)
          flag = PTE_U;
        else
          flag = PTE_W | PTE_U;
        mappages(myproc()->pgdir, (void*) PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.shm_pages[i].frame), flag);
        shm_table.shm_pages[i].refcnt++;
        shm_table.shm_pages[i].pointer =(void *) PGROUNDUP(myproc()->sz);
        cprintf("%x %x\n", shm_table.shm_pages[i].pointer, shm_table.shm_pages[i].frame);
        myproc()->sz += PGSIZE;
        release(&(shm_table.lock));
      }
      return shm_table.shm_pages[i].pointer;
    }
  }
   release(&(shm_table.lock));
   cprintf("shm_attach err : id not found\n");
  return pointer;
}

int sys_shm_open() {
  int i;
  int id, page_count, flag;
  if(argint(0, &id) < 0)
    return -1;
  if(argint(1, &page_count) < 0)
    return -1;
  if(argint(2, &flag) < 0)
    return -1;
 
  acquire(&(shm_table.lock));
  // Shared memory DNE
  for (i = 0; i < SHARED_MEMS_SIZE; ++i) {
    if(shm_table.shm_pages[i].id == 0) {
      shm_table.shm_pages[i].id = id;
      shm_table.shm_pages[i].frame = kalloc();
      shm_table.shm_pages[i].refcnt = 1;
      shm_table.shm_pages[i].owner = myproc()->pid;
      shm_table.shm_pages[i].flags = flag;
      memset(shm_table.shm_pages[i].frame, 0, PGSIZE);
      mappages(myproc()->pgdir, (void*) PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U);
      shm_table.shm_pages[i].pointer =(char *) PGROUNDUP(myproc()->sz);
      myproc()->sz += PGSIZE;
      release(&(shm_table.lock));
      return 1;
    }
  }
  release(&(shm_table.lock));
  return 1;
}


int sys_shm_close() {
  int id;
  if(argint(0, &id) < 0)
    return -1;
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    if(shm_table.shm_pages[i].id == id) {
      shm_table.shm_pages[i].refcnt--;

      if(myproc()->pid == shm_table.shm_pages[i].owner)
        shm_table.shm_pages[i].flags = 0;
      
      if(shm_table.shm_pages[i].refcnt > 0) break;
      shm_table.shm_pages[i].id = 0;
      shm_table.shm_pages[i].frame = 0;
      shm_table.shm_pages[i].refcnt = 0;
      break;
    }
  }
  if(shm_table.shm_pages[i].id != id) { // shared memory item not found
    release(&(shm_table.lock));
    return 1;
  }
  release(&(shm_table.lock));
  return 0;
}
