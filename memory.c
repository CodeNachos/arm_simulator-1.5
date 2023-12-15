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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

#include "werror.h"

/**
 * SIMULATED MEMORY
 * addresses ranging from 0 to size-1
 * 
 * data: uint8_t array
 * size: memory size in bytes
*/
struct memory_data {
    uint8_t* data;
    size_t size;
};

#define _check_mem_integrity(mem) \
do { \
    if (!mem) { \
        error("null reference to memory\n"); \
        return NULL_REFERENCE; \
    } \
    if (!(mem->data && mem->size > 0)) { \
        error("simulated memory corrupted\n"); \
        return DATA_CORRUPTED; \
    } \
} while (0)

memory memory_create(size_t size) {
    if (size <= 0) {
        warning("Invalid memory size %ld \n", size);
        return NULL;
    }

    memory mem = (memory)malloc(sizeof(struct memory_data));
    if (!mem) {
        raise(ALLOC_ERROR, "unnable to alloc memory structure\n");
    }

    mem->size = size;
    mem->data = (uint8_t*)malloc(sizeof(uint8_t)*size);
    if (!mem->data) {
        free(mem);
        raise(ALLOC_ERROR, "unnable to alloc memory data space\n");
    }
    
    return mem;
}

size_t memory_get_size(memory mem) {
    if (!mem) {
        warning("null reference to memory passed as parameter\n");
        return 0;
    } else if (!(mem->data && mem->data > 0)) {
        warning("simulated memory corrupted\n");
        return 0;
    }
    return mem->size;
}

void memory_destroy(memory mem) {
    if (!mem) return;

    free(mem->data);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size) { 
        error("address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    }

    *value = mem->data[address];
    return SUCCESS;

}

int memory_read_half(memory mem, uint32_t address, uint16_t *value, uint8_t be) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size-1) { 
        error("half word address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    } 

    uint16_t aux = 0;
    be = (be > 0) ? MEM_BIG_ENDIAN : MEM_LITTLE_ENDIAN; // cast be to bool
    switch ((uint8_t)(be == is_big_endian()))   // check if be is sytem convention
    {
    case 0: // false inverse value
        aux |= ((uint16_t)(mem->data[address++])) << 8;
        aux |= (uint16_t)(mem->data[address]);
        break;
    case 1: // true read value directly
        aux = ((uint16_t*)(mem->data))[address];
        break;
    default:
        error("Invalid be value");
        return FAILURE;
    } 
    *value = aux;
    return SUCCESS;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value, uint8_t be) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size-3) { 
        error("word address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    } 

    uint32_t aux = 0;
    be = (be > 0) ? MEM_BIG_ENDIAN : MEM_LITTLE_ENDIAN; // cast be to bool
    switch ((uint8_t)(be == is_big_endian()))   // check if be is system convention
    {
    case 0: // false inverse value
        for (int i = 0; i < 4; i++) {
            aux |= ((uint32_t)(mem->data[address+i])) << ((3-i)*8);
        }
        break;
    case 1: // true read value directly
        aux = ((uint32_t*)(mem->data))[address];
        break;
    default:
        error("Invalid be value");
        return FAILURE;
    }
    *value = aux;
    return SUCCESS;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size) { 
        error("address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    }

    mem->data[address] = value;
    return SUCCESS;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size-1) { 
        error("half word address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    }

    be = (be > 0) ? MEM_BIG_ENDIAN : MEM_LITTLE_ENDIAN; // cast be to bool
    switch ((uint8_t)(be == is_big_endian()))   // check if be is system convention
    {
    case 0: // false inverse value
        mem->data[address++] = (uint8_t)(value >> 8);
        mem->data[address] = (uint8_t)(value);
        break;
    case 1: // true write value directly
        ((uint16_t*)(mem->data))[address] = value;
        break;
    default:
        error("Invalid be value");
        return FAILURE;
    }

    return SUCCESS;

}

int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be) {
    _check_mem_integrity(mem);

    if (address < 0 || address >= mem->size-3) { 
        error("word address out of range\n"); 
        return INDEX_OUT_OF_RANGE; 
    }

    be = (be > 0) ? MEM_BIG_ENDIAN : MEM_LITTLE_ENDIAN; // cast be to bool
    switch ((uint8_t)(be == is_big_endian()))   // check if be is system convention
    {
    case 0: // false inverse value
        for (int i = 0; i < 4; i++) {
            mem->data[address+i] = (uint8_t)(value >> (3-i)*8);
        }
        break;
    case 1: // true write value directly
        ((uint32_t*)(mem->data))[address] = value;
        // aux = ((uint32_t*)(mem->data))[address];
        break;
    default:
        error("Invalid be value");
        return FAILURE;
    }

    return SUCCESS;

}
