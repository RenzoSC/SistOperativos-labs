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
    g_list_free (self->cmdargs);
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
    self->cmdargs = g_list_prepend(self->cmdargs, argument);
    return self;
}

void scommand_pop_front(scommand self){
    Glist * last = g_list_last(self->cmdargs);
    self->cmdargs = g_list_remove_link(self->cmdargs, last);
    return self;
}