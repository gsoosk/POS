struct ticketlock {
    int ticket;       // Is the lock held?
    int turn;
    // For debugging:
    char *name;        // Name of lock.
};

 
