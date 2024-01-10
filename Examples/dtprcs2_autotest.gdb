# test dtprcs2
# execute from gdb after seting target to remote
# usage:
# target remote localhost:<port>
# source Examples/dtprcs2_autotest.gdb

set endian big

file Examples/dtprcs2
load

disassemble

set $cur_inst = $pc
set $failed = 0

stepi
if $r0 != 0x0f
    printf "address 0x%x: instruction execution failed \n", $cur_inst
    set $failed = 1
end
set $cur_inst = $pc

if $failed == 0
    stepi
    if $r1 != 0x05
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r2 != 0x5
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r3 != 0x0a
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r4 != 0x0f
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r5 != 0x0a
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r6 != 0x0a
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r7 != 0x14
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r8 != 0x23
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r9 != 0x18
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r10 != -0xf
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end


if $failed == 0
    set $rd = $r0
    stepi
    set $flags = $cpsr & 0xf00000000
    if $flags != 0x0 || $rd != $r0
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
    set $rd = $r4
    stepi
    set $flags = $cpsr & 0xf00000000
    if $flags != 0x0 || $rd != $r4
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    set $rd = $r6
    stepi
    set $flags = $cpsr & 0xf00000000
    if $flags != 0x0 || $rd != $r6
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r11 != 0xf
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r12 != 0xff
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r13 != 0xb
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    stepi
    if $r14 != 0xffffff00
        printf "address 0x%x: instruction execution failed \n", $cur_inst
        set $failed = 1
    end
    set $cur_inst = $pc
end

if $failed == 0
    printf "TEST COMPLETED: passed \n"
end
