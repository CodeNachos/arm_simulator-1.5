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

struct memory_data {
    uint8_t* data;
    size_t size;
};

memory memory_create(size_t size) {
    if (size == 0) {
        warning("memory size is 0,  returning NULL\n");
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
        warning("null reference to mem\n");
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
    if (!mem) {
        return -NULL_REFERENCE;
    }

    return -1;

}

int memory_read_half(memory mem, uint32_t address, uint16_t *value, uint8_t be) {
    return -1;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value, uint8_t be) {
    return -1;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    return -1;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be) {
    return -1;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be) {
    return -1;
}
