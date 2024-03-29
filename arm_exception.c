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
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0
#define Exception_bit_9 (CP15_reg1_EEbit << 9)

//

int arm_exception(arm_core p, uint8_t exception) {

    // basic cpsr mode, ignores everything
    uint32_t cpsr = 0x1d3 | Exception_bit_9;

    /* As there is no operating system in our simulator, we handle
     * software interrupts here :
     * - 0x123456 is the end of the simulation
     * - other opcodes can be used for any custom behavior,
     *   such as my_putchar given as an example
     */
    if (exception == SOFTWARE_INTERRUPT) {
        uint32_t value;
        uint32_t address = arm_read_register(p, 15);
        address -= 8;
        uint32_t instruction;
        arm_read_word(p, address, &instruction);
        instruction &= 0xFFFFFF;
        switch (instruction) {
        case 0x123456:
            return END_SIMULATION;
        case 0x000001:
            value = arm_read_register(p, 0);
            putchar(value);
            return 0;
        }
    }

    else{
        uint32_t restore_pc;
        
        switch (exception){
        /* ARM manual pages A2-18 to A2-26 */
        case RESET:
            arm_write_cpsr(p, cpsr); // enter in interruptions mode
            arm_write_register(p, 15, 0); // reset pc
            break;

        case FAST_INTERRUPT:
            restore_pc = arm_read_register(p, 15);
            restore_pc += 4;
            arm_write_register(p, 14, restore_pc); // save pc
            arm_write_spsr(p, arm_read_cpsr(p)); // save cpsr
            arm_write_cpsr(p, cpsr); // enter in interruptions mode  
            arm_write_register(p, 15, 0x0000001C); // go to interruptions handler
            break;

        case INTERRUPT:
            cpsr = clr_bit(cpsr, 6); // do not disable FIQ
            restore_pc = arm_read_register(p, 15);
            restore_pc += 4; 
            arm_write_register(p, 14, restore_pc); // save pc
            arm_write_spsr(p, arm_read_cpsr(p)); // save cpsr
            arm_write_cpsr(p, cpsr); // enter in interruptions mode
            cpsr = set_bit(cpsr, 6); // restore cpsr config back 
            arm_write_register(p, 15, 0x00000018); // go to interruptions handler
            break;

        case UNDEFINED_INSTRUCTION:
            cpsr = clr_bit(cpsr, 6); // do not disable FIQ
            restore_pc = arm_read_register(p, 15);
            arm_write_register(p, 14, restore_pc); // save pc
            arm_write_spsr(p, arm_read_cpsr(p)); // save cpsr
            arm_write_cpsr(p, cpsr); // enter in interruptions mode 
            cpsr = set_bit(cpsr, 6); // restore cpsr config back 
            arm_write_register(p, 15, 0x00000004); // go to interruptions handler
            break;
        
        case PREFETCH_ABORT:
            cpsr = clr_bit(cpsr, 6); // do not disable FIQ
            restore_pc = arm_read_register(p, 15);
            restore_pc += 4;
            arm_write_register(p, 14, restore_pc); // save pc
            arm_write_spsr(p, arm_read_cpsr(p)); // save cpsr
            arm_write_cpsr(p, cpsr); // enter in interruptions mode
            cpsr = set_bit(cpsr, 6); // restore cpsr config back  
            arm_write_register(p, 15, 0x0000000C); // go to interruptions handler
            break;

        case DATA_ABORT:
            cpsr = clr_bit(cpsr, 6); // do not disable FIQ
            restore_pc = arm_read_register(p, 15);
            restore_pc += 8;
            arm_write_register(p, 14, restore_pc); // save pc
            arm_write_spsr(p, arm_read_cpsr(p)); // save cpsr
            arm_write_cpsr(p, cpsr); // enter in interruptions mode
            cpsr = set_bit(cpsr, 6); // restore cpsr config back  
            arm_write_register(p, 15, 0x00000010); // go to interruptions handler
            break;

        default:
            // not an exception       
            break;
        }
    }
    return exception;
}