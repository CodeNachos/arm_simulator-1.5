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

int instruction_cond(arm_core p, uint32_t inst){ //la condition doit etre remplie afin d'executer l'instruction
    uint32_t code = inst >>28; 
    uint32_t zncv= arm_read_cpsr(p);
    switch(code){
    case 0x0:
        return get_bit(zncv, Z)==1;
        break;
    case 0x1:
        return get_bit(zncv, Z)==0;
        break;
    case 0x2:
        return get_bit(zncv, C)==1;
        break;
    case 0x3:
        return get_bit(zncv, C)==0;
        break;
    case 0x4:
        return get_bit(zncv, N)==1;
        break;
    case 0x5:
        return get_bit(zncv, N)==0;
        break;
    case 0x6:
        return get_bit(zncv, V)==1;
        break;
    case 0x7:
        return get_bit(zncv, V)==0;
        break;
    case 0x8:
        return (get_bit(zncv, C)==1 && get_bit(zncv, Z)==0);
        break;
    case 0x9:
        return (get_bit(zncv, C)==0 || get_bit(zncv, Z)==1);
        break;
    case 0xA:
        return (get_bit(zncv, N) == get_bit(zncv, V));
        break;
    case 0xB:
        return (get_bit(zncv, N) != get_bit(zncv, V));
        break;
    case 0xC:
        return get_bit(zncv, Z) ==0 && (get_bit(zncv, N)==get_bit(zncv, V));
        break;
    case 0xD:
        return get_bit(zncv, Z) ==1 || (get_bit(zncv, N)!=get_bit(zncv, V));
        break;
    case 0xE:
        return 1;
    break;
    }
    return 0;
}


int arm_execute_instruction(arm_core p) {
    uint32_t instruction;
    arm_fetch(p, &instruction); 
    if(!instruction_cond(p,instruction)) return 0; //verifier cond

    switch (get_bits(instruction, 27, 25)) {
        case 0b000:
            if (get_bit(instruction, 4) && get_bit(instruction, 7)) { //5ieme instruction de la table
                    return arm_load_store(p, instruction);
                }
                if (get_bits(instruction, 24, 23) == 2 && get_bit(instruction, 20) == 0) {
                    if (get_bit(instruction, 4) == 0 || (get_bit(instruction, 4) == 1 && get_bit(instruction, 7) == 0)) {
                    return arm_miscellaneous(p, instruction);
                    }
                }
                else 
                    return arm_data_processing_shift(p, instruction); //premiere instruction de la table
        break;
        case 0b001: 
            switch(get_bits(instruction,24,20))
    			{
    				case 0b10010:
                        break;
    				case 0b10110:
    					break;
    				case 0b10000:
                        break;
    				case 0b10100: //undefined
    					break;
    				default:
    					arm_data_processing_immediate_msr(p,instruction);
    					break;
    			}
    	break;
        case 0b010:
            arm_load_store(p, instruction);
        break;
        case 0b011: 
            if(get_bit(instruction, 4)==0)
                arm_load_store(p, instruction);
        break;
        case 0b100: 
            arm_load_store_multiple(p, instruction);
        break;
        case 0b101: 
            arm_branch(p, instruction);
        break;
        case 0b110:
            arm_coprocessor_load_store(p, instruction);
        break;
        case 0b111: 
            arm_coprocessor_others_swi(p, instruction);
            break;
        default:    
            break;
    }
    return 0;
}


int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}


