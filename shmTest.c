#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen



void simple_shm_test();
int main(int argc, char *argv[]) 
{
    shm_init();
    printf(1, "What test do you want to run ? \n");
    printf(1, "1.shared memory between two proccess. simple\n");

  

    char buf[1024];
    read(1, buf, 1024);
    if(atoi(buf) == 1)
    {   
        simple_shm_test();
    }
    
    exit();
}
void simple_shm_test()
{
    int open = shm_open(1, 1, 0);
    int* frame_counter = (int*) shm_attach(1);
    *frame_counter = 0;
    if(open < 0)
    {
        printf(1, "error : can not open\n");
    }
    int pid = fork();
    if(pid == 0)
    {
        wait();
        printf(1, "counter increased to %d\n", (*frame_counter));
        shm_close(1);
    }
    else
    {
        int* child_frame_counter = (int*) shm_attach(1);
        (*child_frame_counter) ++;
        shm_close(1);
    }
}