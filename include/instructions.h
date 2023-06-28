#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h> 
#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "stack_functions.h"
#include "frame_creator.h"
#include "stack_creator.h"
#include "heap_creator.h"
#include "frame_destroyer.h"
#include "heap_finder.h"

extern current_frame * head;
extern current_heap * heap;
FILE *in;             
FILE *out;

void error (void) {
    dprintf("An error has occurred\n");
    head->main_stack->finished_stack = true;
}

void bi_push (void) { 
    int8_t instruction_value = get_text()[head->main_stack->program_counter+1]; //By passing the instruction as a word_t parameter in push, nega tive values that wouldn't be in 8-bits are read correctly in 32-bits and obtain the correct leading sign bit
    push(head,  instruction_value);
    head->main_stack->program_counter += 2;
}

void duplicate (void) {
    word_t top_value = head->main_stack->stack_pointer[head->main_stack->current_stack_size]; 
    push(head,  top_value);
    head->main_stack->program_counter++;
}

void i_add (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    word_t summation_value = top_value + new_top_value;
    push(head, summation_value);
    head->main_stack->program_counter++;
}

void i_sub (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    word_t subtraction_value = new_top_value - top_value;
    push(head, subtraction_value);
    head->main_stack->program_counter++;
}

void i_and (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    word_t and_value = top_value & new_top_value;
    push(head, and_value);
    head->main_stack->program_counter++;
}

void i_or (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    word_t or_value = top_value | new_top_value;
    push(head, or_value);
    head->main_stack->program_counter++;
}

void instruction_pop (void) {
    pop(head);
    head->main_stack->program_counter++;
}

void swap (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    push(head, top_value);
    push(head, new_top_value);
    head->main_stack->program_counter++;
}

void halt (void) {
    head->main_stack->finished_stack = true;
}

void do_nothing (void) {
    head->main_stack->program_counter++;
}

void instruction_input (void) {

    word_t input_value = fgetc(in);

    if (input_value == EOF) {
        input_value = 0;
    } 

    push(head,  input_value);

    head->main_stack->program_counter++;
}

void instruction_output (void) {
    char output_value = pop(head);
    fprintf(out, "%c", output_value);
    head->main_stack->program_counter++;
}

void go_to (void) {
    byte_t* branch_pointer = get_text() + (head->main_stack->program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);
    head->main_stack->program_counter += branch_offset;
}

void if_eq (void) {
    word_t top_value = pop(head);
    byte_t* branch_pointer = get_text() + (head->main_stack->program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value == 0) {
        head->main_stack->program_counter += branch_offset;
    } else {
        head->main_stack->program_counter += 3;
    }
}

void iflt (void) {
    word_t top_value = pop(head); //Need to use int8_t rather than byte_t to check for signed values
    byte_t* branch_pointer = get_text() + (head->main_stack->program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value < 0) {
        head->main_stack->program_counter += branch_offset;
    } else {
        head->main_stack->program_counter += 3;
    }
}

void if_icmpeq (void) {
    word_t top_value = pop(head);
    word_t new_top_value = pop(head);
    byte_t* branch_pointer = get_text() + (head->main_stack->program_counter+1);
    short branch_offset = read_uint16_t(branch_pointer);

    if (top_value == new_top_value) {
        head->main_stack->program_counter += branch_offset;
    } else {
        head->main_stack->program_counter += 3;
    }
}

void ldc_w (void) {
    byte_t* branch_pointer = get_text() + (head->main_stack->program_counter+1);
    short constant_index = read_uint16_t(branch_pointer);
    word_t targeted_constant = get_constant(constant_index);
    push(head, targeted_constant);
    head->main_stack->program_counter += 3;
}

void iload (void) {
    word_t variable_index = (get_text())[head->main_stack->program_counter+1];
    word_t value_at_index = head->local_variables[variable_index];
    push(head, value_at_index);
    head->main_stack->program_counter += 2;
}

void istore (void) {
    word_t variable_index = (get_text())[head->main_stack->program_counter+1];
    word_t top_value = pop(head);
    head->local_variables[variable_index] = top_value;
    head->main_stack->program_counter += 2;
}

void iinc (void) {
    byte_t increment_index = (get_text())[head->main_stack->program_counter+1];
    int8_t increment_value = (get_text())[head->main_stack->program_counter+2]; //In case the numbers are negative, we make it a 32-bit word to read the first bit in its complete length
    word_t absolute_increment_value = (word_t) increment_value;
    head->local_variables[increment_index] += absolute_increment_value;
    head->main_stack->program_counter += 3;
}

