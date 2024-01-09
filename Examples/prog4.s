.global main
.text

@ ce programme divise en deux parties une fonction principale et une auxiliaire
@ il sert a calculer une puissance de deux
@ r5 -> puissance de 2

@ call to main func
b main

@ aux fonction: multiply by 2
@takes a number as parameter at r0
@ return value placed at r0
fois_deux :
lsl r0, r0, #1
b call_back

@ main function
main :
mov r4, #1  @ base val
mov r5, #10 @ power

mov r6, #0  @ loop counter

for:
cmp r6, r5  @ if r6 >= r5 go to end_for
bge end_for

@ call aux func
mov r0, r4
b fois_deux
call_back :
mov r4, r0  @ retrieve result

add r6, r6, #1  @ r6++
b for

end_for:

@ store result in stack
ldr r5, =data_section
str r4, [r5]

end:
swi 0x123456

.data
data_section:
    .word 0x00000000