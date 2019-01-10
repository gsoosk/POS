#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen




struct shm_cnt {
  int cnt;
};

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

    struct shm_cnt *counter;
    shm_open(1,(char **)&counter);
    counter ->cnt = 0;
    printf(1, "counter in parent %d\n", (counter->cnt));
    
    
    // if(open < 0)
    // {
    //     printf(1, "error : can not open\n");
    // }
    int pid = fork();
    if(pid > 0)
    {
       
        printf(1, "b\n", (counter->cnt));
        shm_close(1);
    }
    else
    {   
        int i, j;
        for(i = 0 ; i < 10000; i++)
        {
            for(j = 0 ; j < 10000; j++)
            {
                
            }
        }
       
        shm_attach(1, (char **)&counter);
        counter->cnt++;
         printf(1, "counter in child %d\n", (counter->cnt));
        shm_close(1);
    }
}