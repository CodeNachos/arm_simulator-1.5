CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
# Our simulator :
# - is big-endian
# - does not initialize properly fp (so we omit it), see arm_core.c
# - has a non standard main (no OS so no return value), we disable the warning
AM_CFLAGS=-mbig-endian -fomit-frame-pointer -Wno-main
# Still big-endian for ARM assembly language sources
AM_CCASFLAGS=-mbig-endian
# We don't have any OS, so no stdlib as well
# The linker script maps loadable segments into memory, the overall size must match the simulator memory
# The entry point will always be main
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
# Our simulator is big-endian
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort ldst1 ldst2 ldst3 dtprcs1 dtprcs2 dtprcs3 brch1 prog1 prog2 prog3 prog4

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
insertion_sort_SOURCES=insertion_sort.c my_syscalls.s my_syscalls.h

EXTRA_DIST=linker_script

ldst1_SOURCES=ldst1.s
ldst2_SOURCES=ldst2.s
ldst3_SOURCES=ldst3.s

dtprcs1_SOURCES=dtprcs1.s
dtprcs2_SOURCES=dtprcs2.s
dtprcs3_SOURCES=dtprcs3.s

brch1_SOURCES=brch1.s

prog1_SOURCES=prog1.s
prog2_SOURCES=prog2.s
prog3_SOURCES=prog3.s
prog4_SOURCES=prog4.s

clean-local:
	$(RM) $(PROGS)
