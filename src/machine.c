#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions


// see ijvm.h for descriptions of the below functions

FILE *in;   // use fgetc(in) to get a character from in.
            // This will return EOF if no char is available.
FILE *out;  // use for example fprintf(out, "%c", value); to print value to out

// Global variables to free space after allocating each time in destroy_ijvm
word_t* initial_data_chunks;
word_t* pool_data;
int* text_size;
byte_t* text_data;

struct {

  word_t header;
  int constant_pool_size;
  word_t* constant_pool_data;
  int text_size;
  byte_t* text_data;
  //Stack* current_stack;

} IJVM_machine;

struct {

  int program_counter;
  int top_stack_index;
  bool finished_stack;
  word_t* stack_list;

} Stack;

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

  word_t* initial_data_chunks = malloc(sizeof(int) * 12); //Stores the header, pool origin, and pool size

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

  Stack.program_counter = 0;
  Stack.top_stack_index = 0;
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
  return Stack.stack_list[Stack.top_stack_index];
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
  int instruction_size = get_text_size();
  Stack.stack_list = (word_t*) malloc(instruction_size * 1024);

  //dprintf("The set size is %d\n", instruction_size);

  word_t instruction = (get_text())[Stack.program_counter]; //Fetches byte by byte of the instruction set

  //dprintf("The %d byte of the instruction is %02X\n", Stack.program_counter, instruction);

  // dprintf("The stack index is %d\n", Stack.top_stack_index);
  // dprintf("The counter is %d\n", Stack.program_counter);

  switch(instruction) {

    case OP_ERR: //OPCODE 0XFE
      dprintf("An error has occurred\n");
      Stack.finished_stack = true;
      break;
    case OP_NOP: //OPCODE 0X00
      break;
    case OP_BIPUSH: //OPCODE 0X10
      {
        int8_t instruction_value = get_text()[Stack.program_counter+1];
        word_t extended_next_instruction = (word_t)instruction_value;
        Stack.stack_list[Stack.top_stack_index] = extended_next_instruction;
        //dprintf("Value %d has been added to the stack_list as %02X.\nHere is the value in the stack: %02X\n",instruction_value, instruction_value, Stack.stack_list[Stack.top_stack_index]);
        //Stack.top_stack_index++;
        Stack.program_counter += 2;
        break;
      }
    case OP_DUP: //OPCODE 0X59
      //Stack.stack_list[Stack.top_stack_index+1] = Stack.stack_list[Stack.top_stack_index];
      //Stack.top_stack_index++;
      Stack.program_counter++;
      break;
    case OP_IADD: //OPCODE 0X60
      Stack.program_counter++;
      break;
    case OP_ISUB: //OPCODE 0X64
      Stack.program_counter++;
      break;
    case OP_IAND: //OPCODE 0XIE
      Stack.program_counter++;
      break;
    case OP_IOR: //OPCODE 0XB0
      Stack.program_counter++;
      break;
    case OP_POP: //OPCODE 0X57
      Stack.program_counter++;
      break;
    case OP_SWAP: //OPCODE 0X5F
      Stack.program_counter++;
      break;
    case OP_HALT: //OPCODE 0XFF
      Stack.finished_stack = true;
      break;
    case OP_IN: //OPCODE 0XFC
      Stack.program_counter++;
      break;
    case OP_OUT: //OPCODE 0XFD
      Stack.program_counter++;
      break;
    default:
      dprintf("Incorrect instruction architecture\n");
      break;

  }

  //dprintf("The 2A value is %02X\n", Stack.stack_list[Stack.top_stack_index]);
  // for (int i = 0; i < instruction_size; i++) {
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
