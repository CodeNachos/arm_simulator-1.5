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

//Pointers in the general registers table
#define USR_INDEX 0
#define SVC_INDEX 16
#define ABT_INDEX 32
#define UND_INDEX 48
#define IRQ_INDEX 64
#define FIQ_INDEX 80

//General registers table.
#define GREG_TABLE 96
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
    //Table containing general registers with all modes included.
    uint32_t *genreg;


    // Value of CPSR register:
    uint32_t CPSR;

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
    //Allocation of general registers table:
    r->genreg = (uint32_t *)malloc(sizeof(uint32_t) * GREG_TABLE);
    //Check if successfull allocation
    if(r->genreg == NULL)
        return r;
    
    //Initialize table:
    for (int i = 0; i < GREG_TABLE;i++)
    {
        r->genreg[i] = 0;
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
    if(r->genreg!=NULL)
    {
        free(r->genreg);
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

//Method 'mode_ind' which determines the pointer in our reg table and returns it, based on the mode and the reg number
//Parameters: uint8_t reg, uint8_t mode
//Return: Index (pointer) to our starting registers for a given mode
//        -1 if the mode or reg doesn't exist.
uint32_t mode_ind(uint8_t reg, uint8_t mode)
{
    //Register is unbanked:
    if(reg >=0 && reg<=7)
    {
        //Don't care about mode
        return USR_INDEX;
    }
    //If register is banked and between 8 and 12
    else if(reg>=8 && reg<=12)
    {
        //We only care about two modes:
        //If the mode is FIQ:
        if(mode == FIQ)
            return FIQ_INDEX;
        //Else if it is anything else 
        else if(mode == USR || mode == SYS || mode == SVC|| mode == ABT || mode == UND || mode == IRQ)
            return USR_INDEX;
        //If it is an invalid mode
        else
            return -1;
    }
    //Else if it is a banked register between 13 and 14:
    else if(reg>=13 && reg<=14)
    {
        //We extract our pointer depending on the mode:
        switch(mode)
        {
            case USR:
            case SYS:
                return USR_INDEX;
            case SVC:
                return SVC_INDEX;
            case ABT:
                return ABT_INDEX;
            case UND:
                return UND_INDEX;
            case IRQ:
                return IRQ_INDEX;
            case FIQ:
                return FIQ_INDEX;
            default:
                return -1;
        }
    }
    //If it is PC:
    else if(reg == 15)
    {
        //Then we don't care about the mode:
        return USR_INDEX;
    }
    //Reg number is invalid
    else
    {
        return -1;
    }
}

// Method 'registers_read' which reads the value of a reg given the current mode:
// Parameters: registers r, uint8_t reg, uint8_t mode
// Returns: The value at register reg or 0 if mode or reg number doesn't exist
uint32_t registers_read(registers r, uint8_t reg, uint8_t mode)
{
    uint32_t value = 0;
    //Get pointer to our table, which points at the first register of a given mode
    int Mindex = mode_ind(reg, mode);
    // If an error has occured, return -1;
    if (Mindex == -1)
        return 0;
    value = r->genreg[Mindex + reg];
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
    int Mindex = mode_ind(reg, mode);
    if(Mindex==-1)
        return;
    r->genreg[Mindex + reg] = value;
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
