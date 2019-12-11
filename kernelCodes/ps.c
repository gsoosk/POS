#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char const *argv[])
{
    show_processes_scheduling();
    exit();
}