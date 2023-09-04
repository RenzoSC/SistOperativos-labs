#include <assert.h>
#include <stdbool.h> // true o false
#include <stdio.h> //entrada/salida
#include <stdlib.h>  //func para memoria dinamica
#include <string.h> // strcmp trabaja con cadenas de caracteres 
#include <unistd.h> // chdir proporciona func y constantes especif de UNIX

#include "builtin.h" 
#include "tests/syscall_mock.h"
#include "command.h"

//func aux que comprueba si el comando es cd
static bool is_cd (scommand cmd){
    return (strcmp(scommand_front(cmd), "cd") == 0);
}

//func aux que comprueba si el comando es exit 
static bool is_exit (scommand cmd){
    return (strcmp(scommand_front (cmd), "exit") == 0);
}

//func aux que comprueba si el comando es help
static bool is_help (scommand cmd){
    return (strcmp(scommand_front (cmd), "help") == 0);
}

bool builtin_is_internal(scommand cmd){
    assert(cmd != NULL); //verifica q el comando no sea nulo
    return is_cd(cmd) || is_exit(cmd) || is_help(cmd);
}

//verifica si un pipeline tiene solo un comando interno en el
bool builtin_alone(pipeline p){
    bool result = (pipeline_length(p) == 1) && builtin_is_internal(pipeline_front(p));
    return result;
}


//ejecuta los comandos cd, exit, help
//esta func verifica el tipo de comando interno con las func auxiliares
void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd)); //se fija que el comando sea interno


    if (is_help(cmd)){
        //Si el comando es "help" muestra info sobre los integrantes del grupo
        printf("Integrantes:\n"
        "\t- Acosta Lucas Mateo\n"
        "\t- Condormango Renzo\n"
        "\t- Gallego Lucas\n"
        "\t- Loyola Antonella\n\n"
        "Usage: mybash <command>\n");

    }else if (is_cd(cmd)){
        //Si el comando es "cd"
        if (scommand_length(cmd) <= 1){
            //si no se proporciona argumento, cambia el directorio
            char *homedir = getenv("HOME");
            chdir(homedir);
        }else {
            scommand_pop_front(cmd); //consumo el "cd"
            int res = chdir(scommand_front(cmd));
            if (res == -1){
                //cambia el directorio segun el argumento proporcionado
                // si hay un error, muestra el msj de error.
                char *str =scommand_to_string(cmd);
                perror(str);
                free(str);
            }
        }

    }else if (is_exit(cmd)){
        //si el comando es "exit", muestra un msj y sale del programa
        printf("Bye, Bye!\n");
        exit(EXIT_SUCCESS);
    }
}
