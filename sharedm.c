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

int sys_shm_open(int id, int page_count, int flag)
{
  int i, j;
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
      shm_table.segments[i].ref_count = 1;
      shm_table.segments[i].owner = myproc()->pid;
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
}

void *sys_shm_attach(int id)
{
  char* pointer = 0;
  int i, j;
  acquireticket(&(shm_table.lock));
  for (i = 0; i < SHARED_MEMS_SIZE; i++) 
  {
    if(shm_table.segments[i].id == id) 
    {
      for( j = 0 ; j < shm_table.segments[i].size ; j++)
      {
        if(myproc()->pid == shm_table.segments[i].owner || 
        (shm_table.segments[i].flags & ONLY_CHILD_CAN_ATTACH
        && myproc()->parent->pid == shm_table.segments[i].owner))
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
            pointer = (char *) PGROUNDUP(myproc()->sz);  
        }
        else
          cprintf("shm_attach error : only child can attach to this segment\n");
      } 
    }
  }
  if(pointer == 0)

  releaseticket(&(shm_table.lock));
  return pointer;
}


int sys_shm_close(int id) {
  acquireticket(&(shm_table.lock));
  int i, j;
  for (i = 0; i< SHARED_MEMS_SIZE; i++) {
    if(shm_table.segments[i].id == id) {
      shm_table.segments[i].ref_count--;
      break;
    }      
  }
  if(shm_table.segments[i].ref_count == 0)
      shm_init_segment(i);

  if(shm_table.segments[i].id != id) {
    cprintf("shm_close error: can not found segment with this id");   
    releaseticket(&(shm_table.lock));
    return -1;
  }
  releaseticket(&(shm_table.lock));
  return 0;
}