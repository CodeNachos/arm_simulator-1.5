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

#define UNBANKED_REG 8
#define BANKED_R8_R12 10
#define BANKED_R13_R14 12
#define STAT_REG 5


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

//The SPSR'S are banked registers. However, in the ARM5V architecture,
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
    //Other banked registers for r13-r14. Each mode has its own physical register for each r13/r14 register (except User and System, which share the same).
    //A total of 12 banked registers.
    //each pairwise indices represent r13 and r14 in a mode (0-r13_user, 1-r14_user, 2-r13_fiq, 3-r14_fiq...)
    uint32_t *banked_r13_r14;
    // Value of CPSR register:
    uint32_t CPSR;
    //Value of Program Counter:
    uint32_t PC;

    //Table containning values of the status registers: (SPSR)
    uint32_t *rstat;
};
//We see that the sum of registers:
//-Unbanked = 8
//-Banked r8-12 = 10
//-Banked r13-r14 = 12
//-CPSR
//-PC
//rstat = 5 (5 SPSR registers depending on the mode)
// = 37. So we're good.

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
    //Allocation of 8 general registers (r0-r7):
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

    //Initialize PC register:
    r->PC = 0;

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
//From the ARM5v documentation, we obtain the binary values associated with each mode.
//We therefore try to match it with the one given as parameter. (we also save SPSR accordingly)
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
        case SVC: //(r->stat[2] is SPSR_SUPERVISOR)
            r->rstat[2] = r->CPSR;
            return 1;
        case ABT: //(r->stat[3] is SPSR_ABORT)
            r->rstat[3] = r->CPSR;
            return 1;
        case UND: //(r->stat[4] is SPSR_UNDEFINED)
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
    //If the mode is any of fast_interrupt,interrupt, supervisor,abort,undefined or system, we return supervisor
    switch(mode){
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
        case SYS:
            return SVC;
        default:
            // else we return 0;
            return 0;
    }
}

// Method 'registers_read' which reads the value of a reg given the current mode:
// Parameters: registers r, uint8_t reg, uint8_t mode
// Returns: The value at register reg or 0 if mode doesn't exist
uint32_t registers_read(registers r, uint8_t reg, uint8_t mode)
{
    uint32_t value = 0;
    //If the register is UNBANKED
    if(reg>=0 && reg<=7)
    {
        //Then we don't care about the mode
        value = r->rUnbancked[reg];
    }
    //Else if it is between r8-r12
    else if(reg>=8 && reg<=12)
    {
        //We only need to deal with two modes (FIQ and anything else)
        if(mode!=FIQ)
        {
            value = r->banked_r8_r12[reg - 8]; //ri with i belonging to [[0,4]] is any mode except fiq
        }
        else
        {
            value = r->banked_r8_r12[reg - 8 + 5]; //ri with i belonging to [[5,9]] is fiq mode (ri_fiq)
        }
    }
    else if (reg>=13 && reg<=14)
    {
        switch(mode)
        {
            case USR:
                value = r->banked_r13_r14[reg - 13]; //r13_user and r14_user
                break;
            case FIQ:
                value = r->banked_r13_r14[reg - 13 + 2]; //r13_fiq and r14_fiq
                break;
            case IRQ:
                value = r->banked_r13_r14[reg - 13 + 4]; //r13_irq and r14_riq
                break;
            case SVC:
                value = r->banked_r13_r14[reg - 13 + 6]; //r13_svc and r14_svc
                break;
            case ABT:
                value = r->banked_r13_r14[reg- 13 + 8]; //r13_abort and r14_abort
            case UND:
                value = r->banked_r13_r14[reg -13 + 10]; //r13_undefined and r14_undefined
                break;
            case SYS:
                value = r->banked_r13_r14[reg - 13]; //r13_sys and r14_sys, sys and user share the same registers.
                break;
            default:
                value = value;
            }
    }
    //Reg = 15, so PC
    else if(reg == 15)
    {
        value = r->PC;
    }

    return value;
}

