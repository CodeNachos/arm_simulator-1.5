.global main
.text

main:
    @ multiplication  2*3
    mov r1,#2
    mov r2,#3

    mov r4,#0 @ resultat
    mov r3,#0 @ compteur de boucle

    @ for i=0; i<3; i++
    for :
    	cmp r3,r2
    	bge end_for
    
    	@ resultat += 2
    	add r4,r4,r1

    	add r3,r3,#1
    	b for
    end_for :
    @ end

    swi 0x123456