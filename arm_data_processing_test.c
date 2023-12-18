#include <stdio.h>
#include "util.h"
#include "debug.h"
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

int main(int argc, char **argv) {
    arm_core p = arm_create(registers_create(), memory_create(1024));

    
    arm_destroy(p);
    return 0;
}