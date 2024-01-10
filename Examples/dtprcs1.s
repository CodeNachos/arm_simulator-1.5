.global main
.text
main:
    /* Initialize registers with values */
    MOV R0, #5      /* Set R0 to 5 */
    MOV R1, #10     /* Set R1 to 10 */
    MOV R2, #0      /* Clear R2 */

    /* Test ADD instruction */
    ADD R2, R0, R1   /* R2 = R0 + R1 */

    /* Test SUB instruction */
    SUB R3, R1, R0   /* R3 = R1 - R0 */

    /* Test AND instruction */
    AND R4, R0, R1   /* R5 = R0 & R1 */

    /* Test ORR instruction */
    ORR R5, R0, R1   /* R6 = R0 | R1 */

    /* Test EOR instruction */
    EOR R6, R0, R1   /* R7 = R0 ^ R1 */

    /* Test BIC instruction */
    BIC R7, R0, R1   /* R8 = R0 & ~R1 */


    swi 0x123456
    