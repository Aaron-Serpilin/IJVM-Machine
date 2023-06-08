#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" //here is the ijvm and stack structs
#include "stack_functions.h" //pop and push functions
#include "instructions.h" //instruction set functions

// see ijvm.h for descriptions of the below functions

FILE *in;   // use fgetc(in) to get a character from in.
            // This will return EOF if no char is available.
FILE *out;  // use for example fprintf(out, "%c", value); to print value to out

// Global variables to free space after allocating each time in destroy_ijvm
word_t* initial_data_chunks;
word_t* pool_data;
int* text_size;
byte_t* text_data;

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

  // dprintf("The header is %02X\n", IJVM_machine.header);
  // dprintf("The pool size is %d\n", IJVM_machine.constant_pool_size);

  // for (int i = 0; i < IJVM_machine.constant_pool_size/4; i++) {
  //   dprintf("The %d pool data element is %02X\n", i, IJVM_machine.constant_pool_data[i]); 
  // };
  
  // dprintf("The text size is %d\n", IJVM_machine.text_size);

  // for (int i = 0; i < IJVM_machine.text_size; i++) {
  //   dprintf("The %d text data element is %02X\n", i, IJVM_machine.text_data[i]);
  // };

  fclose(file_pointer);

  Stack.max_stack_size = 1024;
  Stack.stack_list = (word_t*) malloc(get_text_size() * Stack.max_stack_size);
  Stack.program_counter = 0;
  Stack.current_stack_size = -1;
  Stack.finished_stack = false;
  
  return 0;
}

void destroy_ijvm(void) 
{
  // TODO: implement me
  free(initial_data_chunks);
  free(pool_data);
  free(text_size);
  free(text_data);
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
  // this operation should NOT pop (remove top element from Stack)
  // TODO: implement me
  return Stack.stack_list[Stack.current_stack_size];
}

bool finished(void) 
{
  // TODO: implement me
  return Stack.finished_stack;
}

word_t get_local_variable(int i) 
{
  // TODO: implement me
  return 0;
}

void step(void) //Executes the current instruction
{  

  word_t instruction = (get_text())[Stack.program_counter]; //Fetches byte by byte of the instruction set

  switch(instruction) {

    case OP_ERR: //OPCODE 0XFE
      error();
      break;

    case OP_BIPUSH: //OPCODE 0X10
      bi_push();
      break;

    case OP_DUP: //OPCODE 0X59
      duplicate();
      break;
    
    case OP_IADD: //OPCODE 0X60
      i_add();
      break;

    case OP_ISUB: //OPCODE 0X64
      i_sub();
      break;
      
    case OP_IAND: //OPCODE 0XIE
      i_and();
      break;

    case OP_IOR: //OPCODE 0XB0
      i_or();
      break;

    case OP_POP: //OPCODE 0X57
      instruction_pop();
      break;

    case OP_SWAP: //OPCODE 0X5F
      swap();
      break;

    case OP_HALT: //OPCODE 0XFF
      {
        Stack.finished_stack = true;
        break;
      }

    case OP_NOP: //OPCODE 0X00
      Stack.program_counter++;
      break;

    case OP_IN: //OPCODE 0XFC 
      {
        byte_t input_value = fgetc(in);
        instruction_input(input_value);
        break;
      }

    case OP_OUT: //OPCODE 0XFD 
      {
        char output_value = pop();
        fprintf(out, "%c", output_value);
        Stack.program_counter++;
        break;
      }

    default:
      break;

  }

  int instruction_size = get_text_size();

  if (Stack.program_counter >= instruction_size) { //Makes sure the counter does not surpass the size of the instruction set
    Stack.finished_stack = true;
  }

  // dprintf("The value is %02X\n", Stack.stack_list[Stack.current_stack_size]);
  // for (int i = 0; i < Stack.program_counter/2; i++) {
  //   dprintf("The %d element of the stack is %02X\n", i, Stack.stack_list[i]);
  // }

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
