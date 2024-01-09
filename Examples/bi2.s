.global main
.text

main:
    MOV R0,#5
    MOV R1, #0 
    startloop:
                MOV R2, R1
                nestedloop:
                            ADD R2, R2, #1
                            SUBS R3, R0, R2
                            bne nestedloop
                ADD R1, R1, #1
                CMP R1, R0
                bne startloop
    swi 0x123456
.data

                            