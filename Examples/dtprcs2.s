.global main
.text
main:
    // Test AND instruction
    MOV R0, #0x0F
    MOV R1, #0x05
    AND R2, R0, R1  // R2 = R0 AND R1

    // Test EOR instruction
    MOV R3, #0x0A
    EOR R4, R2, R3  // R4 = R2 EOR R3

    // Test SUB instruction
    SUB R5, R4, R2  // R5 = R4 - R2

    // Test RSB instruction
    RSB R6, R2, R4  // R6 = R4 - R2 (reverse subtract)

    // Test ADD instruction
    ADD R7, R5, R6  // R7 = R5 + R6

    // Test ADC instruction
    ADC R8, R7, R4  // R8 = R7 + R4 + carry

    // Test SBC instruction
    SBC R9, R8, R5  // R9 = R8 - R5 - borrow

    // Test RSC instruction
    RSC R10, R9, R6 // R10 = R6 - R9 - borrow (reverse subtract with carry)

    // Test TST instruction
    TST R0, R1  // Test if R0 AND R1, result not stored

    // Test TEQ instruction
    TEQ R2, R3  // Test if R2 EOR R3 , result not stored

    // Test CMP instruction
    CMP R4, R5  // Compare R4 - R5, result not stored

    // Test CMN instruction
    CMN R6, R7  // Compare R6 + R7, result not stored

    // Test ORR instruction
    ORR R11, R4, R5  // R11 = R4 OR R5

    // Test MOV instruction
    MOV R12, #0xFF  // Move immediate value to register

    // Test BIC instruction
    BIC R13, R11, R7  // R13 = R11 AND NOT R7

    // Test MVN instruction
    MVN R14, R12  // R14 = NOT R12

    swi 0x123456
    