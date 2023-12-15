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
#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <sys/types.h>

#define MEM_BIG_ENDIAN 1
#define MEM_LITTLE_ENDIAN 0


/**
 * SIMULATED MEMORY
 * addresses ranging from 0 to size-1
*/
typedef struct memory_data *memory;

/**
 * Allocate simulated memory
 * 
 * @param size: size of memory in bytes
 * @return pointer to memory type instance
 * @warning if size value is <= to 0 return NULL
*/
memory memory_create(size_t size);

/**
 * @param mem: memory type instance
 * @return memory size in bytes
 * @warning if NULL reference is given return 0
*/
size_t memory_get_size(memory mem);

/**
 * Free simulated memory
 * 
 * @param mem: memory type instance
*/
void memory_destroy(memory mem);

/* All these functions perform a read/write access to a byte/half/word data at
 * address a in mem. The result is respectively taken from or stored to the
 * parameter value. The access is made using the given endianess (be == 1 for a
 * big endian access and be == 0 for a little endian access).
 * The return value indicates a succes (0) or a failure (-1).
 */

/**
 * Retrieve byte value at given address
 * 
 * @param mem: memory type instance
 * @param address: valid address in mem
 * @param value: location to store retrieved byte
 * @return 0 in case of success, a positive erro code if failure
*/
int memory_read_byte(memory mem, uint32_t address, uint8_t * value);

/**
 * Retrieve 16-bit value from given address using be endianess convetion
 * @param mem: memory type instance
 * @param address: valid address in mem
 * @param value: location to store retrieved 16-bit value
 * @param be: endianness mode
 * @return 0 in case of success, a positive erro code if failure
 * @warning any positive value for be is treated as big-endian and 0 for 
 * little-endianes
*/
int memory_read_half(memory mem, uint32_t address, uint16_t * value, uint8_t be);

/**
 * Retrieve 32-bit value from given address using be endianess convetion
 * @param mem: memory type instance
 * @param address: valid address in mem
 * @param value: location to store retrieved 32-bit value
 * @param be: endianness mode
 * @return 0 in case of success, a positive erro code if failure
 * @warning any positive value for be is treated as big-endian and 0 for 
 * little-endianes
*/
int memory_read_word(memory mem, uint32_t address, uint32_t * value, uint8_t be);

int memory_write_byte(memory mem, uint32_t address, uint8_t value);
int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be);
int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be);

#endif
