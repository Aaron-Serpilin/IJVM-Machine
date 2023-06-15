#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" //here is the ijvm and stack structs
#include "stack_functions.h" //pop and push functions
#include "instruction_execution.h" // carry out the corresponding instruction
#include "data_sorter.h" // to read all the binary data and store it accodingly

// see ijvm.h for descriptions of the below functions

FILE *in;   // use fgetc(in) to get a character from in.
            // This will return EOF if no char is available.
FILE *out;  // use for example fprintf(out, "%c", value); to print value to out

// Global variables to free space after allocating each time in destroy_ijvm
word_t* initial_data_chunks;
word_t* pool_data;
int* text_size;
byte_t* text_data;

void set_input(FILE *fp) { in = fp; }

void set_output(FILE *fp) { out = fp; }

int init_ijvm(char *binary_path) 
{
  in = stdin;
  out = stdout;
  
  binary_path_data_sorter(binary_path, initial_data_chunks, pool_data, text_size, text_data);

  //Initialization of all Stack variables
  main_frame.main_stack.max_stack_size = 1024;
  main_frame.main_stack.stack_pointer = (word_t*) malloc(sizeof(word_t) * main_frame.main_stack.max_stack_size); //Fixes memory leak for second and third test
  main_frame.main_stack.program_counter = 0;
  main_frame.main_stack.current_stack_size = -1;
  main_frame.main_stack.finished_stack = false;

  //Initialization of all Frame variables

  //Initialization of Local Variables array
  main_frame.local_variables = malloc(sizeof(word_t) * 256); //Allocating the number of variables stored in standard IJVM
  
  return 0;
}

void destroy_ijvm(void) 
{
  // TODO: implement me
  free(initial_data_chunks);
  free(pool_data);
  free(text_size);
  free(text_data);
  free(main_frame.main_stack.stack_pointer);
}

byte_t *get_text(void) { return IJVM_machine.text_data;}

unsigned int get_text_size(void) { return IJVM_machine.text_size;}

word_t get_constant(int i) { return IJVM_machine.constant_pool_data[i];}

unsigned int get_program_counter(void) { return main_frame.main_stack.program_counter;}

word_t tos(void) { return main_frame.main_stack.stack_pointer[main_frame.main_stack.current_stack_size];}

bool finished(void) { return main_frame.main_stack.finished_stack;}

word_t get_local_variable(int i) { return main_frame.local_variables[i];
}

void step(void) //Executes the current instruction
{  

  word_t instruction = (get_text())[main_frame.main_stack.program_counter]; //Fetches byte by byte of the instruction set

  instruction_executioner(instruction);

  int instruction_size = get_text_size();

  if (main_frame.main_stack.program_counter >= instruction_size) { //Makes sure the counter does not surpass the size of the instruction set
    main_frame.main_stack.finished_stack = true;
  }

}

void run(void) 
{
  while (!finished()) 
  {
    step();
  }
}

byte_t get_instruction(void) 
{ 
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
