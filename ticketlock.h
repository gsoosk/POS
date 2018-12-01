struct ticketlock {
  int ticket;       // Is the lock held?
  int turn;

  struct proc *proc;
  
  // For debugging:
  char *name;        // Name of lock.
};