#ifndef INSTRUCTION_EXECUTION_H
#define INSTRUCTION_EXECUTION_H

#include "ijvm.h"
#include "structs.h"
#include "util.h" 
#include "instructions.h"

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
            halt();
            break;

        case OP_NOP: //OPCODE 0X00
            do_nothing();
            break;

        case OP_IN: //OPCODE 0XFC 
            instruction_input();
            break;

        case OP_OUT: //OPCODE 0XFD 
            instruction_output();
            break;
        
        case OP_GOTO: //OPCODE 0XA7
            go_to();
            break;

        case OP_IFEQ: //OPCODE 0X99
            if_eq();
            break;

        case OP_IFLT: //OPCODE 0X9B
            iflt();
            break;

        case OP_IF_ICMPEQ: //OPCODE 0X9F
            if_icmpeq();
            break;

        case OP_LDC_W: //OPCODE 0X13
            ldc_w();
            break;

        case OP_ILOAD: //OPCODE 0X15
            iload();
            break;

        case OP_ISTORE: //OPCODE 0X36
            istore();
            break;

        case OP_IINC: //OPCODE 0X84 
            iinc();
            break;

        case OP_WIDE: //OPCODE 0XC4 
            wide();
            break;

        case OP_INVOKEVIRTUAL: //OPCODE 0XB6 
            invoke_virtual();
            break;
        
        case OP_IRETURN: //OPCODE 0XAC
            ireturn();
            break;

        case OP_TAILCALL: //OPCODE 0xCB
            tailcall();
            break;
        
        default:
            break;

        }

}

#endif
