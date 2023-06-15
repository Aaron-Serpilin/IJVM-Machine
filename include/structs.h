#ifndef STRUCTS_H
#define STRUCTS_H

#include "ijvm.h"

struct {

  word_t header;
  int constant_pool_size;
  word_t* constant_pool_data;
  int text_size;
  byte_t* text_data;

} IJVM_machine;

typedef struct {

  int program_counter;
  int current_stack_size;
  int max_stack_size;
  bool finished_stack;
  word_t* stack_pointer;

} frame_stack;

typedef struct frame {

  struct frame* next_frame;
  frame_stack main_stack;
  word_t* local_variables; //Array for the arguments and variables of each frame
  word_t* previous_stack_pointer;
  int previous_program_counter;

} current_frame;

#endif
