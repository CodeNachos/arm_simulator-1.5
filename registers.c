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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

#define UNBANKED_REG 9
#define BANKED_R8_R12 10
#define BANKED_R13_R14 12
#define STAT_REG 5
#define USER 0b10000
#define FIQ 0b10001
#define IRQ 0b10010
#define SUPERVISOR 0b10011
#define ABORT 0b10111
#define UNDEFINED 0b11011
#define SYSTEM 0b11111


// Based on the ARMv5 documentation, this ARM version contains a grand total of 37 registers:
//- 31 of which are "General Purpose" (means they're used to store, load, variables in our program, and other uses...).
//General Purpose registers are basically the ones we use when writing assemble code, R0,R1,R2,PC,LR,..., ETC
//- 6 of which are "Status Registers", which give us info concerning the Status of our program. 
//These registers are usually consulted, with the purpose of keeping the status of our program in check.
//These registers are basically, CPSR, SPSR_user, SPSR_sys,...,ETC.

//Among the registers, some of them are 'unbanked' and some others are 'banked', 'unbanked' basically means that
//some of them are always used unconditional to the mode we're in (r0-r7), 'banked' means that,
//depending on the mode, the user may, for
//example, be able to access the value at that register (mode : user), or not(mode: supervisor).
//The registers concerned are (r8-14).

//The SPSR'S, even though they're banked registers, do have duplicated. However, in the ARM5V architecture,
//it is indicated that SPSR is only applied to exception modes, so there exists 5 of them.

//As specified in the ARMv5 documentation, there are 7 modes:
//-User (by default)
//-System (Privileged mode)
//-Supervisor(Exception mode)
//-Abort (Exception mode)
//-Undefined (Exception mode)
//-Interrupt (Exception mode)
//-Fast Interrupt(Exception mode)

// With this in mind, we'll be able to properly construct our registers data structure:
struct registers_data{
    //Table containing values of unbanked registers r0-r7 (32 bits since that's how it is in the documentation)
    uint32_t *rUnbancked;

    //Table containing banked registers for r8-r12, as there are only two physical different register for each
    //register. There's only r8_user and r8_fiq (in other modes, it's r8_user).
    //Indices from 0 to 4 are reserved to r8_user - r12_user.
    //Indices from 5 to 9 are reserved to r8_fiq - r12_fiq.
    uint32_t *banked_r8_r12;
    //Other banked registers for r13-r14. Each mode has its own physical register for each r13/r14 register.
    //A total of 12 banked registers.
    //Indices 0 to 5 are reserved to r13 and all its modes (r13_user, r13_sys, etc..) and the rest 
    //for r14 and its modes.
    uint32_t *banked_r13_r14;
    // Value of CPSR register:
    uint32_t CPSR;
    //Table containning values of the status registers: (also 32 bits)
    uint32_t *rstat;
};

//Method 'registers_create' which creates a pointer to our data_register structure:
//Parameters: none
//Return value: -A pointer to our register_data structure if no errors occur
//              -NULL if an error has occured.
registers registers_create() {
    //Memory allocation for our register structure:
    registers r = (registers)malloc(sizeof(struct registers_data));
    // If failed:
    if (r == NULL)
        return r;
    //Allocation of 9 general registers (r0-r8):
    r->rUnbancked = (uint32_t *)malloc(sizeof(uint32_t) * UNBANKED_REG);
    //If failed:
    if(r->rUnbancked==NULL) 
        return r;
    //Initialize register value to 0 (0X00000000)
    for (int i = 0; i < UNBANKED_REG;i++)
    {
        r->rUnbancked[i] = 0;
    }
    //Initialize banked r8-r12 registers:
    r->banked_r8_r12 = (uint32_t *)malloc(sizeof(uint32_t) * BANKED_R8_R12);
    //If failed:
    if(r->banked_r8_r12==NULL)
        return r;
    //Initialize R8-R12 registers values
    for (int i = 0; i < BANKED_R8_R12; i++)
    {
        r->banked_r8_r12[i] = 0;
    }

    //Initialize R13-R14 registers:
    r->banked_r13_r14 = (uint32_t *)malloc(sizeof(uint32_t) * BANKED_R13_R14);
    //If failed:
    if(r->banked_r13_r14==NULL)
    {
        return r;
    }
    //Initialize their values:
    for (int i = 0; i<BANKED_R13_R14;i++)
    {
        r->banked_r13_r14[i] = 0;
    }

    // Initialize CPSR register:
    r->CPSR = 0;

    //Allocation of SPSR registers:
    r->rstat = (uint32_t *)malloc(sizeof(uint32_t) * STAT_REG);
    //If failed:
    if(r->rstat == NULL)
        return r;
    //Initialize Stat Reg:
    for (int i = 0; i < STAT_REG; i++)
    {
        r->rstat[i] = 0;
    }
    //Return our pointor:
    return r;
}

