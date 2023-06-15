#ifndef STACK_CREATOR_H
#define STACK_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" // to access the IJVM struct

extern current_frame * head;

struct stack stack_creator (struct frame* new_frame) {

     //Create a function that creates a new frame and then call invoke, and resetting the current frame to the newly created frame
    struct stack new_stack;
   
    return new_stack;
}

#endif
