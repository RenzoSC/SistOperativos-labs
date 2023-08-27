#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    /* Devuelve NULL cuando hay un error de parseo */

    bool startpipe = false;
    bool startbackground = false;                        

    parser_skip_blanks(p);                  //Para ver que al comienzo de un comando simple no haya      
    parser_op_background(p, &startpipe);    //ni "|" ni "&"(si tenemos algo valido | otro valido y estamos parseando
    parser_op_pipe(p, &startbackground);    //"otro valido" no va a tomar a "|" al principio pues en la funcion        
    if(startpipe || startbackground) {      //parse_pipeline se analiza esto antes de entrar a parse_scommand).*/
        printf("error de sintaxis\n");
        return NULL;
    }


    scommand command = scommand_new();
    arg_kind_t type;
    char* arg;

    while(!parser_at_eof(p)){

        /*parser_skip_blanks(p);
        parser_op_background(p,delbackground);
        parser_op_pipe(p,delpipe);
        if (*delbackground || *delpipe) {
            break;
        }*/

        arg = parser_next_argument(p,&type);    //¿Que hace la funcion cuando ve un &?
                                                //¿El & es tomado tambien como argumento normal?
        /*if (porelmomento == NULL) {
            printf("se llego al final\n");    // no se que pasa cuando devuelve NULL         
            break;
        }*/

        if(type == ARG_INPUT) {
            scommand_set_redir_in(command, arg);
        } else if(type == ARG_OUTPUT) {
            scommand_set_redir_out(command, arg);
        } else if(type == ARG_NORMAL) {
            scommand_push_back(command, arg);       
        } else {
            command = scommand_destroy(command);    //Si encuentra algun simbolo que no sea ninguno de
            return NULL;                            // los tres tipos
        }
        parser_skip_blanks(p);
    }

    return command;
}



pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false,                         
    another_pipe=true;

    cmd = parse_scommand(p); 
    error = (cmd==NULL);        
    while (another_pipe && !error) {
        pipeline_push_back(result, cmd);
        parser_op_pipe(p, &another_pipe);
        if(another_pipe) {
            cmd = parse_scommand(p); 
            error = (cmd==NULL);
        }
    }

    if (error) {
        /*bool garbage = false;
        parser_garbage(p, &garbage);
        char *garbge = parser_last_garbage(p);
        printf("%s: No se reconoce como comando valido\n", garbge);  ??? */
        result = pipeline_destroy(result);
    } else if (!error && !parser_at_eof(p)) {                     /*¿Tiene sentido este else?*/
        bool op_background = false;
        parser_op_background(p, &op_background);            /* Opcionalmente un OP_BACKGROUND al final */
        if (!op_background) {                                // ?????
            pipeline_set_wait(result, op_background)
        }    
    }
    bool garbage = false;
    parser_garbage(p, &garbage);                /* Tolerancia a espacios posteriores */  
    //char *garbge = parser_last_garbage(p);      /* Consumir todo lo que hay inclusive el \n */
    if (garbage) {
        result = pipeline_destroy(result);           /* Si hubo error, hacemos cleanup */
    }

    return result; // MODIFICAR
}


