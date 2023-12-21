# test dtprcs1
# execute from gdb after seting target to remote
# usage:
# target remote localhost:<port>
# source Examples/dtprcs1_autotest.gdb

set endian big

file Examples/dtprcs1
load

disassemble

set $cur_inst = $pc
set $failed = 0

stepi
if $r0 != 5
    printf "address 0x%x: instruction execution failed \n", $cur_inst
    set $failed = 1
end
set $cur_inst = $pc

if $failed == 0
    stepi
    if $r1 != 10
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r2 != 0
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r2 != 15
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r3 != 5
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r4 != 0
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r5 != 0xf
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r6 != 0xf
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r7 != 5
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    printf "TEST COMPLETED: passed \n"
end
