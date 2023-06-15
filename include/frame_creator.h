#ifndef FRAME_CREATOR_H
#define FRAME_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 
#include "stack_creator.h"

extern current_frame * head;
//Create a function that creates a new frame and then call invoke, and resetting the current frame to the newly created frame

void frame_creator (void) {

    struct frame *new_frame = NULL;
    new_frame = malloc(sizeof(struct frame));

    new_frame->previous_frame_pointer = head;
    new_frame->previous_stack_pointer = head->main_stack.stack_pointer;
    new_frame->local_variables = malloc(sizeof(word_t) * 256); //Each frame has its own variables array
    new_frame->main_stack = stack_creator(new_frame);

    head = new_frame;

}

#endif
