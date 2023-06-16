#ifndef STRUCTS_H
#define STRUCTS_H

#include "ijvm.h"

// Global variables to free space after allocating each time in destroy_ijvm
struct {

  word_t* initial_data_chunks;
  word_t* pool_data;
  int* text_size;
  byte_t* text_data;

} global_variables;

struct {

  word_t header;
  int constant_pool_size;
  word_t* constant_pool_data;
  int text_size;
  byte_t* text_data;

} ijvm_machine;

typedef struct stack {

  int program_counter;
  int current_stack_size;
  int max_stack_size;
  bool finished_stack;
  word_t* stack_pointer;

} current_stack;

typedef struct frame {

  struct frame* previous_frame_pointer;

  word_t* local_variables; //Array for the arguments and variables of each frame

  current_stack main_stack;
  word_t* previous_stack_pointer;
  int previous_program_counter;
  int current_program_counter;

} current_frame;

#endif
