#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

#define MAX_LENGTH_SCOMMAND 257

static scommand parse_scommand(Parser p) {
    /* Devuelve NULL cuando hay un error de parseo */
    parser_skip_blanks(p);
    bool startpipe = false;
    bool startbackground = false;                        

                                            //Para ver que al comienzo de un comando simple no haya      
    parser_op_pipe(p, &startpipe);          //ni "|" ni "&"(si tenemos algo valido | otro valido y estamos parseando
                                            //"otro valido" no va a tomar a "|" al principio pues en la funcion
                                            //parse_pipeline se analiza esto antes de entrar a parse_scommand).
    if(startpipe) {
        printf("Error de sintaxis con respecto al operador |\n"); 
        return NULL;
    }

    parser_op_background(p, &startbackground);            
    if(startbackground) {      
        printf("Error de sintaxis con respecto al operador &\n");
        return NULL;
    }
    

                                                 
    scommand command = scommand_new();             
    arg_kind_t type;
    char* arg = NULL;
                              
    
    while(!parser_at_eof(p)){
        
        parser_skip_blanks(p);

        arg = parser_next_argument(p,&type);     
                                                
        if (arg == NULL) {              //Significa que encontro un "|" o "\n"
            break;  
        }

        if (scommand_get_redir_in(command)==NULL && type == ARG_INPUT) {
            scommand_set_redir_in(command, arg);
        } else if (scommand_get_redir_in(command)!=NULL && type == ARG_INPUT) {
            command = scommand_destroy(command);
            break;
        }

        if (scommand_get_redir_out(command)==NULL && type == ARG_OUTPUT) {
            scommand_set_redir_out(command, arg);
        } else if (scommand_get_redir_out(command)!=NULL && type == ARG_OUTPUT) {
            command = scommand_destroy(command);
            break;
        }   

        if (type == ARG_NORMAL) {
            scommand_push_back(command, arg);
        }
        
    }

    return command;
}


pipeline parse_pipeline(Parser p) {
    pipeline result = NULL;
    scommand cmd = NULL;
    bool error = false,                         
    another_pipe=true;

    cmd = parse_scommand(p);
    error = (cmd==NULL);
    if (!error && scommand_is_empty(cmd)) {
        return NULL;
    }
    result = pipeline_new();

    while (another_pipe && !error) {
        if (scommand_length(cmd)>MAX_LENGTH_SCOMMAND) {
           result = pipeline_destroy(result);
           return NULL;
        }
        pipeline_push_back(result, cmd);
        parser_op_pipe(p, &another_pipe);
        if(another_pipe) {
            cmd = parse_scommand(p); 
            error = (cmd==NULL);
        }
    }

    if (error) {
        result = pipeline_destroy(result);                       /* Si hubo error, hacemos cleanup */
        return NULL;
    } else if (!error && !parser_at_eof(p)) {                     /*¿Tiene sentido este else?*/
        bool op_background = false;
        parser_skip_blanks(p);
        parser_op_background(p, &op_background);            /* Opcionalmente un OP_BACKGROUND al final */
        pipeline_set_wait(result, !op_background);
    }

    bool garbage = false;
    parser_garbage(p, &garbage);                /* Tolerancia a espacios posteriores */  
                                      /* Consumir todo lo que hay inclusive el \n */
    if (garbage) {
        char *garbge = parser_last_garbage(p);
        printf("Error de sintaxis con respecto al operador & %s\n", garbge);
        result = pipeline_destroy(result);
    }

    return result;
}



