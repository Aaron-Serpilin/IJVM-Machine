#ifndef FRAME_CREATOR_H
#define FRAME_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 
#include "stack_creator.h"

extern current_frame * head;

void frame_creator (void) {

    struct frame *new_frame = NULL;
    new_frame = malloc(sizeof(struct frame));

    new_frame->previous_frame_pointer = head; //Set the previous frame pointer to the previous frame
    new_frame->previous_stack_pointer = head->main_stack.stack_pointer; 
    new_frame->local_variables = malloc(sizeof(word_t) * 256); //Each frame has its own variables array
    stack_creator(new_frame);
    new_frame->previous_program_counter = head->main_stack.program_counter;

    head = new_frame; //Recursively reassign the current frame (head) to the newly created frame

}

#endif
