/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "werror.h"

#include <debug.h>
#include <stdlib.h>

#define COND_INDEX 28 
#define COND_MASK ((uint32_t)0xF << COND_INDEX)
#define INSTR_BR_INDEX 25
#define INST_BR_MASK ((uint32_t)0x5 << INSTR_BR_INDEX)
#define L_INDEX 24
#define OFFSET_MASK (uint32_t)0x00FFFFFF
#define SWI_INDEX 24
#define SWI_MASK ((uint32_t)0xF << SWI_INDEX)

int arm_branch(arm_core p, uint32_t ins) {
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = (current_CPSR & ((uint32_t)1 << N)) >> N;
	uint8_t Z_bit = (current_CPSR & ((uint32_t)1 << Z)) >> Z;
	uint8_t C_bit = (current_CPSR & ((uint32_t)1 << C)) >> C;
	uint8_t V_bit = (current_CPSR & ((uint32_t)1 << V)) >> V;
	uint8_t cond = (ins & COND_MASK) >> COND_INDEX;

	// check wether a condition is true for an instruction
	int condition_passed = arm_exec_cond_passed(cond, N_bit, Z_bit, C_bit, V_bit);
	if(condition_passed){
		uint8_t codeop = (ins & INST_BR_MASK) >> INSTR_BR_INDEX;

		// not a branching instruction code, raise an error
		if(codeop != 5){
			raise(UNDEFINED_BEHAVIOUR, "arm_branch: Instruction code is not a branching instruction.\n");
			return UNDEFINED_INSTRUCTION;
		}
		else{
			int L = get_bit(ins, L_INDEX); // bit of B or BL
			uint32_t offset = ins & OFFSET_MASK; // get offset for branching
			// extend value to an adress of branching, procedure is specified in manual
			int sign_extention_bit = get_bit(offset, 24);
			if(sign_extention_bit){
				offset|=0x3F000000;
			}
			offset = offset << 2;
			if(L){
				// linking
				arm_write_register(p, 14, arm_read_register(p, 15)); // LR <- PC
			}
			// branching
			arm_write_register(p, 15, offset); // PC <- adress
			return SUCCESSFULLY_DECODED;
		}
	}
	else // condition blocked, skipping
    	return SUCCESSFULLY_DECODED;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = (current_CPSR & ((uint32_t)1 << N)) >> N;
	uint8_t Z_bit = (current_CPSR & ((uint32_t)1 << Z)) >> Z;
	uint8_t C_bit = (current_CPSR & ((uint32_t)1 << C)) >> C;
	uint8_t V_bit = (current_CPSR & ((uint32_t)1 << V)) >> V;
	uint8_t cond = (ins & COND_MASK) >> COND_INDEX;

	// check wether a condition is true for an instruction
	int condition_passed = arm_exec_cond_passed(cond, N_bit, Z_bit, C_bit, V_bit);
	if(condition_passed){
		uint8_t codeop = (ins & SWI_MASK) >> SWI_INDEX;
		if(codeop != 0xF)
	    if (get_bit(ins, 24)) {
        	return SOFTWARE_INTERRUPT;
    	}
    	return UNDEFINED_INSTRUCTION;
	}
	else // condition blocked, skipping
		return SUCCESSFULLY_DECODED;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = (current_CPSR & ((uint32_t)1 << N)) >> N;
	uint8_t Z_bit = (current_CPSR & ((uint32_t)1 << Z)) >> Z;
	uint8_t C_bit = (current_CPSR & ((uint32_t)1 << C)) >> C;
	uint8_t V_bit = (current_CPSR & ((uint32_t)1 << V)) >> V;
	uint8_t cond = (ins & COND_MASK) >> COND_INDEX;

	// check wether a condition is true for an instruction
	int condition_passed = arm_exec_cond_passed(cond, N_bit, Z_bit, C_bit, V_bit);
	if(condition_passed){
		return UNDEFINED_INSTRUCTION;
	}
	else // condition blocked, skipping
		return SUCCESSFULLY_DECODED;
}
