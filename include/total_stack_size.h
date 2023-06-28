#ifndef TOTAL_STACK_SIZE_H
#define TOTAL_STACK_SIZE_H

#include <stdio.h> 
#include "ijvm.h"
#include "structs.h"
#include "util.h" 

extern current_frame * head;

int total_stack_size (void) {

    int stack_size = 0;

    struct frame *current_head = head;

    while (current_head != NULL) { // I loop through all the stacks until I reach the initial frame before main which is NULL

        stack_size += current_head->main_stack->current_stack_size;

        if (current_head->previous_frame_pointer == NULL) {
            break;
        }
        
        current_head = current_head->previous_frame_pointer;
    }

    stack_size *= -1; // I have to do this given that my initial stack size is -1
    return stack_size;
}

#endif
