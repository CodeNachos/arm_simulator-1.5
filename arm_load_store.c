/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

void check_U(int u, uint32_t *address, uint32_t rn, uint32_t offset){
    if (u== 1) *address = rn + offset;
	else *address = rn - offset;
}

int arm_load_store(arm_core p, uint32_t ins) {
	//DOC A5-3
	uint32_t rn = get_bits(ins, 19, 16);
	rn = arm_read_register(p, rn);
    	uint32_t address;
	uint32_t rd = get_bits(ins, 15, 12);
    	uint32_t rm = get_bits(ins, 3, 0);

	int bit_L = get_bit(ins, 20);
	int bit_W = get_bit(ins, 21);
	int bit_B = get_bit(ins, 22);
	int bit_U = get_bit(ins, 23);
	int bit_P = get_bit(ins, 24);
    	uint32_t offset_12 = get_bits(ins, 11, 0);
    
	//Immediate offset/index
	if (get_bits(ins, 27, 25) == 0b010) {
	
		// Load/Store : Word/Unsigned Byte - Immediate offset
		if ((bit_P == 1 && bit_W == 0) || (bit_P == 1 && bit_W == 1) || (bit_P == 0 && bit_W == 0)){
			check_U(bit_U, &address, rn,  offset_12);
            	if (bit_P == 1 && bit_W == 1) {
	                //calculated memory address is writen back to the base register
	                arm_write_register(p, rn, address);
		}
		}
        else if (bit_P == 0 && bit_W == 1) {
			return UNDEFINED_INSTRUCTION;
			}
		
		}
        
	
	
	//Register offset/index
	else if (get_bits(ins, 27, 25)==0b011 && get_bits(ins, 11, 4) == 0b00000000) {
		uint32_t rm_val = arm_read_register(p, rm);
		
		// Load/Store : Word/Unsigned Byte - Register offset
		if ((bit_P == 1 && bit_W == 0 ) || (bit_P == 1 && bit_W == 1) ||(bit_P == 0 && bit_W == 0)) {
			check_U(bit_U, &address, rn, rm_val);
            if(bit_P == 1 &&  bit_W == 1)
                //calculated memory address is writen back to the base register
                arm_write_register(p, rn, address);
		}
        else if (bit_P == 0 &&  bit_W == 1) {
			return UNDEFINED_INSTRUCTION;
		}
	}

	// Scaled register offset/index
	else if (get_bits(ins, 27, 25)==0b011 && !get_bit(ins, 4)) {
        uint32_t index;
		uint32_t shift_imm = get_bits(ins, 11, 7);
		uint8_t shift = get_bits(ins, 6, 5);
		uint32_t rm_val = arm_read_register(p, rm);
		
		if ((bit_P == 1 &&  bit_W == 0) || (bit_P == 1 &&  bit_W == 1 )|| (bit_P == 0 &&  bit_W == 0) ){
			switch(shift) {
				case LSL: 
                    index = rm_val << shift_imm; 
                    break;
				case LSR: 
					if (shift_imm == 0)
                        index = 0;
					else index = rm_val >> shift_imm;
				
				break;
				case ASR:
				if (shift_imm == 0) {
					if (get_bit(rm_val, 31) == 1) 
                        index = 0xFFFFFFFF;
					else index = 0;
				}
				else
					index = asr(rm_val, shift_imm);
				break;
				case ROR:
					if (shift_imm == 0)
						index = (get_bit(arm_read_cpsr(p), C) << 31) | (rm_val >> 1);
					else
						index = ror(rm_val, shift_imm);
				break;
				default: break;
			}

			check_U(bit_U, &address, rn,  index);

            if(bit_P == 1 && bit_W == 1) 
                //calculated memory address is writen back to the base register
			    arm_write_register(p, rn, address);
		    }
            else if (bit_P == 0 && bit_W == 1) {
			    return UNDEFINED_INSTRUCTION;
			}
 	
	}
    //LDRH & STRH
    else if(get_bits(ins, 27, 25) == 0b000 && get_bit(ins, 7) == 0b1 && get_bit(ins, 4) == 0b1){ 
            uint32_t immedH = get_bits(ins, 11, 8);
	        uint32_t immedL = get_bits(ins, 3, 0);
            uint32_t offset_8;
            uint32_t rm_val = arm_read_register(p, rm);

            if ( get_bit(ins, 24)==1){
                //Miscellaneous Loads and Stores - Immediate offset
                if(get_bits(ins, 22, 21)==0b10){
                    offset_8 = (immedH << 4) | immedL;
                    check_U(bit_U, &address, rn,  offset_8);
                }
                //Miscellaneous Loads and Stores - Register offset
                else if(get_bits(ins, 22, 21)==0){
                    check_U(bit_U, &address, rn,  rm_val);
                }
                //Miscellaneous Loads and Stores - Immediate pre-indexed
                else if(get_bits(ins, 22, 21)==0b11){
                    offset_8 = (immedH << 4) | immedL;
                    if (bit_U == 1) rn+= offset_8;
			        else rn-=offset_8;
                    rn=address;
                }
                //Miscellaneous Loads and Stores - Register pre-indexed
                else if(get_bits(ins, 22, 21)==0b01){
                    check_U(bit_U, &address, rn,  rm_val);
                    rn=address;
                }

            }
            else{
                 //Miscellaneous Loads and Stores - Immediate post-indexed
                if(get_bits(ins, 22, 21) ==0b10){
                    address= rn;
                    offset_8 = (immedH << 4) | immedL;
                    if (bit_U == 1) rn+= offset_8;
			        else rn-=offset_8;
                    
                }
                //Miscellaneous Loads and Stores - Register post-indexed
                else if(get_bits(ins, 22, 21) == 0){
                        address=rn;
                         if(bit_U == 1) rn+= rm_val;
			            else rn-=rm_val;
		            }
                }
            

            int bit_S = get_bit(ins, 6);
	        int bit_H = get_bit(ins, 5);

            if(bit_L==0 && bit_S==0 && bit_H==1){ //STRH
                warning("writing address %d\n", address);
                uint16_t half = arm_read_register(p, rd);
                arm_write_half(p, address, half);
            }
            else if(bit_L==1 && bit_S==0 && bit_H==1){//LDRH
                warning("loading address %d\n", address);
                uint16_t half;
                arm_read_half(p, address, &half);
                arm_write_register(p, rd, half);
            }
            else return UNDEFINED_INSTRUCTION;

            return SUCCESSFULLY_DECODED;

    }

        if (bit_L) { //load
            warning("loading address %d\n", address);
            if (bit_B) { //LDRB
                uint8_t octet;
                arm_read_byte(p, address, &octet);
                arm_write_register(p, rd, (uint32_t) octet);
            }
            else {
                uint32_t mot;
                arm_read_word(p, address, &mot);
                arm_write_register(p, rd, mot);
            }
        }

        else { //store
            warning("writing address %d\n", address);
            if (bit_B) { //STRB
                uint8_t octet;
				octet= arm_read_register(p, rd);
                arm_write_byte(p, address, octet);		
            }
            else {
                uint32_t mot;
                mot = arm_read_register(p, rd);
                arm_write_word(p, address, mot);
            }
        }
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
