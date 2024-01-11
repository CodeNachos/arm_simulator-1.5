.global undf_inst
.global pfch_abort
.global data_abort
.global irq
.global fiq

undf_inst:
    movs pc, r14

pfch_abort:
    subs pc, r14, #4

data_abort:
    subs pc, r14, #4

irq:
    subs pc, r14, #4

fiq:
    subs pc, r14, #4
