#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "debug.h"
#include "trace.h"
#include "trace_location.h"
#include "memory.h"
#include "registers.h"
#include "arm.h"
#include "arm_core.h"
#include "arm_constants.h"
#include "arm_exception.h"
#include "arm_branch_other.h"
#include "arm_data_processing.h"

#define NB_INSTR 1000

int randint_range(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

int coinflip() {
    return randint_range(0, 1);
}

int random_2bits() {
    return randint_range(0, 3);
}

int random_4bits() {
    return randint_range(0, 15);
}

int random_5bits() {
    return randint_range(0, 31);    
}

int random_byte() {
    return randint_range(0, 255);
}

int oracle(arm_core p, uint8_t I, uint8_t S, uint8_t opcode,
            uint8_t Rn, uint8_t Rd, uint8_t rotate_imm, uint8_t immed_byte, 
            uint8_t shift_imm, uint8_t shift, uint8_t Rm, uint8_t Rs, 
            uint32_t Rd_value_before, uint23_t CPSR_before, int return_code) {
    return 0;
}

int main(int argc, char **argv) {
    time_t t;
    srand((unsigned)time(&t));

    int i, return_code;
    arm_core p = arm_create(registers_create(), memory_create(1024));
    uint32_t Rd_value_before, CPSR_before, ins, ins_base = (0b111000 << 26);
    uint8_t I = 0, S = 0, opcode = 0, Rn = 0, Rd = 0, rotate_imm = 0, \
            immed_byte = 0, shift_imm = 0, shift = 0, Rm = 0, Rs =0, \
            immed_not_reg_shift;

    for (i = 0; i < NB_INSTR; i++) {
        ins = ins_base;
        I = coinflip();
        S = coinflip();
        opcode = random_4bits();
        Rn = random_4bits();
        Rd = random_4bits();
        ins = set_bits(ins, 25, 25, I);
        ins = set_bits(ins, 20, 20, S);
        ins = set_bits(ins, 24, 21, opcode);
        ins = set_bits(ins, 19, 16, Rn);
        ins = set_bits(ins, 15, 12, Rd);
        if (I) {
            rotate_imm = random_4bits();
            immed_byte = random_byte();
            ins = set_bits(ins, 11, 8, rotate_imm);
            ins = set_bits(ins, 7, 0, immed_byte);
        } else {
            immed_not_reg_shift = coinflip();
            shift = random_2bits();
            Rm = random_4bits();
            ins = set_bits(ins, 6, 5, shift);
            ins = set_bits(ins, 3, 0, Rm);
            if (immed_not_reg_shift) {
                shift_imm = random_5bits();
                ins = set_bits(ins, 11, 7, shift_imm);
            } else {
                Rs = random_4bits();
                ins = set_bits(ins, 11, 8, Rs);
            }
        }

        Rd_value_before = arm_read_register(p, Rd);
        CPSR_before = arm_read_cpsr(p);
        return_code = arm_data_processing_shift(p, ins);
        
        if(oracle(p, I, S, opcode, Rn, Rd, rotate_imm, immed_byte, shift_imm, 
                shift, Rm, Rs, Rd_value_before, CPSR_before, return_code)) {
            // something bad happened
            warning("Instruction hex %x was interpreted poorly\n", ins);
            return 1;
        }
    }
    
    arm_destroy(p);
    return 0;
}