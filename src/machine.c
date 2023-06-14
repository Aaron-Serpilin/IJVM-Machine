#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" //here is the ijvm and stack structs
#include "stack_functions.h" //pop and push functions
#include "instruction_execution.h" // carry out the corresponding instruction

// see ijvm.h for descriptions of the below functions

FILE *in;   // use fgetc(in) to get a character from in.
            // This will return EOF if no char is available.
FILE *out;  // use for example fprintf(out, "%c", value); to print value to out

// Global variables to free space after allocating each time in destroy_ijvm
word_t* initial_data_chunks;
word_t* pool_data;
int* text_size;
byte_t* text_data;
word_t* variables_array; 

void set_input(FILE *fp) 
{ 
  in = fp; 
}

void set_output(FILE *fp) 
{ 
  out = fp; 
}

int init_ijvm(char *binary_path) 
{
  //To avoid memory leaks, important to allocate and read the same number of bytes
  in = stdin;
  out = stdout;
  FILE* file_pointer = fopen(binary_path, "rb");

  if (file_pointer == NULL) {
    dprintf("Input file cannot be opened\n");
    return 1;
  }

  initial_data_chunks = malloc(sizeof(int) * 12); //Stores the header, pool origin, and pool size

  fread(initial_data_chunks, sizeof(word_t), 3, file_pointer); //I read first 3 chunks of 4 bytes

  for (int i = 0; i < 3; i++) {
    
    initial_data_chunks[i] = swap_uint32(initial_data_chunks[i]);

  };

  IJVM_machine.header = initial_data_chunks[0];

  if (IJVM_machine.header != 0x1deadfad) {
    dprintf("Header is incorrect\n");
    return -1;
  }

  IJVM_machine.constant_pool_size = initial_data_chunks[2];

  pool_data = malloc(sizeof(word_t) * (IJVM_machine.constant_pool_size/4)); //Allocate 4 bytes of memory the pool size number of elements

  fread(pool_data, sizeof(word_t), IJVM_machine.constant_pool_size/4, file_pointer); // I read then all the pool data chunks of 4 bytes

  for (int i = 0; i < IJVM_machine.constant_pool_size/4; i++) {

    pool_data[i] = swap_uint32(pool_data[i]);

  }

  IJVM_machine.constant_pool_data = pool_data; //The data has (pool_size/4) elements, and each element has 4 bytes. Can be printed in %02X

  text_size = malloc(sizeof(word_t) * 2); //The 2 is due to the 2 chunks of data that make up the text size

  fread(text_size, 2, sizeof(word_t), file_pointer); //I read the 2 chunks of 4 bytes about the text size

  for (int i = 0; i < 2; i++) {
    text_size[i] = swap_uint32(text_size[i]);
  }

  IJVM_machine.text_size = text_size[1];

  text_data = malloc(sizeof(word_t) * (IJVM_machine.text_size));

  fread(text_data, sizeof(word_t), IJVM_machine.text_size, file_pointer); 

  IJVM_machine.text_data = text_data; //It has size bytes and size elements

  fclose(file_pointer);

  //Initialization of all Stack variables
  Stack.max_stack_size = 1024;
  Stack.stack_list = (word_t*) malloc(sizeof(word_t) * Stack.max_stack_size); //Fixes memory leak for second and third test
  Stack.program_counter = 0;
  Stack.current_stack_size = -1;
  Stack.finished_stack = false;

  //Initialization of all Frame variables
  Stack.previous_program_counter = 0;
  Stack.previous_link_pointer_value = 0;

  //Initialization of Local Variables array
  variables_array = malloc(sizeof(word_t) * 256); //Allocating the number of variables stored in standard IJVM
  
  return 0;
}

void destroy_ijvm(void) 
{
  // TODO: implement me
  free(initial_data_chunks);
  free(pool_data);
  free(text_size);
  free(text_data);
  free(Stack.stack_list);
  free(variables_array);
}

byte_t *get_text(void) 
{
  return IJVM_machine.text_data;
}

unsigned int get_text_size(void) 
{
  return IJVM_machine.text_size;
}

word_t get_constant(int i) 
{
  return IJVM_machine.constant_pool_data[i];
}

unsigned int get_program_counter(void) 
{
  return Stack.program_counter;
}

word_t tos(void) 
{
  return Stack.stack_list[Stack.current_stack_size];
}

bool finished(void) 
{
  return Stack.finished_stack;
}

word_t get_local_variable(int i) 
{
  return variables_array[i];
}

void step(void) //Executes the current instruction
{  

  word_t instruction = (get_text())[Stack.program_counter]; //Fetches byte by byte of the instruction set

  instruction_executioner(instruction);

  int instruction_size = get_text_size();

  if (Stack.program_counter >= instruction_size) { //Makes sure the counter does not surpass the size of the instruction set
    Stack.finished_stack = true;
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
