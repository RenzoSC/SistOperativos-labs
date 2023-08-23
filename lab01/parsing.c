#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    /* Devuelve NULL cuando hay un error de parseo */
    return NULL;
}

pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;

    cmd = parse_scommand(p);
    error = (cmd==NULL); /* Comando inválido al empezar */
    while (another_pipe && !error) {
        /*
         * COMPLETAR
         *
         */
    }
    /* Opcionalmente un OP_BACKGROUND al final */
    }
    /*
     *
     * COMPLETAR
     *
     */

    /* Tolerancia a espacios posteriores */
    /* Consumir todo lo que hay inclusive el \n */
    /* Si hubo error, hacemos cleanup */

    return NULL; // MODIFICAR
}

