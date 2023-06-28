#ifndef HEAP_CREATOR_H
#define HEAP_CREATOR_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 
#include "stack_creator.h"

struct heap* heap_creator (struct heap *heap, int number_variables) {
    
    struct heap *new_heap = NULL;
    new_heap = malloc(sizeof(struct heap));

    new_heap->previous_heap = heap;
    new_heap->heap_stack = stack_creator();
    //new_heap->current_heap_size = -1;
    new_heap->heap_index++;
    
    return new_heap;

}

#endif
