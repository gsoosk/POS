#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

#define ONLY_OWNER_WRITE  0x001
#define ONLY_CHILD_CAN_ATTACH 0x002

struct shm_cnt {
  int cnt;
};

void simple_shm_test();
void shm_write_flag_test();
void shm_child_attach_flag_test();
void shm_more_than_one_page_test();

int main(int argc, char *argv[]) 
{
    while(1) {
        shm_init();
        printf(1, "What test do you want to run ? \n");
        printf(1, "1.shared memory between two proccess. simple\n");
        printf(1, "2.shared memory with ONLY_OWNER_WRITE flag\n");
        printf(1, "3.shared memory with ONLY_CHILD_CAN_ATTACH flag\n");
        printf(1, "4.shared memory with more than one page\n");
    
        char buf[1024];
        read(1, buf, 1024);
        if(atoi(buf) == 1)
            simple_shm_test();
        else if(atoi(buf) == 2)
            shm_write_flag_test();
        else if(atoi(buf) == 3)
            shm_child_attach_flag_test();
        else if(atoi(buf) == 4)
            shm_more_than_one_page_test();
    }
    
    exit();
}
void simple_shm_test()
{

    struct shm_cnt *counter;
    
    acquiresleep_syscalls();
    int pid = fork();
    if(pid > 0)
    {
        shm_open(1, 1, 0);
        counter =  (struct shm_cnt *) shm_attach(1);
        counter ->cnt = 10;
        printf(1, "counter value in parent is : %d\n", (counter->cnt));
        releasesleep_syscalls();
        wait();
        shm_close(1);
    }
    else
    {   
        acquiresleep_syscalls();
        counter =  (struct shm_cnt *) shm_attach(1);
        counter->cnt++;
        printf(1, "counter value increased in child\n");
        printf(1, "counter value in child is : %d\n", (counter->cnt));
        shm_close(1);
        releasesleep_syscalls();
    }
}

void shm_write_flag_test() {
    struct shm_cnt *counter;
    
    acquiresleep_syscalls();
    int pid = fork();
    if(pid > 0)
    {
        shm_open(1, 1, ONLY_OWNER_WRITE);
        counter =  (struct shm_cnt *) shm_attach(1);
        counter ->cnt = 10;
        printf(1, "counter value in parent is : %d\n", (counter->cnt));
        releasesleep_syscalls();
        wait();
        shm_close(1);
    }
    else
    {   
        acquiresleep_syscalls();
        counter =  (struct shm_cnt *) shm_attach(1);
        printf(1, "counter value in child is : %d\n", (counter->cnt));
        counter->cnt++;
        printf(1, "counter value could not increased in child\n");
        printf(1, "counter value in child is : %d\n", (counter->cnt));
        shm_close(1);
        releasesleep_syscalls();
    }
}

void shm_child_attach_flag_test() {
    struct shm_cnt *counter;
    
    acquiresleep_syscalls();
    int pid = fork();
    if(pid > 0)
    {
        shm_open(1, 1, ONLY_CHILD_CAN_ATTACH);
        counter =  (struct shm_cnt *) shm_attach(1);
        counter ->cnt = 10;
        printf(1, "counter value in parent is : %d\n", (counter->cnt));
        releasesleep_syscalls();
        wait();
        shm_close(1);
    }
    else
    {   
        acquiresleep_syscalls();
        counter =  (struct shm_cnt *) shm_attach(1);
        printf(1, "counter value in child is : %d\n", (counter->cnt));
        counter->cnt++;
        printf(1, "counter value increased in child\n");
        printf(1, "counter value in child is : %d\n", (counter->cnt));
        releasesleep_syscalls();

        pid = fork();
        if(pid > 0) {
            printf(1, "child forks and waits for it\n");
            wait();
            shm_close(1);
        }
        else {
            printf(1, "child of child wants to attach shared memory\n");
            counter =  shm_attach(1);
            if(counter != 0)
                counter++;
            else
                printf(1, "could not increase value because error on attach\n");
        }
    }
}

void shm_more_than_one_page_test() {
    struct shm_cnt *counter;
    
    acquiresleep_syscalls();
    int pid = fork();
    if(pid > 0)
    {
        shm_open(1, 5, 0);
        counter =  (struct shm_cnt *) shm_attach(1);
        int i;
        for(i = 0; i < 5; i++) {
            (counter + i) ->cnt = i;
            printf(1, "counter value %d in parent is : %d\n", i, ((counter + i)->cnt));
        }
        releasesleep_syscalls();
        wait();
        shm_close(1);
    }
    else
    {   
        acquiresleep_syscalls();
        counter =  (struct shm_cnt *) shm_attach(1);
        int i;
        for(i = 0; i < 5; i++) {
            (counter + i)->cnt++;
            printf(1, "counter value %d increased in child\n", i);
            printf(1, "counter value %d in child is : %d\n", i, ((counter + i)->cnt));
        }
        shm_close(1);
        releasesleep_syscalls();
    }
}