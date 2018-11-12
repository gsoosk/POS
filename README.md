POS
======

POS is an OS cloned from xv6 kernel.


New Features 
------

### Part 1 : 
* introducing contributers added to boot message.
* moving cursor in shell added to this version.
* sort command added. you can sort 5 numbers in arguments.
### Part 2 :
* invoked_syscalls(int pid) system call : this system call show used system calls of a process and their informations
* get_count(int pid, int systemcallNum) system call : return and show number of a specific system calls in a process 
* sort_syscalls(int pid) : sort all used systemcalls for a process by time 
* log_syscalls() : log all systemcalls used in all processes sorted by time 
* inc_num(int number) : this systemcall using register instead of stack to pass argumans. for doing that first backup from a register then add number to it. (using assembly)
* traceTest : it's a user application in shell for using these systemcalls. you can use it with `traceTest` command in shell.

How to use? 
------

you can make this kernel using `make` command.
also you can run this kernel on qemu virtual machine using `make qemu` command.


