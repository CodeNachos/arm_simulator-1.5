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
#ifndef __ARM_BRANCH_OTHER_H__
#define __ARM_BRANCH_OTHER_H__
#include <stdint.h>
#include "arm_core.h"


/*
	Branching instructions, there are the following types:
	-- B: standard branching
	-- BL: branch with link
	-- BX: branch and exchange
	-- BLX: branch with link and exchange
	instruction is coded in the following format:
	cond(4 bits) + 101 + L(bit) + 24 offset bits
*/
int arm_branch(arm_core p, uint32_t ins);
/*
	Software interrupt exception instruction:
	instruction is coded in the following format:
	cond(4 bits) + 1111 + 24 bits immedeate
*/
int arm_coprocessor_others_swi(arm_core p, uint32_t ins);
/*
	Miscellaneous instructions, implemented:
	Status register access:
	-- MRS: move PSR to general register 
	-- MSR: move general register to PSR
	-- CPS: change proccessor state of CPSR
	-- SETEND: set the endianess of the CPSR
*/
int arm_miscellaneous(arm_core p, uint32_t ins);

#endif
