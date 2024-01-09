.global main
.text

is_even:
    ADD R2,R2,#1
    MOV PC, LR
is_odd:
    ADD R3,R3,#1
    MOV PC,LR
main:
    MOV R0,#0
    MOV R1,#10
    MOV R2,#0
    MOV R3,#0
    startloop: 
                AND R4, R0, #1
                CMP R4,#1
                bne even
                    BL is_odd
                    b treat
                even:
                    BL is_even
                treat:
                    ADD R0,R0,#1 
                    SUBS R4,R0,R1
                    bne startloop
    endloop:

        swi 0x123456
.data


