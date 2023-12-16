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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__
#include <stdint.h>
#include <stdio.h>


//See explanation in .c file for details concerning the registers structure.

//Here we name a pointer to our "struct registers_data" as "registers". 
typedef struct registers_data *registers;

//Method 'registers_create' which creates a pointor to our data_registers structure:
registers registers_create();

//Method 'registers_destroy' which frees our registers pointor:
void registers_destroy(registers r);

// Method 'registers_get_mode' which returns the mode in which we're in.
uint8_t registers_get_mode(registers r);

// Method 'registers_current_mode_has_spsr' to determine whether the current mode has an spsr or not:
int registers_current_mode_has_spsr(registers r);

// Method 'registers_in_a_privileged_mode' which checks whether registers are in a priveleged mode or not:
int registers_in_a_privileged_mode(registers r);

//Method 'registers_read' which reads the value of a reg given the current mode
uint32_t registers_read(registers r, uint8_t reg, uint8_t mode);

// Method 'registers_read_cpsr' which lets us read the value at cpsr
uint32_t registers_read_cpsr(registers r);

//method 'registers_read_spsr' which lets us read the value at spsr, given the mode.
uint32_t registers_read_spsr(registers r, uint8_t mode);

//method 'registers_write' which writes in a register r given the mode a certain value.
void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value);
//Method 'registers_write_cpsr' which writes a value in the register CPSR
void registers_write_cpsr(registers r, uint32_t value);

//Method 'registers_write_spsr' which writes a value in the register SPSR
void registers_write_spsr(registers r, uint8_t mode, uint32_t value);

#endif
