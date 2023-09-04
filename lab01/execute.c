#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "execute.h"
#include "builtin.h"

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FDREAD 0
#define FDWRITE 1
    
static void simple_exec(scommand command){
    assert(command != NULL);
    const unsigned int lenc = scommand_length(command);
    char * redout= scommand_get_redir_out(command);
    char * redin = scommand_get_redir_in(command);
    
    char * myargs[257];
    for (unsigned int i = 0; i < lenc; i++)
    {
        myargs[i] = strdup(scommand_front(command));
        scommand_pop_front(command);
    }
    myargs[lenc] = NULL;
    int rc = fork();
    if (rc < 0)
    {
        perror("Fork");
        exit(1);
    }else if (rc == 0)
    {
        if (redin != NULL)
        {
            close(STDIN_FILENO);
            int input_fd = open(redin, O_RDONLY);
            if (input_fd < 0)
            {
                perror("Open");
                exit(1);
            }
            assert(input_fd == STDIN_FILENO);
        }

        if (redout != NULL)
        {
            close(STDOUT_FILENO);
            int output_fd = open(redout, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
            if (output_fd <0)
            {
                perror("Open");
                exit(1);
            }
        }
        execvp(myargs[0], myargs);
        
        perror("execvp");
        exit(1);
    }else{
        wait(NULL);
    }
}

static void exec_scommands(pipeline apipe, unsigned int pipelen){
    if(pipelen==1){
        simple_exec(pipeline_front(apipe));
        pipeline_pop_front(apipe);
    }else{
        int fd[2];
        int rc;
        pipe(fd);
        scommand cmd1 = pipeline_front(apipe);
        pipeline_pop_front(apipe);

        rc = fork();
        scommand cmd2 = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        if (rc<0)
        {
            perror("Fork");
            exit(EXIT_SUCCESS);
        }else if (rc ==0)
        {
            
            close(fd[FDREAD]);
            dup2(fd[FDWRITE], STDOUT_FILENO);
            close(fd[FDWRITE]);

            simple_exec(cmd1);
        }else{
            wait(NULL);
            close(fd[FDWRITE]);
            dup2(fd[FDREAD], STDIN_FILENO);
            close(fd[FDREAD]);

            simple_exec(cmd2);
        }
    }
}

void execute_pipeline(pipeline apipe){
   unsigned int pipelen = pipeline_length(apipe);
   if (apipe == NULL || pipeline_is_empty(apipe))                    //Caso en el que no hay comandos
   {
    exit(EXIT_SUCCESS);
   }else if (builtin_alone(apipe))                  //Caso en el que es un único comando interno (cd,help,exit)
   {
    builtin_run(pipeline_front(apipe));
    exit(EXIT_SUCCESS);
   }else{                                           //Caso en el que hay más de un comando y no es internal
        bool pipe_wait = pipeline_get_wait(apipe);
        if (pipe_wait)                              //Se corre en primer plano
        {
            exec_scommands(apipe, pipelen);        //deberia correr el programa xd                       
        }else{
            pid_t pid = fork();
            if (pid <0)
            {
                perror("Fork");
                exit(EXIT_SUCCESS);
            }else if (pid == 0){
                //Proceso hijo
                exec_scommands(apipe, pipelen);
            }else{
                //Proceso padre
                wait(NULL);
            }
        }
   }

}