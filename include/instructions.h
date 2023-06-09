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

void go_to () {

    int8_t instruction_value = get_text()[Stack.program_counter+1];
    word_t extended_instruction_value = (word_t)instruction_value;

    int number_offset_bytes = 2;
    int instruction_size = get_text_size();
    //short branch_offset = malloc(number_offset_bytes);
    int8_t first_offset_byte = get_text()[Stack.program_counter+1];
    int8_t second_offset_byte = get_text()[Stack.program_counter+2];
    // char offset_string[] = first_offset_byte + second_offset_byte;
    // dprintf("The string offset is %c\n", offset_string);
    short branch_offset = (short) first_offset_byte << 32 | second_offset_byte;

    dprintf("The instruction size is %d and the offset is %d\n", instruction_size, branch_offset);

    if (branch_offset >= instruction_size) {
        Stack.finished_stack = true;
        Stack.program_counter++;
    } else {
        Stack.program_counter += branch_offset;
    }

    dprintf("The first offset is %02X\nThe second offset is %02X\nThe branch offset is %02X\n", first_offset_byte, second_offset_byte,branch_offset);
    //dprintf("\n--- 02X PRINTING ---\nThe first offset is %02X\nThe second offset is %02X\nThe branch offset is %02X\n --- END ---\n", first_offset_byte, second_offset_byte,branch_offset);

    return;
}

void if_eq (void) {
    return;
}

void iflt (void) {
    return;
}

void if_icmpeq (void) {
    return;
}

#endif
