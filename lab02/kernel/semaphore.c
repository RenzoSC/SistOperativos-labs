#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"

#define NSEMS 20
struct semaphore
{
    struct spinlock lk;
    int inuse;
    int resources;
};

struct semaphore sems[NSEMS];

void sem_init(void) {
  for (unsigned int i = 0; i < NSEMS; i++) {
    sems[i].inuse = 0;
    sems[i].resources =0;
    initlock(&sems[i].lk, "");
  }
    
}

int sem_open(int sem, int value){
    if (sem < 0 || sem >= NSEMS) {
        return -1;  // Semáforo no válido
    }
    int new_sem=0;
    while (sems[new_sem].inuse)
    {
        new_sem++;
    }
    acquire(&sems[new_sem].lk);
    sems[new_sem].inuse =1;
    sems[new_sem].resources=value;
    release(&sems[new_sem].lk);  
    return new_sem;                //devuelve que index de semaforo se está usando
    
}

int sem_close(int sem){
    acquire(&sems[sem].lk);
    sems[sem].inuse = 0;
    release(&sems[sem].lk);
    return sem;
}

int sem_up(int sem){
    acquire(&sems[sem].lk);
    if (sems[sem].resources == 0) {
        wakeup(&sems[sem]);
    }
    sems[sem].resources++;
    release(&sems[sem].lk);
    return sems[sem].resources;
}

int sem_down(int sem){

    acquire(&sems[sem].lk);
    while(sems[sem].resources == 0)
    {
        sleep(&sems[sem], &sems[sem].lk);
    }
    sems[sem].resources -=1;
    release(&sems[sem].lk);
    return sems[sem].resources;

}