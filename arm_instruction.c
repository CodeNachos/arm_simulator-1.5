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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int instruction_cond(arm_core p, uint32_t inst){ //la condition doit etre remplie afin d'executer l'instruction
    uint32_t code = inst >>28; 
    uint32_t status= arm_read_cpsr(p);
    switch(code){
    case EQ:
        return get_bit(status, Z);
        break;
    case NE:
        return !get_bit(status, Z);
        break;
    case HS:
        return get_bit(status, C);
        break;
    case LO:
        return !get_bit(status, C);
        break;
    case MI:
        return get_bit(status, N);
        break;
    case PL:
        return !get_bit(status, N);
        break;
    case VS:
        return get_bit(status, V);
        break;
    case VC:
        return !get_bit(status, V);
        break;
    case HI:
        return (get_bit(status, C) && !get_bit(status, Z));
        break;
    case LS:
        return (!get_bit(status, C) || get_bit(status, Z));
        break;
    case GE:
        return (get_bit(status, N) == get_bit(status, V));
        break;
    case LT:
        return (get_bit(status, N) != get_bit(status, V));
        break;
    case GT:
        return !get_bit(status, Z) && (get_bit(status, N) == get_bit(status, V));
        break;
    case LE:
        return get_bit(status, Z) || (get_bit(status, N) != get_bit(status, V));
        break;
    case AL:
        return 1;
        break;
    default:
        warning("Undefined condition\n");
        return 0;
        break;
    }
}


int arm_execute_instruction(arm_core p) {
    uint32_t instruction;
    int result = UNDEFINED_INSTRUCTION; 

    arm_fetch(p, &instruction);

    if (!instruction_cond(p, instruction)) return 0; //verifier cond

    switch (get_bits(instruction, 27, 25)) {
        case 0b000:
            if (get_bit(instruction, 4) && get_bit(instruction, 7)) { //5ieme instruction de la table
                result = arm_load_store(p, instruction);
            } else if (get_bits(instruction, 24, 23) == 2 && get_bit(instruction, 20) == 0) {
                if (get_bit(instruction, 4) == 0 || (get_bit(instruction, 4) == 1 && get_bit(instruction, 7) == 0)) {
                    result = arm_miscellaneous(p, instruction);
                }
            }
                else {
                    warning("calling data process\n");
                    result = arm_data_processing_shift(p, instruction); //premiere instruction de la table
                }
            break;
        
        case 0b001: 
            switch(get_bits(instruction,24,20)) {
    			case 0b10010:
    			case 0b10110:
    			case 0b10000:
    			case 0b10100: 
    				break;  //undefined
    			default:
                    warning("calling data process\n");
    				result = arm_data_processing_shift(p,instruction);
    				break;
    		}
    	    break;
        
        case 0b010:
            result = arm_load_store(p, instruction);
            break;

        case 0b011: 
            if(get_bit(instruction, 4)==0)
                result = arm_load_store(p, instruction);
            break;
            
        case 0b100: 
            result = arm_load_store_multiple(p, instruction);
        break;
        case 0b101: 
            result = arm_branch(p, instruction);
        break;
        case 0b110:
            result = arm_coprocessor_load_store(p, instruction);
        break;
        case 0b111: 
            result = arm_coprocessor_others_swi(p, instruction);
            break;
        default:    // get bit value is cast to int by default
            result = UNDEFINED_INSTRUCTION;
            break;
    }
    return result;
}


int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}


