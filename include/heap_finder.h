#ifndef HEAP_FINDER_H 
#define HEAP_FINDER_H 

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 

struct heap* heap_finder (struct heap *heap, word_t array_reference) {
    
    struct heap *current_heap = heap;
    struct heap *requested_heap = NULL;

    while (current_heap->heap_index != array_reference) {
        current_heap = current_heap->previous_heap;
    }

    requested_heap = current_heap;

    return requested_heap;

}

#endif
