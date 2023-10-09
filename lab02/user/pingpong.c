/*#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char **argv) {

    if(argc < 2)
    {
        fprintf(2, "ERROR: pingpong 'natural number' \n");
        exit(1);
    }
    int number = atoi(argv[1]);
    if (number <= 0)
    {
        fprintf(2, "ERROR: You should try with natural numbers bro\n");
        exit(1);
    }
    
    int sem_0 = sem_open(0, 1);
    int sem_1 = sem_open(1, 0);
    int pid = fork();

    if (pid < 0) {
        printf("ERROR: del tenedor papiiii\n");
        exit(1);
    }

    if(pid == 0) 
    {
        for (int i = 0; i < number; i++)
        {
            sem_down(sem_1);
            printf("\tPong\n");
            sem_up(sem_0);
        }
        exit(0);
    }else{
        for (int i = 0; i < number; i++)
        {
            sem_down(sem_0);
            printf("Ping\n");
            sem_up(sem_1);
        }
        wait(0);
    }

    sem_close(sem_0);
    sem_close(sem_1);

    return 0;
}*/
#include "kernel/types.h"

#include "kernel/stat.h"

#include "user/user.h"

int main(int argc, char const *argv[]){
    int number;
    if (argc != 2) {
        printf("ERROR: Usage --> pingpong 'natural number' \n"); 
        return 0;
    }
    number = atoi(argv[1]);
    if (number <= 0)
    {
        printf("ERROR: You should try with natural numbers bro\n");
        return 0;
    }
    int sem0= sem_open(0, 0);
    int sem1 = sem_open(1, 0);
    int pid = fork();
    if (pid < 0) {
        printf("ERROR: del tenedor papiiii\n");
        exit(1);
    }

    for (int i = 0 ; i < number ; i++) {
        if (pid == 0) {
            sem_down(sem0);
            printf("\tpong\n");
            sem_up(sem1);
        }
        else{
            printf("ping\n");
            sem_up(sem0);
            sem_down(sem1);
        }
    }
    wait(0);
    sem_close(sem0);
    sem_close(sem1);
    return 0;
}
