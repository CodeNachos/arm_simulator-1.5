
.global main
.text
main:
    // Base register addressing mode
    ldr r0, =data_section  // Load the base address of data_section into r0

    // Post-indexed addressing with 32-bit word
    ldr r1, [r0], #7        // Load word at address r0 to r1, then increment r0 by 7
    str r1, [r0], #-1       // Store the value in r1 to the address in r0, then decrement r0 by 1

    // Pre-indexed addressing with 16-bit halfword
    ldrh r2, [r0, #-2]!     // Decrement r0 by 2, then load the 16-bit value at the new address into r2
    strh r2, [r0, #3]!      // Increment r0 by 3, then store the value in r2 to the address in r0
    
    // Pre-indexed addressing with 8-bit byte
    sub r0, r0, #7        // r0 <- data_section
    ldrb r3, [r0, #6]!    // Increment r0 by 6, then load the 8-bit value at the new address into r3
    strb r3, [r0, #1]!     // Increment r0 by 1, then store the 8-bit value in r3 at adress r0

    sub r0, r0, #7        // r0 <- data_section
    ldrb r4, [r0], #1   // Load an 8-bit value from the address in r0 into r4,
                        // then increment the value in r0 by 1

    strb r4, [r0], #-3  // Store the 8-bit value in r4 to the address in r0,
                         // then decrement the value in r0 by 3

    ldrh r4, [r0], #7   // Load a 16-bit value from the address in r0 into r4,
                        // then increment the value in r0 by 7

    strh r4, [r0], #-11 // Store the 16-bit value in r4 to the address in r0,
                        // then decrement the value in r0 by 11

    swi 0x123456


.data
data_section:
    .word 0x11223344   // 32-bit data
    .hword 0x5566       // 16-bit data
    .byte 0x77         // 8-bit data
    .word 0x00000000   // 32-bit dummy value