//Method 'registers_destroy' which frees our registers pointer:
//Parameter: Registers r
//Return: void
void registers_destroy(registers r) {
    //If any of the allocated memory isn't NULL, we free it.
    if(r->rUnbancked!=NULL)
    {
        free(r->rUnbancked);
    }
    if(r->banked_r8_r12!=NULL)
    {
        free(r->banked_r8_r12);
    }
    if(r->banked_r13_r14!=NULL)
    {
        free(r->banked_r13_r14);
    }
    if(r->rstat!=NULL)
    {
        free(r->rstat);
    }
    if(r!=NULL)
    {
        free(r);
    }
    return;
}

//Method 'registers_get_mode' which returns the mode in which we're in:
//Parameters: registers r
//Return: a 8-bit integer indicating the mode we're in.
uint8_t registers_get_mode(registers r) {
//NOTE: In the ARM5V documentation, it is indicated that the mode in which we're in is encoded by the 5 least 
//significant bits in the CPSR register. We use a mask to obtain this value.
    return (uint8_t) (r->CPSR & (0x1F));
}

//Method 'registers_mode_has_spsr' which returns a boolean indicating whether the mode we're in has
//SPSR or not.
//Parameters: registers r, uint8_t mode (8 bits)
//Return: boolean indicating whether the current mode has a SPSR associated to it or not. 
static int registers_mode_has_spsr(registers r, uint8_t mode) {
//NOTE: Remember, the only modes that have an SPSR associated to them are the Exception modes,
//which are Supervisor, Abort, Undefined, IRQ, fast_IRQ.
//From the ARM5v documentation, the binary values 0b10000 and 0b11111 are those associated to User and System.
//So we basically check whether the mode is different from these values or not. (we also save SPSR accordingly)
//If the mode is any of the values:
    switch(mode)
    {
        case FIQ:
        //We save it into a CPSR (r->stat[0] is SPSR_FIQ)
            r->rstat[0] = r->CPSR;
            return 1;
        case IRQ: //(r->stat[1] is SPSR_IRQ)
            r->rstat[1] = r->CPSR;
            return 1;
        case SUPERVISOR: //(r->stat[2] is SPSR_SUPERVISOR)
            r->rstat[2] = r->CPSR;
            return 1;
        case ABORT: //(r->stat[3] is SPSR_ABORT)
            r->rstat[3] = r->CPSR;
            return 1;
        case UNDEFINED: //(r->stat[4] is SPSR_UNDEFINED)
            r->rstat[4] = r->CPSR;
            return 1;
        default:
            // Else we return 0;
            return 0;
        }
}
// Method 'registers_current_mode_has_spsr' to determine whether the current mode has an spsr or not:
// Parameters: registers r
// Return value: boolean indicating whether true or not
int registers_current_mode_has_spsr(registers r)
{
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

// Method 'registers_in_a_privileged_mode' which checks whether registers are in a priveleged mode or not:
//Parameters: registers r
//Return : boolean int indicating whether true or false
int registers_in_a_privileged_mode(registers r) {
    uint8_t mode = registers_get_mode(r);
    switch(mode){
        case FIQ:
        case IRQ:
        case SUPERVISOR:
        case ABORT:
        case UNDEFINED:
        case SYSTEM:
            return SVC;
        default:
            return 0;
    }
}

//Method 'registers_read' which reads the value of a reg given the current mode:
//Parameters: registers r, uint8_t reg, 
uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    uint32_t value = 0;
    /* � compl�ter... */
    return value;
}

uint32_t registers_read_cpsr(registers r) {
    uint32_t value = 0;
    /* � compl�ter... */
    return value;
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    uint32_t value = 0;
    /* � compl�ter... */
    return value;
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    /* � compl�ter... */
}

void registers_write_cpsr(registers r, uint32_t value) {
    /* � compl�ter... */
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    /* � compl�ter... */
}
