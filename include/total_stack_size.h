#ifndef TOTAL_STACK_SIZE_H
#define TOTAL_STACK_SIZE_H

#include <stdio.h> 
#include "ijvm.h"
#include "structs.h"
#include "util.h" 

int total_stack_size (void) {

    int total_stack_size = 0;

    while (head != NULL) { // I loop through all the stacks until I reach the initial frame before main which is NULL

        total_stack_size += head->main_stack->current_stack_size;

        if (head->previous_frame_pointer == NULL) {
            break;
        }
        
        head = head->previous_frame_pointer;
    }

    total_stack_size *= -1; // I have to do this given that my initial stack size is -1
    return total_stack_size;
}

#endif
