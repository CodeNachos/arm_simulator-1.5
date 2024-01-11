.global main
.text
main:
    // populating vector table

    // adding movs pc, r14 to 0x4
    ldr r0, =data_section
    ldr r1, [r0]
    mov r2, #0x04
    str r1, [r2]

    // raising undefined instruction
    ldrsh r3, [fp, #-12]

    swi 0x123456    

.data
data_section:
    .word 0xe1b0f00e