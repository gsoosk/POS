struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
// int inc_num(int);
int invoked_syscalls(int pid);
int sort_syscalls(int pid);
int get_count(int pid, int sysNum);
int log_syscalls(void);
int disable_enable_trace(void);
//part 3 : sync
int releasesleep_syscalls(void);
int acquiresleep_syscalls(void);
int ticketlockinit(void);
int ticketlocktest(void);
int rwinit(void);
int rwtest(uint pattern);
int wrinit(void);
int wrtest(uint pattern);
//part 4 : sched
int select_sched(int sched);
int set_priority(int p, int pid);
int set_lottery_ticket(int l, int pid);
int set_sched_queue(int q, int pid);
int show_processes_scheduling();
//part 5 : memory management
int shm_init(void);
int shm_open(int id, int page_count, int flags);
void* shm_attach(int id);
int shm_close(int id);

enum schedAlgorithms { ROUND_ROBIN , PRIORITY , FCFS , LOTTERY};

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
char* strnconcat(char* os, const char*cs, int n);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
char* itoa_simple_helper(char *dest, int i);
char* itoa_simple(char *dest, int i);
int int_size(int i);

void delay(int numberOfClocks);