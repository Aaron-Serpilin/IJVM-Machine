#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" //here is the ijvm and stack structs
#include "stack_functions.h" //pop and push functions
#include "instruction_execution.h" // carry out the corresponding instruction
#include "data_sorter.h" // to read all the binary data and store it accodingly
#include "stack_creator.h" // to initialize the stack of the current frame
#include "frame_creator.h" // to initialize the current frame

FILE *in;   
            
FILE *out;  

void set_input(FILE *fp) { in = fp; }

void set_output(FILE *fp) { out = fp; }

// Initialization of Head Frame
struct frame *head = NULL;

int init_ijvm(char *binary_path) {
  in = stdin;
  out = stdout;

  binary_path_data_sorter(binary_path, global_variables.initial_data_chunks, global_variables.pool_data, global_variables.text_size, global_variables.text_data);

  //Initialization of the Frame
  head = malloc(sizeof(struct frame));

  if (head == NULL) {return 1;}

  head->main_stack = stack_creator(head);
  head = frame_creator(head, 256);

  return 0;
}

void destroy_ijvm(void) {
  free(global_variables.initial_data_chunks);
  free(global_variables.pool_data);
  free(global_variables.text_size);
  free(global_variables.text_data);
  free(head->main_stack->stack_pointer);
}

byte_t *get_text(void) { return ijvm_machine.text_data;}

unsigned int get_text_size(void) { return ijvm_machine.text_size;}

word_t get_constant(int i) { return ijvm_machine.constant_pool_data[i];}

unsigned int get_program_counter(void) { return head->main_stack->program_counter;}

word_t tos(void) { return head->main_stack->stack_pointer[head->main_stack->current_stack_size];}

bool finished(void) { return head->main_stack->finished_stack;}

word_t get_local_variable(int i) { return head->local_variables[i];}

//Executes the current instruction 
void step(void) { 

  word_t instruction = (get_text())[head->main_stack->program_counter]; //Fetches byte by byte of the instruction set

  instruction_executioner(instruction);

  int instruction_size = get_text_size();

  if (head->main_stack->program_counter >= instruction_size) { //Makes sure the counter does not surpass the size of the instruction set
    head->main_stack->finished_stack = true;
  }

}

void run(void) {
  while (!finished()) {
    step();
  }
}

byte_t get_instruction(void) { 
  return get_text()[get_program_counter()]; 
}

// Below: methods needed by bonus assignments, see ijvm.h

//int get_call_stack_size(void) 
//{
   // TODO: implement me
//   return sp;
//}


// Checks if reference is a freed heap array. Note that this assumes that 
// 
//bool is_heap_freed(word_t reference) 
//{
   // TODO: implement me
// return 0;
//}
