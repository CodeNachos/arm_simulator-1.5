.global main
.text
main:
    @ calculate mean of r0 and r1 and stores it in r2

    @ r1 and r2 value declarations
    mov r0, #20
    mov r1, #30

    @ r2 <- r0
    mov r2,r0
    @ r2 <- r2 + r1
    add r2,r2,r1
    @ r2 <- r2/2
    lsr r2,r2,#1
    
    swi 0x123456