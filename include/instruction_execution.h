#ifndef INSTRUCTION_EXECUTION_H
#define INSTRUCTION_EXECUTION_H
#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "instructions.h"
#include "stack_functions.h"

FILE *in;             
FILE *out;

void instruction_executioner (word_t current_instruction) {
    
    switch (current_instruction) {

        case OP_ERR: //OPCODE 0XFE
            error();
            break;

        case OP_BIPUSH: //OPCODE 0X10
            bi_push();
            break;

        case OP_DUP: //OPCODE 0X59
            duplicate();
            break;
    
        case OP_IADD: //OPCODE 0X60
            i_add();
            break;

        case OP_ISUB: //OPCODE 0X64
            i_sub();
            break;
        
        case OP_IAND: //OPCODE 0XIE
            i_and();
            break;

        case OP_IOR: //OPCODE 0XB0
            i_or();
            break;

        case OP_POP: //OPCODE 0X57
            instruction_pop();
            break;

        case OP_SWAP: //OPCODE 0X5F
            swap();
            break;

        case OP_HALT: //OPCODE 0XFF
        {
            Stack.finished_stack = true;
            break;
        }

        case OP_NOP: //OPCODE 0X00
        Stack.program_counter++;
        break;

        case OP_IN: //OPCODE 0XFC 
        {
            byte_t input_value = fgetc(in);
            instruction_input(input_value);
            break;
        }

        case OP_OUT: //OPCODE 0XFD 
        {
            char output_value = pop();
            fprintf(out, "%c", output_value);
            Stack.program_counter++;
            break;
        }
        
        case OP_GOTO:
            go_to();
            break;

        case OP_IFEQ:
            if_eq();
            break;

        case OP_IFLT:
            iflt();
            break;

        case OP_IF_ICMPEQ:
            if_icmpeq();
            break;

        case OP_LDC_W:
            ldc_w();
            break;

        case OP_ILOAD:
            iload();
            break;

        case OP_ISTORE:
            istore();
            break;

        case OP_IINC:
            iinc();
            break;

        case OP_WIDE:
            wide();
            break;

        case OP_INVOKEVIRTUAL:
            invoke_virtual();
            break;
        
        case OP_IRETURN:
            ireturn();
            break;
        
        default:
            break;

        }

}

#endif
