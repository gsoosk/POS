#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen

int main(int argc, char *argv[]) 
{
    write(1, "salam\n", 7);
    exit();
}