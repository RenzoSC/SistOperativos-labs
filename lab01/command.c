#include "command.h"
#include <string.h>
#include <stdlib.h>
#include "strextra.h"
#include <glib-2.0/glib.h>
#include <assert.h>

struct scommand_s {
    GList *cmdargs;
    char *redin;
    char *redout;
};

scommand scommand_new(void){
    scommand simplecom = NULL;
    simplecom = malloc(sizeof(struct scommand_s));
    simplecom->cmdargs = NULL;
    simplecom->redout = NULL;
    simplecom->redin = NULL;
    assert(simplecom != NULL && scommand_is_empty (simplecom) && scommand_get_redir_in (simplecom) == NULL && scommand_get_redir_out (simplecom) == NULL);
    return simplecom;
}

scommand scommand_destroy(scommand self){
    assert(self != NULL);

    g_list_free_full(self->cmdargs, (GDestroyNotify)*free);
    self->redout = NULL;
    self->redin = NULL;
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}

bool scommand_is_empty(const scommand self){
    assert(self != NULL);

    return self->cmdargs == NULL;
}

void scommand_push_back(scommand self, char * argument){
    assert(self != NULL && argument != NULL);

    self->cmdargs = g_list_append(self->cmdargs, argument);

    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){
    assert(self != NULL && !scommand_is_empty(self));

    free(g_list_first(self->cmdargs)->data);
    self->cmdargs = g_list_delete_link(self->cmdargs, self->cmdargs);
}

void scommand_set_redir_in(scommand self, char * filename){
    assert(self != NULL);
    
    self->redin = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self != NULL);

    self->redout = filename;
}

unsigned int scommand_length(const scommand self){
    assert(self != NULL);

    return g_list_length (self->cmdargs);
}

char * scommand_front(const scommand self){
    assert(self != NULL && !scommand_is_empty(self));

    return g_list_first(self->cmdargs)->data;
}

char * scommand_get_redir_in(const scommand self){
    assert(self != NULL);

    return self->redin;
}
char * scommand_get_redir_out(const scommand self){
    assert(self != NULL);

    return self->redout;
}

char * scommand_to_string(const scommand self){
    assert(self != NULL);

    char *cmdstring = strdup("");
    if(scommand_is_empty(self)){
        return cmdstring;
    }else{
        GList * pointer = self->cmdargs;
        char *temp;
        while (pointer!=NULL)
        {
            temp = cmdstring;                     // Guardo la cadena anterior
            cmdstring = strmerge(cmdstring, pointer->data);   // Genero nueva memoria con las cadenas concatenadas
            free(temp);                                 // Libero memoria de la cadena anterior
            if (pointer->next != NULL)
            {
                temp = cmdstring;
                cmdstring = strmerge(cmdstring, " ");
                free(temp);
            }

            pointer = pointer->next;
        }
        
        if (self->redout != NULL)
        {
            temp= cmdstring;
            cmdstring = strmerge(cmdstring, " > ");
            free(temp);
            temp = cmdstring;
            cmdstring = strmerge(cmdstring, self->redout);
            free(temp);
        }
        
        if (self->redin != NULL)
        {   
            temp = cmdstring;
            cmdstring = strmerge(cmdstring, " < ");
            free(temp);
            temp = cmdstring;
            cmdstring = strmerge(cmdstring, self->redin);
            free(temp);
        }
    }

    return cmdstring;
}

struct pipeline_s{
    GList *scmdList;
    bool wait;
};

pipeline pipeline_new(void) {
    struct pipeline_s *pipe = malloc(sizeof(struct pipeline_s));
    pipe->scmdList = NULL;
    pipe->wait = true;

    assert(pipe != NULL && pipeline_is_empty(pipe) && pipeline_get_wait(pipe));
    return pipe;
}

static void sdestroy_scommand(scommand cmd) { 
    scommand_destroy(cmd); 
}   //helper

pipeline pipeline_destroy(pipeline self){
    assert(self !=NULL);

    g_list_free_full(self->scmdList, (GDestroyNotify)&sdestroy_scommand);
    free(self);
    self = NULL;

    assert(self ==NULL);
    return self;
}

void pipeline_push_back(pipeline self, scommand sc){
    assert(self !=NULL && sc != NULL);

    self->scmdList = g_list_append(self->scmdList, sc);
}

void pipeline_pop_front(pipeline self){
    assert(self !=NULL && !pipeline_is_empty(self));

    scommand_destroy(g_list_first(self->scmdList)->data);
    self->scmdList = g_list_delete_link(self->scmdList, self->scmdList);
}

void pipeline_set_wait(pipeline self, const bool w){
    assert(self !=NULL);

    self->wait = w;
}

bool pipeline_is_empty(const pipeline self){
    assert(self !=NULL);

    return self->scmdList == NULL;
}

unsigned int pipeline_length(const pipeline self){
    assert(self !=NULL);

    return g_list_length (self->scmdList);
}

scommand pipeline_front(const pipeline self){
    assert(self !=NULL &&  !pipeline_is_empty(self));

    return g_list_first(self->scmdList)->data;
}

bool pipeline_get_wait(const pipeline self){
    assert(self !=NULL);

    return self->wait;
}

char * pipeline_to_string(const pipeline self){
    assert(self !=NULL);

    char * pipestring = strdup("");
    if (pipeline_is_empty(self))
    {
        return pipestring;
    }else{
        GList * pointer = self->scmdList;
        char * temp;
        while (pointer!= NULL)
        {
            char * scmd = scommand_to_string(pointer->data);      //guardo el scommand (se guarda memoria también)
            temp = pipestring;                              //guardo cadena anterior
            pipestring = strmerge(pipestring, scmd);        //concateno las dos cadenas
            free(temp);                                     //libero memoria de la cadena anterior
            free(scmd);                                     //libero memoria de la cadena scommand
            if (pointer->next !=NULL)
            {
                temp = pipestring;
                pipestring = strmerge(pipestring, " | ");
                free(temp);
            }

            pointer = pointer->next;
        }

        if (! self->wait)
        {
            temp = pipestring;
            pipestring = strmerge(pipestring, " & ");
            free(temp);
        }   
    }
    return pipestring;
}
// Pasa make test-command