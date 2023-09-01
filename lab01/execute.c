#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "execute.h"
#include "builtin.h"

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

//FUNC PARA OBTENER REDIRECCIONES DE ENTRADA Y SALIDA
static bool scommand_redirects(scommand scmd){
    char *redir_in = scommand_get_redir_in (scmd); //redireccion in
    char *redir_out = scommand_get_redir_out (scmd); //redireccion out
    
    //si hay redireccion de entrada entonces:
    if(redir_in != NULL);{
        int rd_in = open(redir_in);//abre el archivo en modo lectura
        if(rd_in == -1){  //retorna true si ocurrio un error
            printf("Hubo un error abriendo la redireccion de entrada %s\n",redir_in);
            return true;
        } 
        //FALTA SEGUIR, toy cansada , ESTA DEJAMELA A MI QUE PARECE FACIL XD

    }
}


void simple_exec(scommand command){
    assert(command != NULL);
    unsigned int lenc = scommand_length(command);
    char * redout= scommand_get_redir_out(command);
    char * redin = scommand_get_redir_in(command);

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
        
        char * myargs[lenc+1u];
        for (int i = 0; i < lenc; i++)
        {
            myargs[i] = strdup(scommand_front(command));
            scommand_pop_front(command);
        }
        myargs[lenc] = NULL;
        execvp(myargs[0], myargs);
        
        perror("execvp");
        exit(1);
    }else{
        wait(NULL);
    }
}

void exec_scommands(pipeline pipe, unsigned int pipelen){
    if(pipelen == 1){
        simple_exec(pipeline_front(pipe));
    }else{
        
    }
}

void execute_pipeline(pipeline apipe){
   assert(apipe != NULL);
   unsigned int pipelen = pipeline_length(apipe);
   if (pipeline_is_empty(apipe))                    //Caso en el que no hay comandos
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
            }else if (pid == 0){
                //Proceso hijo
            }else{
                //Proceso padre
                wait(NULL);
            }
        }
   }

}