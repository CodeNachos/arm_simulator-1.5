.global main
.text

main:
    @ calculate text length and stotes it in r4

    mov r0, #0 @ text addr

    @ storing text
    mov r1, #0 @ text addr offset

    @ stores h
    mov r2,#104 
    str r2, [r0, r1, lsl #2]    @ mem[r0+r1*4] <- r2
    add r1,r1,#1    @ r1++

    @ stores e
    mov r2,#101
    str r2, [r0, r1, lsl #2]    @ mem[r0+r1*4] <- r2
    add r1,r1,#1    @ r1++

    @ stores l
    mov r2,#108
    str r2, [r0, r1, lsl #2]    @ mem[r0+r1*4] <- r2
    add r1,r1,#1    @ r1++

    @ stores l
    mov r2,#108
    str r2, [r0, r1, lsl #2]    @ mem[r0+r1*4] <- r2
    add r1,r1,#1    @ r1++

    @ stores o
    mov r2,#111
    str r2, [r0, r1, lsl #2]    @ mem[r0+r1*4] <- r2
    add r1,r1,#1    @ r1++

    @ stores end of string chacter 0
    mov r2,#0 
    str r2, [r0, r1, lsl #2]

    @ compute length

    mov r4, #0

    mov r1,#0 @ text character index

    @ load character and count until the string is over
    do :
        ldr r2, [r0, r1, lsl #2]   @ load character
    while :
        cmp r2, #0  @ compare character
        beq end_while   @ if end of string go to end
        add r1, r1, #1  @ index++
        add r4, r4, #1  @ counter++
        b do            @ restart comparison
    end_while :         @ end

    swi 0x123456

.data
text:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
