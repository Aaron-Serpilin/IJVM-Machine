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

    while (current_heap != NULL) {
        // dprintf("\n---BEGIN SECTION---\n");
        // dprintf("Reference is %d and index is %d\n", array_reference, current_heap->heap_index);
        if (array_reference == current_heap->heap_index) { //We loop until the reference and the index match to get the right heap
            requested_heap = current_heap;
            dprintf("Reference is %d and index is %d\n", array_reference, current_heap->heap_index);
            break;
        } 
        // dprintf("\n---END SECTION---\n");

        if (current_heap->heap_index >= current_heap->heap_stack->current_stack_size) {
            current_heap->heap_stack->finished_stack = true;
            break;
        }

        if (current_heap->previous_heap == NULL) {
            dprintf("No such heap exists\n");
            break;
        }

        current_heap = current_heap->previous_heap;

    }

    // while (current_heap->heap_index != array_reference) {
    //     current_heap = current_heap->previous_heap;
    // }

    // requested_heap = current_heap;

    // if (current_heap->heap_index > current_heap->heap_stack->current_stack_size) {
    //     current_heap->heap_stack->finished_stack = true;
    // }

    return requested_heap;

}

#endif
