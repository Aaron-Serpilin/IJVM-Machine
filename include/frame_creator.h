#ifndef FRAME_CREATOR_H
#define FRAME_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 
#include "stack_creator.h"

struct frame* frame_creator (struct frame *head_frame, int number_variables) {
    
    struct frame *new_frame = NULL;
    new_frame = malloc(sizeof(struct frame));

    new_frame->previous_frame_pointer = head_frame; //Set the previous frame pointer to the previous frame
    new_frame->local_variables = (word_t *) malloc(sizeof(word_t) * number_variables); //Each frame has its own variables array
    new_frame->main_stack = stack_creator(); //Initialization of each frame's stack
    new_frame->previous_program_counter = head_frame->main_stack->program_counter + 3;

    return new_frame;

}

#endif
