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

/* data processing information bits */
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

/* immediate msr's information bit */
#define R 22

int add_overflow(uint32_t a, uint32_t b, uint32_t *result) {
	return __builtin_add_overflow(a, b, result);
}

int add_carry(uint32_t a, uint32_t b, uint32_t *result) {
	return __builtin_add_overflow(a, b, result) || (*result < a);
}

int sub_overflow(uint32_t a, uint32_t b, uint32_t *result) {
	return __builtin_sub_overflow(a, b, result);
}

int sub_carry(uint32_t a, uint32_t b, uint32_t *result) {
	return __builtin_sub_overflow(a, b, result) || (*result > a);
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	/* Read current flag */
	uint32_t current_CPSR = arm_read_cpsr(p);
	uint8_t N_bit = get_bit(current_CPSR, N);
	uint8_t Z_bit = get_bit(current_CPSR, Z);
	uint8_t C_bit = get_bit(current_CPSR, C);
	uint8_t V_bit = get_bit(current_CPSR, V);
	/* Decode instruction */
	uint8_t S_bit = get_bit(ins, S);
	uint8_t opcode = get_bits(ins, 24, 21);
	uint8_t Rn = get_bits(ins, 19, 16);
	uint32_t Rn_value = arm_read_register(p, Rn);
	uint8_t Rd = get_bits(ins, 15, 12);
	uint16_t shifter_operand_code = get_bits(ins, 11, 0);
	uint8_t rotate_imm = get_bits(ins, 11, 8);
	uint32_t immed_byte = get_bits(ins, 7, 0);
	uint8_t shift_imm = get_bits(ins, 11, 7);
	uint8_t Rm = get_bits(ins, 3, 0);
	uint32_t Rm_value = arm_read_register(p, Rm);
	uint8_t Rs = get_bits(ins, 11, 8);
	uint32_t Rs_value = arm_read_register(p, Rs);
	uint8_t Rs_valueLB = get_bits(Rs_value, 7, 0);; // least significant byte
	uint32_t shifter_operand;
	uint8_t shifter_carry_out;
	uint32_t result, tmp;
	int change_Rd;

	/* CALCULATE THE SHIFTER OPERAND */
	if (get_bits(shifter_operand_code, 6, 4) == 0b000) {
		// lsl by immediate
		if (Rm == 15 || Rn == 15) // weird use of r15
			Rm_value = arm_read_register(p, 15) + 4;
		if (shift_imm == 0) { /* Register operand */
			shifter_operand = Rm_value;
			shifter_carry_out = C_bit;
		} else { /* shift_imm > 0 */
			shifter_operand = Rm_value << shift_imm;
			shifter_carry_out = get_bit(Rm_value, 32 - shift_imm);
		}
	} else if (get_bits(shifter_operand_code, 7, 4) == 0b0001) {
		// lsl by register
		if (Rd == 15 || Rm == 15 || Rn == 15 || Rs == 15) {
			warning("UNPREDICTABLE (Attempt to specify PC as register in shift by register)\n");
			return UNDEFINED_INSTRUCTION;
		}
		if (Rs_valueLB == 0) {
			shifter_operand = Rm_value;
			shifter_carry_out = C_bit;
		} else if (Rs_valueLB < 32) {
			shifter_operand = Rm_value << Rs_valueLB;
			shifter_carry_out = get_bit(Rm_value, 32 - Rs_valueLB);
		} else if (Rs_valueLB == 32) {
			shifter_operand = 0;
			shifter_carry_out = get_bit(Rm_value, 0);
		} else { /* Rs[7:0] > 32 */
			shifter_operand = 0;
			shifter_carry_out = 0;
		}
	} else if (get_bits(shifter_operand_code, 6, 4) == 0b010) {
		// lsr by immediate
		if (Rm == 15 || Rn == 15) // weird use of r15
			Rm_value = arm_read_register(p, 15) + 4;
		if (shift_imm == 0) {
			shifter_operand = 0;
			shifter_carry_out = get_bit(Rm_value, 31);
		} else { /* shift_imm > 0 */
			shifter_operand = Rm_value >> shift_imm;
			shifter_carry_out = get_bit(Rm_value, shift_imm - 1);
		}
	} else if (get_bits(shifter_operand_code, 7, 4) == 0b0011) {
		// lsr by register
		if (Rd == 15 || Rm == 15 || Rn == 15 || Rs == 15) {
			warning("UNPREDICTABLE (Attempt to specify PC as register in shift by register)\n");
			return UNDEFINED_INSTRUCTION;
		}
		if (Rs_valueLB == 0) {
			shifter_operand = Rm_value;
			shifter_carry_out = C_bit;
		} else if (Rs_valueLB < 32) {
			shifter_operand = Rm_value >> Rs_valueLB;
			shifter_carry_out = get_bit(Rm_value, Rs_valueLB - 1);
		} else if (Rs_valueLB == 32) {
			shifter_operand = 0;
			shifter_carry_out = get_bit(Rm_value, 31);
		} else { /* Rs[7:0] > 32 */
			shifter_operand = 0;
			shifter_carry_out = 0;
		}
	} else if (get_bits(shifter_operand_code, 6, 4) == 0b100) {
		// asr by immediate
		if (Rm == 15 || Rn == 15) // weird use of r15
			Rm_value = arm_read_register(p, 15) + 4;
		if (shift_imm == 0) {
			if (get_bit(Rm_value, 31) == 0) {
				shifter_operand = 0;
				shifter_carry_out = 0;
			} else { /* Rm[31] == 1 */
				shifter_operand = 0xFFFFFFFF;
				shifter_carry_out = 1;
			}
		} else { /* shift_imm > 0 */
			shifter_operand = asr(Rm_value, shift_imm);
			shifter_carry_out = get_bit(Rm_value, shift_imm - 1);
		}
	} else if (get_bits(shifter_operand_code, 7, 4) == 0b0101) {
		// asr by register
		if (Rd == 15 || Rm == 15 || Rn == 15 || Rs == 15) {
			warning("UNPREDICTABLE (Attempt to specify PC as register in shift by register)\n");
			return UNDEFINED_INSTRUCTION;
		}
		if (Rs_valueLB == 0) {
			shifter_operand = Rm_value;
			shifter_carry_out = C_bit;
		} else if (Rs_valueLB < 32) {
			shifter_operand = asr(Rm_value, Rs_valueLB);
			shifter_carry_out = get_bit(Rm_value, Rs_valueLB - 1);
		} else { /* Rs_valueLB >= 32 */
			if (get_bit(Rm_value, 31) == 0) {
				shifter_operand = 0;
				shifter_carry_out = 0;
			} else { /* Rm[31] == 1 */
				shifter_operand = 0xFFFFFFFF;
				shifter_carry_out = 1;
			}
		}
	} else if (get_bits(shifter_operand_code, 6, 4) == 0b110) {
		// rotate right by immediate
		if (Rm == 15 || Rn == 15) // weird use of r15
			Rm_value = arm_read_register(p, 15) + 4;
		if (shift_imm == 0) { /* rotate right with extend */
			shifter_operand = ((uint32_t)C_bit << 31) | (Rm_value >> 1);
			shifter_carry_out = get_bit(Rm_value, 0);
		} else { /* shift_imm > 0 */
			shifter_operand = ror(Rm_value, shift_imm);
			shifter_carry_out = get_bit(Rm_value, shift_imm - 1);
		}
	} else if (get_bits(shifter_operand_code, 7, 4) == 0b0111) {
		// rotate right by register
		if (Rd == 15 || Rm == 15 || Rn == 15 || Rs == 15) {
			warning("UNPREDICTABLE (Attempt to specify PC as register in shift by register)\n");
			return UNDEFINED_INSTRUCTION;
		}
		if (Rs_valueLB == 0) {
			shifter_operand = Rm_value;
			shifter_carry_out = C_bit;
		} else if (get_bits(Rs_value, 4, 0) == 0) {
			shifter_operand = Rm_value;
			shifter_carry_out = get_bit(Rm_value, 31);
		} else { /* Rs[4:0] > 0 */
			shifter_operand = ror(Rm_value, get_bits(Rs_value, 4, 0));
			shifter_carry_out = get_bit(Rm_value, get_bits(Rs_value, 4, 0) - 1);
		}
	} else
		return UNDEFINED_INSTRUCTION;

	/* CALCULATE RESULT BASED OPCODE */
	switch (opcode) {
	case AND:
		result = Rn_value & shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	case EOR:
		result = Rn_value ^ shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	case SUB:
		result = Rn_value - shifter_operand;
		C_bit = sub_carry(Rn_value, shifter_operand, &result);
		V_bit = sub_overflow(Rn_value, shifter_operand, &result);
		change_Rd = 1;
		break;
	case RSB:
		result = shifter_operand - Rn_value;
		C_bit = sub_carry(shifter_operand, Rn_value, &result);
		V_bit = sub_overflow(shifter_operand, Rn_value, &result);
		change_Rd = 1;
		break;
	case ADD:
		result = Rn_value + shifter_operand;
		C_bit = add_carry(Rn_value, shifter_operand, &result);
		V_bit = add_overflow(Rn_value, shifter_operand, &result);
		change_Rd = 1;
		break;
	case ADC:
		tmp = Rn_value + shifter_operand;
		result = tmp + C_bit;
		C_bit = add_carry(Rn_value, shifter_operand, &tmp) || 
				add_carry(tmp, C_bit, &result);
		V_bit = add_overflow(Rn_value, shifter_operand, &tmp) || 
				add_overflow(tmp, C_bit, &result);
		change_Rd = 1;
		break;
	case SBC:
		tmp = Rn_value - shifter_operand;
		result = tmp - !C_bit;
		C_bit = sub_carry(Rn_value, shifter_operand, &tmp) || 
				sub_carry(tmp, !C_bit, &result);
		V_bit = sub_overflow(Rn_value, shifter_operand, &tmp) || 
				sub_overflow(tmp, !C_bit, &result);
		change_Rd = 1;
		break;
	case RSC:
		tmp = shifter_operand - Rn_value;
		result = tmp - !C_bit;
		C_bit = sub_carry(shifter_operand, Rn_value, &tmp) || 
				sub_carry(tmp, !C_bit, &result);
		V_bit = sub_overflow(shifter_operand, Rn_value, &tmp) || 
				sub_overflow(tmp, !C_bit, &result);
		change_Rd = 1;
		break;
	case TST:
		result = Rn_value & shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 0;
		break;
	case TEQ:
		result = Rn_value ^ shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 0;
		break;
	case CMP:
		result = Rn_value - shifter_operand;
		C_bit = sub_carry(Rn_value, shifter_operand, &result);
		V_bit = sub_overflow(Rn_value, shifter_operand, &result);
		change_Rd = 0;
		break;
	case CMN:
		result = Rn_value + shifter_operand;
		C_bit = add_carry(Rn_value, shifter_operand, &result);
		V_bit = add_overflow(Rn_value, shifter_operand, &result);
		change_Rd = 0;
		break;
	case ORR:
		result = Rn_value | shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	case MOV:
		result = shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	case BIC:
		result = Rn_value & ~shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	case MVN:
		result = ~shifter_operand;
		C_bit = shifter_carry_out;
		change_Rd = 1;
		break;
	default:
		return UNDEFINED_INSTRUCTION;
		break;
	}
	// Get the 2 other flags 
	N_bit = get_bit(result, 31);
	Z_bit = (result == 0);
	// Write result in Rd if there is a need to
	if (change_Rd)
		arm_write_register(p, Rd, result);

	/* CHANGE CPSR ACCORDING TO S */
	if (S_bit && Rd == 15) {
		if (arm_current_mode_has_spsr(p)) {
			arm_write_cpsr(p, arm_read_spsr(p));
			return SUCCESSFULLY_DECODED;
		} else {
			warning("UNPREDICTABLE (Attempt to use SPSR but current mode does not have SPSR)\n");
			return UNDEFINED_INSTRUCTION;
		}
	} else if (S_bit) {
		// Write in CPSR using a call to msr
		ins = 0xE328F200 | (N_bit << 3) | (Z_bit << 2) | (C_bit << 1) | V_bit;
		return arm_data_processing_immediate_msr(p, ins);
	} else 
		return SUCCESSFULLY_DECODED;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
	uint8_t R_bit = get_bit(ins, R);
	uint8_t field_mask = get_bits(ins, 19, 16);
	uint8_t rotate_imm = get_bits(ins, 11, 8);
	uint32_t immed_byte = get_bits(ins, 7, 0);
	
	/* Operand is an rotation of an immediate byte */
	uint32_t operand = ror(immed_byte, rotate_imm * 2);
	if (operand & UnallocMask) {
		warning("UNPREDICTABLE (Attempt to set reserved bits)\n");
		return UNDEFINED_INSTRUCTION;
	}

	/* Choose what byte to modfiy according to field_mask */
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

	if (R_bit == 0) {
		// Change CPSR according to operand, byte_mask and mode masks..
		if (arm_in_a_privileged_mode(p)) {
			if (operand & StateMask) {
				warning("UNPREDICTABLE (Attempt to set non-ARM execution state)\n");
				return UNDEFINED_INSTRUCTION;
			} else
				mask = byte_mask & (UserMask | PrivMask);
		} else
			mask = byte_mask & UserMask;
		arm_write_cpsr(p, (arm_read_cpsr(p) & ~mask) | (operand & mask));
		return SUCCESSFULLY_DECODED;
	} else { // R == 1
		// Change SPSR according to operand, byte_mask and mode masks..
		if (arm_current_mode_has_spsr(p)) {
			mask = byte_mask & (UserMask | PrivMask | StateMask);
			arm_write_spsr(p, (arm_read_spsr(p) & ~mask) | (operand & mask));
			return SUCCESSFULLY_DECODED;
		} else {
			warning("UNPREDICTABLE (Attempt to use SPSR but current mode does not have SPSR)\n");
			return UNDEFINED_INSTRUCTION;
		}
	}
}

