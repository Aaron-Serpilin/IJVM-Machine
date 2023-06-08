#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <stdio.h>  // for getc, printf
#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "stack_functions.h"

void error (void) {
    dprintf("An error has occurred\n");
    Stack.finished_stack = true;
}

void bi_push (void) {
    //dprintf("The index is %d and the counter is %d\n", Stack.current_stack_size, Stack.program_counter);
    Stack.current_stack_size++;
    int8_t instruction_value = get_text()[Stack.program_counter+1];
    word_t extended_instruction_value = (word_t)instruction_value;
    Stack.stack_list[Stack.current_stack_size] = extended_instruction_value;
    Stack.program_counter += 2;
}

void duplicate (void) {
    word_t top_value = Stack.stack_list[Stack.current_stack_size]; 
    Stack.current_stack_size++;
    Stack.stack_list[Stack.current_stack_size] = top_value;
    Stack.program_counter++;
}

void i_add (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t summation_value = top_value + new_top_value;
    push(summation_value);
    Stack.program_counter++;
}

void i_sub (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t subtraction_value = new_top_value - top_value;
    push(subtraction_value);
    Stack.program_counter++;
}

void i_and (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t and_value = top_value & new_top_value;
    push(and_value);
    Stack.program_counter++;
}

void i_or (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t or_value = top_value | new_top_value;
    push(or_value);
    Stack.program_counter++;
}

void instruction_pop (void) {
    pop();
    Stack.program_counter++;
}

void swap (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    push(top_value);
    push(new_top_value);
    Stack.program_counter++;
}

void instruction_input (byte_t input_value) {

    if (!input_value) {
        input_value = 0;
    } 

    push((word_t) input_value);

    Stack.program_counter++;
}

#endif
