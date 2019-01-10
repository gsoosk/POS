#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ticketlock.h"
#define SHARED_MEMS_SIZE 32
#define MAX_SHARED_FRAMES 10
#define ONLY_OWNER_WRITE  0x001
#define ONLY_CHILD_CAN_ATTACH 0x002

struct shm_segment {
  int id;
  int owner;
  int flags;
  char *frames[MAX_SHARED_FRAMES];
  int size;
  int ref_count;
};

struct {
  struct ticketlock lock;
  struct shm_segment segments[SHARED_MEMS_SIZE];
}  shm_table ;

void shm_init_segment(int segment_num) {
    int j;
    shm_table.segments[segment_num].id = 0;
    for( j = 0 ; j < MAX_SHARED_FRAMES ; j++)
      shm_table.segments[segment_num].frames[j] = 0;
    shm_table.segments[segment_num].ref_count = 0;
    shm_table.segments[segment_num].size = 0;
    shm_table.segments[segment_num].flags = 0;
}

int sys_shm_init()
{
  int i;
  initticketlock(&shm_table.lock, "table lock");
  acquireticket(&(shm_table.lock));
  for (i = 0; i < SHARED_MEMS_SIZE ; i++) {
    shm_init_segment(i);
  }     
  releaseticket(&(shm_table.lock));
  return 1;
}

int sys_shm_open()
{
  int i, j;
  int id, page_count, flag;
  if(argint(0, &id) < 0)
    return -1;
  if(argint(1, &page_count) < 0)
    return -1;
  if(argint(2, &flag) < 0)
    return -1;
 
  
  acquireticket(&(shm_table.lock));
  if(id <= 0)
  {
    cprintf("shm_open error: id should be a posetive number.\n");
  }
  for(i = 0 ; i < SHARED_MEMS_SIZE ; i++)
  {
    if(shm_table.segments[i].id == id)
    {
      cprintf("shm_open error: this segment id had been taken by someone else\n");
      return -1;
    }
  }
  for(i = 0 ; i < SHARED_MEMS_SIZE ; i++)
  {
    if(shm_table.segments[i].id == 0)
    {
      shm_table.segments[i].id = id;
      shm_table.segments[i].flags = flag;
      shm_table.segments[i].owner = myproc()->pid;
      shm_table.segments[i].size = page_count;
      for( j = 0 ; j < page_count; j++)
      {
        shm_table.segments[i].frames[j] = kalloc();
        memset(shm_table.segments[i].frames[j], 0, PGSIZE);
        mappages(myproc()->pgdir, (void*) PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.segments[i].frames[j]), PTE_W|PTE_U );
        myproc()->sz += PGSIZE;
      }
    releaseticket(&(shm_table.lock));
    return 1;
    }
  }
  return 1;
}

void *sys_shm_attach()
{
  int id;
  char* pointer = 0;
  if(argint(0, &id) < 0)
    return pointer;
  int i, j;
  acquireticket(&(shm_table.lock));
  for (i = 0; i < SHARED_MEMS_SIZE; i++) 
  {
    if(shm_table.segments[i].id == id) 
    {
      for( j = 0 ; j < shm_table.segments[i].size ; j++)
      {

        int enter = 0;
        if(myproc()->pid == shm_table.segments[i].owner )
          enter = 1;
        else if((shm_table.segments[i].flags & ONLY_CHILD_CAN_ATTACH) == 0)
          enter = 1;
        else if(myproc()->parent->pid == shm_table.segments[i].owner)
          enter = 1;
        if(enter)
        { 
          int flag;
          if(myproc()->pid == shm_table.segments[i].owner)
            flag = PTE_W | PTE_U;
          else if(shm_table.segments[i].flags & ONLY_OWNER_WRITE)
            flag = PTE_U;
          else
            flag = PTE_W | PTE_U;

          mappages(myproc()->pgdir, (void*) PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.segments[i].frames[j]), flag);
          shm_table.segments[i].ref_count++;
          myproc()->sz += PGSIZE;
          if(j == 0)
            pointer = (void *) PGROUNDUP(myproc()->sz); 
             *((char*) PGROUNDUP(myproc()->sz)) = 'c';
            cprintf("%c",   *((char*) PGROUNDUP(myproc()->sz)));
        }
        else
          cprintf("shm_attach error : only child can attach to this segment\n");
      } 
    }
  }

  releaseticket(&(shm_table.lock));
  return pointer;
}


int sys_shm_close() {
  int id;
  if(argint(0, &id) < 0)
    return -1;
  
  acquireticket(&(shm_table.lock));
  int i;
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    
    if(shm_table.segments[i].id == id) {
      shm_table.segments[i].ref_count--;
      break;
    }      
  }
   if(shm_table.segments[i].id != id) {
    cprintf("shm_close error: can not found segment with this id");   
    releaseticket(&(shm_table.lock));
    return -1;
  }
  if(shm_table.segments[i].ref_count == 0)
      shm_init_segment(i);
  releaseticket(&(shm_table.lock));
  return 0;
}