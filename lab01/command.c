#include "command.h"
#include <string.h>
#include <stdlib.h>

#include <glib-2.0/glib.h>


struct scommand_s {
    GList *cmdargs,
    char * redout,
    char * redin,
}

scommand scommand_new(void){
    scommand simplecom = NULL;
    simplecom = malloc(sizeof(struct scommand_s));
    simplecom->cmdargs = NULL;
    simplecom->redout = NULL;
    simplecom->redin = NULL;
    return simplecom;
}

scommand scommand_destroy(scommand self){
<<<<<<< Updated upstream
    g_list_free (self->cmdargs);
=======
    g_list_free_full(self->cmdargs, (GDestroyNotify)*free);
>>>>>>> Stashed changes
    self->redout = NULL;
    self->redin = NULL;
    free(self);
    self = NULL;
    return self;
}

bool scommand_is_empty(const scommand self){
    return self->cmdargs == NULL;
}

void scommand_push_back(scommand self, char * argument){
<<<<<<< Updated upstream
    self->cmdargs = g_list_prepend(self->cmdargs, argument);
    return self;
}

void scommand_pop_front(scommand self){
    Glist * last = g_list_last(self->cmdargs);
    self->cmdargs = g_list_remove_link(self->cmdargs, last);
    return self;
=======
    self->cmdargs = g_list_append(self->cmdargs, argument);
}

void scommand_pop_front(scommand self){
    free(g_list_first(self->args)->data);
    self->cmdargs = g_list_delete_link(self->cmdargs, self->cmdargs);
}

void scommand_set_redir_in(scommand self, char * filename){
    self->redin = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
    self->redout = filename;
}

unsigned int scommand_length(const scommand self){
    return g_list_length (self->cmdargs);
}

char * scommand_front(const scommand self){
    return g_list_first(self->cmdargs)->data;
}

char * scommand_get_redir_in(const scommand self){
    return self->redin;
}
char * scommand_get_redir_out(const scommand self){
    return self->redout;
}

char * scommand_to_string(const scommand self){
    /*Lo termino dsp xdddd*/
    return NULL;
}

struct pipeline_s{
    GList *scmd,
    bool wait
}

pipeline pipeline_new(void) {
    struct pipeline_s *pipe = malloc(sizeof(struct pipeline_s));
    pipe->scmdList = NULL;
    pipe->wait = true;
    return pipe;
}

static void sdestroy_scommand(scommand cmd) { scommand_destroy(cmd); }

pipeline pipeline_destroy(pipeline self){
    g_list_free_full(self->scmdList, (GDestroyNotify)&sdestroy_scommand);
    free(self);
    self = NULL;
    return self;
}

void pipeline_push_back(pipeline self, scommand sc){
    self->scmdList = g_list_append(self->scmdList, sc);
}

void pipeline_pop_front(pipeline self){
    scommand_destroy(g_list_first(self->scmdList)->data);
    self->scmdList = g_list_delete_link(self->scmdList, self->scmdList);
}

void pipeline_set_wait(pipeline self, const bool w){
    self->wait = w;
}

bool pipeline_is_empty(const pipeline self){
    return self->scmdList == NULL;
}

unsigned int pipeline_length(const pipeline self){
    return g_list_length (self->scmdList);
}

scommand pipeline_front(const pipeline self){
    return g_list_first(pipe->scmdList)->data;
}

bool pipeline_get_wait(const pipeline self){
    return self->wait;
}

char * pipeline_to_string(const pipeline self){
    /* Lo termino dsp xddd */
    return NULL;
>>>>>>> Stashed changes
}