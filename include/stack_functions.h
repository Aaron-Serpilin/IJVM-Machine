#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H
#include "ijvm.h"
#include "structs.h"

word_t pop (void) {

    word_t top_value = 0;
    word_t garbage_value = 00;

    if (main_frame.main_stack.current_stack_size == -1) {
        dprintf("Cannot pop value. main_frame.main_stack is empty\n");
    } else {
        main_frame.main_stack.stack_pointer[main_frame.main_stack.current_stack_size+1] = garbage_value;
        top_value = main_frame.main_stack.stack_pointer[main_frame.main_stack.current_stack_size];
        main_frame.main_stack.current_stack_size--;
    }

    return top_value;

}

void push (word_t value) {

    // if (main_frame.main_stack.current_stack_size == main_frame.main_stack.max_stack_size) {
    //     realloc(main_frame.main_stack.stack_pointer, main_frame.main_stack.max_stack_size);
    // }

    main_frame.main_stack.current_stack_size++;
    main_frame.main_stack.stack_pointer[main_frame.main_stack.current_stack_size] = value;

}

#endif
