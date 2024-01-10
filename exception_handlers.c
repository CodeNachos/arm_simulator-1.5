#include "exception_handlers.h"


void reset(arm_core p) {
    ;
}

void undefined_instruction(arm_core p) {
    uint32_t quit_adr;
    quit_adr = arm_read_register(p, 14);
    arm_write_cpsr(p, arm_read_spsr(p)); // restore cpsr
    arm_write_register(p, 15, quit_adr); // quit interruption
}

void prefetch_abort(arm_core p) {
    uint32_t quit_adr;
    quit_adr = arm_read_register(p, 14) - 4;
    arm_write_cpsr(p, arm_read_spsr(p)); // restore cpsr
    arm_write_register(p, 15, quit_adr); // quit interruption
}

void data_abort(arm_core p) {
    uint32_t quit_adr;
    quit_adr = arm_read_register(p, 14) - 8;
    arm_write_cpsr(p, arm_read_spsr(p)); // restore cpsr
    arm_write_register(p, 15, quit_adr); // quit interruption
}

void fiq(arm_core p) {
    uint32_t quit_adr;
    quit_adr = arm_read_register(p, 14) - 4; // restore pc address
    arm_write_cpsr(p, arm_read_spsr(p)); // restore cpsr
    arm_write_register(p, 15, quit_adr); // quit interruption
}

void irq(arm_core p) {
    uint32_t quit_adr;
    quit_adr = arm_read_register(p, 14) - 4; // restore pc address
    arm_write_cpsr(p, arm_read_spsr(p)); // restore cpsr
    arm_write_register(p, 15, quit_adr); // quit interruption
}

