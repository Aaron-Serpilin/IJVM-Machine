#ifndef DATA_SORTER_H
#define DATA_SORTER_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "structs.h" // to access the IJVM struct

int binary_path_data_sorter (char* binary_path, word_t* initial_data_chunks, word_t* pool_data, int* text_size, byte_t* text_data) {

    FILE* file_pointer = fopen(binary_path, "rb");

    if (file_pointer == NULL) {
        dprintf("Input file cannot be opened\n");
        return 1;
    }

  //To avoid memory leaks, it is important to allocate and read the same number of bytes
  initial_data_chunks = malloc(sizeof(int) * 12); //Stores the header, pool origin, and pool size

  fread(initial_data_chunks, sizeof(word_t), 3, file_pointer); //I read first 3 chunks of 4 bytes

  for (int i = 0; i < 3; i++) {
    
    initial_data_chunks[i] = swap_uint32(initial_data_chunks[i]);

  };

  ijvm_machine.header = initial_data_chunks[0];

  if (ijvm_machine.header != 0x1deadfad) {
    dprintf("Header is incorrect\n");
    return -1;
  }

  ijvm_machine.constant_pool_size = initial_data_chunks[2];

  pool_data = malloc(sizeof(word_t) * (ijvm_machine.constant_pool_size/4)); //Allocate 4 bytes of memory the pool size number of elements

  fread(pool_data, sizeof(word_t), ijvm_machine.constant_pool_size/4, file_pointer); // I read then all the pool data chunks of 4 bytes

  for (int i = 0; i < ijvm_machine.constant_pool_size/4; i++) {

    pool_data[i] = swap_uint32(pool_data[i]);

  }

  ijvm_machine.constant_pool_data = pool_data; //The data has (pool_size/4) elements, and each element has 4 bytes. Can be printed in %02X

  text_size = malloc(sizeof(word_t) * 2); //The 2 is due to the 2 chunks of data that make up the text size

  fread(text_size, 2, sizeof(word_t), file_pointer); //I read the 2 chunks of 4 bytes about the text size

  for (int i = 0; i < 2; i++) {
    text_size[i] = swap_uint32(text_size[i]);
  }

  ijvm_machine.text_size = text_size[1];

  text_data = malloc(sizeof(word_t) * (ijvm_machine.text_size));

  fread(text_data, sizeof(word_t), ijvm_machine.text_size, file_pointer); 

  ijvm_machine.text_data = text_data; //It has size bytes and size elements

  fclose(file_pointer);

  return 0;
}

#endif