// Method 'registers_read_cpsr' which lets us read the value at cpsr
//Parameters: registers r
//Return: Value of CPSR
uint32_t registers_read_cpsr(registers r)
{
    uint32_t value = r->CPSR;
    return value;
}

//method 'registers_read_spsr' which lets us read the value at spsr, given the mode.
//Parameters: registers r, uint8_t mode
//Return: value at SPSR_mode if exists, 0 else
uint32_t registers_read_spsr(registers r, uint8_t mode) {
//REMINDER:
//-rstat[0] = spsr_FIQ
//-rstat[1] = spsr_IRQ
//-rstat[2] = spsr_SUPERVISOR
//-rstat[3] = spsr_ABORT
//-rstat[4] = spsr_UNDEFINED
    uint32_t value = 0;
    switch(mode)
    {
        case FIQ:
            value = r->rstat[0];
            break;
        case IRQ:
            value = r->rstat[1];
            break;
        case SVC:
            value = r->rstat[2];
            break;
        case ABT:
            value = r->rstat[3];
            break;
        case UND:
            value = r->rstat[4];
            break;
        default:
            value = 0;
        }
    return value;
}

//method 'registers_write' which writes in a register r given the mode a certain value.
//Parameters: registers r, uint8_t reg, uint8_t mode, uint32_t value
//return : void 
void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    //if register is UNBANCKED
    if(reg>=0 && reg<=7)
    {
        //then we don't care about the mode
        r->rUnbancked[reg] = value;
    }
    //Else if it is between r8-r12
    else if(reg>=8 && reg<=12)
    {
        //We only need to deal with two modes (FIQ and anything else)
        if(mode!=FIQ)
        {
            r->banked_r8_r12[reg - 8] = value; //ri with i belonging to [[0,4]] is any mode except fiq
        }
        else
        {
            r->banked_r8_r12[reg - 8 + 5] = value; //ri with i belonging to [[5,9]] is fiq mode (ri_fiq)
        }
    }
    else if (reg>=13 && reg<=14)
    {
        switch(mode)
        {
            case USR:
                r->banked_r13_r14[reg - 13] = value; //r13_user and r14_user
                break;
            case FIQ:
                r->banked_r13_r14[reg - 13 + 2] = value; //r13_fiq and r14_fiq
                break;
            case IRQ:
                r->banked_r13_r14[reg - 13 + 4] = value; //r13_irq and r14_riq
                break;
            case SVC:
                r->banked_r13_r14[reg - 13 + 6] = value; //r13_svc and r14_svc
                break;
            case ABT:
                r->banked_r13_r14[reg- 13 + 8] = value; //r13_abort and r14_abort
            case UND:
                r->banked_r13_r14[reg -13 + 10] = value; //r13_undefined and r14_undefined
                break;
            case SYS:
                r->banked_r13_r14[reg - 13] = value; //r13_sys and r14_sys, sys and user share the same registers.
                break;
            default:
                value = value;
            }
    }
    //Reg = 15, so PC
    else if (reg == 15)
    {
        r->PC = value;
    }
    return;
}

//Method 'registers_write_cpsr' which writes a value in the register CPSR
//Parameter: registers r, uint32_t value
//return: nothing
void registers_write_cpsr(registers r, uint32_t value) {
    r->CPSR = value;
    return;
}

//Method 'registers_write_spsr' which writes a value in the register SPSR
//Paramaters: registers r, uint8_t mode, uint32_t value
//Return: nothing
void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    //REMINDER:
//-rstat[0] = spsr_FIQ
//-rstat[1] = spsr_IRQ
//-rstat[2] = spsr_SUPERVISOR
//-rstat[3] = spsr_ABORT
//rstat[4] = spsr_UNDEFINED
    switch(mode)
    {
        case FIQ:
            r->rstat[0] = value;
            break;
        case IRQ:
            r->rstat[1] = value;
            break;
        case SVC:
            r->rstat[2] = value;
            break;
        case ABT:
            r->rstat[3] = value;
            break;
        case UND:
            r->rstat[4] = value;
            break;
        default:
            value = value;
        }
    return;
}
