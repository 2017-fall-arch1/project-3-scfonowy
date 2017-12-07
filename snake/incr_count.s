.file "_incr_count.c"

.text
.balign 2
.global incrCount

.text
incrCount: ; pointer to char in R12
add #1, 0(r12)
pop r0

.ident  "GCC: (GNU) 4.9.1 20140707 (prerelease (msp430-14r1-364)) (GNUPro 14r1) (Based on: GCC 4.8 GDB 7.7 Binutils 2.24 Newlib 2.1)"
