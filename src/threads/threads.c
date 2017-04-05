
#include <msp430.h>
#include <stdint.h>

#include "threads.h"

typedef void *thread_t;

volatile thread_t threads[MAX_THREADS];
volatile size_t thread_count;
volatile size_t current_thread;

extern void thread_exit_handler();

void threads_init() {
	// We don't need to initialize the main threads stack pointer, because
	// it is set when the first task switch occurs
	thread_count = 1;
	current_thread = 0;

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	// SMCLK = 1MHz; TACLK = SMCLK / 1 = 1MHz; t = 1 / TACLK = 1us; 16-bit-timer => overflow after 10.000ms
	TACTL = TASSEL_2 | MC_1; // SMCLK, Up to CCR0
	CCR0 = 9999;
	CCTL0 = 0;
}

void threads_join() {
	while(thread_count > 1) {
		// FIXME: polling is bad
	}
}

int threads_create(void *fn, void *stack) {
	if(thread_count >= MAX_THREADS) {
		return -1;
	}

	__disable_interrupt();

	uint16_t *stack_ptr = stack;

	// Exit handler
	*stack_ptr = (uint16_t) thread_exit_handler;

	// PC (Program counter)
	stack_ptr--;
	*stack_ptr = (uint16_t) fn;

	// SR (Status register)
	stack_ptr--;
	*stack_ptr = GIE;

	// Registers R4-R15
	for(int i = 0; i < 12; i++) {
		stack_ptr--;
		*stack_ptr = 0;
	}

	threads[thread_count] = stack_ptr;

	thread_count++;

	// Enable timer interrupt
	CCTL0 |= CCIE;
	__enable_interrupt();

	return 0;
}
