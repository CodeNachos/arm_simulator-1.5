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

/* general masks */
#define COND_MASK ((uint32_t)0xF << 28)
#define ROTATE_IMM_MASK ((uint32_t)0xF << 8)
#define BYTE_IMMEDIATE_MASK (uint32_t)0xFF

/* general indexes */
#define COND_INDEX 28
#define ROTATE_IMM_INDEX 8
#define BYTE_IMMEDIATE_INDEX 0

/* data processing instruction's masks */
#define OPCODE_MASK ((uint32_t)0xF << 21)
#define RN_MASK ((uint32_t)0xF << 16)
#define RD_MASK ((uint32_t)0xF << 12)
#define SHIFTER_OPERAND_MASK (uint32_t)0xFFF

/* data processing instruction's indexes */
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

/* immediate msr's masks */
#define FIELD_MASK_MASK ((uint32_t)0xF << 16)

/* immediate msr's indexes */
#define FIELD_MASK_INDEX 16

/* immediate msr's information bit */
#define R 22

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
	
	uint8_t I_bit = get_bit(ins, I);
	uint8_t S_bit = get_bit(ins, S);
	uint8_t opcode = (ins & OPCODE_MASK) >> OPCODE_INDEX;
	uint8_t Rn = (ins & RN_MASK) >> RN_INDEX;
	uint8_t Rd = (ins & RD_MASK) >> RD_INDEX;
	uint16_t shifter_operand_code = (ins & SHIFTER_OPERAND_MASK) >> SHIFTER_OPERAND_INDEX;
	uint32_t shifter_operand;
	uint8_t shifter_carry_out;
	uint8_t rotate_imm;
	uint32_t byte_immediate;
	uint8_t shift_imm;
	uint8_t Rs;
	uint8_t Rm;
	uint32_t result;
	
	if (I_bit) {
		rotate_imm = (ins & ROTATE_IMM_MASK) >> ROTATE_IMM_INDEX;
		byte_immediate = (ins & BYTE_IMMEDIATE_MASK) >> BYTE_IMMEDIATE_INDEX;
		shifter_operand = ror(byte_immediate, rotate_imm * 2);
		if (rotate_imm == 0)
			shifter_carry_out = C_bit;
		else
			shifter_carry_out = get_bit(shifter_operand, 31);
	} else if ((shifter_operand_code & (0b111 << 4)) == 0) {
		// lsl by immediate
		if (shift_imm == 0) {
			shifter_operand = arm_read_register(p, Rm);
			shifter_carry_out = C_bit;
		} else {
			shifter_operand = arm_read_register(p, Rm) << shift_imm;
			shifter_carry_out = get_bit(arm_read_register(p, Rm), 32 - shift_imm);
		}
	} else if ((shifter_operand_code & (0xF << 4)) == (1 << 4)) {
		// lsl by register

	} else if ((shifter_operand_code & (0b111 << 4)) == (1 << 5)) {
		// lsr by immediate

	} else if ((shifter_operand_code & (0xF << 4)) == (0b11 << 4)) {
		// lsr by register

	} else if ((shifter_operand_code & (0b111 << 4)) == (1 << 6)) {
		// asr by immediate

	} else if ((shifter_operand_code & (0xF << 4)) == (0b101 << 4)) {
		// asr by register

	} else if ((shifter_operand_code & (0b111 << 4)) == (0b11 << 5)) {
		// rotate right by immediate

	} else if ((shifter_operand_code & (0xF << 4)) == (0b111 << 4)) {
		// rotate right by register

	} else if ((shifter_operand_code & (0xFF << 4)) == (0b11 << 5)) {
		// rotate right with extend

	} else
		return UNDEFINED_INSTRUCTION;

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
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = get_bit(current_CPSR, N);
	uint8_t Z_bit = get_bit(current_CPSR, Z);
	uint8_t C_bit = get_bit(current_CPSR, C);
	uint8_t V_bit = get_bit(current_CPSR, V);

	uint8_t cond = (ins & COND_MASK) >> COND_INDEX;
	if (!arm_exec_cond_passed(cond, N_bit, Z_bit, C_bit, V_bit))
		return SUCCESSFULLY_DECODED;
	
	uint8_t R_bit = get_bit(ins, R);
	uint8_t field_mask = (ins & FIELD_MASK_MASK) >> FIELD_MASK_INDEX;
	uint8_t rotate_imm = (ins & ROTATE_IMM_MASK) >> ROTATE_IMM_INDEX;
	uint32_t byte_immediate = (ins & BYTE_IMMEDIATE_MASK) >> BYTE_IMMEDIATE_INDEX;
	
	uint32_t operand = ror(byte_immediate, rotate_imm * 2);
	if (operand & UnallocMask)
		return UNDEFINED_INSTRUCTION;

	uint32_t byte_mask = 0;
	if (get_bit(field_mask, 0))
		byte_mask |= 0x000000FF;
	if (get_bit(field_mask, 1))
		byte_mask |= 0x0000FF00;
	if (get_bit(field_mask, 2))
		byte_mask |= 0x00FF0000;
	if (get_bit(field_mask, 3))
		byte_mask |= 0xFF000000;
	
	uint32_t mask;
	uint32_t current_SPSR;

	if (R_bit == 0) {
		if (arm_in_a_privileged_mode(p)) {
			if (operand & StateMask)
				return UNDEFINED_INSTRUCTION;
			else
				mask = byte_mask & (UserMask | PrivMask);
		} else
			mask = byte_mask & UserMask;
		arm_write_cpsr(p, (current_CPSR & ~mask) | (operand & mask));
		return SUCCESSFULLY_DECODED;
	} else { // R == 1
		if (arm_current_mode_has_spsr(p)) {
			mask = byte_mask & (UserMask | PrivMask | StateMask);
			current_SPSR = arm_read_spsr(p);
			arm_write_spsr(p, (arm_read_spsr(p) & ~mask) | (operand & mask));
			return SUCCESSFULLY_DECODED;
		} else
			return UNDEFINED_INSTRUCTION;
	}
}

