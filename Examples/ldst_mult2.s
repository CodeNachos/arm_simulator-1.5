.global main
.text
main:
    // Base register addressing mode for Load Multiple
    ldr r0, =data_section   // Load the base address of data_section into r0

    // Load multiple 32-bit words with register offset (Increment After)
    mov r1, #4
    ldmia r0!, {r2, r3, r4, r5}   // Load the values at the address (r0, r0 + 4, r0 + 8, r0 + 12) into r2, r3, r4, r5

    // Base register addressing mode for Store Multiple
    // ldr r0, =data_section   // Load the base address of data_section into r0

    // Store multiple 32-bit words with register offset (Increment After)
    mov r1, #4
    stmia r0!, {r2, r3, r4, r5}   // Store the values from r2, r3, r4, r5 to the addresses (r0, r0 + 4, r0 + 8, r0 + 12)

    swi 0x123456

.data
data_section:
    .word 0x12345678      // 32-bit value
    .word 0x87654321      // 32-bit value
    .word 0xABCDEF01      // 32-bit value
    .word 0x76543210      // 32-bit value
