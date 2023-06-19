#ifndef STACK_CREATOR_H
#define STACK_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" // to access the IJVM struct

struct stack* stack_creator (void) {

    struct stack *new_stack = NULL;
    new_stack = malloc(sizeof(struct stack));

    new_stack->max_stack_size = 1024;
    new_stack->stack_pointer = (word_t *) malloc(sizeof(word_t) * new_stack->max_stack_size); 
    new_stack->program_counter = 0;
    new_stack->current_stack_size = -1;
    new_stack->finished_stack = false;

    return new_stack;

}

#endif
