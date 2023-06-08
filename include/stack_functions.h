#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H
#include "ijvm.h"
#include "structs.h"

word_t pop (void) {

    word_t top_value;
    word_t garbage_value = 00;

    if (Stack.current_stack_size == -1) {
        dprintf("Cannot pop value. Stack is empty\n");
    } else {
        Stack.stack_list[Stack.current_stack_size+1] = garbage_value;
        top_value = Stack.stack_list[Stack.current_stack_size];
        Stack.current_stack_size--;
    }

    return top_value;

}

void push (word_t value) {

    if (Stack.current_stack_size == Stack.max_stack_size) {
        realloc(Stack.stack_list, Stack.max_stack_size);
    }

    Stack.current_stack_size++;
    Stack.stack_list[Stack.current_stack_size] = value;

}

#endif