void wide (void) {
    // We treat the wide instruction and the modified instruction as a single step
    word_t next_instruction = (word_t) (get_text()[head->main_stack->program_counter+1]);

    switch (next_instruction) {

        case OP_ISTORE: 
        {
            byte_t* extended_index_pointer = (get_text() + (head->main_stack->program_counter+2)); 
            word_t extended_index = read_uint16_t(extended_index_pointer);
            word_t top_value = pop(head);
            head->local_variables[extended_index] = top_value;
            head->main_stack->program_counter += 4;
            break;
        }
            
        case OP_ILOAD: 
        {
            byte_t* extended_index_pointer = (get_text() + (head->main_stack->program_counter+2));
            word_t extended_index = read_uint16_t(extended_index_pointer);
            word_t value_at_index = head->local_variables[extended_index];
            push(head, value_at_index);
            head->main_stack->program_counter += 4;
            break;
        }
            
        case OP_IINC:
        {
            byte_t* extended_index_pointer = (get_text() + (head->main_stack->program_counter+2));
            word_t extended_index = read_uint16_t(extended_index_pointer);
            int8_t increment_value = (get_text())[head->main_stack->program_counter+4]; 
            word_t absolute_increment_value = increment_value;
            head->local_variables[extended_index] += absolute_increment_value;
            head->main_stack->program_counter += 5;
            break;
        }
           
        default:
            break;
    }
}

void invoke_virtual (void) {

    byte_t* starting_address_pointer = get_text() + (head->main_stack->program_counter+1);
    short starting_address_index = read_uint16_t(starting_address_pointer);
    word_t constant_value = get_constant(starting_address_index);
    short offset = constant_value + 4;

    byte_t* number_arguments_pointer = get_text() + (constant_value);
    word_t number_arguments = read_uint16_t(number_arguments_pointer);
    byte_t* number_variables_pointer = get_text() + (constant_value+2);
    word_t number_variables = read_uint16_t(number_variables_pointer);

    word_t frame_argument;
    word_t frame_arguments_array[number_arguments];

    head = frame_creator(head, number_variables + number_arguments);
    
    for (int i = number_arguments - 1; i >= 0; i--) {
        frame_argument = pop(head->previous_frame_pointer);
        frame_arguments_array[i] = frame_argument;
    }

    //Adding the arguments to the local variables array
    for (int i = 0; i < number_arguments; i++) {
        head->local_variables[i] = frame_arguments_array[i];
    }

    head->main_stack->program_counter += offset;

}

void ireturn (void) {

    int new_counter = head->previous_program_counter;
    struct frame* frame_to_be_destroyed = head;
    word_t frame_return_value = pop(head);
    head = head->previous_frame_pointer;
    head->main_stack->program_counter = new_counter;
    frame_destroyer(frame_to_be_destroyed);
    push(head, frame_return_value);

}

void tailcall (void) {
    
    byte_t* starting_address_pointer = get_text() + (head->main_stack->program_counter+1);
    short starting_address_index = read_uint16_t(starting_address_pointer);
    word_t constant_value = get_constant(starting_address_index);
    short offset = constant_value + 4;

    byte_t* number_arguments_pointer = get_text() + (constant_value);
    word_t number_arguments = read_uint16_t(number_arguments_pointer);
    byte_t* number_variables_pointer = get_text() + (constant_value+2);
    word_t number_variables = read_uint16_t(number_variables_pointer);

    word_t frame_argument;
    word_t frame_arguments_array[number_arguments];

    struct frame* intermediate_frame = head; // Store frame to be deleted
    struct frame* new_return_frame = head->previous_frame_pointer; //Store new frame for head to return to

    head = frame_creator(head, number_variables + number_arguments); 
    head->previous_program_counter = intermediate_frame->previous_program_counter; // Restore head's previous PC to the new_return_frame's PC

    for (int i = number_arguments - 1; i >= 0; i--) {
        frame_argument = pop(head->previous_frame_pointer);
        frame_arguments_array[i] = frame_argument;
    }

    for (int i = 0; i < number_arguments; i++) {
        head->local_variables[i] = frame_arguments_array[i];
    }

    frame_destroyer(intermediate_frame); // Clear intermediate frame
    head->previous_frame_pointer = new_return_frame;

    head->main_stack->program_counter += offset;
}

void new_array (void) {

    word_t heap_size = pop(head);
    heap = heap_creator(heap, heap_size);
    push(head, heap->heap_index);
    head->main_stack->program_counter++;

}

void iaload (void) {

    word_t array_reference = pop(head);
    word_t variable_index = pop(head);
    struct heap *referenced_heap = heap_finder(heap, array_reference);
    word_t value_at_index = referenced_heap->heap_stack->stack_pointer[variable_index];
    push(head, value_at_index);
    head->main_stack->program_counter++;

}

void iastore (void) {

    word_t array_reference = pop(head);
    word_t variable_index = pop(head);
    word_t top_value = pop(head);
    struct heap *referenced_heap = heap_finder(heap, array_reference);
    referenced_heap->heap_stack->stack_pointer[variable_index] = top_value;
    head->main_stack->program_counter++;

}

#endif
