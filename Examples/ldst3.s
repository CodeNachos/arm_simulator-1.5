.global main
.text
main:
    // Base register addressing mode
    ldr r0, =data_section  // Load the base address of data_section into r0

    // Load 8-bit value with register offset
    mov r2, #3
    ldrb r1, [r0, r2, lsl #1] // Load the value at the address (r0 + r2 * 2) into r1
    mov r2, #2
    strb r1, [r0, r2, lsl #2] // Store the value at the address (r0 + r2 * 4) into r11

    // Load 32-bit word with register offset
    ldr r3, [r0, r2, lsl #1] // Load the value at the address (r0 + r2 * 1) into r3
    
    swi 0x123456

.data
data_section:
    .word 0x12345678      // 32-bit value
    .hword 0xABCD         // 16-bit value
    .byte 0xEF            // 8-bit value
    .word 0x0000000       // 32-bit value
