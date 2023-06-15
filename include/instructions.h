#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h> 
#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "stack_functions.h"
#include "frame_creator.h"

extern current_frame * head;

void error (void) {
    dprintf("An error has occurred\n");
    head->main_stack.finished_stack = true;
}

void bi_push (void) {
    head->main_stack.current_stack_size++;
    int8_t instruction_value = get_text()[head->main_stack.program_counter+1]; //These two lines read 8-bit values that are negative in 32-bits and should be read as such
    word_t extended_instruction_value = (word_t)instruction_value;
    head->main_stack.stack_pointer[head->main_stack.current_stack_size] = extended_instruction_value;
    head->main_stack.program_counter += 2;
}

void duplicate (void) {
    word_t top_value = head->main_stack.stack_pointer[head->main_stack.current_stack_size]; 
    head->main_stack.current_stack_size++;
    head->main_stack.stack_pointer[head->main_stack.current_stack_size] = top_value;
    head->main_stack.program_counter++;
}

void i_add (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t summation_value = top_value + new_top_value;
    push(summation_value);
    head->main_stack.program_counter++;
}

void i_sub (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t subtraction_value = new_top_value - top_value;
    push(subtraction_value);
    head->main_stack.program_counter++;
}

void i_and (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t and_value = top_value & new_top_value;
    push(and_value);
    head->main_stack.program_counter++;
}

void i_or (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    word_t or_value = top_value | new_top_value;
    push(or_value);
    head->main_stack.program_counter++;
}

void instruction_pop (void) {
    pop();
    head->main_stack.program_counter++;
}

void swap (void) {
    word_t top_value = pop();
    word_t new_top_value = pop();
    push(top_value);
    push(new_top_value);
    head->main_stack.program_counter++;
}

void instruction_input (byte_t input_value) {

    if (!input_value) {
        input_value = 0;
    } 

    push((word_t) input_value);

    head->main_stack.program_counter++;
}

void go_to (void) {
    byte_t* branch_pointer = get_text() + (head->main_stack.program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);
    head->main_stack.program_counter += branch_offset;
}

void if_eq (void) {
    byte_t top_value = pop();
    byte_t* branch_pointer = get_text() + (head->main_stack.program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value == 0) {
        head->main_stack.program_counter += branch_offset;
    } else {
        head->main_stack.program_counter += 3;
    }
}

void iflt (void) {
    int8_t top_value = pop(); //Need to use int8_t rather than byte_t to check for signed values
    byte_t* branch_pointer = get_text() + (head->main_stack.program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value < 0) {
        head->main_stack.program_counter += branch_offset;
    } else {
        head->main_stack.program_counter += 3;
    }
}

void if_icmpeq (void) {
    byte_t top_value = pop();
    byte_t new_top_value = pop();
    byte_t* branch_pointer = get_text() + (head->main_stack.program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value == new_top_value) {
        head->main_stack.program_counter += branch_offset;
    } else {
        head->main_stack.program_counter += 3;
    }
}

void ldc_w (void) {
    byte_t* branch_pointer = get_text() + (head->main_stack.program_counter+1);
    short constant_index = read_uint16_t(branch_pointer);
    word_t targeted_constant = get_constant(constant_index);
    push(targeted_constant);
    head->main_stack.program_counter += 3;
}

void iload (void) {
    byte_t variable_index = (get_text())[head->main_stack.program_counter+1];
    word_t value_at_index = head->local_variables[variable_index];
    push(value_at_index);
    head->main_stack.program_counter += 2;
}

void istore (void) {
    byte_t variable_index = (get_text())[head->main_stack.program_counter+1];
    word_t top_value = pop();
    head->local_variables[variable_index] = top_value;
    head->main_stack.program_counter += 2;
}

void iinc (void) {
    byte_t increment_index = (get_text())[head->main_stack.program_counter+1];
    int8_t increment_value = (get_text())[head->main_stack.program_counter+2]; //In case the numbers are negative, we make it a 32-bit word to read the first bit in its complete length
    word_t absolute_increment_value = (word_t) increment_value;
    head->local_variables[increment_index] += absolute_increment_value;
    head->main_stack.program_counter += 3;
}

void wide (void) {
    // We treat the wide instruction and the modified instruction as a single step
    word_t next_instruction = get_text()[head->main_stack.program_counter+1];

    switch (next_instruction) {

        case OP_ISTORE: 
        {
            byte_t* extended_index_pointer = get_text() + (head->main_stack.program_counter+2); 
            short extended_index = read_uint16_t(extended_index_pointer);
            word_t top_value = pop();
            head->local_variables[extended_index] = top_value;
            head->main_stack.program_counter += 4;
            break;
        }
            
        case OP_ILOAD: 
        {
            byte_t* extended_index_pointer = get_text() + (head->main_stack.program_counter+2);
            short extended_index = read_uint16_t(extended_index_pointer);
            word_t value_at_index = head->local_variables[extended_index];
            push(value_at_index);
            head->main_stack.program_counter += 4;
            break;
        }
            
        case OP_IINC:
        {
            byte_t* extended_index_pointer = get_text() + (head->main_stack.program_counter+2);
            short extended_index = read_uint16_t(extended_index_pointer);
            int8_t increment_value = (get_text())[head->main_stack.program_counter+4]; 
            word_t absolute_increment_value = (word_t) increment_value;
            head->local_variables[extended_index] += absolute_increment_value;
            head->main_stack.program_counter += 5;
            break;
        }
           
        default:
            break;
    }
}

void invoke_virtual (void) {
    
    byte_t* starting_address_pointer = get_text() + (head->main_stack.program_counter+1);
    short starting_address_index = read_uint16_t(starting_address_pointer);
    word_t constant_value = get_constant(starting_address_index);

    byte_t* number_arguments_pointer = get_text() + (constant_value);
    short number_arguments = read_uint16_t(number_arguments_pointer);
    byte_t* number_variables_pointer = get_text() + (constant_value+2);
    short number_variables = read_uint16_t(number_variables_pointer);

     //frame_creator();

}

// void ireturn (void) {

//     //Pop the top value of the stack
//     //Pop the rest
//     //Push the initial top value
//     return;
// }

#endif
