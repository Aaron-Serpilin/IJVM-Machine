#ifndef INSTRUCTION_EXECUTION_H
#define INSTRUCTION_EXECUTION_H

#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "instructions.h"
#include "stack_functions.h"

FILE *in;             
FILE *out;

extern current_frame * head;
extern bool is_finished;

void instruction_executioner (word_t current_instruction) {
    
    switch (current_instruction) {

        case OP_ERR: //OPCODE 0XFE
            // dprintf("ERR\n");
            error();
            break;

        case OP_BIPUSH: //OPCODE 0X10
            // dprintf("BIPUSH\n");
            bi_push();
            break;

        case OP_DUP: //OPCODE 0X59
            // dprintf("DUP\n");
            duplicate();
            break;
    
        case OP_IADD: //OPCODE 0X60
            // dprintf("IADD\n");
            i_add();
            break;

        case OP_ISUB: //OPCODE 0X64
            // dprintf("ISUB\n");
            i_sub();
            break;
        
        case OP_IAND: //OPCODE 0XIE
            // dprintf("IAND\n");
            i_and();
            break;

        case OP_IOR: //OPCODE 0XB0
            // dprintf("IOR\n");
            i_or();
            break;

        case OP_POP: //OPCODE 0X57
            // dprintf("POP\n");
            instruction_pop();
            break;

        case OP_SWAP: //OPCODE 0X5F
            // dprintf("SWAP\n");
            swap();
            break;

        case OP_HALT: //OPCODE 0XFF
        {
            // dprintf("HALT\n");
            head->main_stack->finished_stack = true;
            break;
        }

        case OP_NOP: //OPCODE 0X00
            // dprintf("NOP\n");
            head->main_stack->program_counter++;
            break;

        case OP_IN: //OPCODE 0XFC 
        {
            // dprintf("IN\n");
            word_t input_value = fgetc(in);
            instruction_input(input_value);
            break;
        }

        case OP_OUT: //OPCODE 0XFD 
        {
            // dprintf("OUT\n");
            char output_value = pop(head);
            //fprintf(stderr, "%x\n", output_value);
            fprintf(out, "%c", output_value);
            head->main_stack->program_counter++;
            break;
        }
        
        case OP_GOTO:
            // dprintf("GOTO\n");
            go_to();
            break;

        case OP_IFEQ:
            // dprintf("IFEQ\n");
            if_eq();
            break;

        case OP_IFLT:
            // dprintf("IFLT\n");
            iflt();
            break;

        case OP_IF_ICMPEQ:
            // dprintf("IFCMPEQ\n");
            if_icmpeq();
            break;

        case OP_LDC_W:
            // dprintf("LDC_W\n");
            ldc_w();
            break;

        case OP_ILOAD:
            // dprintf("ILOAD\n");
            iload();
            break;

        case OP_ISTORE:
            // dprintf("ISTORE\n");
            istore();
            break;

        case OP_IINC:
            // dprintf("IINC\n");
            iinc();
            break;

        case OP_WIDE:
            // dprintf("WIDE\n");
            wide();
            break;

        case OP_INVOKEVIRTUAL:
            // dprintf("INVOKEVIRTUAL\n");
            invoke_virtual();
            break;
        
        case OP_IRETURN:
            // dprintf("IRETURN\n");
            ireturn();
            break;
        
        default:
            break;

        }

}

#endif
