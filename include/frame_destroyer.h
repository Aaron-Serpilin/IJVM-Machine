#ifndef FRAME_DESTROYER_H
#define FRAME_DESTROYER_H

#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include "ijvm.h"
#include "util.h" 
#include "structs.h" 
#include "stack_creator.h"

void frame_destroyer (struct frame* frame_to_be_destroyed) {
    free(frame_to_be_destroyed);
}

#endif
