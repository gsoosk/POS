struct ticketlock {
    int ticket;       // Is the lock held?
    int turn;
    struct spinlock lk;

    // For debugging:
    char *name;        // Name of lock.
    int pid;     
};