.file "_incr_count.c"

.text
.balign 2
.global incrCount
.global incrScore

.text
incrCount: ; pointer to char in R12
add #1, 0(r12)
pop r0

incrScore: ; pointer to char array in r12, score in r13
	push r6 ; use r6 to save pointer
	push r7	; use r7 to save score
	mov r13, r7
	mov r12, r6
	mov #48, 0(r6)
	mov r13, r12
	mov #10, r13
	call #div
	add r12, 0(r6)
	mov #48, 1(r6)
	mov r13, r12
	mov #10, r13
	call #mod
	add r12, 1(r6)
	pop r7 ; restore r7
	pop r6 ; restore r6
	pop r0 ; return

div: ; dividend in r12, divisor in r13
	push #0 ; quotient high
	push #0 ; quotient low
	push r13 ; divisor high
	push #0 ; divisor low
	cmp r4, r4 ; clear carry
	rrc 2(r1) ; shift divisor
	rrc 0(r1) ; continue shift
	push #8000 ; position of quotient bit

divwhile: cmp #0, 0(r1)
	jz divdone
	mov #0, r5 ; dividend high
	mov 4(r1), r6 ; divisor high
	cmp 2(r1), r12
	subc r6, r5
	jc divendif
	sub 2(r1), r12 ; subtract from dividend
	bis 0(r1), 6(r1) ; set quotient

divendif: cmp r4, r4
	rrc 4(r1)
	rrc 2(r1) ; shift divisor
	rrc 0(r1) ; shift position
	jmp divwhile

divdone: mov 6(r1), r12 ; return quotient
	add #10, r1
	pop r0


mod: ; dividend in r12, divisor in r13
	push #0 ; quotient high
	push #0 ; quotient low
	push r13 ; divisor high
	push #0 ; divisor low
	cmp r4, r4 ; clear carry
	rrc 2(r1) ; shift divisor
	rrc 0(r1) ; continue shift
	push #8000 ; position of quotient bit

modwhile: cmp #0, 0(r1)
	jz moddone
	mov #0, r5 ; dividend high
	mov 4(r1), r6 ; divisor high
	cmp 2(r1), r12
	subc r6, r5
	jc modendif
	sub 2(r1), r12 ; subtract from dividend
	bis 0(r1), 6(r1) ; set quotient

modendif: cmp r4, r4	
	rrc 4(r1)
	rrc 2(r1) ; shift divisor
	rrc 0(r1) ; shift position
	jmp modwhile

moddone: mov 6(r1), r12 ; return quotient
	add #10, r1
	pop r0

.ident  "GCC: (GNU) 4.9.1 20140707 (prerelease (msp430-14r1-364)) (GNUPro 14r1) (Based on: GCC 4.8 GDB 7.7 Binutils 2.24 Newlib 2.1)"
