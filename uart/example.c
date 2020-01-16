#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "uart.h"

#define BAUD_RATE 9600

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// write a string to the uart
#define uart_print(s) uart_print_P(PSTR(s))
void uart_print_P(const char *str)
{
	char c;
	while (1) {
		c = pgm_read_byte(str++);
		if (!c) break;
		uart_putchar(c);
	}
}

int main(void)
{
	char c;

	CPU_PRESCALE(0);  // run at 16 MHz

    DDRD |= (1<<6); // output to LED on D6
	uart_init(BAUD_RATE);

	_delay_ms(2000); // wait for HC06 module to start up

	uart_print("AT+NAMEAmiigo"); // give HC06 module a name
	int okCount = 0;
	while (1) {
		if (uart_available()) {
			c = uart_getchar();
			if (okCount == 0 && c == 'O') {
			    okCount++;
			} else if (okCount == 1 && c == 'K') {
			    okCount++;
			} else if (okCount < 2) {
			    okCount = 0;
			}
			if (okCount == 2) {
                PORTD |= (1<<6); // set PD6
			}
		}
	}
}