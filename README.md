POS
======

POS is an OS cloned from xv6 kernel.
* [Part 1](#part-1-introduction-to-xv6)
* [Part 2](#part-2-system-calls)
* [Part 3](#part-3-synchronization-and-concurrency)
* [Part4](#part-4-cpu-scheduling)


New Features 
------

### Part 1 (Introduction to XV6): 
* introducing contributers added to boot message.
* moving cursor in shell added to this version.
* sort command added. you can sort 5 numbers in arguments.
### Part 2 (System Calls) :
* invoked_syscalls(int pid) system call : this system call show used system calls of a process and their informations
* get_count(int pid, int systemcallNum) system call : return and show number of a specific system calls in a process 
* sort_syscalls(int pid) : sort all used systemcalls for a process by time 
* log_syscalls() : log all systemcalls used in all processes sorted by time 
* inc_num(int number) : this systemcall using register instead of stack to pass argumans. for doing that first backup from a register then add number to it. (using assembly)
* traceTest : it's a user application in shell for using these systemcalls. you can use it with `traceTest` command in shell.
* ctrl + p : with using `ctrl + p` you can see count of systemcalls in one process
### Part 3 (Synchronization And Concurrency):
* ticketlock added to OS
* readers writers problem added
* command `lockTest` added and you can test ticketlock and readers writers problem using this command
### Part 4 (CPU Scheduling):
* adding a multi-level feed back scheduler including :
	* A Lottery scheduler for first priority
	* A FCFS scheduler for second priority
	* A Priority scheduler for third priority
		*	PS all of these schedulers queues use round robin policy
* adding `ps` command that show processes and their information such as state and scheduler queue 
* adding `sf` command that fork a CPU bound process in lottery queue with 1000 lottery ticket
*  adding `setQueue` command to change scheduler queue of a process
* adding `setPriority` command to change priority of a process in priority queue
* adding `setLottery` command to change number of lottery tickets of a process in lottery queue
* adding `schedTest` command that fork 15 process :
	* 5 CPU bound processs in Lottery scheduler queue
	* 5 CPU bound process in FCFS scheduler queue
	* 5 IO bound process in Priority scheduler queue

How to use? 
------

you can make this kernel using `make` command.
also you can run this kernel on qemu virtual machine using `make qemu` command.


