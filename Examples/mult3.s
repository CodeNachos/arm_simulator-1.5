.global main
.text
main:
    ldr r0, =data_section   // Charge l'adresse de data_section dans r0

    // Load multiple 32-bit words with register offset (Increment Before)
    mov r1, #0
    ldmib r0!, {r2, r3, r4, r5}   // Charge les valeurs aux adresses (r0 + 4, r0 + 8, r0 + 12, r0 + 16) dans r2, r3, r4, r5

    // Store multiple 32-bit words with register offset (Increment Before)
    mov r1, #0
    stmib r0!, {r2, r3, r4, r5}   // Stocke les valeurs de r2, r3, r4, r5 aux adresses (r0 + 4, r0 + 8, r0 + 12, r0 + 16)

    swi 0x123456

.data
data_section:
    .word 0x12345678      // 32-bit value
    .word 0x87654321      // 32-bit value
    .word 0xABCDEF01      // 32-bit value
    .word 0x76543210      // 32-bit value
    .word 0xbebebebe
