#ifndef STRUCTS_H
#define STRUCTS_H
#include "ijvm.h"

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
  int current_stack_size;
  int max_stack_size;
  bool finished_stack;
  word_t* stack_list;

} Stack;

#endif