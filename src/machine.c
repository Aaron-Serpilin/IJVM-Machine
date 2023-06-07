#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions


// see ijvm.h for descriptions of the below functions

FILE *in;   // use fgetc(in) to get a character from in.
            // This will return EOF if no char is available.
FILE *out;  // use for example fprintf(out, "%c", value); to print value to out

struct {

  word_t header;
  int constant_pool_size;
  word_t* constant_pool_data;
  int text_size;
  word_t* text_data;

} ijvm_machine;

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

  in = stdin;
  out = stdout;
  // TODO: implement me
  FILE* starting_file_pointer = fopen(binary_path, "rb");

  if (starting_file_pointer == NULL) {
    dprintf("Input file cannot be opened\n");
    return 1;
  }

  word_t machine_data_array[3]; //Stores the header, pool origin, and pool size

  fread(machine_data_array, 4, 3, starting_file_pointer);

  for (int i = 0; i < 3; i++) {
    
    machine_data_array[i] = swap_uint32(machine_data_array[i]);

  };

  ijvm_machine.header = machine_data_array[0];

  if (ijvm_machine.header == 0x1deadfad) {
    dprintf("Header is correct\n");
  } else {
    dprintf("Incorrect header\n");
    return -1;
  }

  ijvm_machine.constant_pool_size = machine_data_array[2];

  word_t* pool_data = calloc(ijvm_machine.constant_pool_size/4, 4); //Allocate 4 bytes of memory the pool size number of elements

  fread(pool_data, 4, ijvm_machine.constant_pool_size/8, starting_file_pointer);
  // fread(pool_data, 4, ijvm_machine.constant_pool_size/4, starting_file_pointer);

  dprintf("The pool size is %d\n", ijvm_machine.constant_pool_size);

  for (int i = 0; i < ijvm_machine.constant_pool_size/4; i++) {

    pool_data[i] = swap_uint32(pool_data[i]);

  }

  ijvm_machine.constant_pool_data = pool_data;

  int* text_size = calloc(4, 4);
  
  dprintf("The header is %02X\n", ijvm_machine.header);
  dprintf("The pool size is %d\n", ijvm_machine.constant_pool_size);
  dprintf("The first part of the pool data is %02X\n", ijvm_machine.constant_pool_data[0]);
  dprintf("The second part of the pool data is %02X\n", ijvm_machine.constant_pool_data[1]);
  dprintf("The third part of the pool data is %02X\n", ijvm_machine.constant_pool_data[2]);
  // dprintf("The header is %02x\n", machine_data_array[0]);
  // dprintf("The origin is %02X\n", machine_data_array[1]);
  // dprintf("The size is %d\n", machine_data_array[2]);

  
  /* --- Loop --- */

  // int individual_bytes;
  // int* byte_array = malloc(sizeof(int) * 5);
 
  // for (int i = 0; (individual_bytes = getc(starting_file_pointer)) != EOF; i++) {
  //   dprintf("%02X", individual_bytes);

  //       if (i % 4 == 3) // Guarantees each line is 8 characters/4 bytes
  //           putc('\n', out); 
  // }

  // dprintf("\n");
  //return -1;

  fclose(starting_file_pointer);
  
  return -1;
}

void destroy_ijvm(void) 
{
  // TODO: implement me
}

byte_t *get_text(void) 
{
  // TODO: implement me
  return NULL;
}

unsigned int get_text_size(void) 
{
  // TODO: implement me
  return 0;
}

word_t get_constant(int i) 
{
  // TODO: implement me
  return 0;
}

unsigned int get_program_counter(void) 
{
  // TODO: implement me
  return 0;
}

word_t tos(void) 
{
  // this operation should NOT pop (remove top element from stack)
  // TODO: implement me
  return -1;
}

bool finished(void) 
{
  // TODO: implement me
  return false;
}

word_t get_local_variable(int i) 
{
  // TODO: implement me
  return 0;
}

void step(void) 
{
  // TODO: implement me

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
