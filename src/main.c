
#include <msp430.h>
#include <stdint.h>

#include "threads/threads.h"

#define STACK_SIZE 64

void thread1();
void thread2();

static uint16_t thread1_stack[STACK_SIZE];
static uint16_t thread2_stack[STACK_SIZE];

int main() {
	WDTCTL = WDTPW | WDTHOLD;

	P1DIR = 0xff;
	P1OUT = 0x00;
	P2DIR = 0xff;
	P2OUT = 0x00;

	threads_init();

	P1DIR |= BIT0 | BIT6;
	P1OUT &= ~(BIT0 | BIT6);

	threads_create(thread1, &thread1_stack[sizeof(thread1_stack) / sizeof(*thread1_stack) - 1]);
	threads_create(thread2, &thread2_stack[sizeof(thread2_stack) / sizeof(*thread2_stack) - 1]);

	threads_join();

	P1OUT &= ~(BIT0 | BIT6);

	__disable_interrupt();
	__bis_SR_register(LPM4_bits);
}

void thread1() {
	for(int i = 0; i < 10; i++) {
		P1OUT ^= BIT0;
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
	}

	P1OUT |= BIT0;
}

void thread2() {
	for(int i = 0; i < 10; i++) {
		P1OUT ^= BIT6;
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
		__delay_cycles(65535);
	}

	P1OUT |= BIT6;
}
