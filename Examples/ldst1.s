.global main
.text
main:
    // Base register addressing mode
    ldr r0, =data_section  // Load the base address of data_section into r0
    add r7, r0, #7         // Adress for writing

    // Store and Load 32-bit word (str/ldr)
    ldr r1, [r0]           // Load the value at the address in r0 into r1
    str r1, [r7]           // Store the value in r1 to the address in r7

    // Store and Load 16-bit halfword (strh/ldrh)
    ldrh r2, [r0, #4]      // Load the value at the address in r0 + 4 into r2
    strh r2, [r0, #7]      // Store the value in r2 to the address in r0 + 7

    // Store and Load 8-bit byte (strb/ldrb)
    ldrb r3, [r0, #6]      // Load the value at the address in r0 + 6 into r6
    strb r3, [r0, #7]      // Store the value in r3 to the address in r0 + 6

    swi 0x123456

.data
data_section:
    .word 0x12345678      // 32-bit value
    .hword 0xABCD         // 16-bit value
    .byte 0xEF            // 8-bit value
    .word 0x44332211      // 32-bit dummy value