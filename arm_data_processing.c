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
#include "werror.h"

int exec_cond(int cond) {
	switch (cond) {
	case EQ:
		return 1;
		break;
	case NE:
		return 1;
		break;
	case HS:
		return 1;
		break;
	case LO:
		return 1;
		break;
	case MI:
		return 1;
		break;
	case PL:
		return 1;
		break;
	case VS:
		return 1;
		break;
	case VC:
		return 1;
		break;
	case HI:
		return 1;
		break;
	case LS:
		return 1;
		break;
	case GE:
		return 1;
		break;
	case LT:
		return 1;
		break;
	case GT:
		return 1;
		break;
	case LE:
		return 1;
		break;
	case AL:
		return 1;
		break;
	default:
		return 1;
		break;
	}
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	int cond = (ins & COND_MASK) >> COND_INDEX;
	if (!exec_cond(cond))
		return SUCCESSFULLY_DECODED;
	int instr = (ins & INSTR_MASK) >> INSTR_INDEX;
	if (instr != 0)
		raise(UNDEFINED_BEHAVIOUR, "arm_data_processing_shift: Instruction code is not of the corresponding type.\n");
	
	int I = (ins & I_MASK) >> I_INDEX;
	int opcode = (ins & OPCODE_MASK) >> OPCODE_INDEX;
	int S = (ins & S_MASK) >> S_INDEX;
	int Rn = (ins & RN_MASK) >> RN_INDEX;
	int Rd = (ins & RD_MASK) >> RD_INDEX;
	int shifter_operand = (ins & SHIFTER_OPERAND_MASK) >> SHIFTER_OPERAND_INDEX;

	switch (opcode) {
	case AND:
		return SUCCESSFULLY_DECODED;
		break;
	case EOR:
		return SUCCESSFULLY_DECODED;
		break;
	case SUB:
		return SUCCESSFULLY_DECODED;
		break;
	case RSB:
		return SUCCESSFULLY_DECODED;
		break;
	case ADD:
		return SUCCESSFULLY_DECODED;
		break;
	case ADC:
		return SUCCESSFULLY_DECODED;
		break;
	case SBC:
		return SUCCESSFULLY_DECODED;
		break;
	case RSC:
		return SUCCESSFULLY_DECODED;
		break;
	case TST:
		return SUCCESSFULLY_DECODED;
		break;
	case TEQ:
		return SUCCESSFULLY_DECODED;
		break;
	case CMP:
		return SUCCESSFULLY_DECODED;
		break;
	case CMN:
		return SUCCESSFULLY_DECODED;
		break;
	case ORR:
		return SUCCESSFULLY_DECODED;
		break;
	case MOV:
		return SUCCESSFULLY_DECODED;
		break;
	case BIC:
		return SUCCESSFULLY_DECODED;
		break;
	case MVN:
		return SUCCESSFULLY_DECODED;
		break;
	default:
		return UNDEFINED_INSTRUCTION;
		break;
	}
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

