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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* data processing instruction code */
#define INSTR_CODE 0

/* data processing instruction's masks */
#define COND_MASK ((uint32_t)0xF << 28)
#define INSTR_MASK ((uint32_t)3 << 26)
#define OPCODE_MASK ((uint32_t)0xF << 21)
#define RN_MASK ((uint32_t)0xF << 16)
#define RD_MASK ((uint32_t)0xF << 12)
#define SHIFTER_OPERAND_MASK (uint32_t)0xFFF

/* data processing instruction's indexes */
#define COND_INDEX 28
#define INSTR_INDEX 26
#define OPCODE_INDEX 21
#define RN_INDEX 16
#define RD_INDEX 12
#define SHIFTER_OPERAND_INDEX 0

/* data processing information bits */
#define I 25
#define S 20

/* data processing instructions */
#define AND 0
#define EOR 1
#define SUB 2
#define RSB 3
#define ADD 4
#define ADC 5
#define SBC 6
#define RSC 7
#define TST 8
#define TEQ 9
#define CMP 10
#define CMN 11
#define ORR 12
#define MOV 13
#define BIC 14
#define MVN 15

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = get_bit(current_CPSR, N);
	uint8_t Z_bit = get_bit(current_CPSR, Z);
	uint8_t C_bit = get_bit(current_CPSR, C);
	uint8_t V_bit = get_bit(current_CPSR, V);

	uint8_t cond = (ins & COND_MASK) >> COND_INDEX;
	if (!arm_exec_cond_passed(cond, N_bit, Z_bit, C_bit, V_bit))
		return SUCCESSFULLY_DECODED;

	uint8_t instr = (ins & INSTR_MASK) >> INSTR_INDEX;
	if (instr != INSTR_CODE)
		return UNDEFINED_INSTRUCTION;
	
	uint8_t I_bit = get_bit(ins, I);
	uint8_t S_bit = get_bit(ins, S);
	uint8_t opcode = (ins & OPCODE_MASK) >> OPCODE_INDEX;
	uint8_t Rn = (ins & RN_MASK) >> RN_INDEX;
	uint8_t Rd = (ins & RD_MASK) >> RD_INDEX;
	uint16_t shifter_operand_code = (ins & SHIFTER_OPERAND_MASK) >> SHIFTER_OPERAND_INDEX;
	uint32_t shifter_operand;
	uint8_t shifter_carry_out;
	uint32_t result;
	
	if (I_bit)
		shifter_operand = 0;
	else
		if (get_bit(shifter_operand_code, 7) && get_bit(shifter_operand_code, 4))
			return UNDEFINED_INSTRUCTION;
		else
			shifter_operand = 0;

	switch (opcode) {
	case AND:
		return UNDEFINED_INSTRUCTION;
		break;
	case EOR:
		return UNDEFINED_INSTRUCTION;
		break;
	case SUB:
		return UNDEFINED_INSTRUCTION;
		break;
	case RSB:
		return UNDEFINED_INSTRUCTION;
		break;
	case ADD:
		return UNDEFINED_INSTRUCTION;
		break;
	case ADC:
		return UNDEFINED_INSTRUCTION;
		break;
	case SBC:
		return UNDEFINED_INSTRUCTION;
		break;
	case RSC:
		return UNDEFINED_INSTRUCTION;
		break;
	case TST:
		return UNDEFINED_INSTRUCTION;
		break;
	case TEQ:
		return UNDEFINED_INSTRUCTION;
		break;
	case CMP:
		return UNDEFINED_INSTRUCTION;
		break;
	case CMN:
		return UNDEFINED_INSTRUCTION;
		break;
	case ORR:
		return UNDEFINED_INSTRUCTION;
		break;
	case MOV:
		return UNDEFINED_INSTRUCTION;
		break;
	case BIC:
		return UNDEFINED_INSTRUCTION;
		break;
	case MVN:
		return UNDEFINED_INSTRUCTION;
		break;
	default:
		return UNDEFINED_INSTRUCTION;
		break;
	}

	arm_write_register(p, Rd, result);

	if (S_bit && Rd == 15) {
		if (arm_current_mode_has_spsr(p)) {
			arm_write_cpsr(p, arm_read_spsr(p));
			return SUCCESSFULLY_DECODED;
		} else
			return UNDEFINED_INSTRUCTION;
	} else if (S_bit) {
		arm_data_processing_immediate_msr(p, 0x000000000); // to complete
		return SUCCESSFULLY_DECODED;
	} else 
		return SUCCESSFULLY_DECODED;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

