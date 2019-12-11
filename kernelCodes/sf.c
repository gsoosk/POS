#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

#define NCHILD 1

void createBoundingProcess();

int main(int argc, char const *argv[])
{
    set_lottery_ticket(50, getpid());
    createBoundingProcess();
    exit();
}

void createBoundingProcess() {
    int pid = fork();
       
    if(pid < 0)
    {
        printf(2, "fork error\n");
    }
    else if(pid == 0)
    {
        int i;
        for(i = 0 ; i < 900000 ; i++)
            delay(2000000000);
    }
    else
    {
        wait();
    }
}