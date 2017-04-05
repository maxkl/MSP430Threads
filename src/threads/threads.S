
#define __ASSEMBLY__

#include <msp430.h>

#include "threads.h"

.section ".bss"
thread_exited: .byte 0

.section ".text"

.global thread_exit_handler
thread_exit_handler:
	dint
	nop

	mov.b #1, thread_exited

	; Point R6 to the first thread entry
	mov current_thread, r4
	mov #threads, r6
	jmp 2f
	1:
		add #2, r6
		dec r4
	2:
		tst r4
		jne 1b

	; Shift all thread entries left by one
	mov current_thread, r4
	mov thread_count, r5
	dec r5
	jmp 2f
	1:
		mov 2(r6), 0(r6)
		add #2, r6
		inc r4
	2:
		cmp r4, r5
		jne 1b

	dec thread_count

	eint

1:	bis #CPUOFF, r1
	jmp 1b

TIMER0_A0_ISR:
	; Don't save context if the thread just exited
	; it is no longer in `threads` so this would overwrite the next thread
	; and mess everything up
	tst thread_exited
	jz 1f
	clr thread_exited
	jmp load_next_thread$
	1:

	; R0 and R2 have been pushed by hardware
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	; The stack now looks like this: ..., R0, R2, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15

	; Save stack pointer of current thread
	mov #0, r4
	mov current_thread, r5
	jmp 2f
	1:
		add #2, r4
		dec r5
	2:
		tst r5
		jne 1b
	add #threads, r4
	mov r1, @r4

	; Choose next thread
	inc current_thread

load_next_thread$:

	cmp thread_count, current_thread
	jl 1f
	mov #0, current_thread
	1:

	; Load new stack pointer
	mov #0, r4
	mov current_thread, r5
	jmp 2f
	1:
		add #2, r4
		dec r5
	2:
		tst r5
		jne 1b
	add #threads, r4
	mov @r4, r1

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	; R0 and R2 will be popped by hardware

	; Stop interrupts if there are no more threads
	cmp #1, thread_count
	jne 1f
	bic.b #CCIE, &CCTL0
	1:

	reti

.section "__interrupt_vector_10", "ax", @progbits
.word TIMER0_A0_ISR
