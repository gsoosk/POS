#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char const *argv[])
{
    printf(1, "ps excuting\n");
    show_processes_scheduling();
    exit();
}