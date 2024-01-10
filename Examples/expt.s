#include "exception_handlers.h"

.global main
.text

main:
    ldr r0, =irq
    mov r1, #0x18
    str r0, [r1]
    
    swi 0x4

    swi 0x123456

my_func:
    bx lr

.data