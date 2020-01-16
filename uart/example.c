#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "uart.h"

#define BAUD_RATE 9600

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

int led = 0;

int main(void)
{
	uint8_t c;
	CPU_PRESCALE(0);  // run at 16 MHz

    DDRD |= (1<<6); // output to LED on D6
	uart_init(BAUD_RATE);

	while (1) {
		if (uart_available()) {
			c = uart_getchar();
			uart_putchar(c);

			if (c == 'a') {
                if (led) {
                    PORTD &= ~(1<<6); // clear PD6
                } else {
                    PORTD |= (1<<6); // set PD6
                }
			    led = 1 - led;
			}

		}
	}
}
