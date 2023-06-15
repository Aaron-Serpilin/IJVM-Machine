#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "ijvm.h"
#include "structs.h"

extern current_frame * head;

word_t pop (void) {

    word_t top_value = 0;
    word_t garbage_value = 00;

    if (head->main_stack.current_stack_size == -1) {
        dprintf("Cannot pop value. head->main_stack is empty\n");
    } else {
        head->main_stack.stack_pointer[head->main_stack.current_stack_size+1] = garbage_value;
        top_value = head->main_stack.stack_pointer[head->main_stack.current_stack_size];
        head->main_stack.current_stack_size--;
    }

    return top_value;

}

void push (word_t value) {

    // if (head->main_stack.current_stack_size == head->main_stack.max_stack_size) {
    //     realloc(head->main_stack.stack_pointer, head->main_stack.max_stack_size);
    // }

    head->main_stack.current_stack_size++;
    head->main_stack.stack_pointer[head->main_stack.current_stack_size] = value;

}

#endif
