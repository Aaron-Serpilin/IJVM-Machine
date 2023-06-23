#ifndef TOTAL_STACK_SIZE_H
#define TOTAL_STACK_SIZE_H

#include <stdio.h> 
#include "ijvm.h"
#include "structs.h"
#include "util.h" 

extern current_frame * head;

int total_stack_size (void) {

    int total_stack_size = 0;

    //dprintf("The current stack size is %d\n", head->main_stack->current_stack_size);
    while (head != NULL) {

        total_stack_size += head->main_stack->current_stack_size;

        if (head->previous_frame_pointer == NULL) {
            break;
        }
        head = head->previous_frame_pointer;
    }

    total_stack_size *= -1;
    return total_stack_size;
}

#endif
