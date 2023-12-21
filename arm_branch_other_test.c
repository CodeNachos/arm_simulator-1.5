#include <stdio.h>
#include "util.h"
#include "trace.h"
#include "trace_location.h"
#include "memory.h"
#include "registers.h"
#include "arm.h"
#include "arm_core.h"
#include "arm_constants.h"
#include "arm_exception.h"
#include "arm_branch_other.h"
#include "arm_data_processing.h"
#include "werror.h"

#define NB_INSTR 1

int main(int argc, char **argv) {
    int i, return_code;
    uint32_t ins = 0xEE000000; //test value
    arm_init();
    arm_core p = arm_create(registers_create(), memory_create(1024));

    for (i = 0; i < NB_INSTR; i++) {
        return_code = arm_coprocessor_others_swi(p, ins);
        if (return_code)
            warning("There is an exception with code %d\n", return_code);
    }
    
    arm_destroy(p);

    return 0;
}