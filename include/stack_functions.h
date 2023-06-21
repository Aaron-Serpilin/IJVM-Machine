#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "ijvm.h"
#include "stack_creator.h"

word_t pop (struct frame *head_frame) {

    word_t top_value = 0;

    if (head_frame->main_stack->current_stack_size == -1) {
        dprintf("Cannot pop value. head_frame->main_stack->is empty\n");
    } else {
        top_value = head_frame->main_stack->stack_pointer[head_frame->main_stack->current_stack_size];
        head_frame->main_stack->current_stack_size--;
    }

    return top_value;

}

void push (struct frame *head_frame, word_t value) {

    head_frame->main_stack->current_stack_size++;
    head_frame->main_stack->stack_pointer[head_frame->main_stack->current_stack_size] = value;

}

#endif
