# test dtprcs3
# execute from gdb after seting target to remote
# usage:
# target remote localhost:<port>
# source Examples/dtprcs3_autotest.gdb

set endian big

file Examples/dtprcs3
load

disassemble

set $failed = 0

break AND
cont

if $r1 != 10 || $r2 != 20 || \
    $r3 != 30 || $r4 != 40 || $r5 != 50 || $r6 != 60 || $r7 != 70 || $r8 != 80 || $r9 != 90
    printf "address 0x%x: instruction execution failed in main \n"
    set $failed = 1
end
set $cur_inst = $pc

if $failed == 0
    stepi
    if $r1 != 0x10
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r3 != 0x1a
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r6 != -570
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r9 != 340
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r13 != 165
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r1 != 0x14
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    set $cflag = $cpsr & 0x200000000
    if $r5 != ($r6 - ($r7 >> 5) - 1-$cflag) 
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    set $cflag = $cpsr & 0x200000000
    if $r8 != (($r10 >> $r1) - $r9 - 1-$cflag) 
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end


if $failed == 0
    set $rd = $r12
    stepi
    set $flags = $cpsr & 0xf00000000
    if $flags != 0x0 || $rd != $r12
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    set $rd = $r2
    stepi
    set $flags = $cpsr & 0xf00000000
    if $flags != 0x0 || $rd != $r2
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    printf "TEST COMPLETED: passed \n"
end

