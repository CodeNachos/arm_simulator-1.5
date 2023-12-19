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
#include "debug.h"

#include <debug.h>
#include <stdlib.h>

//macros for branching instructions
#define BR_INDEX 25
#define BR_MASK ((uint32_t) 5 << BR_INDEX)
#define BR_CODE 5
#define OFFSET_MASK (uint32_t)0x00FFFFFF

//macros for SWI instruction
#define SWI_INDEX 24
#define SWI_MASK ((uint32_t)15 << SWI_INDEX)
#define SWI_CODE 15

//macros for MRS instruction
#define MRS_INDEX 23
#define MRS_MASK ((uint32_t)2 << MRS_INDEX)
#define MRS_CODE 2
#define Rd_INDEX 12
#define Rd_MASK ((uint32_t)15 << Rd_INDEX)


int arm_branch(arm_core p, uint32_t ins) {
	uint8_t codeop = (ins & BR_MASK) >> BR_INDEX;
	// not a branching instruction code, raise an error
	if(codeop != BR_CODE){
		warning("UNPREDICTABLE (arm_branch: Instruction code is not a branching instruction)\n");
		return UNDEFINED_INSTRUCTION;
	}
	else{
		int L = get_bit(ins, 24); // bit of B or BL
		uint32_t offset = ins & OFFSET_MASK; // get offset for branching

		// extend value to an adress of branching, procedure is specified in manual
		int sign_extention_bit = get_bit(offset, 23);
		if(sign_extention_bit){
			offset = offset | 0x3F000000; // extend with values 1 to 30 bits
		}
		offset = offset << 2; // shift left by two

		uint32_t temp_pc = arm_read_register(p, 15); // save pc

		if(L){ // if L == 1, then BL
			// linking
			arm_write_register(p, 14, temp_pc); // LR <- PC
		}
		// branching
		arm_write_register(p, 15, temp_pc+offset); // PC <- PC + offset
		return SUCCESSFULLY_DECODED;
	}
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins){
	uint8_t codeop = (ins & SWI_MASK) >> SWI_INDEX; // get the instruction code
	// not an SWI instruction code, raise an error
	if(codeop != SWI_CODE){
		warning("UNPREDICTABLE (arm_branch: Instruction code is not an SWI instruction)\n");
		return UNDEFINED_INSTRUCTION;
	}
	else 
		return SOFTWARE_INTERRUPT;
}

int arm_miscellaneous(arm_core p, uint32_t ins){
	uint8_t codeop = (ins & MRS_MASK) >> MRS_INDEX; // get the instruction code
	// not an MSR instruction code, raise an error
	if(codeop != MRS_CODE){
		warning("UNPREDICTABLE (rm_branch: Instruction code is not a MRS instruction)\n");
		return UNDEFINED_INSTRUCTION;
	}
	else{
		int R_bit = get_bit(ins, 22); // get CPSR or SPSR
		uint8_t return_reg = (ins & Rd_MASK) >> Rd_INDEX; // get the register to put data

		if(R_bit){ // R == 1 <=> read from SPSR
			if (arm_current_mode_has_spsr(p)) { //check wether getting SPSR is valid for the current mode
				arm_write_register(p, return_reg, arm_read_spsr(p));
				return SUCCESSFULLY_DECODED;
			} 
			// error, mode does not have SPSR
			else {
				warning("UNPREDICTABLE (Attempt to use SPSR but current mode does not have SPSR)\n");
				return UNDEFINED_INSTRUCTION;
			}
		}
		else{ // R == 0 <=> read from CPSR
			arm_write_register(p, return_reg, arm_read_cpsr(p));
			return SUCCESSFULLY_DECODED;
		}
	}
}
