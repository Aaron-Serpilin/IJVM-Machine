#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "ijvm.h"
#include "stack_creator.h"

word_t pop (struct frame *head_frame) {

    word_t top_value = 0;
    word_t garbage_value = 00;

    if (head_frame->main_stack->current_stack_size == -1) {
        dprintf("Cannot pop value. head_frame->main_stack->is empty\n");
    } else {
        head_frame->main_stack->stack_pointer[head_frame->main_stack->current_stack_size+1] = garbage_value;
        top_value = head_frame->main_stack->stack_pointer[head_frame->main_stack->current_stack_size];
        head_frame->main_stack->current_stack_size--;
    }

    return top_value;

}

void push (struct frame *head_frame, word_t value) {

    // if (head_frame->main_stack->current_stack_size == head_frame->main_stack->max_stack_size) {
    //     realloc(head_frame->main_stack->stack_pointer, head_frame->main_stack->max_stack_size);
    // }

    head_frame->main_stack->current_stack_size++;
    head_frame->main_stack->stack_pointer[head_frame->main_stack->current_stack_size] = value;

}

#endif
